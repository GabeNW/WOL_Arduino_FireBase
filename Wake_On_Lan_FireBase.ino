#include <WiFi.h>
#include<Firebase_ESP_Client.h>
#include <WiFiUDP.h>
#include <WakeOnLan.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define ssid "YOUR NETWORK NAME"
#define password "YOUR PASSWORD"
#define firebHost "https://exemple.firebaseio.com/"
#define API_KEY "PDjeifPEKFOdt_PDjeifPEKFOdt-uYTRaxy_YOP"

activate = "0";

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOk = false;

//WOL
WiFiUDP UDP;
const char *MACAddress = "01:a2:34:5b:6c:7d";
WakeOnLan WOL(UDP);
void sendWOL();

void setup() {
  //Serial
  Serial.begin(115200);
  Serial.print("ESP32 Initialized");
  
  //WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  //Resset
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    trespontos();
    if(attempts > 5) {
      ESP.restart();
    }
    attempts++;
  }

  //+WiFi
  Serial.println("\nConnected to the WiFi network");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  //Firebase ESP Client
  config.api_key = API_KEY;
  config.database_url = firebHost;
  if(Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("signUp Ok");
    signupOk = true;
  }
  else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    ESP.restart();
  }
  else{
    //Saving sensor data from ESP32 to Firebase RTDB
    if (Firebase.ready() && signupOk && (millis() - sendDataPrevMillis > 2000 || sendDataPrevMillis == 0)) 
    {
      sendDataPrevMillis = millis();
      //Read and send
      SendData();
    }
  }
}

void SendData() {
  if(Firebase.RTDB.getString(&fbdo, "/WOL")) {
    //Serial.println("Successful Read from " + fbdo.dataPath() + ": " + activate + " (" + fbdo.dataType() + ")");    
    if(fbdo.dataType() == "string") {
      activate = fbdo.stringData();
      //Serial.println("Successful Read from " + fbdo.dataPath() + ": " + activate + " (" + fbdo.dataType() + ")");
      if (activate == "1"){        
        //Send WOL
        WOL.sendMagicPacket(MACAddress, 7);

        //Send User FeedBack
        Serial.println("Sent");

        //Reset value
        activate = "0";

        //Set new Data (Send new Values)
        if(Firebase.RTDB.setString(&fbdo, "/WOL", activate)) {
          //Serial.println(); Serial.print(activate);
          //Serial.println(" - successfully saved to: " + fbdo.dataPath());
        }
        else {Serial.println("Failed 1: " + fbdo.errorReason());}
      }
      else 
        Serial.println("Reading completed, no changes found");
    }
  }
  else {Serial.println("Failed 2: " + fbdo.errorReason());}
}

void trespontos() {
  delay(500);
  Serial.print(".");
  delay(500);
  Serial.print(".");
  delay(500);
  Serial.println(".");
}
