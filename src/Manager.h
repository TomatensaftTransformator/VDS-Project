// A minimalistic BDD library, following Wolfgang Kunz lecture slides
//
// Written by Markus Wedler 2014

#ifndef mwBDD_H
#define mwBDD_H

#include <cassert>
#include <iostream>
#include <list>
#include <vector>
#include <unordered_map>
#include <map> 
#include <set>
#include <string>
#include <tuple>
#include <climits>
#include "ManagerInterface.h"

namespace ClassProject {
    const BDD_ID ID_TRUE = 1;
    const BDD_ID ID_FALSE = 0;
    const BDD_ID LATEST_VALUE_INIT = 1;


    /// class to manage boolean-functions represented as ROBDD's
    ///
    /// new varibles can be added and then boolean-expression be build through logic-functions
    class Manager : public ManagerInterface {
    public:
        Manager();

        /// if-then-else computation for 3 input BDD's
        ///
        /// ite(i, t, e) = or(and(i, t), and(not(i), e))
        /// used to implement boolean-functions e.g. and, or, not, nand on BDD nodes
        /// @param i BDD_ID of the root of the funnction that represents the "if"
        /// @param t BDD_ID of the root of the funnction that represents the "then"
        /// @param e BDD_ID of the root of the funnction that represents the "else"
        BDD_ID ite(const BDD_ID i, const BDD_ID t, const BDD_ID e);


        /// checks if given BDD node is a node of a variable
        ///
        /// BDD nodes of a variable always have child_high = True and child_low = False
        /// @param x is the BDD_ID of the BDD node on that the check is performed
        bool isVariable(const BDD_ID x);   //returns true if x is a variable
        
        
        /// checks if given BDD node is a constant node
        ///
        /// only the BDD node's of True and False are constant.
        /// there child_low = child_high = parent holds.
        /// @param f is the BDD_ID of the node which we want to know whether this node is constant or not
        bool isConstant(const BDD_ID f);    //returns true if f is a leaf-node


        /// returns the BDD_ID of the top-variable.
        ///
        /// returns the BDD_ID of the top-variable that the given BDD node has as its top-variable.
        BDD_ID topVar(const BDD_ID f);      //returns the ID of the top_variable of the node f
        

        /// creates a BDD node for a given variable name
        ///
        ///
        /// @param label name of the new variable
        /// @warning variable-names have to be unique!
        BDD_ID createVar(const std::string &label);     //creates new variable for the BDD
        
        
        /// returns name of the top-variable
        ///
        ///
        /// @param root BDD node from which we want to know its top-variable name
        std::string getTopVarName(const BDD_ID &root);  //returns asghar the name of the top-variable of the node root


        /// returns the set of BDD nodes that are reachable from the given root
        ///
        /// @param root for which we want to know all reachable nodes
        /// @param nodes_of_root set to which we will add all reachable nodes
        void findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root);    //returns the set of BDD nodes whih are reachable from the BDD node root(including itself)
        
        
        
        /// returns the set of all variables that are top-variables of reachable nodes
        ///
        /// @param root for which we want to know the top-variable names of all reachable nodes
        /// @param vars_of_root set to which we will add al top-variable names 
        void findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root);


        /// performs a "and" of two boolean expressions
        ///
        /// performs logic-and of two given boolean-expressions given through two BDD_ID's
        /// @return BDD_ID that represents the boolean-expression of the result
        BDD_ID and2(const BDD_ID a, const BDD_ID b);
        
    
        /// performs a "or" of two boolean expressions
        ///
        /// performs logic-or of two given boolean-expressions given through two BDD_ID's
        /// @return BDD_ID that represents the boolean-expression of the result
        BDD_ID or2(const BDD_ID a, const BDD_ID b);
        
        
        /// performs a "xor" of two boolean expressions
        ///
        /// performs logic-xor of two given boolean-expressions given through two BDD_ID's
        /// @return BDD_ID that represents the boolean-expression of the result
        BDD_ID xor2(const BDD_ID a, const BDD_ID b);
        
        
        /// performs a "negation" of a boolean expressions
        ///
        /// performs logic-negation of a given boolean-expressions given through a BDD_ID
        /// @return BDD_ID that represents the boolean-expression of the result
        BDD_ID neg(const BDD_ID a);
        
        
        /// performs a "nand" of two boolean expressions
        ///
        /// performs logic-nand of two given boolean-expressions given through two BDD_ID's
        /// @return BDD_ID that represents the boolean-expression of the result
        BDD_ID nand2(const BDD_ID a, const BDD_ID b);
        
        
        /// performs a "nor" of two boolean expressions
        ///
        /// performs logic-nor of two given boolean-expressions given through two BDD_ID's
        /// @return BDD_ID that represents the boolean-expression of the result
        BDD_ID nor2(const BDD_ID a, const BDD_ID b);


        /// returns coFactorFalse of given BDD root-node and the BDD_ID of the variable set to "False"
        ///
        /// returns BDD of boolean-function that is given when we set the top-variable x to "False"
        /// @param x is the BDD_ID of the variable we will set to "False"
        BDD_ID coFactorFalse(const BDD_ID f, BDD_ID x);
        
        
        /// returns child_low of given BDD node
        ///
        /// returns BDD of boolean-function that is given when we set the top-variable of f to "False"
        BDD_ID coFactorFalse(const BDD_ID f);


        /// returns coFactorFalse of given BDD root-node and the BDD_ID of the variable set to "True"
        ///
        /// returns BDD of boolean-function that is given when we set the top-variable x to "True"
        /// @param x is the BDD_ID of the variable we will set to "True"
        BDD_ID coFactorTrue(const BDD_ID f, BDD_ID x);


        /// returns child_high of given BDD node
        ///
        /// returns BDD of boolean-function that is given when we set the top-variable to "True"
        BDD_ID coFactorTrue(const BDD_ID f);


        /// returns BDD_ID of the node of the boolean value "True"
        ///
        ///
        const BDD_ID &True();
        
        
        /// returns BDD_ID of the node of the boolean value "False"
        ///
        ///
        const BDD_ID &False();


        /// returns the size of the unique-table
        ///
        /// size of unique-table of this manager
        size_t uniqueTableSize();


    private:

        class Unique_identifier {
            public:
                Unique_identifier() {};

                std::string top_var;
                BDD_ID id_low;
                BDD_ID id_high;
    
                // define "==" operation for unique_table_key
                bool operator==(const Unique_identifier &other) const
                { return (top_var == other.top_var
                            && id_low == other.id_low
                            && id_high == other.id_high);
                }
        };


            class MyHashFunction2 { 
        public: 
            size_t operator()(const Unique_identifier& key) const
            {
                return (((key.id_low  << 12)  +  key.id_high) << 12) + key.top_var.size();
            } 
        }; 


        class Unique_table_entry {
                public:
                    Unique_table_entry() {};


                    Unique_identifier identifier;
                    BDD_ID id;
                    std::string label;
                    bool is_variable;   //is true if the entry of this ID is a variable; false if not.
            };


        class ite_id {
        public:
            ite_id(BDD_ID x, BDD_ID y, BDD_ID z);
            BDD_ID i;
            BDD_ID t;
            BDD_ID e;


            bool operator==(const ite_id &other) const
            { return (i == other.i
            && t == other.t
            && e == other.e);
            }
        };

        class MyHashFunction { 
        public: 
            size_t operator()(const ite_id& key) const
            {
                return (((key.i  << 12)  +  key.t) << 12) + key.e;
            } 
        }; 


        BDD_ID latest_id_value;
        std::unordered_map<BDD_ID, Unique_table_entry> unique_table;
        std::unordered_map<Unique_identifier, BDD_ID, MyHashFunction2> unique_table_reverse;

        std::unordered_map<ite_id, BDD_ID, MyHashFunction> hashing_computed_table;


        std::unordered_map<std::string, BDD_ID> variable_to_id_map;

        BDD_ID add_table_entry(Unique_identifier identifier, std::string label);
        BDD_ID coFactorCase(const BDD_ID f, BDD_ID x, bool cofactor_case);

    };
}




#endif

// Local Variables:
// mode: c++
// End:
