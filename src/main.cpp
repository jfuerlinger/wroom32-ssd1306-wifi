#include <Wire.h>
#include "SSD1306.h"

#include <WiFi.h>

const char *ssid = "FuJo-Orbi";
const char *password = "#HUE7zPN";

SSD1306 display(0x3c, 5, 4);

String translateEncryptionType(wifi_auth_mode_t encryptionType)
{

  switch (encryptionType)
  {
  case (WIFI_AUTH_OPEN):
    return "Open";
  case (WIFI_AUTH_WEP):
    return "WEP";
  case (WIFI_AUTH_WPA_PSK):
    return "WPA_PSK";
  case (WIFI_AUTH_WPA2_PSK):
    return "WPA2_PSK";
  case (WIFI_AUTH_WPA_WPA2_PSK):
    return "WPA_WPA2_PSK";
  case (WIFI_AUTH_WPA2_ENTERPRISE):
    return "WPA2_ENTERPRISE";
  }
}

void scanNetworks()
{

  int numberOfNetworks = WiFi.scanNetworks();

  Serial.print("Number of networks found: ");
  Serial.println(numberOfNetworks);

  for (int i = 0; i < numberOfNetworks; i++)
  {

    Serial.print("Network name: ");
    Serial.println(WiFi.SSID(i));

    Serial.print("Signal strength: ");
    Serial.println(WiFi.RSSI(i));

    Serial.print("MAC address: ");
    Serial.println(WiFi.BSSIDstr(i));

    Serial.print("Encryption type: ");
    String encryptionTypeDescription = translateEncryptionType(WiFi.encryptionType(i));
    Serial.println(encryptionTypeDescription);
    Serial.println("-----------------------");
  }
}

void connectToNetwork()
{
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Establishing connection to WiFi..");
  }

  Serial.println("Connected to network");
}

void setup()
{
  Serial.begin(9600);

  display.init();
  display.flipScreenVertically();

  scanNetworks();
  connectToNetwork();

  display.drawString(0, 0, "Connection Details:");

  Serial.println(WiFi.macAddress());
  display.drawString(0, 10, WiFi.macAddress());

  Serial.println(WiFi.localIP());
  display.drawString(0, 20, WiFi.localIP().toString());

  WiFi.disconnect(true);
  Serial.println(WiFi.localIP());

  display.display();
}

void loop()
{
}