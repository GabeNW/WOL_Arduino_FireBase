# WOL_Arduino_Firebase
Wake On Lan with Firebase and ESP32

Explanation:
This code is to turn your pc on using an arduino and the Firebase database.
To use it just modify the script with the following information and you can already upload it to your ESP32 (I didn't have an ESP8266, so I didn't test it, but it should work).
The info you'll need is:

ssid: Your WiFi name.
password: Self-explanatory.
firebHost: Your firebase database link (I'll not explain here how to create a firebase project).
API_KEY: Your firebase API key project.
MACAddress: Self-explanatory.

Just open the .ino file, find the variables and change it.
Once uploaded to your ESP32, just access your database and change the "activate" (or whatever name you use it) variable to "1", and the magic package is sent.

Notes:
Keep in mind you have to keep your PC connected to a power source and have it turned on at least once, otherwise the magic packet won't work.