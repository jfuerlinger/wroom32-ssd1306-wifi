#include "SerialMonitorTracingService.h"

SerialMonitorTracingService::SerialMonitorTracingService(void (*callback)(char* message)) {
    SerialMonitorTracingService::callback = callback;
}

void SerialMonitorTracingService::Trace(char* message) {
    callback(message);
}