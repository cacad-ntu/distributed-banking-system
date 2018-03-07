#ifndef UTILS
#define UTILS
#include<string>
#include<cstring>
using namespace std;
namespace utils{
    int unmarshalInt(char *b);
    string unmarshalString(char *b, int length);
    float unmarshalFloat(char *b);

    void marshalInt(int x, char *b);
    void marshalFloat(float x, char *b);
    void marshalString(string s, char *b);
}

#endif
