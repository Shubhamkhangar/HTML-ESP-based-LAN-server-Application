# HTML-ESP-based-LAN-server-Application

* The project implements a web server on ESP8266 microcontrollers, enabling control of LEDs and display of sensor data through a web interface.
* The system allows users to interact with the microcontroller remotely only within the personal area network by toggling LEDs on and off and retrieving sensor data via a user-friendly web page.
* The implementation includes AJAX XMLHttp communication for real-time updates and login credentials for secure access to the web interface. Additionally, the code features HTML pages stored in program memory, 
  minimizing resource usage.
* Any node within the intranet can access the webpage to control and monitor the systems connected to the MCU unit by visiting the URL and hitting the IP address of the node server in the web browser assigned by
  the host router.
* Every time a new user tries to access the web page from the server the user needs to authenticate himself by providing the login credentials while logging in with the server.
* once authentification is done the server renders the static HTML webpage to a specific user address.
* if the user wants to log out from the server he can log himself out by hitting the logout button on the webpage.
* user can control and monitor the sensor and actuator systems using the interactive webpage.



**Click on the below links to jump to specific folders where the code is stored**

[ESP12F Code link](ESP12F.c)

[User WebPage HTML Code link](UserWebPage.html)

[Logout WebPage HTML Code link](logout.html)


**Programming Language**
![Static Badge](https://img.shields.io/badge/Embedded_C-red)![Static Badge](https://img.shields.io/badge/HTML5-white)![Static Badge](https://img.shields.io/badge/CSS-green)![Static Badge](https://img.shields.io/badge/JavaScript-yellow)

**Hardware** 
![Static Badge](https://img.shields.io/badge/ESP-12F-black)![Static Badge](https://img.shields.io/badge/NodeMCU-white)

**Software**
![Static Badge](https://img.shields.io/badge/Arduino_IDE-green)![Static Badge](https://img.shields.io/badge/Visual_Studio_Code-blue)



