#include "Admin.h"

/**
   Returns client's IP address
 */
struct sockaddr_in Admin::getAddress(){
    return address;
}

/**
   Returns length of client's IP address
 */
unsigned Admin::getLength(){
    return len;
}

/**
   Returns user elapsed time
 */
int Admin::getDuration(){
    auto finish = std::chrono::high_resolution_clock::now();
	double elapsed = ((std::chrono::duration<double>)(finish-start)).count();
    return (int)(elapsed * 1000);
}

/**
   Returns remaining time
 */
int Admin::getRemaining(){
    return interval - getDuration();
}

/**
   Returns whether user elapsed time is still within interval
 */
bool Admin::isAvailable(){
    return getDuration() < interval;
}

Admin::Admin(struct sockaddr_in _address, unsigned _len, chrono::time_point<std::chrono::high_resolution_clock> _start, int _interval){
    address = _address;
    len = _len;
    start = _start;
    interval = _interval;
}
