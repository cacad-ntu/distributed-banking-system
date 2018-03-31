#include "Admin.h"

struct sockaddr_in Admin::getAddress(){
    return address;
}
unsigned Admin::getLength(){
    return len;
}

int Admin::getDuration(){
    auto finish = std::chrono::high_resolution_clock::now();
	double elapsed = ((std::chrono::duration<double>)(finish-start)).count();
    return (int)(elapsed * 1000);
}

int Admin::getRemaining(){
    return interval - getDuration();
}

bool Admin::isAvailable(){
    return getDuration() < interval;
}

Admin::Admin(struct sockaddr_in _address, unsigned _len, chrono::time_point<std::chrono::high_resolution_clock> _start, int _interval){
    address = _address;
    len = _len;
    start = _start;
    interval = _interval;
}
