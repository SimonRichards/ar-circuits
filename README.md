#Notes on libraries

##ElecSim1
+ Does not work
+ Needs to be changed to using KCL, this will have the advantage of making ElecSim3 easier

##ElecSim2
+ Completed
+ Instead of using a 'Manager' class (Simulation.java), each object in the AR module can just own its own Gate object and any modification to the circuit can set a global flag signaling that all outputs need to be refresed.