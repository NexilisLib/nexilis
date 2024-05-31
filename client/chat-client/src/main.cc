#include "program.hh"

#include <nexilis/logger/file_handler.hh>

int main(int argc, char** argv)
{
#define NEXILIS_DEBUG
#ifdef NEXILIS_DEBUG
    std::string date = nexilis::Util::getDateAndTime();
    std::stringstream ss;
    ss << "../../../logs/" << date << "nexilis_data.txt";
    nexilis::Log::addHandler(nexilis::logger::FileHandler(ss.str()));
    nexilis::Log::setMinimumLevel(nexilis::logger::LogLevel::DEBUG);
#endif

    Program program(argc, argv);
    program.start();

    while (true)
    {
        program.update();
    }

    return 0;
}
