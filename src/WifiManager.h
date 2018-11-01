
#ifndef WifiManager_h
#define WifiManager_h

#include <WiFi.h>
#include "WiFiClient.h"
#include "ITracingService.h"

class WifiManager
{

  public:
    WifiManager(ITracingService *tracingService);

    void connect();
    void disconnect();
    WiFiClient getWifiClient();


  private:
    ITracingService *_tracingService;
    WifiClient _wifiClient;
};

#endif