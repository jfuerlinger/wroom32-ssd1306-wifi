#ifndef ITracingService_h
#define ITracingService_h

class ITracingService {
    public:
        virtual void Trace(const char* message) = 0;
};

#endif