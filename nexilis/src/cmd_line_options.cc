#include <memory>
#include <nexilis/cmd_line_options.hh>

#include <algorithm>

namespace nexilis
{

CmdLineOptions::Argument::Argument(const std::vector<std::string>& names, const std::vector<std::shared_ptr<IValue>>& values)
    : m_names(names),
      m_values(values)
{
}

CmdLineOptions::CmdLineOptions(int argc, char** argv)
{
    // Basic command line parsing
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg[0] == '-')
        {
            // This is a command line option
            std::vector<std::string> names = { arg };
            std::vector<std::shared_ptr<IValue>> values;

            // Check if the next argument is a value
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                std::string val = argv[++i];
                values.push_back(std::make_shared<Value<std::string>>(val)); // Store the value as a string
            }
            m_arguments.emplace_back(names, values);
        }
    }
}

const CmdLineOptions::Argument* CmdLineOptions::getArgument(const std::string& name) const
{
    for (const auto& arg : m_arguments)
    {
        if (std::find(arg.getNames().begin(), arg.getNames().end(), name) != arg.getNames().end()) {
            return &arg;
        }
    }
    return nullptr;
}

}
