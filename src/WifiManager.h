#include "ITracingService.h"

class WifiManager {

    public:
        
        WifiManager(ITracingService* tracingService);
        
        void connect();
        void disconnect();

    private:
        ITracingService* tracingService;

};