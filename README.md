# VDS-Project

C++ Package to build ROBDD representation of a given boolean-function.

Further, given a FSM = (InitialState, InputVariables, TransitionFunction) represented through boolean-functions over the state-variables, next-state-variables and input-variables can compute the reachable state-space of the FSM, represented symbolicaly through a boolean-function over the state-variables, such that a state is in the reachable state-space iff the state satisfies the boolean-function.


## Build
To build this project run:

 ```
$ git clone
$ cd VDS-Project
$ mkdir build
$ cd build
$ cmake ..
$ make
```
 
 
 ## run tests ROBDD manager
To run the tests for the ROBDD manager run after you have build the project:

 ```
$ VDS-Project/build/src/test/VDSProject_test
```
 
 
 
  
 ## run tests reachable state-space
To run the tests for computing the reachable state-space of a FSM run after you have build the project:

 ```
$ VDS-Project/build/reachable/VDSProject_reachable
```
