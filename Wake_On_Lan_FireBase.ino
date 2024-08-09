#include <WakeOnLan.h>
#include <FirebaseClient.h>

//Defining the correct libraries
#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W) || defined(ARDUINO_GIGA)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#elif __has_include(<WiFiNINA.h>) || defined(ARDUINO_NANO_RP2040_CONNECT)
#include <WiFiNINA.h>
#elif __has_include(<WiFi101.h>)
#include <WiFi101.h>
#elif __has_include(<WiFiS3.h>) || defined(ARDUINO_UNOWIFIR4)
#include <WiFiS3.h>
#elif __has_include(<WiFiC3.h>) || defined(ARDUINO_PORTENTA_C33)
#include <WiFiC3.h>
#elif __has_include(<WiFi.h>)
#include <WiFi.h>
#endif

//Defining the correct WiFi client
#if defined(ESP32) || defined(ESP8266) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
#include <WiFiClientSecure.h>
WiFiClientSecure ssl_client;
#elif defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_UNOWIFIR4) || defined(ARDUINO_GIGA) || defined(ARDUINO_PORTENTA_C33) || defined(ARDUINO_NANO_RP2040_CONNECT)
#include <WiFiSSLClient.h>
WiFiSSLClient ssl_client;
#endif

//Your Changes Here
#define ssid "YOUR NETWORK NAME"
#define password "YOUR PASSWORD"
#define firebHost "https://exemple.firebaseio.com/"
#define API_KEY "PDjeifPEKFOdt_PDjeifPEKFOdt-uYTRaxy_YOP"
#define USER_EMAIL "youremail@gmail.com"
#define USER_PASSWORD "yourpassword"

//Firebase stuff
using AsyncClient = AsyncClientClass;                       //Define the AsyncClient class as the one to be used
//Firebase Objects
FirebaseApp app;
AsyncResult aResult;
DefaultNetwork network;
RealtimeDatabase Database;
AsyncClient aClient(ssl_client, getNetwork(network));       //Create a client using the WiFiClientSecure object and the network object
UserAuth user_auth(API_KEY, USER_EMAIL, USER_PASSWORD);     //Create a user authentication object

//WOL
WiFiUDP UDP;
const char *MACAddress = "01:a2:34:5b:6c:7d";               //MAC Address of the device to be turned on
WakeOnLan WOL(UDP);                                         //Create a WakeOnLan object

//Usefull variables
unsigned long lastMillis = 0;                               //Variable to store the last time the task was executed
const int maxReconnectAttempts = 6;                         //Variable to store the maximum number of reconnection attempts

void setup() 
{
  //Serial Initialization
  Serial.begin(115200);
  Serial.print("Arduino Initialized");
  
  //WiFi Manager
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  //Resset WiFi
  WifiReset();

  //WiFi Feedback
  Serial.println("\nConnected to the WiFi network");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  //Firebase Setup
  Firebase.printf("Firebase Client v%s\n", FIREBASE_CLIENT_VERSION);  //Print the version of the Firebase library
  Serial.println("Initializing app...");


#if defined(ESP32) || defined(ESP8266) || defined(PICO_RP2040)        //Set the client to accept any SSL certificate (for test purposes only)
    ssl_client.setInsecure();
#if defined(ESP8266)
    ssl_client.setBufferSizes(4096, 1024);
#endif
#endif

  initializeApp(aClient, app, getAuth(user_auth), aResult);           //Initialize the Firebase app with the created user_auth object
  app.getApp<RealtimeDatabase>(Database);                             //Get the Realtime Database
  Database.url(firebHost);                                            //Set the database URL
}

void loop() 
{
  //Firebase process
  app.loop();                                                        //Process the incoming data and events and make the connection alive
  Database.loop();                                                   //Process writing and reading data from the database
  
  //Check each second
  if (millis() - lastMillis >= 1000)
  {
    lastMillis = millis();
    //Verify if the connection is alive
    if (WiFi.status() == WL_CONNECTED)
    {
      //User FeedBack
      Serial.print("Ready - ");

      //Get the value from the database and store it in aResult
      Database.get(aClient, "/YourLocalWOL/YourVar", aResult);
    }
    else 
    {
      //Reset WiFi
      WifiReset();
    }
  }

  if (aResult.available()) 
  {
    //aResult is the AsyncResult object. It is used to store the result of the get() function
    RealtimeDatabaseResult &databaseResult = aResult.to<RealtimeDatabaseResult>();

    //Store the value in aResult in a int variable
    int value = databaseResult.to<int>();

    //Send user FeedBack
    Serial.print("Value: ");
    Serial.println(value);

    if (value == 1) 
    {
      //Send WOL
      bool wake = WOL.sendMagicPacket(MACAddress, 7);
      if (wake)
      {
        //Set the database value to 0 and store it in aResult
        Database.set<int>(aClient, "/YourLocalWOL/YourVar", 0, aResult);

        //Send user FeedBack
        Serial.println("WOL Sent");
      }
      else 
      {
        //Set the database value to 0 and store it in aResult
        Database.set<int>(aClient, "/YourLocalWOL/YourVar", 0, aResult);

        //Send user FeedBack
        Serial.println("WOL Failed");
      }
      
    }
  }
  
}

//Unnecessary functions
void WifiReset()
{
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED) 
  {
    WiFi.disconnect();
    WiFi.begin(ssid, password);
    Serial.print("Connecting");
    Dots();
    if(attempts > maxReconnectAttempts) 
    {
      Restart();
    }
    attempts++;
  }
}

void Dots() 
{
  //Function to print dots while waiting
  delay(300);
  Serial.print(".");
  delay(300);
  Serial.print(".");
  delay(300);
  Serial.println(".");
}

void Restart() 
{
  //Function to restart the Arduino
  Serial.println("Restarting");
  delay(2000);        //Optional delay before restart
  ESP.restart();      //Restart the Arduino
}