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



TEST(DataStructureTest, Test_IDs) {
    ClassProject::ManagerImplementation manager;
    ClassProject::Unique_identifier id1;
    ClassProject::Unique_identifier id2;
    manager.init();
    Unique_identifier_EQ(id1, id2);
}


TEST(DataStructureTest, Test_Entries) {
    ClassProject::Unique_table_entry entry1;
    ClassProject::Unique_table_entry entry2;
    Unique_table_entry_EQ(entry1, entry2);
}




TEST(InitializeTest, unique_table_size) {
    ClassProject::ManagerImplementation manager;
    manager.init();
    EXPECT_EQ(manager.uniqueTableSize(), 2);
}

TEST(InitializeTest, true_entry) {
    ClassProject::ManagerImplementation manager;
    manager.init();

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
    manager.init();

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
    manager.init();

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
    manager.init();


    EXPECT_EQ(manager.True(), 1);
    EXPECT_EQ(manager.False(), 0);

}



TEST(ManagerTest, topVarTest) {
    ClassProject::ManagerImplementation manager;
    manager.init();

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
    manager.init();

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
    manager.init();

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







TEST(ManagerTest, iteORTest) {
    ClassProject::ManagerImplementation manager;
    manager.init();

    ClassProject::BDD_ID id_a = manager.createVar("a");
    ClassProject::BDD_ID id_b = manager.createVar("b");

    //or(a,b) = ite(a, 1, b)
    ClassProject::BDD_ID result_id = manager.or2(id_a, id_b);

    ClassProject::Unique_identifier identifier_result;
    identifier_result.id_high = ClassProject::ID_TRUE;
    identifier_result.id_low  = id_b;
    identifier_result.top_var = "a";
    //(a,1,3)

    Unique_identifier_EQ(manager.get_table_entry(result_id).identifier, identifier_result);


    //and(a,b) = ite(b, a, 0)
    ClassProject::BDD_ID result_id2 = manager.or2(id_b, id_a);

    EXPECT_EQ(result_id, result_id2);

    identifier_result;
    identifier_result.id_high = ClassProject::ID_TRUE;
    identifier_result.id_low  = id_b;
    identifier_result.top_var = "a";
    //(a,3,0)

    Unique_identifier_EQ(manager.get_table_entry(result_id2).identifier, identifier_result);


    ClassProject::BDD_ID result_id3 = manager.or2(id_b, 1);    //or(b,1) = 1

    identifier_result;
    identifier_result.id_high = ClassProject::ID_TRUE;
    identifier_result.id_low  = ClassProject::ID_TRUE;
    identifier_result.top_var = "1";
    //(a,3,0)

    Unique_identifier_EQ(manager.get_table_entry(result_id3).identifier, identifier_result);



    ClassProject::BDD_ID result_id4 = manager.or2(id_b, 0);    //or(b,0) = b

    identifier_result;
    identifier_result.id_high = ClassProject::ID_TRUE;
    identifier_result.id_low  = ClassProject::ID_FALSE;
    identifier_result.top_var = "b";
    //(a,3,0)

    Unique_identifier_EQ(manager.get_table_entry(result_id4).identifier, identifier_result);
}



TEST(ManagerTest, iteANDTest) {
    ClassProject::ManagerImplementation manager;
    manager.init();

    ClassProject::BDD_ID id_a = manager.createVar("a");
    ClassProject::BDD_ID id_b = manager.createVar("b");

    //and(a,b) = ite(a, b, 0)
    ClassProject::BDD_ID result_id = manager.and2(id_a, id_b);

    ClassProject::Unique_identifier identifier_result;
    identifier_result.id_high = id_b;
    identifier_result.id_low  = ClassProject::ID_FALSE;
    identifier_result.top_var = "a";
    //(a,3,0)

    Unique_identifier_EQ(manager.get_table_entry(result_id).identifier, identifier_result);


    //and(a,b) = ite(b, a, 0)
    ClassProject::BDD_ID result_id2 = manager.and2(id_b, id_a);

    EXPECT_EQ(result_id, result_id2);

    identifier_result;
    identifier_result.id_high = id_b;
    identifier_result.id_low  = ClassProject::ID_FALSE;
    identifier_result.top_var = "a";
    //(a,3,0)

    Unique_identifier_EQ(manager.get_table_entry(result_id2).identifier, identifier_result);


    ClassProject::BDD_ID result_id3 = manager.and2(id_b, 1);    //and(b,1) = b

    identifier_result;
    identifier_result.id_high = ClassProject::ID_TRUE;
    identifier_result.id_low  = ClassProject::ID_FALSE;
    identifier_result.top_var = "b";
    //(a,3,0)

    Unique_identifier_EQ(manager.get_table_entry(result_id3).identifier, identifier_result);
}



TEST(ManagerTest, NegationTest) {
    ClassProject::ManagerImplementation manager;
    manager.init();

    ClassProject::BDD_ID id_a = manager.createVar("a");
    ClassProject::BDD_ID id_b = manager.createVar("b");

    //and(a,b) = ite(a, b, 0)
    ClassProject::BDD_ID id_result =  manager.neg(id_a); //neg(a)
    ClassProject::Unique_table_entry result_entry = manager.get_table_entry(id_result);

    ClassProject::Unique_identifier identifier_result;
    identifier_result.id_high = ClassProject::ID_FALSE;
    identifier_result.id_low  = ClassProject::ID_TRUE;
    identifier_result.top_var = "a";
    //(a,3,0)

    Unique_identifier_EQ(manager.get_table_entry(id_result).identifier, identifier_result);

    id_result = manager.neg(id_b);
    result_entry = manager.get_table_entry(id_result);

    identifier_result;
    identifier_result.id_high = ClassProject::ID_FALSE;
    identifier_result.id_low  = ClassProject::ID_TRUE;
    identifier_result.top_var = "b";
    //(a,3,0)

    Unique_identifier_EQ(manager.get_table_entry(id_result).identifier, identifier_result);



}





TEST(ManagerTest, NandTest) {
    ClassProject::ManagerImplementation manager;
    manager.init();

    ClassProject::BDD_ID id_a = manager.createVar("a");
    ClassProject::BDD_ID id_b = manager.createVar("b");

    //and(a,b) = ite(a, b, 0)
    ClassProject::BDD_ID id_result = manager.nand2(id_a,id_b); //nand(2,3) = not(a) + not(b)
    //should create through recursion a entry for not b and for and(a,b).
    EXPECT_EQ(manager.uniqueTableSize(), 7);

    ClassProject::Unique_identifier identifier_result;
    identifier_result.id_high = 5;  //id of not(b)
    identifier_result.id_low  = ClassProject::ID_TRUE;
    identifier_result.top_var = "a";
    //(a,3,0)

    Unique_identifier_EQ(manager.get_table_entry(id_result).identifier, identifier_result);

}



TEST(ManagerTest, NorTest) {
    ClassProject::ManagerImplementation manager;
    manager.init();

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
    //(a,3,0)

    Unique_identifier_EQ(manager.get_table_entry(id_result).identifier, identifier_result);

}


TEST(ManagerTest, XorTest) {
    ClassProject::ManagerImplementation manager;
    manager.init();

    ClassProject::BDD_ID id_a = manager.createVar("a");
    ClassProject::BDD_ID id_b = manager.createVar("b");

    //and(a,b) = ite(a, b, 0)
    ClassProject::BDD_ID id_result = manager.xor2(id_a,id_b); //nand(2,3) = not(a) + not(b)
    //should create through recursion a entry for not b and for and(a,b).
    EXPECT_EQ(manager.uniqueTableSize(), 9);

    ClassProject::Unique_identifier identifier_result;
    identifier_result.id_high = 5;      //id of not(b)
    identifier_result.id_low  = 3;      //id of b
    identifier_result.top_var = "a";
    //(a,3,0)

    Unique_identifier_EQ(manager.get_table_entry(id_result).identifier, identifier_result);

}


TEST(ManagerTest, UniqueTableExampleTest) {
    ClassProject::ManagerImplementation manager;
    manager.init();

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
    //(a,3,0)

    Unique_identifier_EQ(manager.get_table_entry(9).identifier, identifier_result);
    
}



TEST(ManagerTest, findNodesTest) {
    ClassProject::ManagerImplementation manager;
    manager.init();

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
    manager.init();

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
    manager.init();

    ClassProject::BDD_ID id_a = manager.createVar("a");
    ClassProject::BDD_ID id_b = manager.createVar("b");
    ClassProject::BDD_ID id_c = manager.createVar("c");
    ClassProject::BDD_ID id_d = manager.createVar("d");
    
    ClassProject::BDD_ID id_or_result = manager.or2(id_a, id_b);
    ClassProject::BDD_ID id_and_result =manager.and2(id_c, id_d);
    manager.and2(id_or_result, id_and_result);
    //manager.and2(manager.or2(id_a, id_b), manager.and2(id_c, id_d));

    ClassProject::BDD_ID result_id = manager.coFactorTrue(9, id_a);
    EXPECT_EQ(result_id, 7);


    ClassProject::BDD_ID result_id2 = manager.coFactorFalse(7, id_c);
    EXPECT_EQ(result_id2, 0);

    ClassProject::BDD_ID result_id3 = manager.coFactorTrue(9, id_b);
    ClassProject::Unique_identifier identifier_result;
    identifier_result.id_high = 5;
    identifier_result.id_low  = 0;
    identifier_result.top_var = "c";

    Unique_identifier_EQ(manager.get_table_entry(result_id3).identifier, identifier_result);

    ClassProject::BDD_ID result_id4 = manager.coFactorTrue(9, 100); //should do nothing
    ClassProject::Unique_identifier identifier_result2;
    identifier_result2.id_high = 5;
    identifier_result2.id_low  = 0;
    identifier_result2.top_var = "c";

    //here we can reduce the BDD again!!!!

    //Unique_identifier_EQ(manager.get_table_entry(result_id4).identifier, identifier_result2);
}







TEST(ManagerTest, DuplicateEntryTest) {
    ClassProject::ManagerImplementation manager;
    manager.init();

    manager.createVar("a");
    manager.createVar("b");

    //and(a,b) = ite(a, b, 0)
    manager.ite(2, 3, 0.);
    size_t s = manager.uniqueTableSize();
     //and(a,b) = ite(a, b, 0)
    manager.ite(2, 3, 0.);
    //size should not increase after this operation



    EXPECT_EQ(s, manager.uniqueTableSize());
}






#endif //VDS_PROJECT_TESTS_H

