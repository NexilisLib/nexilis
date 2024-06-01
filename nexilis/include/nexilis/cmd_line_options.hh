#ifndef NEXILIS_CMD_LINE_OPTIONS_HH
#define NEXILIS_CMD_LINE_OPTIONS_HH

#include <cstddef>
#include <cstdint>
#include <sstream>
#include <string>
#include <vector>
#include <memory>

namespace nexilis
{

class CmdLineOptions
{
public:
    // Base class for type erasure
    class IValue
    {
    public:
        virtual ~IValue() = default;
    };

    // Template class to hold the actual value
    template <typename T>
    class Value : public IValue
    {
    public:
        Value(const T& val) : value(val) {}
        T value;
    };

    class Argument
    {
    public:
        /// Constructor.
        /// \param names The aliases for command like "--argument".
        /// \param value The values for the argument.
        explicit Argument(const std::vector<std::string>& names, const std::vector<std::shared_ptr<IValue>>& values = {});

        template <typename T>
        void addValues(const std::vector<T>& values)
        {
            for (const auto& value : values)
            {
                m_values.emplace_back(std::make_shared<Value<T>>(value));
            }
        }

        // Function to get the value
        template <typename T>
        T getValue() const
        {
            auto derived = std::dynamic_pointer_cast<Value<T>>(m_values);
            if (derived)
            {
                return derived->value;
            }
            throw std::bad_cast();
        }

        // Function to get the argument names
        const std::vector<std::string>& getNames() const
        {
            return m_names;
        }

        uint64_t getNameCount() const
        {
            return m_names.size();
        }

        uint64_t getValueCount() const
        {
            return m_values.size();
        }

    private:
        std::vector<std::string> m_names;
        std::vector<std::shared_ptr<IValue>> m_values;
    };

    /// Constructor.
    CmdLineOptions(int argc, char** argv);

    template <typename T>
    void addArgument(const std::vector<std::string>& names, const std::vector<T>& value)
    {
        Argument arg(names);
        arg.addValues(value);
        m_arguments.emplace_back(arg);
    }

    const Argument* getArgument(const std::string& name) const;

private:
    std::vector<Argument> m_arguments;
};

}

#endif
