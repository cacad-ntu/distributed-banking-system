#include "utils.h"

int utils::unmarshalInt(char *b)
{
    return *(b+0) << 24 | (*(b+1) & 0xFF) << 16 | (*(b+2) & 0xFF) << 8 | (*(b+3) & 0xFF);
}

float utils::unmarshalFloat(char *b)
{
    int x = unmarshalInt(b);
    float ret;
    memcpy(&ret,&x,4);
    return ret;
}

string utils::unmarshalString(char *b, int length)
{
    return string(b,length);
}

void utils::marshalInt(int x, char *b)
{
    char *cur = b;
    *cur = x>>24;
    cur++;
    *cur = x>>16;
    cur++;
    *cur = x>>8;
    cur++;
    *cur = x>>0;
}

void utils::marshalFloat(float x, char *b)
{
    int temp;
    memcpy(&temp,&x,sizeof(x));
    marshalInt(temp,b);
}

void utils::marshalString(string s, char *b)
{
    char *cur = b;
    for(int i=0;i<(int)s.size();i++)
        *cur = s[i], cur++;
}
