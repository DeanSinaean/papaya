#ifndef UTILS_H
#define UTILS_H
#include "valType.h"
void  memcp(char*dest,char*src,int bytes);
void printf(char*format,...);
int getchar(void);
boolean loadfile(char*path,char*addr);
#endif
