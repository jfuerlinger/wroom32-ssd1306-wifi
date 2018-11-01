#include "ITracingService.h"

class SerialMonitorTracingService : public ITracingService {

    public:
        SerialMonitorTracingService(void (*callback)(char *));
        
        void Trace(char *message);

    private:
        void (*callback)(char *);
};