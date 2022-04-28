
<br>
<h1 align="center" style="padding-bottom: 200px;">Direction finding using nRF52832</h1>
<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>

  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

This repository is containg all the code and documention for our project for direction finding using an 1x2 antenna patch array mounted on a robotic arm with three DOF. We are implementing the program using C and Nordic Semiconductor's SDK. The ```code```-folder contains the source code, excluding the build as you will need to do this by yourself when testing the system. This program is applied to our specific system, which is described under prerequisites.

We have used the nRF52832 DK and are currently working on making the code more dynamic, so it can be used on other development kits, such as nRF52840. Most of the code regarding BLE are used from the example codes in the SDK, and we have focused on developing the algorithms used to maneuver the servo, reading the enocder and the searching algorithm itself.

You can find the .3mf files for our self made robotic arm parts on [Printables.com](https://www.printables.com/social/218149-haavardok/about). There you can find all the files needed to print the parts used in this project. We used a Prusa Mini for 3D-printing where most of the parts were printed with PLA on 0.10 mm layer height and ~20 % infill. Some parts as the axle from the first servo to the next were printed with 100 % infill for structural integrity reasons.

<p align="right">(<a href="#top">back to top</a>)</p>



### Code built With

* [Nordic Semiconductor nFR5 SDK](https://www.nordicsemi.com/Products/Development-software/nRF5-SDK)
* [Microsoft Visual Studio Code](https://code.visualstudio.com/)


<p align="right">(<a href="#top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started

Follow the instructions on the wiring diagrams and documentation to assemble the system. Read the following prerequisites and installation-guide to complete the setup before testing.

### Prerequisites

NOTE: We used Digi-Key Electronics as distributor for all our parts in the parts list, so you may find variations in price and product names if you choose to use another distributor.

## Parts
| Part                   | Product details     | Producer                             | Quantity | Unit price (NOK) | Extended price (NOK) |
| :--------------------- | :------------------ | :----------------------------------- | :------- |:---------------- | :------------------- |
| Antenna patch          | ANT-2.4-CPA         | Linx Technoologies Inc.              | 2        | 34,12            | 68,24                |
| Breadboard             | 920-0031-01         | Smartboard Inc.                      | 1        | 71,39            | 71,39                |
| Coaxial cable 152,4 mm | BU-4150029006       | Mueller Eletric Co.                  | 2        | 89,53            | 179,06               |
| Coaxial cable 457,2 mm | 415-0029-018        | Cinch Connectivity Solutions Johnson | 2        | 133,53           | 267,06               |
| Encoder cable          | CUI-435-1FT         | CUI Devices                          | 2        | 52,24            | 108,48               |
| Evaluation board       | EK42442-01          | pSemi                                | 1        | 942,90           | 942,90               |
| Incremental encoder    | AMT102-2048-N4000-S | CUI Devices                          | 2        | 207,46           | 414,92               |
| Jumper cables (I/O)    | 1528-1964-ND        | Adafruit Industries LLC              | 1        | 17,51            | 17,51                | 
| Laser pointer          | 1528-1391-ND        | Adafruit Industries LLC              | 1        | 53,43            | 53,43                |
| Micro servo            | SER0049             | DFRobot                              | 1        | 44,90            | 44,90                |
| Multiprotocol SoC      | nNRF52-DK           | Nordic Semiconductor                 | 2        | 349,05           | 698,10               |
| Pigtail SMA-SWF        | MXHS83QE3000        | Murata Electronics                   | 1        | 275,48           | 275,58               |
| PSU                    | GST40A07-P1J        | Mean Well USA Inc.                   | 1        | 182,76           | 182,76               |
| SMA-connector          | 60312202114514      | Würth                                | 2        | 64,84            | 129,68               |
| Standard servo         | SER0038             | DFRobot                              | 2        | 133,36           | 266,72               |
| Tactile button         | CKN12302-ND         | C&K                                  | 2        | 0,90             | 1,80                 |
| Total                  |                     |                                      |          |                  | 3723,33              |


## Software
| Software     | Software details     | Version          | Produser             |
| :----------- | :------------------- | :--------------- | :------------------- |
| IDE          | Visual Studio Code   | 1.66 or later    | Microsoft            |
| SDK          | nRF Connect SDK      | v.1.9.1 or later | Nordic Semiconductor |



### Installation

2. Clone the repo using:
   ```
   $git clone https://github.com/anderszk/nRF52832_dev.git
   ```
3. Install SDK and Toolchain here:
   ```
   https://www.nordicsemi.com/Products/Development-software/nRF5-SDK/Download#infotabs
   ```
4. Connect the nRF52 Development kit to your computer
5. Start building using the interface or ```CTRL+P -> build```
  

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- USAGE EXAMPLES -->
## Usage

The usage of this application relates primarly to direction findings using stationary points. This code is meant to be flexible, meaning that you are able to add more degrees of freedom to the robot arm and expand to a 2x2 array to use simultaneously searching. 

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE.txt` for more information.

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- CONTACT -->
## Contact

contact person? - 


<p align="right">(<a href="#top">back to top</a>)</p>

