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
        explicit Argument(const std::string& name, const std::vector<std::shared_ptr<IValue>>& values = {});

        /// Move constructor.
        Argument(Argument&& other);

        /// Move assignment overload.
        Argument& operator=(Argument&& other);

        /// Deleted copy constructor.
        Argument(const Argument& other) = delete;

        /// Deleted copy assignment overload.
        Argument& operator=(const Argument& other) = delete;

        // Function to get the value
        template <typename T>
        T getValue(uint64_t index = 0) const
        {
            auto derived = std::dynamic_pointer_cast<Value<T>>(m_values[index]);
            if (derived)
            {
                return derived->value;
            }
            throw std::bad_cast();
        }

        const std::string& getName() const
        {
            return m_name;
        }

        uint64_t getValueCount() const
        {
            return m_values.size();
        }

    private:
        std::string m_name;
        std::vector<std::shared_ptr<IValue>> m_values;
    };

    /// Constructor.
    CmdLineOptions(int argc, char** argv);

    /// Move Constructor.
    CmdLineOptions(CmdLineOptions&& other);

    /// Move assignment operator.
    CmdLineOptions& operator=(CmdLineOptions&& other);

    /// Deleted copy constructor.
    CmdLineOptions(const CmdLineOptions& other) = delete;

    /// Deleted copy assignment overload.
    CmdLineOptions& operator=(const CmdLineOptions& other) = delete;

    /// Get the argument based on any passing name.
    const Argument* getArgument(const std::string& name) const;

private:
    std::vector<Argument> m_arguments;
};

}

#endif
