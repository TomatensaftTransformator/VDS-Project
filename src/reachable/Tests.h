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
    
    //comp.init();
    EXPECT_EQ(comp.getStateSize(), 10);

    std::vector<BDD_ID> states = comp.getStates();
    EXPECT_EQ(states.size(), 10);
    for (int i = 0 ; i < states.size(); i++) {
        ASSERT_TRUE(( states[i] != comp.True() ) && (states[i] != comp.False() ));
    }


    ClassProject::Reachable comp2(0);

    //comp2.init();
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
}




TEST(managerTest, xnorTest) {

    
    ClassProject::Reachable comp(10);
    
    //comp.init();

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
    
    //comp.init();

    BDD_ID id_a = comp.createVar("a");
    BDD_ID id_b = comp.createVar("b");


    BDD_ID xnor_ab = comp.xnor2(id_a, id_b);

    BDD_ID id_res = comp.or2(comp.and2(id_a, id_b), comp.and2(comp.neg(id_a), comp.neg(id_b)));

    EXPECT_EQ(xnor_ab, id_res);
}




TEST(managerTest, HowTo_Example) {
    ClassProject::Reachable comp(2);
    
    //comp.init();
    
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
    
    //comp.init();
    
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
    
    //comp.init();
    
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
    
    //comp.init();
    
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




    for(bool b0 : {false, true}){
        for(bool b1 : {false, true}){
            for(bool b2 : {false, true}){
                for(bool b3 : {false, true}){
                    if (b0 == true && b1 == false && b2 == false && b3 == false) continue;
                    if (b0 == true && b1 == true && b2 == false && b3 == false) continue;
                    if (b0 == true && b1 == true && b2 == true && b3 == false) continue;
                    if (b0 == true && b1 == true && b2 == true && b3 == true) continue;

                    ASSERT_FALSE(comp.is_reachable({b0, b1, b2, b3}));
                }
            }
        }
    }
}



TEST(managerTest, FSM4Test) {
    ClassProject::Reachable comp(3);
    
    //comp.init();
    
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
    
    //comp.init();
    
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

    ASSERT_TRUE(comp.is_reachable({true,false, false, true}));
    ASSERT_TRUE(comp.is_reachable({true, true, false, false}));
    ASSERT_TRUE(comp.is_reachable({false,true, true, false}));
    ASSERT_TRUE(comp.is_reachable({false,false, true, true}));


    ASSERT_FALSE(comp.is_reachable({false,false, false, false}));
    ASSERT_FALSE(comp.is_reachable({true, true, true, true}));
    ASSERT_FALSE(comp.is_reachable({false,true, false, false}));
    ASSERT_FALSE(comp.is_reachable({true,false, true, true}));



}





TEST(managerTest, FSM6Test) {
    ClassProject::Reachable comp(3);
        
    auto states = comp.getStates();
    std::vector<BDD_ID> functions;

    auto s0 = states.at(0);
    auto s1 = states.at(1);
    auto s2 = states.at(2);

    comp.setInitState({true,true, true});

    comp.compute_reachable_states();

    //FSM only init state is reachable
    ASSERT_TRUE(comp.is_reachable({true,true, true}));

    ASSERT_FALSE(comp.is_reachable({false, false, false}));
    ASSERT_FALSE(comp.is_reachable({false, false, true}));
    ASSERT_FALSE(comp.is_reachable({false,  true, false}));
    ASSERT_FALSE(comp.is_reachable({false,true, true}));
    ASSERT_FALSE(comp.is_reachable({true,false, false}));
    ASSERT_FALSE(comp.is_reachable({true,false, true}));
    ASSERT_FALSE(comp.is_reachable({true,true, false}));
    //ASSERT_FALSE(comp.is_reachable({true,true, true}));
}





TEST(managerTest, FSM7Test) {
    ClassProject::Reachable comp(3);
        
    auto states = comp.getStates();
    std::vector<BDD_ID> functions;

    auto s0 = states.at(0);
    auto s1 = states.at(1);
    auto s2 = states.at(2);

    comp.compute_reachable_states();


    ASSERT_FALSE(comp.is_reachable({false, false, false}));
    ASSERT_FALSE(comp.is_reachable({false, false, true}));
    ASSERT_FALSE(comp.is_reachable({false,  true, false}));
    ASSERT_FALSE(comp.is_reachable({false,true, true}));
    ASSERT_FALSE(comp.is_reachable({true,false, false}));
    ASSERT_FALSE(comp.is_reachable({true,false, true}));
    ASSERT_FALSE(comp.is_reachable({true,true, false}));
}




TEST(managerTest, FSM8Test) {
    ClassProject::Reachable comp(4);
        
    auto states = comp.getStates();
    std::vector<BDD_ID> functions;

    auto s0 = states.at(0);
    auto s1 = states.at(1);
    auto s2 = states.at(2);
    auto s3 = states.at(3);

    comp.setInitState({true,true, false, false});

    functions.push_back(comp.neg(comp.and2(s0, comp.and2(s1, comp.and2(s2, s3))))); //s0' = not ( s0 and s1 and s2 and s3)
    functions.push_back(comp.neg(comp.and2(s0, comp.and2(s1, comp.and2(s2, s3))))); //s1' = not ( s0 and s1 and s2 and s3)
    functions.push_back(comp.xnor2(s0, s1)); //s2' = (s1 = s2)
    functions.push_back(comp.xnor2(s2, s3)); //s3' = (s3 = s4)
    comp.setDelta(functions);
    comp.compute_reachable_states();

    ASSERT_TRUE(comp.is_reachable({false, false, true, true}));
    ASSERT_TRUE(comp.is_reachable({true,true, false, false}));
    ASSERT_TRUE(comp.is_reachable({true,true, true, true}));


    for(bool b0 : {false, true}){
        for(bool b1 : {false, true}){
            for(bool b2 : {false, true}){
                for(bool b3 : {false, true}){
                    if (b0 == true && b1 == true && b2 == true && b3 == true) continue;
                    if (b0 == false && b1 == false && b2 == true && b3 == true) continue;
                    if (b0 == true && b1 == true && b2 == false && b3 == false) continue;

                    ASSERT_FALSE(comp.is_reachable({b0, b1, b2, b3}));
                }
            }
        }
    }
}


TEST(managerTest, privateFunctionsTest) {
    ClassProject::Reachable comp(4);
        
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
