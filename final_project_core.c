// WSN Final Project

/*
*
* The main idea is to implement a wsn that can detect if a person has fallen or not in  a given area.
* For this, we'll use the gyroscope and accelerator in the cookies nodes and an additional pulsometer wired to the cookie nodes.
* 
* Implementation details:
*   1. Cookie communication (we need a protocol to initialize the communication between server and clients.
*      We should try to use clients as hopping nodes but this might be counterproductive as too much info might be flowing through a node and it might overload it.)
*      6-7
*
*   2. Sensor signal detection (Done by thee clients, we'll use a process for each of the sensors and save the the data in a global 
*      variable as there's no main to use a pointer.)
*
*   3. Processing (Done buy another process, here we have to dedcide whether we send it to the serrver already processed or 
*      we process it on the server. Done in the main thread)
*
* I like the idea of presenting the efficiencies for each of the options and reducing the difficulty of 
* the implementations, but doing many of them to compre which is best. Talk with Gabriel about it.
*/