#include <cassert>

#include "Manager.h"


// Local Variables:
// mode: c++
// End:
    

namespace ClassProject {


    ite_id::ite_id(BDD_ID x, BDD_ID y, BDD_ID z){
        i = x;
        t = y;
        e = z;
    }

    Manager::Manager(){
        //here initialize data-structure
        latest_id_value = LATEST_VALUE_INIT;

        
        //add entry for TRUE into unique table
        Unique_identifier TRUE_identifier;
        TRUE_identifier.id_high = ID_TRUE;
        TRUE_identifier.id_low  = ID_TRUE;
        TRUE_identifier.top_var = "1";


        Unique_table_entry TRUE_entry;
        TRUE_entry.id =ID_TRUE;
        TRUE_entry.label = "1";
        TRUE_entry.identifier = TRUE_identifier;
        TRUE_entry.is_variable = false;


        auto true_entrie_pair = std::make_pair(ID_TRUE, TRUE_entry);
        unique_table.insert (true_entrie_pair);
        //variable_to_id_map["1"] = ID_TRUE;
        unique_table_reverse[TRUE_identifier] = ID_TRUE;
        //unique_table[ID_TRUE] = TRUE_entry;  //add new variable entry to the table


        //add entry for FALSE into unique table
        Unique_identifier FALSE_identifier;
        FALSE_identifier.id_high = ID_FALSE;
        FALSE_identifier.id_low  = ID_FALSE;
        FALSE_identifier.top_var = "0";


        Unique_table_entry FALSE_entry;
        FALSE_entry.id =ID_FALSE;
        FALSE_entry.label = "0";
        FALSE_entry.identifier = FALSE_identifier;
        FALSE_entry.is_variable = false;

        //unique_table[ID_FALSE] = FALSE_entry;  //add new variable entry to the table

        auto false_entrie_pair = std::make_pair(ID_FALSE, FALSE_entry);
        unique_table.insert (false_entrie_pair);
        unique_table_reverse[FALSE_identifier] = ID_FALSE;    
        //variable_to_id_map["0"] = ID_FALSE;
    }


    BDD_ID Manager::ite(const BDD_ID i, const BDD_ID t, const BDD_ID e){
        //ite(i,t,e) = i*t + not(i)*e
        //case i) all 3 values are boolean then just evaluate i*t + not(i)*e
        if(i == 1) return t;
        if(i == 0) return e;

        if (t==e) return t;
        if(t == 1 && e== 0) return i;


        //case i) all 3 values are boolean then just evaluate i*t + not(i)*e
        if((i == 0 || i == 1) && (t == 0 || t == 1) && (e == 0 || e == 1)){
            bool i_b = (bool)i;
            bool t_b = (bool)t;
            bool e_b = (bool)e;

            bool ite_evaluation;
            ite_evaluation = i&&t || (!i)&&e;
            return (BDD_ID)ite_evaluation;
        }
        //end of terminal cases

        //check for entry in computed-table
        if( hashing_computed_table.find(ite_id(i, t, e)) != hashing_computed_table.end()) return hashing_computed_table[ite_id(i,t,e)];



        //get top variable of ID; top variable is variable with least-ranking of our 3 ID's
        std::string top_var_priority;
        int top_var_id_order = INT_MAX;

        Unique_identifier identifier_i = unique_table.at(i).identifier;
        Unique_identifier identifier_t = unique_table.at(t).identifier;
        Unique_identifier identifier_e = unique_table.at(e).identifier;


        if (!(i == 0 || i == 1)){
            top_var_priority = identifier_i.top_var;
            top_var_id_order = variable_to_id_map[top_var_priority];    //the id of the variable equals the priority of the variable
        }
        
        if (!(t == 0 || t == 1)) {
            BDD_ID t_var_id = variable_to_id_map[identifier_t.top_var]; 
            if(t_var_id < top_var_id_order){
                top_var_priority = identifier_t.top_var;
                top_var_id_order = t_var_id;    //the id of the variable equals the priority of the variable
            }
        }

        if (!(e == 0 || e == 1)){
            BDD_ID e_var_id = variable_to_id_map[identifier_e.top_var];
            if(e_var_id < top_var_id_order){
                top_var_priority = identifier_e.top_var;
                top_var_id_order = e_var_id;    //the id of the variable equals the priority of the variable
            }
        }
        

        BDD_ID i_high, i_low, t_high, t_low, e_high, e_low;

        if (identifier_i.top_var == top_var_priority){
            i_high = identifier_i.id_high;
            i_low = identifier_i.id_low;
        } else{
            i_high = i;
            i_low =  i;
        }

        if (identifier_t.top_var == top_var_priority){
            t_high = identifier_t.id_high;
            t_low = identifier_t.id_low;
        } else{
            t_high = t;
            t_low = t;
        }

        if (identifier_e.top_var == top_var_priority){
            e_high = identifier_e.id_high;
            e_low  = identifier_e.id_low;
        } else{
            e_high = e;
            e_low = e;
        }

  
        BDD_ID recursion_high = ite(i_high, t_high, e_high);
        BDD_ID recursion_low = ite(i_low, t_low, e_low);

        if (recursion_high == recursion_low) return recursion_high;


        Unique_identifier identifier_result;

        //add result to unique_table, if its not already in table
        identifier_result.top_var = top_var_priority;
        identifier_result.id_high = recursion_high;
        identifier_result.id_low = recursion_low;

        //check for duplicates before adding
        auto it = unique_table_reverse.find(identifier_result);
        if (it != unique_table_reverse.end()) return unique_table_reverse[identifier_result];
        
        int bdd_id = add_table_entry(identifier_result, "label"); //improve label
        hashing_computed_table[ite_id(i, t, e)] = bdd_id; //add entry in computed_table_entry(i, t, e);

        return bdd_id;
    }


    const BDD_ID&  Manager::True(){
        return ID_TRUE;
    }

    const BDD_ID&  Manager::False(){
        return ID_FALSE;
    }

    bool Manager::isConstant(const BDD_ID f){
        if(f == ID_FALSE || f == ID_TRUE) return true;
        return false;
    }

    size_t Manager::uniqueTableSize(){
        return unique_table.size();
    }

    bool Manager::isVariable(const BDD_ID x){
        return unique_table[x].is_variable;
    }

    BDD_ID Manager::createVar(const std::string &label){
        //first check variable already exists to avoid duplicate variable
        if(variable_to_id_map.find(label) != variable_to_id_map.end()){
            //variable already exists just return the id of this variable
            std::cout << "New variable already exists!" << std::endl;
            return variable_to_id_map[label];
        }
        //construct new Unique_table_entry and add to unique_table
        latest_id_value++;
        
        Unique_identifier identifier;
        identifier.id_high = ID_TRUE;   //for a variable id_high has the value of the "True" node
        identifier.id_low  = ID_FALSE;  //for a variable id_high has the value of the "False" node
        identifier.top_var = label;


        Unique_table_entry new_entry;
        new_entry.id =latest_id_value;
        new_entry.label = label;
        new_entry.identifier = identifier;
        new_entry.is_variable = true;

        variable_to_id_map[label] = latest_id_value;
        unique_table[latest_id_value] = new_entry;  //add new variable entry to the table
        unique_table_reverse[identifier] = latest_id_value;

        if (latest_id_value == 0) std::cout << "error not enough overflow of BDD_ID integer in table" << std::endl;
        return latest_id_value;
    }

    BDD_ID Manager::topVar(const BDD_ID f){      //returns the ID of the top_variable of the node f
        return variable_to_id_map[unique_table.at(f).identifier.top_var];
    }


    BDD_ID Manager::and2(const BDD_ID a, const BDD_ID b){
        return ite(a, b, ID_FALSE);
    }

    BDD_ID Manager::or2(const BDD_ID a, const BDD_ID b){
        return ite(a, ID_TRUE, b);
        //return neg(and2(neg(a), neg(b)));
    }

    BDD_ID Manager::xor2(const BDD_ID a, const BDD_ID b){
        //xor(a,b) = a*not(b) + not(a)*b
        //BDD_ID not_a = neg(a);
        //BDD_ID not_b =neg(b);
        //BDD_ID and_case_1 =and2(a, not_b);
        //BDD_ID and_case_2 =and2(not_a, b);
        return ite(ite(b, ID_FALSE, a), ID_TRUE, ite(a, ID_FALSE, b));
        //return or2(and_case_1, and_case_2);
    }

    BDD_ID Manager::neg(const BDD_ID a){
        return ite(a, ID_FALSE, ID_TRUE);
    }

    BDD_ID Manager::nand2(const BDD_ID a, const BDD_ID b){
        
        return neg(and2(a,b));
    }

    BDD_ID Manager::nor2(const BDD_ID a, const BDD_ID b){
        return neg(or2(a,b));
        //return and2(neg(a), neg(b));
    }


    BDD_ID Manager::coFactorFalse(const BDD_ID f, BDD_ID x){
        return coFactorCase(f,x,false);
    }

    
      BDD_ID Manager::coFactorTrue(const BDD_ID f, BDD_ID x){
        return coFactorCase(f,x,true);
    }
    

    BDD_ID Manager::coFactorTrue(const BDD_ID f){
        return unique_table.at(f).identifier.id_high;
    }

    BDD_ID Manager::coFactorFalse(const BDD_ID f){
        return unique_table.at(f).identifier.id_low;
    }

 
      BDD_ID Manager::coFactorCase(const BDD_ID f, BDD_ID x, bool cofactor_case){
        // cofactorcase == TRUE : coFactorTrue ; cofactorcase == FALSE : coFactorFalse
        //f is input function we use for our recursion
        //x is the id of the variable we want to do our cofactoring

        //get id of the top_variable
        Unique_identifier identifier = unique_table.at(f).identifier; 
        BDD_ID id_high = identifier.id_high;
        BDD_ID id_low = identifier.id_low;

        if(topVar(f) == x ) {
            if (cofactor_case) return id_high;
            return id_low;
        }

        if(isConstant(f)) return f; //f is leaf node thus stop recursion
        
        
        
        BDD_ID recursion_high = coFactorCase(id_high, x, cofactor_case);
        BDD_ID recursion_low  = coFactorCase(id_low, x, cofactor_case);

        if (recursion_high == recursion_low) return recursion_high;

        //add result to unique_table, if its not already in table
        identifier.id_high = recursion_high;
        identifier.id_low = recursion_low;

        //check for duplicates before adding
        auto it = unique_table_reverse.find(identifier);
        if (it != unique_table_reverse.end()) return unique_table_reverse[identifier];

        return add_table_entry(identifier, "label"); //improve label
    }


    void Manager::findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root){    //returns the set of BDD nodes whih are reachable from the BDD node root(including itself)
        //traverse through "tree" starting at root.
        //go down until
        nodes_of_root.insert(root);
        if (root == ID_FALSE || root == ID_TRUE) return;
        Unique_table_entry node = unique_table.at(root);

        findNodes(node.identifier.id_low, nodes_of_root);
        findNodes(node.identifier.id_high, nodes_of_root);
    }


    std::string Manager::getTopVarName(const BDD_ID &root){
        return unique_table.at(root).identifier.top_var;
    }



    void Manager::findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root){
        if (root == ID_FALSE || root == ID_TRUE) return;

        std::string var_name = getTopVarName(root);
        BDD_ID var_id = variable_to_id_map[var_name];
        vars_of_root.insert(var_id);
        Unique_table_entry node = unique_table.at(root);

        findVars(node.identifier.id_low, vars_of_root);
        findVars(node.identifier.id_high, vars_of_root);
    }


    BDD_ID Manager::add_table_entry(Unique_identifier identifier, std::string label){
        latest_id_value++;

        Unique_table_entry new_entry;
        new_entry.id =latest_id_value;
        new_entry.label = label;
        new_entry.identifier = identifier;
        new_entry.is_variable = false;

        unique_table[latest_id_value] = new_entry;  //add new variable entry to the table
        unique_table_reverse[identifier] = latest_id_value;    
        if (latest_id_value == 0) std::cout << "error not enough overflow of BDD_ID integer in table" << std::endl;
        return latest_id_value;
    }
}