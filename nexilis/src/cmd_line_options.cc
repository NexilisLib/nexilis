#include <nexilis/cmd_line_options.hh>
#include <nexilis/log.hh>

#include <algorithm>

namespace nexilis
{

// Helper function to check if a string can be parsed as a specific type
template <typename T>
bool tryParse(const std::string& str, T& result)
{
    std::istringstream iss(str);
    // `noskipws` considers whitespace an error
    iss >> std::noskipws >> result;
    return iss.eof() && !iss.fail();
}

CmdLineOptions::Argument::Argument(const std::string& name, const std::vector<std::shared_ptr<IValue>>& values)
    : m_name(name),
      m_values(values)
{
}

CmdLineOptions::Argument::Argument(CmdLineOptions::Argument&& other)
    : m_name(std::move(other.m_name)),
      m_values(std::move(other.m_values))
{
}

CmdLineOptions::Argument& CmdLineOptions::Argument::operator=(Argument&& other)
{
    if (this != &other)
    {
        m_name = std::move(other.m_name);
        m_values = std::move(other.m_values);
    }
    return *this;
}

CmdLineOptions::CmdLineOptions(CmdLineOptions&& other) :
    m_arguments(std::move(other.m_arguments))
{
}

CmdLineOptions& CmdLineOptions::operator=(CmdLineOptions&& other)
{
    if (this != &other)
    {
        m_arguments = std::move(other.m_arguments);
    }
    return *this;
}

CmdLineOptions::CmdLineOptions(int argc, char** argv)
{
    // Basic command line parsing
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg[0] == '-')
        {
            std::string name = arg;
            std::vector<std::shared_ptr<IValue>> values;

            // Collect all following non-option arguments as values
            while (i + 1 < argc && argv[i + 1][0] != '-')
            {
                std::string val = argv[++i];

                // Try to parse as different types
                if (int intValue; tryParse(val, intValue))
                {
                    values.emplace_back(std::make_shared<Value<int>>(intValue));
                }
                else if (double doubleValue; tryParse(val, doubleValue))
                {
                    values.emplace_back(std::make_shared<Value<double>>(doubleValue));
                }
                else if (std::string stringValue; tryParse(val, stringValue))
                {
                    values.emplace_back(std::make_shared<Value<std::string>>(val));
                }
                else
                {
                    Log::error("Undefined value as command line option!");
                }
            }
            m_arguments.emplace_back(name, values);
        }
    }
}

const CmdLineOptions::Argument* CmdLineOptions::getArgument(const std::string& name) const
{
    for (const auto& arg : m_arguments)
    {
        if (arg.getName() == name)
        {
            return &arg;
        }
    }
    return nullptr;
}

}
