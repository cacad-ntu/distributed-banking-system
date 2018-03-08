#include "SmartPtr.h"

explicit SmartPtr::SmartPtr(T *p = NULL) { ptr = p;}
SmartPtr::~SmartPtr() { delete(ptr); }
T & SmartPtr::operator * () { return *ptr; }
T * SmartPtr::operator -> () {return ptr; }
