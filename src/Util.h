#ifndef UTIL_H
#define UTIL_H

#include <string>

void print(const char* s);
void print(std::string s);
void print(int i);
void print(float f);
void print(bool b);
std::string readFile(std::string filename);

#endif
