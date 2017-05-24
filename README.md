# IoT_Autonomo_TTN
Sending sensor data to The Things Network with Autonomo + LoRaBee

1. Install Arduino IDE
2. Add URL to Board Defenitions
3. Download Board Defenitions
4. Add LoRaBee Sketch Library

1. Install Arduino IDE
Download and install the Arduino IDE via: https://www.arduino.cc/en/Main/Software

2. Add URL to Board Defenitions
Copy this URL: http://downloads.sodaq.net/package_sodaq_samd_index.json
In the Arduino IDE GoTo: Preferences->Additional Board Manager URLs: and add it to this list.

3. Download Board Defenitions
In the Arduino IDE open the Board Manger via Tools->Board->Board Manger. You can then search for and install the 'SODAQ' boards by searching for SODAQ.

4. Add LoRaBee Sketch Library
Go to https://github.com/jpmeijers/RN2483-Arduino-Library and download this Repo as a .zip-file
In the Arduino IDE GoTo: Sketch => Include Library => Add .zip Library...
Search for and choose the just downloaded Repo 'RN2483-Arduino-Library-master.zip' 

5. Use code in this Repo
Now you are ready to use, enjoy, and compile my code...

Enjoy ;-)

These instructions are geathered from 
- https://www.arduino.cc/en/guide/macOSX
- http://support.sodaq.com/sodaq-one/autonom%D0%BE/getting-started-autonomo/
- https://www.arduino.cc/en/Guide/Libraries
