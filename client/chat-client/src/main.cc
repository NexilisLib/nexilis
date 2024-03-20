#include "program.hh"

#include <nexilis/logger/function_handler.hh>

int main(int argc, char** argv)
{
    /*
    auto ncursesDebug = [](nexilis::logger::LogLevel logLevel, const std::string& data)
    {
        // End ncurses temporarily to print to the console.
        endwin();

        // Print to the standard console.
        nexilis::Util::sendColorMessageToConsole(logLevel, data);

        // Re-initialize ncurses.
        initscr();

        // Refresh the window to display changes.
        refresh();
    };

    nexilis::logger::FunctionHandler functionHandler(ncursesDebug);
    nexilis::Log::addHandler(std::move(functionHandler));
    nexilis::Log::setMinimumLevel(nexilis::logger::LogLevel::INFO);
    */

    Program program(argc, argv);

    while (true)
    {
        program.update();
    }
}
