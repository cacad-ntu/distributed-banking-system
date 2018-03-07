#include "utils.h"

int utils::unmarshalInt(char *b)
{
  return *(b+0) << 24 | (*(b+1) & 0xFF) << 16 | (*(b+2) & 0xFF) << 8 | (*(b+3) & 0xFF);
}
