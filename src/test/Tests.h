//
// Created by tobias on 21.11.16.
//

#ifndef VDS_PROJECT_GTESTS_H
#define VDS_PROJECT_GTESTS_H

#include "gtest/gtest.h"
#include "../Manager.h"



//define a custom function to compare 2 Unique_identifier
void Unique_identifier_EQ(ClassProject::Unique_identifier id1, ClassProject::Unique_identifier id2){
    EXPECT_EQ(id1.top_var, id2.top_var);
    EXPECT_EQ(id1.id_low,  id2.id_low);
    EXPECT_EQ(id1.id_high, id2.id_high);
}


//define a custom function to compare 2 Unique_table_entry
void Unique_table_entry_EQ(ClassProject::Unique_table_entry entry1, ClassProject::Unique_table_entry entry2){
    Unique_identifier_EQ(entry1.identifier, entry2.identifier);
    EXPECT_EQ(entry1.id,  entry2.id);
    EXPECT_EQ(entry1.label, entry2.label);
    EXPECT_EQ(entry1.is_variable, entry2.is_variable);
}


struct ManagerTest: testing::Test {
    ManagerTest(){
        manager = ClassProject::ManagerImplementation ();
    }
    
    ClassProject::ManagerImplementation manager;
};



TEST(InitializeTest, unique_table_size) {
    ClassProject::ManagerImplementation manager;
    //manager.init();
    EXPECT_EQ(manager.uniqueTableSize(), 2);
}

TEST(InitializeTest, true_entry) {
    ClassProject::ManagerImplementation manager;


    ClassProject::Unique_identifier TRUE_identifier;
    TRUE_identifier.id_high = ClassProject::ID_TRUE;
    TRUE_identifier.id_low  = ClassProject::ID_TRUE;
    TRUE_identifier.top_var = "1";


    ClassProject::Unique_table_entry TRUE_entry;
    TRUE_entry.id = ClassProject::ID_TRUE;
    TRUE_entry.label = "1";
    TRUE_entry.identifier = TRUE_identifier;
    TRUE_entry.is_variable = false;



    Unique_table_entry_EQ(manager.get_table_entry(ClassProject::ID_TRUE), TRUE_entry);
}


TEST(InitializeTest, false_entry) {
    ClassProject::ManagerImplementation manager;


    ClassProject::Unique_identifier FALSE_identifier;
    FALSE_identifier.id_high = ClassProject::ID_FALSE;
    FALSE_identifier.id_low  = ClassProject::ID_FALSE;
    FALSE_identifier.top_var = "0";


    ClassProject::Unique_table_entry FALSE_entry;
    FALSE_entry.id = ClassProject::ID_FALSE;
    FALSE_entry.label = "0";
    FALSE_entry.identifier = FALSE_identifier;
    FALSE_entry.is_variable = false;



    Unique_table_entry_EQ(manager.get_table_entry(ClassProject::ID_FALSE), FALSE_entry);
}




TEST(ManagerTest, uniqueTableSizeTest) {
    ClassProject::ManagerImplementation manager;

    size_t s = manager.uniqueTableSize();
    manager.createVar("a");
    manager.createVar("b");
    manager.createVar("c");
    manager.createVar("d");
    manager.createVar("e");

    s = s + 5;

    EXPECT_EQ(manager.uniqueTableSize(), s);
}



TEST(ManagerTest, ID_TRUE_FALSE) {
    ClassProject::ManagerImplementation manager;


    EXPECT_EQ(manager.True(), 1);
    EXPECT_EQ(manager.False(), 0);

}



TEST(ManagerTest, topVarTest) {
    ClassProject::ManagerImplementation manager;

    ClassProject::BDD_ID id_a = manager.createVar("a");
    ClassProject::BDD_ID id_b = manager.createVar("b");
    ClassProject::BDD_ID id_c = manager.createVar("c");



    EXPECT_EQ(manager.topVar(id_a), id_a);
    EXPECT_EQ(manager.topVar(id_b), id_b);

    ClassProject::BDD_ID id_and = manager.and2(id_a, id_b);
    EXPECT_EQ(manager.topVar(id_and), id_a);


    ClassProject::BDD_ID id_or = manager.and2(id_c, id_b);
    EXPECT_EQ(manager.topVar(id_or), id_b);


   ClassProject::BDD_ID id_and2 = manager.and2(id_or, id_and);
    EXPECT_EQ(manager.topVar(id_and2), id_a);
}



TEST(ManagerTest, iteTermincalCasesTest) {
    ClassProject::ManagerImplementation manager;

    //test the pure boolean cases first
    EXPECT_EQ(manager.ite(0,0,0), 0);
    EXPECT_EQ(manager.ite(1,0,0), 0);
    EXPECT_EQ(manager.ite(0,1,0), 0);
    EXPECT_EQ(manager.ite(1,1,0), 1);
    EXPECT_EQ(manager.ite(0,0,1), 1);
    EXPECT_EQ(manager.ite(1,0,1), 0);
    EXPECT_EQ(manager.ite(0,1,1), 1);
    EXPECT_EQ(manager.ite(1,1,1), 1);


    //test cases i is unknown(variable or sth else), t,e are boolean
    EXPECT_EQ(manager.ite(2,0,0), 0);
    EXPECT_EQ(manager.ite(2,1,1), 1);


    //test cases t is unknown(variable or sth else), i,e are boolean
    EXPECT_EQ(manager.ite(0,2,0), 0);
    EXPECT_EQ(manager.ite(0,2,1), 1);


    //test cases e is unknown(variable or sth else), i,t are boolean
    EXPECT_EQ(manager.ite(1,0,2), 0);
    EXPECT_EQ(manager.ite(1,1,2), 1);


    //test cases where ite returns one of its inputs
    EXPECT_EQ(manager.ite(0,1,3), 3);
    EXPECT_EQ(manager.ite(0,0,3), 3);
    EXPECT_EQ(manager.ite(1,5,0), 5);
    EXPECT_EQ(manager.ite(5,1,0), 5);
    EXPECT_EQ(manager.ite(1,5,1), 5);
}

TEST(ManagerTest, createVarTest) {
    ClassProject::ManagerImplementation manager;

    ClassProject::BDD_ID id_a = manager.createVar("a");
    
    
    
    
    ClassProject::Unique_identifier identifier_a;
    identifier_a.id_high = ClassProject::ID_TRUE;
    identifier_a.id_low  = ClassProject::ID_FALSE;
    identifier_a.top_var = "a";

    ClassProject::Unique_table_entry entry_a;
    entry_a.id = 2;
    entry_a.label = "a";
    entry_a.identifier = identifier_a;
    entry_a.is_variable = true;


    Unique_table_entry_EQ(manager.get_table_entry(id_a), entry_a);


    ClassProject::BDD_ID id_b = manager.createVar("b");


    ClassProject::Unique_identifier identifier_b;
    identifier_b.id_high = ClassProject::ID_TRUE;
    identifier_b.id_low  = ClassProject::ID_FALSE;
    identifier_b.top_var = "b";

    ClassProject::Unique_table_entry entry_b;
    entry_b.id = 3;
    entry_b.label = "b";
    entry_b.identifier = identifier_b;
    entry_b.is_variable = true;



    Unique_table_entry_EQ(manager.get_table_entry(id_b), entry_b);


    //variable names should be unique; thus in a cases of adding same variable twice just return the first id
    ClassProject::BDD_ID id_b_duplicate = manager.createVar("b");
    Unique_table_entry_EQ(manager.get_table_entry(id_b_duplicate), entry_b);


    ClassProject::BDD_ID id_empty_string = manager.createVar("");


    ClassProject::Unique_identifier identifier_empty;
    identifier_empty.id_high = ClassProject::ID_TRUE;
    identifier_empty.id_low  = ClassProject::ID_FALSE;
    identifier_empty.top_var = "";

    ClassProject::Unique_table_entry entry_empty_string;
    entry_empty_string.id = 4;
    entry_empty_string.label = "";
    entry_empty_string.identifier = identifier_empty;
    entry_empty_string.is_variable = true;


    Unique_table_entry_EQ(manager.get_table_entry(id_empty_string), entry_empty_string);
}







TEST(ManagerTest, OrTest) {
    ClassProject::ManagerImplementation manager;

    ClassProject::BDD_ID id_a = manager.createVar("a");
    ClassProject::BDD_ID id_b = manager.createVar("b");

    //or(a,b) = ite(a, 1, b)
    ClassProject::BDD_ID result_id = manager.or2(id_a, id_b);

    ClassProject::Unique_identifier identifier_result;
    identifier_result.id_high = ClassProject::ID_TRUE;
    identifier_result.id_low  = id_b;
    identifier_result.top_var = "a";

    Unique_identifier_EQ(manager.get_table_entry(result_id).identifier, identifier_result);


    //and(a,b) = ite(b, a, 0)
    ClassProject::BDD_ID result_id2 = manager.or2(id_b, id_a);

    EXPECT_EQ(result_id, result_id2);

    identifier_result;
    identifier_result.id_high = ClassProject::ID_TRUE;
    identifier_result.id_low  = id_b;
    identifier_result.top_var = "a";

    Unique_identifier_EQ(manager.get_table_entry(result_id2).identifier, identifier_result);


    ClassProject::BDD_ID result_id3 = manager.or2(id_b, 1);    //or(b,1) = 1

    identifier_result;
    identifier_result.id_high = ClassProject::ID_TRUE;
    identifier_result.id_low  = ClassProject::ID_TRUE;
    identifier_result.top_var = "1";

    Unique_identifier_EQ(manager.get_table_entry(result_id3).identifier, identifier_result);



    ClassProject::BDD_ID result_id4 = manager.or2(id_b, 0);    //or(b,0) = b

    identifier_result;
    identifier_result.id_high = ClassProject::ID_TRUE;
    identifier_result.id_low  = ClassProject::ID_FALSE;
    identifier_result.top_var = "b";
    //(a,3,0)

    Unique_identifier_EQ(manager.get_table_entry(result_id4).identifier, identifier_result);
}



TEST(ManagerTest, AndTest) {
    ClassProject::ManagerImplementation manager;

    ClassProject::BDD_ID id_a = manager.createVar("a");
    ClassProject::BDD_ID id_b = manager.createVar("b");



    ClassProject::BDD_ID result_id = manager.and2(id_a, id_b);

    ClassProject::Unique_identifier identifier_result;
    identifier_result.id_high = id_b;
    identifier_result.id_low  = ClassProject::ID_FALSE;
    identifier_result.top_var = "a";

    Unique_identifier_EQ(manager.get_table_entry(result_id).identifier, identifier_result);


    ClassProject::BDD_ID result_id2 = manager.and2(id_b, id_a);

    EXPECT_EQ(result_id, result_id2);

    identifier_result;
    identifier_result.id_high = id_b;
    identifier_result.id_low  = ClassProject::ID_FALSE;
    identifier_result.top_var = "a";

    Unique_identifier_EQ(manager.get_table_entry(result_id2).identifier, identifier_result);


    ClassProject::BDD_ID result_id3 = manager.and2(id_b, 1);    //and(b,1) = b

    identifier_result;
    identifier_result.id_high = ClassProject::ID_TRUE;
    identifier_result.id_low  = ClassProject::ID_FALSE;
    identifier_result.top_var = "b";

    Unique_identifier_EQ(manager.get_table_entry(result_id3).identifier, identifier_result);
}



TEST(ManagerTest, NegationTest) {
    ClassProject::ManagerImplementation manager;

    ClassProject::BDD_ID id_a = manager.createVar("a");
    ClassProject::BDD_ID id_b = manager.createVar("b");

    //and(a,b) = ite(a, b, 0)
    ClassProject::BDD_ID id_result =  manager.neg(id_a); //neg(a)
    ClassProject::Unique_table_entry result_entry = manager.get_table_entry(id_result);

    ClassProject::Unique_identifier identifier_result;
    identifier_result.id_high = ClassProject::ID_FALSE;
    identifier_result.id_low  = ClassProject::ID_TRUE;
    identifier_result.top_var = "a";

    Unique_identifier_EQ(manager.get_table_entry(id_result).identifier, identifier_result);

    id_result = manager.neg(id_b);
    result_entry = manager.get_table_entry(id_result);

    identifier_result;
    identifier_result.id_high = ClassProject::ID_FALSE;
    identifier_result.id_low  = ClassProject::ID_TRUE;
    identifier_result.top_var = "b";

    Unique_identifier_EQ(manager.get_table_entry(id_result).identifier, identifier_result);


    id_result = manager.neg(0);
    result_entry = manager.get_table_entry(id_result);

    identifier_result;
    identifier_result.id_high = ClassProject::ID_TRUE;
    identifier_result.id_low  = ClassProject::ID_TRUE;
    identifier_result.top_var = "1";


    Unique_identifier_EQ(manager.get_table_entry(id_result).identifier, identifier_result);
}





TEST(ManagerTest, NandTest) {
    ClassProject::ManagerImplementation manager;


    ClassProject::BDD_ID id_a = manager.createVar("a");
    ClassProject::BDD_ID id_b = manager.createVar("b");

    ClassProject::BDD_ID id_result = manager.nand2(id_a,id_b); //nand(a,b) = not(a) + not(b)
    //should create through recursion a entry for not b and for and(a,b).
    EXPECT_EQ(manager.uniqueTableSize(), 7);

    ClassProject::Unique_identifier identifier_result;
    identifier_result.id_high = 5;  //id of not(b)
    identifier_result.id_low  = ClassProject::ID_TRUE;
    identifier_result.top_var = "a";

    Unique_identifier_EQ(manager.get_table_entry(id_result).identifier, identifier_result);



    id_result = manager.nand2(id_a, 0); //nand(a,0) = not(a) + not(0) = 1

    identifier_result;
    identifier_result.id_high = ClassProject::ID_TRUE;
    identifier_result.id_low  = ClassProject::ID_TRUE;
    identifier_result.top_var = "1";

    Unique_identifier_EQ(manager.get_table_entry(id_result).identifier, identifier_result);


    id_result = manager.nand2(1, 1); //nand(1,1) = not(1) + not(1) = 0

    identifier_result;
    identifier_result.id_high = ClassProject::ID_FALSE;
    identifier_result.id_low  = ClassProject::ID_FALSE;
    identifier_result.top_var = "0";

    Unique_identifier_EQ(manager.get_table_entry(id_result).identifier, identifier_result);




}



TEST(ManagerTest, NorTest) {
    ClassProject::ManagerImplementation manager;


    ClassProject::BDD_ID id_a = manager.createVar("a");
    ClassProject::BDD_ID id_b = manager.createVar("b");

    //and(a,b) = ite(a, b, 0)
    ClassProject::BDD_ID id_result = manager.nor2(id_a,id_b); //nand(2,3) = not(a) + not(b)
    //should create through recursion a entry for not b and for and(a,b).
    EXPECT_EQ(manager.uniqueTableSize(), 7);

    ClassProject::Unique_identifier identifier_result;
    identifier_result.id_high = ClassProject::ID_FALSE;
    identifier_result.id_low  = 5;      //id of not(b)
    identifier_result.top_var = "a";

    Unique_identifier_EQ(manager.get_table_entry(id_result).identifier, identifier_result);


    id_result = manager.nor2(id_a,0); //nand(a,0) = not(a) and not(0) = not(a) and 1 = not(a)
    //should create through recursion a entry for not b and for and(a,b).

    identifier_result;
    identifier_result.id_high = ClassProject::ID_FALSE;
    identifier_result.id_low  = ClassProject::ID_TRUE;
    identifier_result.top_var = "a";



    Unique_identifier_EQ(manager.get_table_entry(id_result).identifier, identifier_result);
}


TEST(ManagerTest, XorTest) {
    ClassProject::ManagerImplementation manager;


    ClassProject::BDD_ID id_a = manager.createVar("a");
    ClassProject::BDD_ID id_b = manager.createVar("b");

    ClassProject::BDD_ID id_result = manager.xor2(id_a,id_b); //nand(2,3) = not(a) + not(b)
    //should create through recursion a entry for not b and for and(a,b).
    EXPECT_EQ(manager.uniqueTableSize(), 9);

    ClassProject::Unique_identifier identifier_result;
    identifier_result.id_high = 5;      //id of not(b)
    identifier_result.id_low  = 3;      //id of b
    identifier_result.top_var = "a";


    Unique_identifier_EQ(manager.get_table_entry(id_result).identifier, identifier_result);
}


TEST(ManagerTest, UniqueTableExampleTest) {
    ClassProject::ManagerImplementation manager;


    ClassProject::BDD_ID id_a = manager.createVar("a");
    ClassProject::BDD_ID id_b = manager.createVar("b");
    ClassProject::BDD_ID id_c = manager.createVar("c");
    ClassProject::BDD_ID id_d = manager.createVar("d");
    
    ClassProject::BDD_ID id_or_result = manager.or2(id_a, id_b);
    ClassProject::BDD_ID id_and_result =manager.and2(id_c, id_d);
    manager.and2(id_or_result, id_and_result);
    //manager.and2(manager.or2(id_a, id_b), manager.and2(id_c, id_d));

    EXPECT_EQ(manager.uniqueTableSize(), 10);

    
    ClassProject::Unique_identifier identifier_result;
    identifier_result.id_high = 7;
    identifier_result.id_low  = 8;
    identifier_result.top_var = "a";

    Unique_identifier_EQ(manager.get_table_entry(9).identifier, identifier_result);
}


TEST(ManagerTest, UniqueTable2Test) {
    ClassProject::ManagerImplementation manager;


    ClassProject::BDD_ID id_a = manager.createVar("a");
    ClassProject::BDD_ID id_b = manager.createVar("b");
    ClassProject::BDD_ID id_c = manager.createVar("c");
    ClassProject::BDD_ID id_d = manager.createVar("d");
    
    ClassProject::BDD_ID id_or_result = manager.or2(id_a, id_b);
    ClassProject::BDD_ID id_and_result = manager.and2(id_c, id_d);
    ClassProject::BDD_ID final_result = manager.and2(id_or_result, id_and_result);
    //now we have the table as in the example pdf file

    ClassProject::BDD_ID id_and_result_2 = manager.and2(id_c, id_d);

    ClassProject::Unique_identifier identifier_result;
    identifier_result.id_high = id_d;
    identifier_result.id_low  = ClassProject::ID_FALSE;
    identifier_result.top_var = "c";

    Unique_identifier_EQ(manager.get_table_entry(id_and_result_2).identifier, identifier_result);
    EXPECT_EQ(id_and_result_2, id_and_result);


    size_t size_before = manager.uniqueTableSize();
    ClassProject::BDD_ID final_result_2 = manager.and2(final_result, id_and_result_2);//should not change the table
    EXPECT_EQ(final_result, final_result_2);
    EXPECT_EQ(size_before, manager.uniqueTableSize());

    size_before = manager.uniqueTableSize();
    ClassProject::BDD_ID final_result_3 = manager.and2(final_result, final_result);//should not change the table
    EXPECT_EQ(final_result, final_result_3);
    EXPECT_EQ(size_before, manager.uniqueTableSize());


    size_before = manager.uniqueTableSize();
    ClassProject::BDD_ID final_result_4 = manager.or2(final_result, final_result);//should not change the table
    EXPECT_EQ(final_result, final_result_4);
    EXPECT_EQ(size_before, manager.uniqueTableSize());
}





TEST(ManagerTest, UniqueTable3Test) {
    ClassProject::ManagerImplementation manager;


    ClassProject::BDD_ID id_a = manager.createVar("a");
    ClassProject::BDD_ID id_b = manager.createVar("b");
    ClassProject::BDD_ID id_c = manager.createVar("c");
    ClassProject::BDD_ID id_d = manager.createVar("d");
    
    ClassProject::BDD_ID id_or_result = manager.or2(id_a, id_b);
    ClassProject::BDD_ID id_and_result = manager.and2(id_c, id_d);
    ClassProject::BDD_ID final_result = manager.and2(id_or_result, id_and_result);


    ClassProject::BDD_ID id_e = manager.createVar("e");

    size_t size_before = manager.uniqueTableSize();
    ClassProject::BDD_ID final_result_2 = manager.or2(final_result, id_e);
    EXPECT_EQ(size_before + 4, manager.uniqueTableSize());  //should create 4 new nodes



    ClassProject::Unique_identifier identifier_result;
    identifier_result.id_high = ClassProject::ID_TRUE;
    identifier_result.id_low  = id_e;
    identifier_result.top_var = "d";

    Unique_identifier_EQ(manager.get_table_entry(size_before).identifier, identifier_result);


    identifier_result.id_high = size_before;
    identifier_result.id_low  = id_e;
    identifier_result.top_var = "c";

    Unique_identifier_EQ(manager.get_table_entry(size_before +1).identifier, identifier_result);

    
    identifier_result.id_high = size_before + 1;
    identifier_result.id_low  = id_e;
    identifier_result.top_var = "b";

    Unique_identifier_EQ(manager.get_table_entry(size_before +2).identifier, identifier_result);


    identifier_result.id_high = size_before + 1;
    identifier_result.id_low  = size_before + 2;
    identifier_result.top_var = "a";

    Unique_identifier_EQ(manager.get_table_entry(final_result_2).identifier, identifier_result);




    ClassProject::BDD_ID id_f = manager.createVar("f");

    size_before = manager.uniqueTableSize();
    ClassProject::BDD_ID final_result_3 = manager.and2(final_result_2, id_f);
    EXPECT_EQ(size_before + 5, manager.uniqueTableSize());



    identifier_result.id_high = id_f;
    identifier_result.id_low  = ClassProject::ID_FALSE;
    identifier_result.top_var = "e";
    ClassProject::BDD_ID new_e_node = size_before;

    Unique_identifier_EQ(manager.get_table_entry(new_e_node).identifier, identifier_result);


    identifier_result.id_high = id_f;
    identifier_result.id_low  = new_e_node;
    identifier_result.top_var = "d";
    ClassProject::BDD_ID new_d_node = size_before +1;


    Unique_identifier_EQ(manager.get_table_entry(new_d_node).identifier, identifier_result);

    
    identifier_result.id_high = new_d_node;
    identifier_result.id_low  = new_e_node;
    identifier_result.top_var = "c";
    ClassProject::BDD_ID new_c_node = size_before +2;


    Unique_identifier_EQ(manager.get_table_entry(new_c_node).identifier, identifier_result);



    identifier_result.id_high = new_c_node;
    identifier_result.id_low  = size_before;
    identifier_result.top_var = "b";
    ClassProject::BDD_ID new_b_node = size_before +3;


    Unique_identifier_EQ(manager.get_table_entry(new_b_node).identifier, identifier_result);



    identifier_result.id_high = new_c_node;
    identifier_result.id_low  = new_b_node;
    identifier_result.top_var = "a";
    ClassProject::BDD_ID new_a_node = final_result_3;


    Unique_identifier_EQ(manager.get_table_entry(final_result_3).identifier, identifier_result);



    size_before = manager.uniqueTableSize();
    ClassProject::BDD_ID final_result_4 = manager.or2(final_result_3, id_c);
    //EXPECT_EQ(size_before + 5, manager.uniqueTableSize());


    identifier_result.id_high = ClassProject::ID_TRUE;
    identifier_result.id_low  = new_e_node;
    identifier_result.top_var = "c";
    ClassProject::BDD_ID new_c_node_2 = size_before;



    Unique_identifier_EQ(manager.get_table_entry(new_c_node_2).identifier, identifier_result);

    Unique_identifier_EQ(manager.get_table_entry(final_result_4).identifier, identifier_result);




    //check if old result stays untouched of our new computations
    identifier_result.id_high = new_c_node;
    identifier_result.id_low  = new_b_node;
    identifier_result.top_var = "a";

    Unique_identifier_EQ(manager.get_table_entry(final_result_3).identifier, identifier_result);




}








TEST(ManagerTest, findNodesTest) {
    ClassProject::ManagerImplementation manager;


    ClassProject::BDD_ID id_a = manager.createVar("a");
    ClassProject::BDD_ID id_b = manager.createVar("b");
    ClassProject::BDD_ID id_c = manager.createVar("c");
    ClassProject::BDD_ID id_d = manager.createVar("d");
    
    ClassProject::BDD_ID id_or_result = manager.or2(id_a, id_b);
    ClassProject::BDD_ID id_and_result =manager.and2(id_c, id_d);
    manager.and2(id_or_result, id_and_result);
    //manager.and2(manager.or2(id_a, id_b), manager.and2(id_c, id_d));

    std::set<ClassProject::BDD_ID> nodes_of_root;
    ClassProject::BDD_ID id_f = 9;
    manager.findNodes(id_f, nodes_of_root);

    std::set<ClassProject::BDD_ID> compare_result_set;
    compare_result_set.insert(9);
    compare_result_set.insert(8);
    compare_result_set.insert(0);
    compare_result_set.insert(7);
    compare_result_set.insert(5);
    compare_result_set.insert(1);


    EXPECT_EQ(nodes_of_root, compare_result_set);

}


TEST(ManagerTest, findVarsTest) {
    ClassProject::ManagerImplementation manager;


    ClassProject::BDD_ID id_a = manager.createVar("a");
    ClassProject::BDD_ID id_b = manager.createVar("b");
    ClassProject::BDD_ID id_c = manager.createVar("c");
    ClassProject::BDD_ID id_d = manager.createVar("d");
    
    ClassProject::BDD_ID id_or_result = manager.or2(id_a, id_b);
    ClassProject::BDD_ID id_and_result =manager.and2(id_c, id_d);
    manager.and2(id_or_result, id_and_result);
    //manager.and2(manager.or2(id_a, id_b), manager.and2(id_c, id_d));

    std::set<ClassProject::BDD_ID> vars_of_root;
    ClassProject::BDD_ID id_f = 9;
    manager.findVars(id_f, vars_of_root);

    std::set<ClassProject::BDD_ID> compare_result_set;
    compare_result_set.insert(id_a);
    compare_result_set.insert(id_b);
    compare_result_set.insert(id_c);
    compare_result_set.insert(id_d);

    std::set<ClassProject::BDD_ID> vars_of_root2;
    manager.findVars(8, vars_of_root2);

    std::set<ClassProject::BDD_ID> compare_result_set2;
    compare_result_set2.insert(id_c);
    compare_result_set2.insert(id_b);
    compare_result_set2.insert(id_d);



    EXPECT_EQ(vars_of_root2, compare_result_set2);
}





TEST(ManagerTest, CoFactorTrueTest) {
    ClassProject::ManagerImplementation manager;


    ClassProject::BDD_ID id_a = manager.createVar("a");
    ClassProject::BDD_ID id_b = manager.createVar("b");
    ClassProject::BDD_ID id_c = manager.createVar("c");
    ClassProject::BDD_ID id_d = manager.createVar("d");
    
    ClassProject::BDD_ID id_or_result = manager.and2(id_a, id_b);
    ClassProject::BDD_ID id_and_result =manager.and2(id_c, id_d);
    ClassProject::BDD_ID result = manager.and2(id_or_result, id_and_result);
    //manager.and2(manager.or2(id_a, id_b), manager.and2(id_c, id_d));


    size_t size_before = manager.uniqueTableSize();
    ClassProject::BDD_ID result_id = manager.coFactorTrue(result, id_b);
    EXPECT_EQ(size_before + 1, manager.uniqueTableSize());

    ClassProject::Unique_identifier identifier_result;
    identifier_result.id_high = id_and_result;
    identifier_result.id_low  = ClassProject::ID_FALSE;
    identifier_result.top_var = "a";


    
    Unique_identifier_EQ(manager.get_table_entry(result_id).identifier, identifier_result);

}



TEST(ManagerTest, CoFactorTrue2Test) {
    ClassProject::ManagerImplementation manager;


    ClassProject::BDD_ID id_a = manager.createVar("a");
    ClassProject::BDD_ID id_b = manager.createVar("b");
    ClassProject::BDD_ID id_c = manager.createVar("c");
    ClassProject::BDD_ID id_d = manager.createVar("d");
    
    ClassProject::BDD_ID id_or_result = manager.or2(id_a, id_b);
    ClassProject::BDD_ID id_and_result =manager.and2(id_c, id_d);
    ClassProject::BDD_ID result =manager.and2(id_or_result, id_and_result);




    size_t size_before = manager.uniqueTableSize();
    ClassProject::BDD_ID result_id = manager.coFactorTrue(result, id_c);
    EXPECT_EQ(size_before + 2, manager.uniqueTableSize());


    ClassProject::Unique_identifier identifier_result;
    identifier_result.id_high = id_d;
    identifier_result.id_low  = ClassProject::ID_FALSE;
    identifier_result.top_var = "b";
    ClassProject::BDD_ID new_b = size_before;


    Unique_identifier_EQ(manager.get_table_entry(new_b).identifier, identifier_result);


    identifier_result.id_high = id_d;
    identifier_result.id_low  = new_b;
    identifier_result.top_var = "a";

    Unique_identifier_EQ(manager.get_table_entry(result_id).identifier, identifier_result);


    ClassProject::BDD_ID result_id_2 = manager.coFactorTrue(result_id, id_d);
    ClassProject::BDD_ID result_id_3 = manager.coFactorTrue(result_id_2, id_a);
    ClassProject::BDD_ID result_id_4 = manager.coFactorTrue(result_id_3, id_b);


    identifier_result.id_high = ClassProject::ID_TRUE;
    identifier_result.id_low  = ClassProject::ID_TRUE;
    identifier_result.top_var = "1";

    Unique_identifier_EQ(manager.get_table_entry(result_id_4).identifier, identifier_result);


}




TEST(ManagerTest, CoFactorFalseTest) {
    ClassProject::ManagerImplementation manager;


    ClassProject::BDD_ID id_a = manager.createVar("a");
    ClassProject::BDD_ID id_b = manager.createVar("b");
    ClassProject::BDD_ID id_c = manager.createVar("c");
    ClassProject::BDD_ID id_d = manager.createVar("d");
    
    ClassProject::BDD_ID id_or_result = manager.and2(id_a, id_b);
    ClassProject::BDD_ID id_and_result =manager.and2(id_c, id_d);
    ClassProject::BDD_ID result = manager.and2(id_or_result, id_and_result);
    //manager.and2(manager.or2(id_a, id_b), manager.and2(id_c, id_d));


    size_t size_before = manager.uniqueTableSize();
    ClassProject::BDD_ID result_id = manager.coFactorFalse(result, id_b);
    //EXPECT_EQ(size_before + 1, manager.uniqueTableSize());

    ClassProject::Unique_identifier identifier_result;
    identifier_result.id_high = ClassProject::ID_FALSE;
    identifier_result.id_low  = ClassProject::ID_FALSE;
    identifier_result.top_var = "0";


    
    Unique_identifier_EQ(manager.get_table_entry(result_id).identifier, identifier_result);

}



TEST(ManagerTest, CoFactorFalse2Test) {
    ClassProject::ManagerImplementation manager;


    ClassProject::BDD_ID id_a = manager.createVar("a");
    ClassProject::BDD_ID id_b = manager.createVar("b");
    ClassProject::BDD_ID id_c = manager.createVar("c");
    ClassProject::BDD_ID id_d = manager.createVar("d");
    
    ClassProject::BDD_ID id_or_result = manager.or2(id_a, id_b);
    ClassProject::BDD_ID id_and_result =manager.and2(id_c, id_d);
    ClassProject::BDD_ID result =manager.and2(id_or_result, id_and_result);




    ClassProject::BDD_ID result_id = manager.coFactorFalse(result, id_c);


    ClassProject::Unique_identifier identifier_result;
    identifier_result.id_high = ClassProject::ID_FALSE;
    identifier_result.id_low  = ClassProject::ID_FALSE;
    identifier_result.top_var = "0";


    Unique_identifier_EQ(manager.get_table_entry(result_id).identifier, identifier_result);



    size_t size_before = manager.uniqueTableSize();
    ClassProject::BDD_ID result_id_2 = manager.coFactorFalse(result, id_b);
    EXPECT_EQ(size_before + 1, manager.uniqueTableSize());


    identifier_result;
    identifier_result.id_high = id_and_result;
    identifier_result.id_low  = ClassProject::ID_FALSE;
    identifier_result.top_var = "a";


    Unique_identifier_EQ(manager.get_table_entry(result_id_2).identifier, identifier_result);





    size_before = manager.uniqueTableSize();
    ClassProject::BDD_ID result_id_3 = manager.coFactorFalse(result, id_a);
    EXPECT_EQ(size_before, manager.uniqueTableSize());


    identifier_result;
    identifier_result.id_high = id_and_result;
    identifier_result.id_low  = ClassProject::ID_FALSE;
    identifier_result.top_var = "b";


    Unique_identifier_EQ(manager.get_table_entry(result_id_3).identifier, identifier_result);

}







TEST(ManagerTest, DuplicateEntryTest) {
    ClassProject::ManagerImplementation manager;


    ClassProject::BDD_ID id_a = manager.createVar("a");
    ClassProject::BDD_ID id_b = manager.createVar("b");

    manager.and2(id_a, id_b);
    size_t s = manager.uniqueTableSize();


    manager.and2(id_a, id_b);
    manager.and2(id_a, id_b);
    manager.and2(id_a, id_b);
    manager.and2(id_a, id_b);

    //size should not increase after these operations

    EXPECT_EQ(s, manager.uniqueTableSize());
}






#endif //VDS_PROJECT_TESTS_H

