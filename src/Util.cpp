#include "Util.h"

#include <iostream>
#include <fstream>

void print(const char* s)
{
	std::cout << s << std::endl;
}

void print(std::string s)
{
    std::cout << s << std::endl;
}

void print(int i)
{
	std::cout << i << std::endl;
}

void print(float f)
{
	std::cout << f << std::endl;
}

void print(bool b)
{
	if(b) std::cout << "true" << std::endl;
	else std::cout << "false" << std::endl;
}

std::string readFile(const std::string& filename)
{
    std::ifstream file(filename.c_str());
    std::string s;
    std::string tmp;
    
    if(file.is_open())
    {
        while(file.good())
        {
            std::getline(file, tmp);
            s += tmp;
        }
        
        return s;
    }
    else
    {
        print("Failed to open shader:");
        print(filename);
        return std::string("");
    }
}
