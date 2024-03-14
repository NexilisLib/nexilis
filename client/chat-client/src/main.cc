#include "nexilis_client.hh"
#include "program.hh"

int main(int argc, char** argv)
{
    NexilisClient nexilisClient;
    nexilisClient.start();

    Program program(argc, argv);

    while (true)
    {
        program.update();
    }
}
