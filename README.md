# WOL_Arduino_Firebase
 Wake On Lan with Firebase and ESP32

This is a code to use your arduino to turn your pc on, using and Firebase database.
To use it just modify the script with the following information and you can already upload it to your ESP32 (I didn't have an ESP8266, so I don't test it, but should work).
The info you'll need is: Your WIFI name and password, your firebase database link (I'll not explain here how to create a firebase project), your firebase API key project and your MAC Address.
Just open the .ino and find the variables you need to change, the names are the same or similar.
Once uploaded to your ESP32, just access your database and change the activate string to 1, and enjoy it.
OBS: I use a string because I want to use AppInventor to access my database with my Phone, and the app don't let me use an int.
