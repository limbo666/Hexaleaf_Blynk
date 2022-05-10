#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "Nanohex.h"

BlynkTimer timerUpdate; // Creating a blynk timer

/* SETUP */
// Remember to setup in Nanohex.h as well. 

const char* ssid = "Your_SSID"; // The SSID (name) of the Wi-Fi network you want to connect to
const char* pass = "Your_Password"; // The password of the Wi-Fi network
const char* auth = "BlynkAuthenticationKey"; //The Blynk Authorization Token

Nanohex *hexController;

void setup() {
  Serial.begin(9600);
  Serial.println("> Setup.");
  hexController = new Nanohex();
  Blynk.begin(auth, ssid, pass);
   timerUpdate.setInterval(10L, updateX); //Setup the timer to run every 10 millisec 
}

void updateX() // This function is called by the blynk timer
{
 hexController->update();
}

/* Get the main mode of operation */
BLYNK_WRITE(V1)
{
  uint8_t pinValue = param.asInt(); // Mode
  Serial.printf("Received mode %d \n", pinValue);
  hexController->set_mode(pinValue);
}

/* Primary color */
BLYNK_WRITE(V2)  
{
    int r = param[0].asInt();
    int b = param[1].asInt();
    int g = param[2].asInt();
    hexController->set_primary(CRGB(r,g,b));
    Serial.printf("Primary: (%d, %d, %d) \n", r, g, b);
}

/* Secondary color */
BLYNK_WRITE(V3)  
{
    int r = param[0].asInt();
    int b = param[1].asInt();
    int g = param[2].asInt();
    hexController->set_secondary(CRGB(r,g,b));
    Serial.printf("Secondary: (%d, %d, %d) \n", r, g, b);
}

/* Get the min animation time */
BLYNK_WRITE(V4)
{
  uint16_t pinValue = param.asInt();
  hexController->set_fadetime_min(pinValue * 10);
}

/* Get the animation time */
BLYNK_WRITE(V5)
{
  uint16_t pinValue = param.asInt();
  hexController->set_fadetime_max(pinValue * 10);
}

/* Hue randomness */
BLYNK_WRITE(V6)
{
  uint8_t pinValue = param.asInt(); 
  hexController->set_hue_randomness(pinValue);
}

long ctr = 0;
void loop() {
    Blynk.run();
 
  //  hexController->update(); 
  // Removed from original code because cuse the device to stuck 
  // and get disconnected from network. 
  // We use a Blynk timer instead as it described on
  // Blynk manual https://docs.blynk.io/en/legacy-platform/legacy-articles/keep-your-void-loop-clean
  //
      timerUpdate.run();   // Fire the timer to update hexController

}
