//#include <stdio.h>
#include <string.h>
#include <WiFi.h>

#include "WifiManager.h"

#include "credentials.h"


WifiManager::WifiManager(ITracingService* tracingService) {
    WifiManager::tracingService = tracingService;
}


void WifiManager::connect()
{
    int count = 0;
    tracingService->Trace("Connecting to:");
    tracingService->Trace(mySSID);
    WiFi.begin(mySSID, myPW);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        tracingService->Trace(".");
        count++;
        if (count > 15)
            ESP.restart();
    }
    tracingService->Trace("");
    tracingService->Trace("WiFi connected");
    tracingService->Trace("IP address: ");
    tracingService->Trace(WiFi.localIP().toString().c_str());
}

void WifiManager::disconnect()
{
    WiFi.disconnect(true);
}