#ifndef ADMIN
#define ADMIN
#include <netinet/in.h>
#include <chrono>
using namespace std;

struct Admin{
private:
    struct sockaddr_in address;
    unsigned len;
    chrono::time_point<std::chrono::high_resolution_clock> start;
    int interval;
public:
    struct sockaddr_in getAddress();
    unsigned getLength();
    
    int getDuration();
    int getRemaining();
    
    bool isAvailable();
    Admin(struct sockaddr_in _address, unsigned _len, chrono::time_point<std::chrono::high_resolution_clock> _start, int _interval);
};

#endif
