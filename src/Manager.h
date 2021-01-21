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
            size_t a = key.i;
            size_t b = key.t;
            a = (a + b)*(a + b + 1)/2 + b;
            b = key.e;
            return (a + b)*(a + b + 1)/2 + b;
                
            //return key.i * 3 + key.t * 5 + key.e * 7; //just used first noob function that came to my mind; can or rather sould be improved!!!!
            //return key.i * 2 + key.t * 4 + key.e * 8; //just used first noob function that came to my mind; can or rather sould be improved!!!!

        } 
    }; 



    class Unique_identifier {
        public:
            Unique_identifier() {};        //constructor

            std::string top_var;
            BDD_ID id_low;
            BDD_ID id_high;
  
            // define "==" operation for unique_table_key
            bool operator==(const Unique_identifier &other) const
            { return (top_var == other.top_var
                        && id_low == other.id_low
                        && id_high == other.id_high);
            }

            bool operator<(const Unique_identifier &ob) const
            {
                if (id_low != ob.id_low) return id_low < ob.id_high;
                if (id_high != ob.id_high) id_high < ob.id_high;
                return top_var.compare(ob.top_var) < 0;
            }
    };



        class MyHashFunction2 { 
    public: 
        size_t operator()(const Unique_identifier& key) const
        { 
            
            size_t a = key.id_low;
            size_t b = key.id_high;
            a = (a + b)*(a + b + 1)/2 + b;
            b = key.top_var.size();
            return (a + b)*(a + b + 1)/2 + b;
            //return key.id_low * 3 + key.id_high * 5 + key.top_var.size() * 7; //just used first noob function that came to my mind; can or rather sould be improved!!!!
            //return key.id_low + key.id_high * 5 + key.top_var.size() * 7; //just used first noob function that came to my mind; can or rather sould be improved!!!!
        } 
    }; 



    //end of classes for absract-syntax-tree handling
    class Unique_table_entry {
        public:
            Unique_table_entry() {};   //constructor


            Unique_identifier identifier;
            BDD_ID id;
            std::string label;
            bool is_variable;   //is true if the entry of this ID is a variable; false if not.
    };





    class Manager : public ManagerInterface {
    public:
        Manager();    //Constructor

        BDD_ID ite(const BDD_ID i, const BDD_ID t, const BDD_ID e);



        bool isVariable(const BDD_ID x);   //returns true if x is a variable
        bool isConstant(const BDD_ID f);    //returns true if f is a leaf-node




        BDD_ID topVar(const BDD_ID f);      //returns the ID of the top_variable of the node f
        BDD_ID createVar(const std::string &label);     //creates new variable for the BDD
        std::string getTopVarName(const BDD_ID &root);  //returns asghar the name of the top-variable of the node root


        void findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root);    //returns the set of BDD nodes whih are reachable from the BDD node root(including itself)
        void findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root);


        BDD_ID and2(const BDD_ID a, const BDD_ID b);
        BDD_ID or2(const BDD_ID a, const BDD_ID b);
        BDD_ID xor2(const BDD_ID a, const BDD_ID b);
        BDD_ID neg(const BDD_ID a);
        BDD_ID nand2(const BDD_ID a, const BDD_ID b);
        BDD_ID nor2(const BDD_ID a, const BDD_ID b);

        BDD_ID coFactorFalse(const BDD_ID f, BDD_ID x);
        BDD_ID coFactorFalse(const BDD_ID f);

        BDD_ID coFactorTrue(const BDD_ID f, BDD_ID x);
        BDD_ID coFactorTrue(const BDD_ID f);

        BDD_ID coFactorCase(const BDD_ID f, BDD_ID x, bool cofactor_case);



        const BDD_ID &True();
        const BDD_ID &False();


        size_t uniqueTableSize();   //returns the number of nodes currently exist in the table of the manager


    private:
        BDD_ID latest_id_value;
        std::unordered_map<BDD_ID, Unique_table_entry> unique_table;
        std::unordered_map<Unique_identifier, BDD_ID, MyHashFunction2> unique_table_reverse;

        std::unordered_map<ite_id, BDD_ID, MyHashFunction> hashing_computed_table;


        std::unordered_map<std::string, BDD_ID> variable_to_id_map;

        BDD_ID add_table_entry(Unique_identifier identifier, std::string label);
    };
}




#endif

// Local Variables:
// mode: c++
// End:
