//
// Created by ludwig on 27.11.18.
//

#include "Reachable.h"

namespace ClassProject {
       
    Reachable::Reachable(unsigned int x) : ReachableInterface(x){
    stateSize=x;
        for (int i = 0; i< stateSize; i++){
            BDD_ID var_id = createVar("s" + std::to_string(i));
            states.push_back(var_id);
        }


        for (int i = 0; i< stateSize; i++){
            BDD_ID var_id = createVar("s'" + std::to_string(i));
            nextStates.push_back(var_id);
        }


        for (int i = 0; i< stateSize; i++){
            stateTransitions.push_back(False()); //default stateTransition
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
        if(transitionFunctions.size() != stateSize){
            //give error message
            return;
        }
        //size is fine
        stateTransitions = transitionFunctions;
    }

    void Reachable::setInitState(const std::vector<bool>& stateVector){
        //initial state is set by the user
        if(stateVector.size() != stateSize){
            //give error message
            std::cout << "Size of InitialStateVector and states of FSM do not fit!" << std::endl;
            initialStateCharacteristicFunction = False();
            return;
        }

        //compute characteristic boolean-function for initial-state
        //characteristic-function(state) = 1 <-> state = initial-state
        BDD_ID tmp = True();
        for(int i = 0; i < stateSize; i++){
            tmp = and2(tmp, xnor2(stateVector[i], states[i]));
        }
        initialStateCharacteristicFunction = tmp;
    }

    BDD_ID Reachable::compute_reachable_states(){
        //output is BDD_ID of a boolean function that represents the set of reachable-states
        //compute transition-relation
        
        for (int i =0; i < stateSize; i++) {
            //BDD_ID x = or2(and2(nextStates[i], stateTransitions[i]), and2(neg(nextStates[i]), neg(stateTransitions[i])));
            BDD_ID x = xnor2(nextStates[i], stateTransitions[i]);
            transitionRelationBitwise.push_back(x);
        }

        BDD_ID tmp = True();
        for(int i = 0; i < stateSize; i++){
            tmp = and2(tmp, transitionRelationBitwise[i]);
        }
        transitionRelation = tmp;


        BDD_ID characteristicFunction;
        BDD_ID characteristicFunctionIteration = initialStateCharacteristicFunction;
        bool b = true;

        while(b){
            characteristicFunction = characteristicFunctionIteration;

            BDD_ID characteristicFunctionAndTransition = and2(characteristicFunction, transitionRelation);
            BDD_ID tmp = characteristicFunctionAndTransition;
            BDD_ID img;

            //existential quantifier: state_variable
            for (int i = 0 ; i < stateSize; i++){
                tmp = or2(coFactorTrue(tmp, states[i]), coFactorFalse(tmp, states[i]));
            }


            //existential quantifier: input_variables
            for (int i = 0 ; i < inputVariables.size(); i++){
                tmp = or2(coFactorTrue(tmp, inputVariables[i]), coFactorFalse(tmp, inputVariables[i]));
            }

            img = tmp; //img_function depens on next_state yet!
            
            tmp = True();
            //rename next_s to s in image_function
            for (int i = 0; i< stateSize; i++) {
                tmp = and2(tmp, xnor2(states[i], nextStates[i])); //AND(state[i] == nextState[i])
            }
            
            img = and2(img, tmp);
            
            
            //existential quantifier: nextStateVariables
            for (int i = 0 ; i <stateSize; i++){
                img = or2(coFactorTrue(img, nextStates[i]), coFactorFalse(img, nextStates[i]));
            }
            //now img function depens on the stateVariables!

            characteristicFunctionIteration = or2(characteristicFunction, img); //epxand the set of reachableStates
            b = (characteristicFunction != characteristicFunctionIteration); // loop ends when equality holds.
        }

        reachableStatesCharacteristicFunction = characteristicFunction;
        return characteristicFunction;
    }

    bool Reachable::is_reachable(const std::vector<bool>& stateVector){
        //check if the given state is in the set of reachable-states
        if(stateVector.size() != stateSize){
            //give error message
            return false;
        }
        BDD_ID tmp = reachableStatesCharacteristicFunction;
        for (int i = 0; i < stateSize; i++) {
            if (topVar(tmp) == states[i]) {
                if (stateVector[i]){
                    tmp = coFactorTrue(tmp);
                }else {
                    tmp = coFactorFalse(tmp);
                }
            }
        }
        if (True() == tmp) return true;
        if (False() != tmp) std::cout << "Error 'is_reachable' result should either be true or false" << std::endl;
        return false;
    }
            


    BDD_ID Reachable::getInitalStateCharacteristic(){
        return initialStateCharacteristicFunction;
    }



    BDD_ID Reachable::createInputVariable(const std::string &label){
        BDD_ID id = createVar(label);
        inputVariables.push_back(id);
        return id;
    }


}
