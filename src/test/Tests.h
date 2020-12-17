//
// Created by tobias on 21.11.16.
//

#ifndef VDS_PROJECT_GTESTS_H
#define VDS_PROJECT_GTESTS_H

#include "gtest/gtest.h"
#include "../Manager.h"



struct ManagerTest: testing::Test {
    ManagerTest(){
        manager = ClassProject::ManagerImplementation ();
    }
    
    ClassProject::ManagerImplementation manager;
};



TEST(InitializeTest, unique_table_size) {
    ClassProject::ManagerImplementation manager;
    EXPECT_EQ(manager.uniqueTableSize(), 2);
}

TEST(InitializeTest, true_entry) {
    ClassProject::ManagerImplementation manager;


    EXPECT_EQ(manager.True(), 1);
    EXPECT_EQ(manager.topVar(manager.True()), 1);
    EXPECT_EQ(manager.coFactorTrue(manager.True()), 1);
    EXPECT_EQ(manager.coFactorFalse(manager.True()), 1);
    EXPECT_EQ(manager.getTopVarName(manager.True()), "1");
}


TEST(InitializeTest, false_entry) {
    ClassProject::ManagerImplementation manager;


    EXPECT_EQ(manager.False(), 0);
    EXPECT_EQ(manager.topVar(manager.False()), 0);
    EXPECT_EQ(manager.coFactorTrue(manager.False()), 0);
    EXPECT_EQ(manager.coFactorFalse(manager.False()), 0);
    EXPECT_EQ(manager.getTopVarName(manager.False()), "0");
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



TEST(ManagerTest, isConstantTest) {
    ClassProject::ManagerImplementation manager;

    size_t s = manager.uniqueTableSize();
    ClassProject::BDD_ID id_a = manager.createVar("a");
    ClassProject::BDD_ID id_b = manager.createVar("b");
    ClassProject::BDD_ID id_c = manager.createVar("c");
    ClassProject::BDD_ID id_d = manager.createVar("d");
    ClassProject::BDD_ID id_e = manager.createVar("e");

    EXPECT_EQ(manager.isConstant(manager.True()), true);
    EXPECT_EQ(manager.isConstant(manager.False()), true);



    EXPECT_EQ(manager.isConstant(id_b), false);
    EXPECT_EQ(manager.isConstant(id_e), false);



    EXPECT_EQ(manager.isConstant(manager.and2(id_a, id_e)), false);
    EXPECT_EQ(manager.isConstant(manager.nand2(id_b, id_c)), false);
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



    EXPECT_EQ(manager.topVar(manager.True()), manager.True());


}

TEST(ManagerTest, getTopVarNameTest){
    ClassProject::ManagerImplementation manager;


    ClassProject::BDD_ID id_a = manager.createVar("a");
    ClassProject::BDD_ID id_b = manager.createVar("b");

    EXPECT_EQ("a", manager.getTopVarName(id_a));

    EXPECT_EQ("b", manager.getTopVarName(id_b));


    EXPECT_EQ("1", manager.getTopVarName(manager.True()));
    EXPECT_EQ("0", manager.getTopVarName(manager.False()));


    ClassProject::BDD_ID res = manager.and2(id_a, id_b);
    EXPECT_EQ("a", manager.getTopVarName(res));

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




TEST(ManagerTest, iteTermincal2CasesTest) {
    ClassProject::ManagerImplementation manager;


    ClassProject::BDD_ID id_a = manager.createVar("a");
    ClassProject::BDD_ID id_b = manager.createVar("b");

    ClassProject::BDD_ID res1 = manager.ite(1, id_a, id_b);
    ClassProject::BDD_ID res2 = manager.ite(0, id_a, id_b);


    EXPECT_EQ(id_a, res1);
    EXPECT_EQ(id_b, res2);

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



    EXPECT_EQ(manager.getTopVarName(id_a), "a");
    EXPECT_EQ(manager.topVar(id_a), id_a);
    EXPECT_EQ(manager.coFactorTrue(id_a), manager.True());
    EXPECT_EQ(manager.coFactorFalse(id_a), manager.False());
    EXPECT_EQ(manager.isVariable(id_a), true);



    ClassProject::BDD_ID id_b = manager.createVar("b");

    EXPECT_EQ(manager.getTopVarName(id_b), "b");
    EXPECT_EQ(manager.topVar(id_b), id_b);
    EXPECT_EQ(manager.coFactorTrue(id_b), manager.True());
    EXPECT_EQ(manager.coFactorFalse(id_b), manager.False());
    EXPECT_EQ(manager.isVariable(id_b), true);



    ClassProject::BDD_ID id_b_duplicate1 = manager.createVar(manager.getTopVarName(id_b));
    EXPECT_EQ(id_b_duplicate1, id_b);


    //variable names should be unique; thus in a cases of adding same variable twice just return the first id
    size_t size_before = manager.uniqueTableSize();
    ClassProject::BDD_ID id_b_duplicate = manager.createVar("b");
    EXPECT_EQ(manager.getTopVarName(id_b_duplicate), "b");
    EXPECT_EQ(manager.topVar(id_b), id_b_duplicate);
    EXPECT_EQ(manager.coFactorTrue(id_b_duplicate), manager.True());
    EXPECT_EQ(manager.coFactorFalse(id_b_duplicate), manager.False());
    EXPECT_EQ(manager.isVariable(id_b_duplicate), true);



    EXPECT_EQ(manager.uniqueTableSize(), size_before);
    EXPECT_EQ(id_b, id_b_duplicate);

    ClassProject::BDD_ID id_empty_string = manager.createVar("");

    EXPECT_EQ(manager.getTopVarName(id_empty_string), "");
    EXPECT_EQ(manager.topVar(id_empty_string), id_empty_string);
    EXPECT_EQ(manager.coFactorTrue(id_empty_string), manager.True());
    EXPECT_EQ(manager.coFactorFalse(id_empty_string), manager.False());
    EXPECT_EQ(manager.isVariable(id_empty_string), true);
}







TEST(ManagerTest, OrTest) {
    ClassProject::ManagerImplementation manager;

    ClassProject::BDD_ID id_a = manager.createVar("a");
    ClassProject::BDD_ID id_b = manager.createVar("b");

    //or(a,b) = ite(a, 1, b)
    ClassProject::BDD_ID result_id = manager.or2(id_a, id_b);


    EXPECT_EQ(manager.getTopVarName(result_id), "a");
    EXPECT_EQ(manager.topVar(result_id), id_a);
    EXPECT_EQ(manager.coFactorTrue(result_id), manager.True());
    EXPECT_EQ(manager.coFactorFalse(result_id), id_b);
    EXPECT_EQ(manager.isVariable(result_id), false);





    ClassProject::BDD_ID result_id2 = manager.or2(id_b, id_a);
    EXPECT_EQ(result_id, result_id2);


    ClassProject::BDD_ID result_id3 = manager.or2(id_b, 1);    //or(b,1) = 1
    EXPECT_EQ(result_id3, manager.True());


    ClassProject::BDD_ID result_id4 = manager.or2(id_b, 0);    //or(b,0) = b
    EXPECT_EQ(result_id4, id_b);
}



TEST(ManagerTest, AndTest) {
    ClassProject::ManagerImplementation manager;

    ClassProject::BDD_ID id_a = manager.createVar("a");
    ClassProject::BDD_ID id_b = manager.createVar("b");



    ClassProject::BDD_ID result_id = manager.and2(id_a, id_b);
    EXPECT_EQ(manager.getTopVarName(result_id), "a");
    EXPECT_EQ(manager.topVar(result_id), id_a);
    EXPECT_EQ(manager.coFactorTrue(result_id), id_b);
    EXPECT_EQ(manager.coFactorFalse(result_id), manager.False());
    EXPECT_EQ(manager.isVariable(result_id), false);



    ClassProject::BDD_ID result_id2 = manager.and2(id_b, id_a);
    EXPECT_EQ(result_id, result_id2);




    ClassProject::BDD_ID result_id3 = manager.and2(id_b, 1);    //and(b,1) = b
    EXPECT_EQ(result_id3, id_b);
}



TEST(ManagerTest, NegationTest) {
    ClassProject::ManagerImplementation manager;

    ClassProject::BDD_ID id_a = manager.createVar("a");
    ClassProject::BDD_ID id_b = manager.createVar("b");



    ClassProject::BDD_ID result_id =  manager.neg(id_a); //neg(a)
    EXPECT_EQ(manager.getTopVarName(result_id), "a");
    EXPECT_EQ(manager.topVar(result_id), id_a);
    EXPECT_EQ(manager.coFactorTrue(result_id), manager.False());
    EXPECT_EQ(manager.coFactorFalse(result_id), manager.True());
    EXPECT_EQ(manager.isVariable(result_id), false);



    result_id = manager.neg(id_b);
    EXPECT_EQ(manager.getTopVarName(result_id), "b");
    EXPECT_EQ(manager.topVar(result_id), id_b);
    EXPECT_EQ(manager.coFactorTrue(result_id), manager.False());
    EXPECT_EQ(manager.coFactorFalse(result_id), manager.True());
    EXPECT_EQ(manager.isVariable(result_id), false);


    result_id = manager.neg(0);
    EXPECT_EQ(result_id, manager.True());


    result_id = manager.neg(1);
    EXPECT_EQ(result_id, manager.False());
}





TEST(ManagerTest, NandTest) {
    ClassProject::ManagerImplementation manager;


    ClassProject::BDD_ID id_a = manager.createVar("a");
    ClassProject::BDD_ID id_b = manager.createVar("b");

    ClassProject::BDD_ID not_b = manager.neg(id_b);
    ClassProject::BDD_ID id_result = manager.nand2(id_a,id_b); //nand(a,b) = not(a) + not(b)
    //should create through recursion a entry for not b and for and(a,b).
    EXPECT_EQ(manager.uniqueTableSize(), 7);


    EXPECT_EQ(manager.getTopVarName(id_result), "a");
    EXPECT_EQ(manager.topVar(id_result), id_a);
    EXPECT_EQ(manager.coFactorTrue(id_result), not_b);
    EXPECT_EQ(manager.coFactorFalse(id_result), manager.True());
    EXPECT_EQ(manager.isVariable(id_result), false);



    id_result = manager.nand2(id_a, 0); //nand(a,0) = not(a) + not(0) = 1
    EXPECT_EQ(id_result, manager.True());



    id_result = manager.nand2(1, id_b); //nand(1,b) = not(1) + not(b) = not(b)
    EXPECT_EQ(manager.getTopVarName(id_result), "b");
    EXPECT_EQ(manager.topVar(id_result), id_b);
    EXPECT_EQ(manager.coFactorTrue(id_result), manager.False());
    EXPECT_EQ(manager.coFactorFalse(id_result), manager.True());
    EXPECT_EQ(manager.isVariable(id_result), false);



    id_result = manager.nand2(1, 1); //nand(1,1) = not(1) + not(1) = 0
    EXPECT_EQ(id_result, manager.False());
}



TEST(ManagerTest, NorTest) {
    ClassProject::ManagerImplementation manager;


    ClassProject::BDD_ID id_a = manager.createVar("a");
    ClassProject::BDD_ID id_b = manager.createVar("b");

    ClassProject::BDD_ID id_not_b = manager.neg(id_b);
    ClassProject::BDD_ID id_result = manager.nor2(id_a,id_b); //nor(a,b) = not(or(a,b))
    EXPECT_EQ(manager.uniqueTableSize(), 7);


    EXPECT_EQ(manager.getTopVarName(id_result), "a");
    EXPECT_EQ(manager.topVar(id_result), id_a);
    EXPECT_EQ(manager.coFactorTrue(id_result), manager.False());
    EXPECT_EQ(manager.coFactorFalse(id_result), id_not_b);
    EXPECT_EQ(manager.isVariable(id_result), false);



    ClassProject::BDD_ID id_not_a = manager.neg(id_a);
    id_result = manager.nor2(id_a,0); //nor(a,0) = not(a + 0) = not(a)
    EXPECT_EQ(id_result, id_not_a);


    id_result = manager.nor2(id_a, 1); //nor(a,1) = not(a + 1) = not(1) = 0
    EXPECT_EQ(id_result, manager.False());
}


TEST(ManagerTest, XorTest) {
    ClassProject::ManagerImplementation manager;


    ClassProject::BDD_ID id_a = manager.createVar("a");
    ClassProject::BDD_ID id_b = manager.createVar("b");

    ClassProject::BDD_ID id_not_b = manager.neg(id_b); //xor(a,b) = a*not(b) + not(a)*b
    ClassProject::BDD_ID id_result = manager.xor2(id_a,id_b); //xor(a,b) = a*not(b) + not(a)*b
    //should create through recursion a entry for not(b) and for not(a)).
    //should create a entry for a*not(b) ; not(b)*a
    EXPECT_EQ(manager.uniqueTableSize(), 9); //this size is 2 leafs + 2 vars + 2 nots + 2 ands + final result = 10


    EXPECT_EQ(manager.getTopVarName(id_result), "a");
    EXPECT_EQ(manager.topVar(id_result), id_a);
    EXPECT_EQ(manager.coFactorTrue(id_result), id_not_b);
    EXPECT_EQ(manager.coFactorFalse(id_result), id_b);
    EXPECT_EQ(manager.isVariable(id_result), false);



    size_t size_before = manager.uniqueTableSize();
    id_result = manager.xor2(id_b, id_a); //should not change the table
    EXPECT_EQ(manager.uniqueTableSize(), size_before);


    ClassProject::BDD_ID id_result2 = manager.xor2(id_a,0); //xor(a,0) = a*1 + not(a)*0 = a
    EXPECT_EQ(id_result2, id_a);


    size_before = manager.uniqueTableSize();
    ClassProject::BDD_ID id_result3 = manager.xor2(1, id_b); //xor(1,b) = 1*not b + 0*b = notb
    EXPECT_EQ(id_result3, id_not_b);
}


TEST(ManagerTest, UniqueTableExampleTest) {
    ClassProject::ManagerImplementation manager;


    ClassProject::BDD_ID id_a = manager.createVar("a");
    ClassProject::BDD_ID id_b = manager.createVar("b");
    ClassProject::BDD_ID id_c = manager.createVar("c");
    ClassProject::BDD_ID id_d = manager.createVar("d");
    
    ClassProject::BDD_ID id_or_result = manager.or2(id_a, id_b);
    ClassProject::BDD_ID id_and_result =manager.and2(id_c, id_d);
    ClassProject::BDD_ID id_result = manager.and2(id_or_result, id_and_result);

    EXPECT_EQ(manager.uniqueTableSize(), 10);
    size_t size_before = manager.uniqueTableSize();
    ClassProject::BDD_ID neg_result = manager.neg(id_result);
    EXPECT_EQ(manager.uniqueTableSize(), size_before + 4);



    //root node of same function as in bdd_example.pdf
    EXPECT_EQ(manager.getTopVarName(id_result), "a");
    EXPECT_EQ(manager.topVar(id_result), id_a);
    EXPECT_EQ(manager.coFactorTrue(id_result), id_and_result);
    EXPECT_EQ(manager.coFactorFalse(id_result),id_result - 1);
    EXPECT_EQ(manager.isVariable(id_result), false);



    //node with id=8 in bdd_example.pdf
    EXPECT_EQ(manager.getTopVarName(id_result -1), "b");
    EXPECT_EQ(manager.topVar(id_result - 1), id_b);
    EXPECT_EQ(manager.coFactorTrue(id_result - 1), id_and_result);
    EXPECT_EQ(manager.coFactorFalse(id_result - 1), manager.False());
    EXPECT_EQ(manager.isVariable(id_result - 1), false);
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

    EXPECT_EQ(manager.getTopVarName(id_and_result), "c");
    EXPECT_EQ(manager.topVar(id_and_result), id_c);
    EXPECT_EQ(manager.coFactorTrue(id_and_result), id_d);
    EXPECT_EQ(manager.coFactorFalse(id_and_result), manager.False());
    EXPECT_EQ(manager.isVariable(id_and_result), false);









    ClassProject::BDD_ID id_and_result_2 = manager.and2(id_c, id_d);
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


    EXPECT_EQ(manager.getTopVarName(size_before), "d");
    EXPECT_EQ(manager.topVar(size_before), id_d);
    EXPECT_EQ(manager.coFactorTrue(size_before), manager.True());
    EXPECT_EQ(manager.coFactorFalse(size_before), id_e);
    EXPECT_EQ(manager.isVariable(size_before), false);








    EXPECT_EQ(manager.getTopVarName(size_before  +1), "c");
    EXPECT_EQ(manager.topVar(size_before + 1), id_c);
    EXPECT_EQ(manager.coFactorTrue(size_before + 1), size_before);
    EXPECT_EQ(manager.coFactorFalse(size_before + 1), id_e);
    EXPECT_EQ(manager.isVariable(size_before + 1) , false);







    EXPECT_EQ(manager.getTopVarName(size_before + 2), "b");
    EXPECT_EQ(manager.topVar(size_before + 2), id_b);
    EXPECT_EQ(manager.coFactorTrue(size_before + 2), size_before + 1);
    EXPECT_EQ(manager.coFactorFalse(size_before + 2), id_e);
    EXPECT_EQ(manager.isVariable(size_before + 2) , false);










    EXPECT_EQ(manager.getTopVarName(final_result_2), "a");
    EXPECT_EQ(manager.topVar(final_result_2), id_a);
    EXPECT_EQ(manager.coFactorTrue(final_result_2), size_before + 1);
    EXPECT_EQ(manager.coFactorFalse(final_result_2), size_before + 2);
    EXPECT_EQ(manager.isVariable(final_result_2) , false);



    ClassProject::BDD_ID id_f = manager.createVar("f");

    size_before = manager.uniqueTableSize();
    ClassProject::BDD_ID final_result_3 = manager.and2(final_result_2, id_f);
    EXPECT_EQ(size_before + 5, manager.uniqueTableSize());



    ClassProject::BDD_ID new_e_node = size_before;




    EXPECT_EQ(manager.getTopVarName(new_e_node), "e");
    EXPECT_EQ(manager.topVar(new_e_node), id_e);
    EXPECT_EQ(manager.coFactorTrue(new_e_node), id_f);
    EXPECT_EQ(manager.coFactorFalse(new_e_node), manager.False());
    EXPECT_EQ(manager.isVariable(new_e_node) , false);





    ClassProject::BDD_ID new_d_node = size_before +1;





    EXPECT_EQ(manager.getTopVarName(new_d_node), "d");
    EXPECT_EQ(manager.topVar(new_d_node), id_d);
    EXPECT_EQ(manager.coFactorTrue(new_d_node), id_f);
    EXPECT_EQ(manager.coFactorFalse(new_d_node), new_e_node);
    EXPECT_EQ(manager.isVariable(new_d_node) , false);





    ClassProject::BDD_ID new_c_node = size_before +2;






    EXPECT_EQ(manager.getTopVarName(new_c_node), "c");
    EXPECT_EQ(manager.topVar(new_c_node), id_c);
    EXPECT_EQ(manager.coFactorTrue(new_c_node), new_d_node);
    EXPECT_EQ(manager.coFactorFalse(new_c_node), new_e_node);
    EXPECT_EQ(manager.isVariable(new_c_node) , false);









    ClassProject::BDD_ID new_b_node = size_before +3;





    EXPECT_EQ(manager.getTopVarName(new_b_node), "b");
    EXPECT_EQ(manager.topVar(new_b_node), id_b);
    EXPECT_EQ(manager.coFactorTrue(new_b_node), new_c_node);
    EXPECT_EQ(manager.coFactorFalse(new_b_node), new_e_node);
    EXPECT_EQ(manager.isVariable(new_b_node) , false);















    ClassProject::BDD_ID new_a_node = final_result_3;


    EXPECT_EQ(manager.getTopVarName(final_result_3), "a");
    EXPECT_EQ(manager.topVar(final_result_3), id_a);
    EXPECT_EQ(manager.coFactorTrue(final_result_3), new_c_node);
    EXPECT_EQ(manager.coFactorFalse(final_result_3), new_b_node);
    EXPECT_EQ(manager.isVariable(final_result_3) , false);







    size_before = manager.uniqueTableSize();
    ClassProject::BDD_ID final_result_4 = manager.or2(final_result_3, id_c);
    //EXPECT_EQ(size_before + 5, manager.uniqueTableSize());




    EXPECT_EQ(manager.getTopVarName(final_result_4), "c");
    EXPECT_EQ(manager.topVar(final_result_4), id_c);
    EXPECT_EQ(manager.coFactorTrue(final_result_4), manager.True());
    EXPECT_EQ(manager.coFactorFalse(final_result_4), new_e_node);
    EXPECT_EQ(manager.isVariable(final_result_4) , false);
}



TEST(ManagerTest, UniqueTable4Test) {
    ClassProject::ManagerImplementation manager;


    ClassProject::BDD_ID id_a = manager.createVar("a");
    ClassProject::BDD_ID id_b = manager.createVar("b");
    ClassProject::BDD_ID id_c = manager.createVar("c");
    ClassProject::BDD_ID id_d = manager.createVar("d");
    ClassProject::BDD_ID id_e = manager.createVar("e");
    ClassProject::BDD_ID id_f = manager.createVar("f");

    ClassProject::BDD_ID id_and_acdf = manager.and2(manager.and2(manager.and2(id_a, id_c), id_d), id_f);
    ClassProject::BDD_ID id_and_bcd =  manager.and2(manager.and2(id_b, id_c), id_d);
    ClassProject::BDD_ID final_result = manager.or2(id_and_acdf, id_and_bcd);

    EXPECT_EQ(manager.getTopVarName(final_result), "a");
    EXPECT_EQ(manager.topVar(final_result), id_a);
    EXPECT_EQ(manager.coFactorTrue(final_result), 16);
    EXPECT_EQ(manager.coFactorFalse(final_result), 15);
    EXPECT_EQ(manager.isVariable(final_result) , false);


    EXPECT_EQ(manager.getTopVarName(16), "b");
    EXPECT_EQ(manager.topVar(16), id_b);
    EXPECT_EQ(manager.coFactorTrue(16), 9);
    EXPECT_EQ(manager.coFactorFalse(16), 12);
    EXPECT_EQ(manager.isVariable(16) , false);


    EXPECT_EQ(manager.getTopVarName(15), "b");
    EXPECT_EQ(manager.topVar(15), id_b);
    EXPECT_EQ(manager.coFactorTrue(15), 9);
    EXPECT_EQ(manager.coFactorFalse(15), 0);
    EXPECT_EQ(manager.isVariable(15) , false);


    EXPECT_EQ(manager.getTopVarName(9), "c");
    EXPECT_EQ(manager.topVar(9), id_c);
    EXPECT_EQ(manager.coFactorTrue(9), 5);
    EXPECT_EQ(manager.coFactorFalse(9), 0);
    EXPECT_EQ(manager.isVariable(9) , false);


    EXPECT_EQ(manager.getTopVarName(5), "d");
    EXPECT_EQ(manager.topVar(5), id_d);
    EXPECT_EQ(manager.coFactorTrue(5), 1);
    EXPECT_EQ(manager.coFactorFalse(5), 0);
    EXPECT_EQ(manager.isVariable(5) , true);



    EXPECT_EQ(manager.getTopVarName(12), "c");
    EXPECT_EQ(manager.topVar(12), id_c);
    EXPECT_EQ(manager.coFactorTrue(12), 11);
    EXPECT_EQ(manager.coFactorFalse(12), 0);
    EXPECT_EQ(manager.isVariable(12) , false);



    EXPECT_EQ(manager.getTopVarName(11), "d");
    EXPECT_EQ(manager.topVar(11), id_d);
    EXPECT_EQ(manager.coFactorTrue(11), 7);
    EXPECT_EQ(manager.coFactorFalse(11), 0);
    EXPECT_EQ(manager.isVariable(11) , false);



    EXPECT_EQ(manager.getTopVarName(7), "f");
    EXPECT_EQ(manager.topVar(7), id_f);
    EXPECT_EQ(manager.coFactorTrue(7), 1);
    EXPECT_EQ(manager.coFactorFalse(7), 0);
    EXPECT_EQ(manager.isVariable(7) , true);




}







TEST(ManagerTest, findNodesTest) {
    ClassProject::ManagerImplementation manager;


    ClassProject::BDD_ID id_a = manager.createVar("a");
    ClassProject::BDD_ID id_b = manager.createVar("b");
    ClassProject::BDD_ID id_c = manager.createVar("c");
    ClassProject::BDD_ID id_d = manager.createVar("d");
    
    ClassProject::BDD_ID id_or_result = manager.or2(id_a, id_b);
    ClassProject::BDD_ID id_and_result =manager.and2(id_c, id_d);
    ClassProject::BDD_ID id_res = manager.and2(id_or_result, id_and_result);
    //manager.and2(manager.or2(id_a, id_b), manager.and2(id_c, id_d));

    std::set<ClassProject::BDD_ID> nodes_of_root;
    manager.findNodes(id_res, nodes_of_root);

    std::set<ClassProject::BDD_ID> compare_result_set;
    compare_result_set.insert(9);
    compare_result_set.insert(8);
    compare_result_set.insert(0);
    compare_result_set.insert(7);
    compare_result_set.insert(5);
    compare_result_set.insert(1);


    EXPECT_EQ(nodes_of_root, compare_result_set);

    manager.findNodes(id_res, nodes_of_root);   //set should stay unchanged after apllying with same root node again
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

    EXPECT_EQ(vars_of_root, compare_result_set);


    std::set<ClassProject::BDD_ID> vars_of_root2;
    manager.findVars(8, vars_of_root2);

    std::set<ClassProject::BDD_ID> compare_result_set2;
    compare_result_set2.insert(id_c);
    compare_result_set2.insert(id_b);
    compare_result_set2.insert(id_d);



    EXPECT_EQ(vars_of_root2, compare_result_set2);
}




TEST(ManagerTest, findVars2Test) {
    ClassProject::ManagerImplementation manager;


    ClassProject::BDD_ID id_a = manager.createVar("a");
    ClassProject::BDD_ID id_b = manager.createVar("b");
    ClassProject::BDD_ID id_c = manager.createVar("c");
    ClassProject::BDD_ID id_d = manager.createVar("d");
    
    ClassProject::BDD_ID id_or_result =  manager.or2(id_a, id_b);
    ClassProject::BDD_ID id_and_result = manager.and2(id_c, id_d);
    ClassProject::BDD_ID id_result = manager.and2(id_or_result, id_and_result);
    //manager.and2(manager.or2(id_a, id_b), manager.and2(id_c, id_d));
    id_result = manager.coFactorFalse(id_result, id_b);
    std::set<ClassProject::BDD_ID> vars_of_root;
    manager.findVars(id_result, vars_of_root);

    std::set<ClassProject::BDD_ID> compare_result_set;
    compare_result_set.insert(id_a);
    compare_result_set.insert(id_c);
    compare_result_set.insert(id_d);

    EXPECT_EQ(vars_of_root, compare_result_set);
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


    EXPECT_EQ(manager.topVar(result_id), id_a);
    EXPECT_EQ(manager.coFactorTrue(result_id), id_and_result);
    EXPECT_EQ(manager.coFactorFalse(result_id), manager.False());
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

    size_before = manager.uniqueTableSize();
    ClassProject::BDD_ID id_b_and_d = manager.and2(id_b, id_d);
    EXPECT_EQ(size_before, manager.uniqueTableSize());



    EXPECT_EQ(manager.getTopVarName(id_b_and_d), "b");
    EXPECT_EQ(manager.topVar(id_b_and_d), id_b);
    EXPECT_EQ(manager.coFactorTrue(id_b_and_d), id_d);
    EXPECT_EQ(manager.coFactorFalse(id_b_and_d), manager.False());
    EXPECT_EQ(manager.isVariable(id_b_and_d), false);





    EXPECT_EQ(manager.getTopVarName(result_id), "a");
    EXPECT_EQ(manager.topVar(result_id), id_a);
    EXPECT_EQ(manager.coFactorTrue(result_id), id_d);
    EXPECT_EQ(manager.coFactorFalse(result_id), id_b_and_d);
    EXPECT_EQ(manager.isVariable(result_id), false);



    ClassProject::BDD_ID result_id_x = manager.coFactorFalse(result_id, id_a);
    EXPECT_EQ(result_id_x, id_b_and_d);
    


    ClassProject::BDD_ID result_id_2 = manager.coFactorTrue(result_id, id_d);
    ClassProject::BDD_ID result_id_3 = manager.coFactorTrue(result_id_2, id_a);
    ClassProject::BDD_ID result_id_4 = manager.coFactorTrue(result_id_3, id_b);


    EXPECT_EQ(result_id_4, manager.True());
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
    EXPECT_EQ(size_before, manager.uniqueTableSize());
    EXPECT_EQ(result_id, manager.False());


    EXPECT_EQ(manager.coFactorFalse(id_d), manager.False());

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
    EXPECT_EQ(result_id, manager.False());


    size_t size_before = manager.uniqueTableSize();
    ClassProject::BDD_ID result_id_2 = manager.coFactorFalse(result, id_b);
    EXPECT_EQ(size_before + 1, manager.uniqueTableSize());


    EXPECT_EQ(manager.getTopVarName(result_id_2), "a");
    EXPECT_EQ(manager.topVar(result_id_2), id_a);
    EXPECT_EQ(manager.coFactorTrue(result_id_2), id_and_result);
    EXPECT_EQ(manager.coFactorFalse(result_id_2), manager.False());
    EXPECT_EQ(manager.isVariable(result_id_2), false);




    size_before = manager.uniqueTableSize();
    ClassProject::BDD_ID result_id_3 = manager.coFactorFalse(result, id_a);
    EXPECT_EQ(size_before, manager.uniqueTableSize());


    EXPECT_EQ(manager.getTopVarName(result_id_3), "b");
    EXPECT_EQ(manager.topVar(result_id_3), id_b);
    EXPECT_EQ(manager.coFactorTrue(result_id_3), id_and_result);
    EXPECT_EQ(manager.coFactorFalse(result_id_3), manager.False());
    EXPECT_EQ(manager.isVariable(result_id_3), false);
}


TEST(ManagerTest, CoFactorAndVariableTest) {
    ClassProject::ManagerImplementation manager;


    ClassProject::BDD_ID id_a = manager.createVar("a");
    ClassProject::BDD_ID id_b = manager.createVar("b");
    ClassProject::BDD_ID id_c = manager.createVar("c");
    ClassProject::BDD_ID id_d = manager.createVar("d");
    ClassProject::BDD_ID id_e = manager.createVar("e");
    ClassProject::BDD_ID id_f = manager.createVar("f");

    ClassProject::BDD_ID id_and_acdf = manager.and2(manager.and2(manager.and2(id_a, id_c), id_d), id_f);
    ClassProject::BDD_ID id_and_bcd =  manager.and2(manager.and2(id_b, id_c), id_d);
    ClassProject::BDD_ID final_result = manager.or2(id_and_acdf, id_and_bcd);


    std::set<ClassProject::BDD_ID> compare_result_set;
    compare_result_set.insert(id_a);
    compare_result_set.insert(id_c);
    compare_result_set.insert(id_d);
    compare_result_set.insert(id_b);
    compare_result_set.insert(id_f);

    std::set<ClassProject::BDD_ID> result_set;
    manager.findVars(final_result, result_set);
    EXPECT_EQ(result_set, compare_result_set);





    //test to check if function is no longer dependent on "c" after we used coFactorTrue(function, c)
    ClassProject::BDD_ID final_result2 = manager.coFactorTrue(final_result, id_c);
    std::set<ClassProject::BDD_ID> compare_result_set2;
    compare_result_set.insert(id_a);
    compare_result_set.insert(id_d);
    compare_result_set.insert(id_b);
    compare_result_set.insert(id_f);

    std::set<ClassProject::BDD_ID> result_set2;
    manager.findVars(final_result2, result_set);
    EXPECT_EQ(result_set, compare_result_set);


}





TEST(ManagerTest, DuplicateEntryTest) {
    ClassProject::ManagerImplementation manager;


    ClassProject::BDD_ID id_a = manager.createVar("a");
    ClassProject::BDD_ID id_b = manager.createVar("b");
    ClassProject::BDD_ID id_b2 = manager.createVar("b");
    ClassProject::BDD_ID id_one = manager.createVar("1");
    ClassProject::BDD_ID id_zero = manager.createVar("0");



    EXPECT_EQ(manager.False(), id_zero);
    EXPECT_EQ(manager.True(), id_one);
    EXPECT_EQ(id_b, id_b2);

    manager.and2(id_a, id_b);
    size_t s = manager.uniqueTableSize();


    manager.and2(id_a, id_b);
    manager.and2(id_b, id_a);


    //size should not increase after these operations

    EXPECT_EQ(s, manager.uniqueTableSize());

    manager.and2(0, 0);
    manager.and2(1, 0);
    manager.nand2(1, 1);
    manager.xor2(0, 1);



    EXPECT_EQ(s, manager.uniqueTableSize());

}






#endif //VDS_PROJECT_TESTS_H

