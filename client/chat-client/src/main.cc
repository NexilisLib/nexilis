#include "program.hh"

int main(int argc, char** argv)
{
    Program program(argc, argv);

    while (true)
    {
        program.update();
    }
}
