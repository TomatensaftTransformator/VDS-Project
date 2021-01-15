//
// Created by ludwig on 27.11.18.
//

#ifndef VDSPROJECT_IMGCOMP_H
#define VDSPROJECT_IMGCOMP_H


#include <map>
#include <iostream>
#include "ReachableInterface.h"

namespace ClassProject {

    class Reachable : public ReachableInterface {
        public:
            using ReachableInterface::ReachableInterface; //get constructor of ReachableInterface!

            BDD_ID xnor2(BDD_ID a, BDD_ID b);
            
            const std::vector<BDD_ID> &getStates() const;
            
            void setDelta(const std::vector<BDD_ID> &transitionFunctions);

            void setInitState(const std::vector<bool>& stateVector);

            BDD_ID compute_reachable_states();

            bool is_reachable(const std::vector<bool>& stateVector);
            
            void init();

            BDD_ID getInitalStateCharacteristic();

        private:
            std::vector<BDD_ID> states;
            std::vector<BDD_ID> nextStates;
            std::vector<BDD_ID> stateTransitions;
            std::vector<BDD_ID> transitionRelationBitwise;

            BDD_ID transitionRelation;
            BDD_ID initialStateCharacteristicFunction;
            BDD_ID reachableStatesCharacteristicFunction;

    };

}
#endif //VDSPROJECT_IMGCOMP_H
