#include "GLEngine.h"

int main(int argc, char** argv)
{
    GLEngine* ge = new GLEngine(argc, argv);
    return ge->begin();
}
