#include "error.h"
#include <iostream>

/*
#define STRING(x) _STRING(x)
#define _STRING(x) #x
#define POS __FILE__ ": " STRING(__LINE__)
*/

using std::cout;
using std::endl;

void error(const char *e, const char *pos)
{
      cout << e << ": " << pos << endl;
      exit(-1);
}

/*
int main()
{
      error("hello", POS);
}
*/