//
// Created by tobias on 29.11.18.
//

#ifndef VDSPROJECT_TESTS_H
#define VDSPROJECT_TESTS_H

#include "Reachable.h"
#include "gtest/gtest.h"
using namespace ClassProject;


//TODO: More tests of complete FSM!!!!!!
//      add functionality such that the FSM can depend over input-variables x as well!!!!
//      more tests for "weird" input

TEST(managerTest, initTest) {

    
    ClassProject::Reachable comp(10);
    
    comp.init();
    EXPECT_EQ(comp.getStateSize(), 10);

    std::vector<BDD_ID> states = comp.getStates();
    EXPECT_EQ(states.size(), 10);
    for (int i = 0 ; i < states.size(); i++) {
        ASSERT_TRUE(( states[i] != comp.True() ) && (states[i] != comp.False() ));
    }


    ClassProject::Reachable comp2(0);

    comp2.init();
    EXPECT_EQ(comp2.getStateSize(), 0);


    std::vector<BDD_ID> states2 = comp2.getStates();
    EXPECT_EQ(states2.size(), 0);
    for (int i = 0 ; i < states2.size(); i++) {
        ASSERT_TRUE(( states2[i] != comp2.True() ) && (states2[i] != comp2.False() ));
    }
    //actually not nice at all!!!!!
    //int i = -10;
    //ClassProject::Reachable comp3(i);
    //comp3.init();
    //EXPECT_EQ(comp3.getStateSize(), i);
}




TEST(managerTest, xnorTest) {

    
    ClassProject::Reachable comp(10);
    
    comp.init();

    EXPECT_EQ(comp.xnor2(comp.True(), comp.True()), comp.True());
    EXPECT_EQ(comp.xnor2(comp.True(), comp.False()), comp.False());
    EXPECT_EQ(comp.xnor2(comp.False(), comp.True()), comp.False());
    EXPECT_EQ(comp.xnor2(comp.False(), comp.False()), comp.True());

    BDD_ID id_a = comp.createVar("a");
    BDD_ID id_b = comp.createVar("b");

    EXPECT_EQ(comp.xnor2(id_a, comp.False()), comp.neg(id_a));
    EXPECT_EQ(comp.xnor2(id_b, comp.True()), id_b);

    BDD_ID xnor_ab = comp.xnor2(id_a, id_b);
    
    EXPECT_EQ(comp.neg(comp.xnor2(id_a, id_b)), comp.xor2(id_b, id_a));

    EXPECT_EQ(comp.coFactorTrue(comp.coFactorTrue(xnor_ab, id_a), id_b), comp.True());
    EXPECT_EQ(comp.coFactorFalse(comp.coFactorFalse(xnor_ab, id_a), id_b), comp.True());
    EXPECT_EQ(comp.coFactorFalse(comp.coFactorTrue(xnor_ab, id_a), id_b), comp.False());
    EXPECT_EQ(comp.coFactorTrue(comp.coFactorFalse(xnor_ab, id_a), id_b), comp.False());

    EXPECT_EQ(comp.topVar(xnor_ab), id_a);
    EXPECT_EQ(comp.topVar(comp.coFactorTrue(xnor_ab)), id_b);
    EXPECT_EQ(comp.topVar(comp.coFactorFalse(xnor_ab)), id_b);

}


TEST(managerTest, xnor2Test) {

    
    ClassProject::Reachable comp(10);
    
    comp.init();

    BDD_ID id_a = comp.createVar("a");
    BDD_ID id_b = comp.createVar("b");


    BDD_ID xnor_ab = comp.xnor2(id_a, id_b);

    BDD_ID id_res = comp.or2(comp.and2(id_a, id_b), comp.and2(comp.neg(id_a), comp.neg(id_b)));

    EXPECT_EQ(xnor_ab, id_res);
}




/*

TEST(managerTest, initialStateTest) {

    
    ClassProject::Reachable comp(3);
    
    comp.init();

    auto states = comp.getStates();
    std::vector<BDD_ID> functions;

    auto s0 = states.at(0);
    auto s1 = states.at(1);
    auto s2 = states.at(2);

    comp.setInitState({false, false, false});

    BDD_ID characteristicFunction = comp.getInitalStateCharacteristic();

    EXPECT_EQ(comp.coFactorFalse(comp.coFactorFalse(comp.coFactorFalse(characteristicFunction))), comp.True());
    EXPECT_EQ(comp.coFactorFalse(comp.coFactorTrue(comp.coFactorFalse(characteristicFunction))), comp.False());
    EXPECT_EQ(comp.coFactorFalse(comp.coFactorFalse(comp.coFactorTrue(characteristicFunction))), comp.False());
    EXPECT_EQ(comp.coFactorTrue(comp.coFactorFalse(comp.coFactorFalse(characteristicFunction))), comp.False());
    EXPECT_EQ(comp.coFactorTrue(comp.coFactorTrue(comp.coFactorTrue(characteristicFunction))), comp.False());

    EXPECT_EQ(comp.topVar(characteristicFunction), s0);
    EXPECT_EQ(comp.topVar(comp.coFactorFalse(characteristicFunction)), s1);
    EXPECT_EQ(comp.topVar(comp.coFactorFalse(comp.coFactorFalse(characteristicFunction))), s2);

}



TEST(managerTest, initialState2Test) {

    
    ClassProject::Reachable comp(5);
    
    comp.init();

    auto states = comp.getStates();
    std::vector<BDD_ID> functions;

    auto s0 = states.at(0);
    auto s1 = states.at(1);
    auto s2 = states.at(2);

    comp.setInitState({true, false, false, true, false});

    BDD_ID characteristicFunction = comp.getInitalStateCharacteristic();

    EXPECT_EQ(comp.coFactorFalse(comp.coFactorTrue(comp.coFactorFalse(comp.coFactorFalse(comp.coFactorTrue(characteristicFunction))))), comp.True());
    EXPECT_EQ(comp.coFactorFalse(characteristicFunction), comp.False());
    EXPECT_EQ(comp.coFactorTrue(comp.coFactorTrue(characteristicFunction)), comp.False());
    EXPECT_EQ(comp.coFactorTrue(comp.coFactorFalse(comp.coFactorTrue(characteristicFunction))), comp.False());



    EXPECT_EQ(comp.topVar(characteristicFunction), s0);


}
*/

/*
TEST(managerTest, initialState3Test) {

    
    ClassProject::Reachable comp(5);
    
    comp.init();

    auto states = comp.getStates();
    std::vector<BDD_ID> functions;

    auto s0 = states.at(0);
    auto s1 = states.at(1);
    auto s2 = states.at(2);

    comp.setInitState({true, false, false, true});

    BDD_ID characteristicFunction = comp.getInitalStateCharacteristic();


    EXPECT_EQ(characteristicFunction, comp.False());


}
*/

TEST(managerTest, HowTo_Example) {
    ClassProject::Reachable comp(2);
    
    comp.init();
    
    auto states = comp.getStates();
    std::vector<BDD_ID> functions;

    auto s0 = states.at(0);
    auto s1 = states.at(1);
    //s0' = not(s0)
    functions.push_back(comp.neg(s0));
    //s1' = not(s1)
    functions.push_back(comp.neg(s1));
    //Add transition functions
    comp.setDelta(functions);
    //Add init state
    comp.setInitState({false,false});

    comp.compute_reachable_states();

    ASSERT_TRUE(comp.is_reachable({true,true}));
    ASSERT_TRUE(comp.is_reachable({false,false}));
    ASSERT_FALSE(comp.is_reachable({true,false}));
    ASSERT_FALSE(comp.is_reachable({false,true}));
}



TEST(managerTest, FSM1Test) {
    ClassProject::Reachable comp(3);
    
    comp.init();
    
    auto states = comp.getStates();
    std::vector<BDD_ID> functions;

    auto s0 = states.at(0);
    auto s1 = states.at(1);
    auto s2 = states.at(2);
    //s0' = not(s0)
    functions.push_back(comp.True());//s0'
    //s1' = not(s1)
    functions.push_back(comp.False()); //s1'
    functions.push_back(comp.True()); //s2'
    //Add transition functions
    comp.setDelta(functions);
    //Add init state
    comp.setInitState({true,false, true});

    comp.compute_reachable_states();

    //FSM has only self-loop on initial-state
    ASSERT_TRUE(comp.is_reachable({true,false, true}));
    ASSERT_FALSE(comp.is_reachable({true,true, true}));
    ASSERT_FALSE(comp.is_reachable({false,false, false}));
    ASSERT_FALSE(comp.is_reachable({true,false, false}));
    ASSERT_FALSE(comp.is_reachable({false,false, true}));
    ASSERT_FALSE(comp.is_reachable({false,false, false}));
}




TEST(managerTest, FSM2Test) {
    ClassProject::Reachable comp(3);
    
    comp.init();
    
    auto states = comp.getStates();
    std::vector<BDD_ID> functions;

    auto s0 = states.at(0);
    auto s1 = states.at(1);
    auto s2 = states.at(2);


    functions.push_back(comp.True());//s0'
    functions.push_back(comp.False()); //s1'
    functions.push_back(comp.True()); //s2'
    //Add transition functions
    comp.setDelta(functions);
    //Add init state
    comp.setInitState({true,true, true});

    comp.compute_reachable_states();

    //FSM has only transition to (101); init-state=(1,1,1)
    ASSERT_TRUE(comp.is_reachable({true,false, true}));
    ASSERT_TRUE(comp.is_reachable({true,true, true}));
    ASSERT_FALSE(comp.is_reachable({false,false, false}));
    ASSERT_FALSE(comp.is_reachable({true,false, false}));
    ASSERT_FALSE(comp.is_reachable({false,false, true}));
    ASSERT_FALSE(comp.is_reachable({false,false, false}));
}





TEST(managerTest, FSM3Test) {
    ClassProject::Reachable comp(4);
    
    comp.init();
    
    auto states = comp.getStates();
    std::vector<BDD_ID> functions;

    auto s0 = states.at(0);
    auto s1 = states.at(1);
    auto s2 = states.at(2);
    auto s3 = states.at(3);


    functions.push_back(s0); //s0'
    functions.push_back(comp.or2(s1, s0)); //s1'
    functions.push_back(comp.or2(s1, s2)); //s2'
    functions.push_back(comp.or2(s2, s3)); //s3'

    //Add transition functions
    comp.setDelta(functions);
    //Add init state
    comp.setInitState({true,false, false, false});

    comp.compute_reachable_states();

    //FSM has only transition to (101); init-state=(1,1,1)
    ASSERT_TRUE(comp.is_reachable({true,false, false, false}));
    ASSERT_TRUE(comp.is_reachable({true,true, false, false}));
    ASSERT_TRUE(comp.is_reachable({true,true, true, false}));
    ASSERT_TRUE(comp.is_reachable({true,true, true, true}));


    ASSERT_FALSE(comp.is_reachable({false, false, false, false}));
    ASSERT_FALSE(comp.is_reachable({true, false, true, false}));
    ASSERT_FALSE(comp.is_reachable({false, true, false, false}));
    ASSERT_FALSE(comp.is_reachable({false, false, true, false}));
    ASSERT_FALSE(comp.is_reachable({false, false, false, true}));
    ASSERT_FALSE(comp.is_reachable({false, true, true, false}));
    ASSERT_FALSE(comp.is_reachable({false, true, true, true}));

}



TEST(managerTest, FSM4Test) {
    ClassProject::Reachable comp(3);
    
    comp.init();
    
    auto states = comp.getStates();
    std::vector<BDD_ID> functions;

    auto s0 = states.at(0);
    auto s1 = states.at(1);
    auto s2 = states.at(2);


    auto x1 = comp.createInputVariable("x1");
    auto x2 = comp.createInputVariable("x2");
    auto x3 = comp.createInputVariable("x3");

    functions.push_back(x1); //s0' = x1
    functions.push_back(x2); //s1' = x2 
    functions.push_back(x3); //s2' = x3

    //Add transition functions
    comp.setDelta(functions);
    //Add init state
    comp.setInitState({true,false, false});

    comp.compute_reachable_states();

    //FSM has only transition to (101); init-state=(1,1,1)
    ASSERT_TRUE(comp.is_reachable({true,true, true}));
    ASSERT_TRUE(comp.is_reachable({false,true, true}));
    ASSERT_TRUE(comp.is_reachable({true,false, true}));
    ASSERT_TRUE(comp.is_reachable({false,false, true}));
    ASSERT_TRUE(comp.is_reachable({true,true, false}));
    ASSERT_TRUE(comp.is_reachable({false,true, false}));
    ASSERT_TRUE(comp.is_reachable({true,false, false}));
    ASSERT_TRUE(comp.is_reachable({false,false, false}));


}




TEST(managerTest, FSM5Test) {
    ClassProject::Reachable comp(4);
    
    comp.init();
    
    auto states = comp.getStates();
    std::vector<BDD_ID> functions;

    auto s0 = states.at(0);
    auto s1 = states.at(1);
    auto s2 = states.at(2);
    auto s3 = states.at(3);


    functions.push_back(s3); //s0' = x1
    functions.push_back(s0); //s1' = x2 
    functions.push_back(s1); //s2' = x3
    functions.push_back(s2); //s2' = x3


    //Add transition functions
    comp.setDelta(functions);
    //Add init state
    comp.setInitState({true,false, false, true});

    comp.compute_reachable_states();

    //FSM has only transition to (101); init-state=(1,1,1)
    ASSERT_TRUE(comp.is_reachable({true,false, false, true}));
    ASSERT_TRUE(comp.is_reachable({true, true, false, false}));
    ASSERT_TRUE(comp.is_reachable({false,true, true, false}));
    ASSERT_TRUE(comp.is_reachable({false,false, true, true}));


    ASSERT_FALSE(comp.is_reachable({false,false, false, false}));
    ASSERT_FALSE(comp.is_reachable({true, true, true, true}));
    ASSERT_FALSE(comp.is_reachable({false,true, false, false}));
    ASSERT_FALSE(comp.is_reachable({true,false, true, true}));



}



TEST(managerTest, privateFunctionsTest) {
    ClassProject::Reachable comp(4);
    
    comp.init();
    
    auto states = comp.getStates();
    EXPECT_EQ(comp.getStates().size(), 4);

    states.push_back(10);
    states.push_back(10);
    states.push_back(10);
    states.push_back(10);
    states.push_back(10);


    EXPECT_EQ(comp.getStates().size(), 4);

}


#endif //VDSPROJECT_TESTS_H
