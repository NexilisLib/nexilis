#include "debug.hh"
#include "program.hh"

#include <nexilis/cmd_line_options.hh>
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

    nexilis::CmdLineOptions opts(argc, argv);

    Program program(opts);
    program.start();

    while (true)
    {
        program.update();
    }

    return 0;
}
