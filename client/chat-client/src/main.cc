#include "debug.hh"
#include "program.hh"

#include <nexilis/logger/file_handler.hh>
#include <nexilis/cmd_line_options.hh>

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

    auto playerName = opts.getArgument("--name");

    std::stringstream data;
    data << "Argument name: " << playerName->getName() << ", player name: " << playerName->getValue<std::string>() << " int: " << playerName->getValue<int>(1);
    debug(data.str());

    Program program(std::move(opts));
    program.start();

    while (true)
    {
        program.update();
    }

    return 0;
}
