# WOL_Arduino_Firebase
This code is made to turn your pc on using your arduino (I used an ESP32) with the Wake on lan function and the Firebase database.

Explanation:
To use it just modify the script with your information and you can already upload it to your arduino (If you have some trouble, look into the "FirebaseClient" library documentation).
The info you need to change is:

"ssid": Your WiFi name.
"password": Self-explanatory.
"firebHost": Your firebase database link (I'll not explain here how to create a firebase project, you can find how searching on youtube or in the "FirebaseClient" library documentation).
"API_KEY": Your firebase API key project.
"USER_EMAIL": Your email attached to your firebase client (I'll also not explain here how to create an email to authenticate to your firebase project, this is another thing you can search on youtube or the oficial documentation).
"USER_PASSWORD": Self-explanatory.
"MACAddress": The Mac Address of your device.

Just open the .ino file, find the variables and change it.
Once uploaded to your arduino, just access your database and change your variable to "1", and the magic package is sent.

Notes:
Keep in mind you have to keep your PC connected to a power source and have it turned on at least once, otherwise the packet won't work.
If the package is not sent, try changing the WOL port number (7 or 9 should work).
Have fun.