
<h1 align="center">Direction finding using nRF52832</h1>



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

This repository is containg all the code and documention for our project for direction finding using an antenna patch array and a robot arm. We are implementing the program using C and Nordic Semiconductors SDK. The ```code ```-folder contains the source code, excluding the build as you will need to do this by yourself when testing the system. This program is applied to a system, which is described under prerequisites.

We have used the nRF52832 DK and are currently working on making the code more dynamic, so it can be used on other development kits, such as nRF52840. Most of the code regarding BLE are used from the example codes in the SDK, and we have focused on developing the algorithms used to maneuver the servo, reading the enocder and the searching algorithm itself.

You can find the printing files in the  ```Fusion 360 ```-folder. It contains all the files needed to print the parts used in this projects, but is also available to modify and adjust certain elements.

<p align="right">(<a href="#top">back to top</a>)</p>



### Code built With

* [Nordic Semiconductor nFR5 SDK](https://www.nordicsemi.com/Products/Development-software/nRF5-SDK)
* [Microsoft Visual Studio Code](https://code.visualstudio.com/)


<p align="right">(<a href="#top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started

Follow the instructions on the wiring diagrams and documentation to assemble the system. Read the following prerequisites and installation-guide to complete the setup before testing.

### Prerequisites

2x nRF52 Developer Kit <br>
USB-2B Cable <br>
Pigtail SMA-SWF <br>
IDE that supports C (VSCode) <br>

4xCoax with SMA connectors <br>
Evaluation board <br>
Hybrid 180 degrees coupler <br>

3xServo 270degrees <br>
3D-printed <br>



### Installation

2. Clone the repo using:
   ```sh
   $git clone https://github.com/anderszk/nRF52832_dev.git
   ```
3. Install SDK and Toolchain here:
   ```sh
    https://www.nordicsemi.com/Products/Development-software/nRF5-SDK/Download#infotabs
   ```
4. Connect the nRF52 Developer chip to your computer
5. Start building using the interface or ```sh
    CTRL+P -> build
   ```
  

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

Project Link: [https://github.com/github_username/repo_name](https://github.com/github_username/repo_name)

<p align="right">(<a href="#top">back to top</a>)</p>

