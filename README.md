#Overview
This project aims to provide an interactive simulation of simple electrical circuits using 
augmented reality with a tangible interface and animated 3d graphics.

##Working Notes
###Marker tracking
Dan?

###Graphics
No work completed yet, wires paths will need to be calculated between two points and their
models generated.

###ElecSim1 - Simple DC Analysis
+ Does not work
+ Needs to be changed to using KCL, this will have the advantage of making ElecSim3 easier

###ElecSim2 - Digital logic simulator
+ Completed
+ Instead of using a 'Manager' class (Simulation.java), each object in the AR module can 
+ just own its own Gate object and any modification to the circuit can set a global flag 
+ signaling that all outputs need to be refresed.
 
###ElecSim3 - LCR Analysis
+ Not yet started
