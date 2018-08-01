# Pi-Hole stats. on a Nextion display
project to explore using a nextion display for the first time

Connects to wifi, makes an http request to a pi-hole server and parses the data using ArduinoJson. Relevant parsed data is then formatted and sent to the Nextion display over a serial connection. 

added a status indicator to show if Pi-Hole service is currently enabled or disabled. 


![PiHoleNextion](https://raw.githubusercontent.com/arejaycodes/piholenextion/master/images/pihole-nextion-status-1.png "Front of Pi-Hole Nextion Display with status indicator added")
