#include "Util.h"

#include <fstream>

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
            s += tmp + "\n";
        }
        
        return s;
    }
    else
    {
        printf("Failed to open shader: %s\n", filename.c_str());
        return std::string("");
    }
}
