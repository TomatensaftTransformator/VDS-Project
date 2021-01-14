//
// Created by ludwig on 27.11.18.
//

#include "Reachable.h"

namespace ClassProject {

    void Reachable::init(){
        std::cout << getStateSize() << std::endl;
        for (int i = 0; i< getStateSize(); i++){
            BDD_ID var_id = createVar("s" + std::to_string(i));
            states.push_back(var_id);
        }

        std::cout << states.size() << std::endl;

        for (int i = 0; i< getStateSize(); i++){
            BDD_ID var_id = createVar("s'" + std::to_string(i));
            nextStates.push_back(var_id);
        }



    }




    BDD_ID Reachable::xnor2(BDD_ID a, BDD_ID b){
        return neg(xor2(a, b));
    }

    const std::vector<BDD_ID>& Reachable::getStates() const{
        return states;
    }


    void Reachable::setDelta(const std::vector<BDD_ID> &transitionFunctions){
        //transition-function delta is given by the user
        //one logic function for each bit
        if(transitionFunctions.size() != getStateSize()){
            //give error message
            return;
        }
        //size is fine
        stateTransitions = transitionFunctions;
    }

    void Reachable::setInitState(const std::vector<bool>& stateVector){
        //initial state is set by the user
        if(stateVector.size() != getStateSize()){
            //give error message
            return;
        }
        initialState = stateVector;

        //compute characteristic boolean-function for initial-state
        //characteristic-function(state) = 1 <-> state = initial-state
        BDD_ID tmp = xnor2(initialState[0], states[0]);
        for(int i = 1; i < getStateSize(); i++){
            tmp = and2(tmp, xnor2(initialState[i], states[i]));
        }
        initialStateCharacteristicFunction = tmp;
    }

    BDD_ID Reachable::compute_reachable_states(){
        //output is BDD_ID of a boolean function that represents the set of reachable-states

        //compute transition-relation
        for (int i =0; i < getStateSize(); i++) {
            BDD_ID x = or2(and2(nextStates[i], stateTransitions[i]), and2(neg(nextStates[i]), neg(stateTransitions[i])));
            transitionRelationBitwise.push_back(x);
        }

        BDD_ID tmp = transitionRelationBitwise[0];
        for(int i = 1; i < getStateSize(); i++){
            tmp = and2(tmp, transitionRelationBitwise[i]);
        }
        transitionRelation = tmp;


        //now only loop is missing!!!!
        //then call it a day!



        return 0;
    }

    bool Reachable::is_reachable(const std::vector<bool>& stateVector){
        //check if the given state is in the set of reachable-states
        return true;
    }
            

}