//
// Created by ludwig on 27.11.18.
//

#ifndef VDSPROJECT_IMGCOMP_H
#define VDSPROJECT_IMGCOMP_H


#include <map>
#include <iostream>
#include "ReachableInterface.h"

namespace ClassProject {

    /// class computes the reachable state set of a finite-state-machine(FSM)
    ///
    ///
    class Reachable : public ReachableInterface {
        public:
            using ReachableInterface::ReachableInterface; //get constructor of ReachableInterface!
            //Reachable() = delete;

            /**
             * Constructor creates stateSize state bits for the user
             * @param stateSize state size
             */
            //explicit 
            Reachable(unsigned int x);
            ~Reachable() = default;


            /// performs a "xnor" of two boolean expressions
            ///
            /// performs logic-xnor of two given boolean-expressions given through two BDD_ID's
            /// @return BDD_ID that represents the boolean-expression of the result
            BDD_ID xnor2(BDD_ID a, BDD_ID b);
            
            /// returns vector of state variables
            ///
            /// vector of BDD_ID's
            /// each BDD_ID represents a boolean state-variable
            const std::vector<BDD_ID> &getStates() const;
            

            /// set a transition function for the FSM
            ///
            /// the transition-function is given through a boolean function as a BDD_ID
            void setDelta(const std::vector<BDD_ID> &transitionFunctions);



            /// set intial state of FSM
            ///
            /// set intial state given as boolean-vector
            void setInitState(const std::vector<bool>& stateVector);

            BDD_ID compute_reachable_states();

            /// checks if given state is reachable in FSM
            ///
            /// @warning makes only sense to use after FSM has been set-up and compute_reachable_state has been used
            bool is_reachable(const std::vector<bool>& stateVector);
            
            //void init();

            unsigned int getStateSize() {return stateSize;}

            BDD_ID createInputVariable(const std::string &label);


        private:
            BDD_ID getInitalStateCharacteristic();


            std::vector<BDD_ID> states;
            std::vector<BDD_ID> nextStates;
            std::vector<BDD_ID> stateTransitions;
            std::vector<BDD_ID> transitionRelationBitwise;

            std::vector<BDD_ID> inputVariables; 

            BDD_ID transitionRelation;
            BDD_ID initialStateCharacteristicFunction;
            BDD_ID reachableStatesCharacteristicFunction;

    };

}
#endif //VDSPROJECT_IMGCOMP_H
