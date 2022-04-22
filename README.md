# PREXT

##Content
1. Introduction
2. Main Features
3. Installation 
4. Demo & Virtual Machine
5. Further Information
6. Citation  

## 1. Introduction
PREXT is a unified and extensible framework that simulate pseudonym change schemes (i.e., privacy schemes) in VANET. Although PREXT is mainly developed for VANET scenarios, but it can be used/adapted for simulations that let mobile nodes broadcast their spatio-temporal information periodically. The main assumption in PREXT is that nodes broadcast beacon messages every short time (e.g., 0.1-1s) containing its position, speed, time stamp and a changing pseudonym (i.e., temporary node identity). The general concept of privacy schemes in such scenarios is to change pseudonyms periodically followed by ceasing/encrypting beacon messages for a while to prevent linking consecutive messages originated from each vehicle. If the adversary is able to link messages over time, it can de-anonymize vehicle traces, re-identify drivers, and profile their trajectories which is a serious privacy risk. This privacy concept is different from preserving privacy in LBS, because VANET safety applications require precise and frequent location updates and cannot afford LBS privacy schemes such as k-anonymity, transformation, ...etc. 

## 2. Main Features
- A modular design of a privacy layer that facilitates implementation of new privacy schemes.
- Seven popular privacy schemes that are based on different approaches such as silent periods, context- based and cryptographic mix-zones. These built-in schemes allow non-privacy specialists to consider privacy aspects in their VANET simulations.
- A global passive adversary (GPA) that eavesdrops beacon messages of all vehicles and tries to reconstruct vehicle traces even with changing pseudonyms and silence periods. Vehicle tracker is based on the nearest neighbor probabilistic data association (NNPDA) algorithm which is able to track vehicles effectively and efficiently. The strength of the adversary is fully- controlled by determining its coverage of the road network.
- Several privacy metrics are supported such as traceability, entropy, anonymity set size and pseudonyms statistics.

## 3. Installation 
(These steps are tested on ubuntu 16.04. Please tell us if these steps are working with other systems as well.)
- Install omnet++-5.0, sumo-0.25.0, veins-4.4 as explained in http://veins.car2x.org/tutorial/
- Download "Eigen 3.3" library http://eigen.tuxfamily.org/  and uncompress it to, let's say, "~/Eigen"
- Download PREXT and move "Prext" folder to veins-4.4\src\veins\modules\
- Change veins configurations to include PREXT during its build. Open veins properties by right click on "veins" project in the omnet++ IDE and select "Properties". Then, change the following:

1. On the left tree, Expand "C/C++ General" node -> Select "Paths and Symbols":
  * On the right view, Select "Includes" tab"/**GNU C++** ->  add the following paths "~/Eigen", "omnetpp-5.0/include/omnetpp" and "/veins/src/veins/modules/Prext/include"

![](img/config1.png?raw=true)

  * On the same view, Select "Library Paths" tab -> add the following path "/veins/src/veins/modules/Prext/lib"

![](img/config2.png?raw=true)

2. On the left tree, Expand "OMNET++" -> Select Makemake: On the right view, Expand "veins: custom makefile" -> Select "src: makemake..." -> On the right, click on "Options..." button under **Makemake** radio button. On the pop-up dialog, Select "Link" tab -> press **More>>** -> add a library in the **first** list "Additional libraries to link with: (-l option)" by clicking on the small button just above the list -> 
  * In the pop-up dialog, write "NNPDA" (or "NNPDA_win" for windows installation), then click "OK"

![](img/config3.png?raw=true)

3. Press "OK" on all previous dialogs and Rebuild veins

## 4. Demo & Virtual Machine
We created an example scenario for PREXT (prextdemo folder) on a map of the city center of Munich, Germany. This map is obtained from OpenStreetMap and converted into a SUMO network using netconvert and polyconvert tools included in SUMO. We adopted the recommended options specified  in the tool wikipages. The randomTrips script is used to generate random vehicle trips with an arrival rate of one vehicle per second with total of 169 traces. Vehicles are successively arriving for 5 min then another 5 min are left to let vehicles exit the simulation (thus, the simulation time should not exceed 10 min in total). The median trace lifetime is 290 s, while the median trace distance is about  2 km. To  let beacons reach eavesdroppers without communication problems, we reduced the obstacle shadowing to 0.1 dB per cut and 0.0001 dB per meter. These shadowing values are not realistic and should be reconfigured in config.xml file in real-world scenarios. In this case, the full coverage mode of the adversary should NOT be used, because it does not consider the location of the eavesdropper nodes if they are inside buildings or in a free-space beside the road. This factor will significantly affect the eavesdropping capability of the adversary.
  
To run the demo, copy "prextdemo" folder to "/veins/examples/". In the omnet.ini configuration file, we have added a configuration section for each privacy scheme with example parameter settings. To test one of these schemes, you need to run the simulation with its corresponding configuration section. Don't forgot to run the "sumolaunch" script in background. At the end of the simulation, the tracking results are displayed in the simulation log showing various privacy metrics such as traceability and entropy. In addition, 

We compiled a virtual machine running ubuntu 16.04 in which OMNeT++, SUMO, Veins and PREXT installed and configured. You can download this machine here: https://tinyurl.com/ycyum5kx. To run this virtual machine, you need to install VirtualBox (https://www.virtualbox.org/wiki/Downloads). Also, you need to change the last line in the ~/.bash_rc file which starts with "export SUMO_HOME = ..." to "export SUMO_HOME=$HOME/src/sumo-0.25.0" without quotes

To run the simulation, open a command terminal (Ctrl+Alt+T) and type this command to run SUMO and press Enter.
~/src/veins-veins-4.4/sumo-launchd.py -vv -c ~/src/sumo-0.25.0/bin/sumo

In OMNeT++ IDE, select the omnet.ini file and press run in the toolbar.

## 5. Further Information
Please refer to the publication included in the doc folder for a detailed description of the PREXT extension.

## 6. Citation
If you use PREXT, we appreciate citing our paper:
Karim Emara, "Poster: PREXT: Privacy Extension for Veins VANET Simulator", IEEE Vehicular Networking Conference (VNC), Dec. 2016, Columbus, Ohio, USA (to appear)
