#ifndef M_ERROR_H
#define M_ERROR_H

#define STRING(x) _STRING(x)
#define _STRING(x) #x
#define POS __FILE__ ": " STRING(__LINE__)

void error(const char *e, const char *pos);

#endif