/* Copyright (C) 2026 Valtteri Viirret
   This file is part of the Nexilis Project.

   This file is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This file is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this file.  If not, see <https://gnu.org>. */

#ifndef NEXILIS_CMD_LINE_OPTIONS_HH
#define NEXILIS_CMD_LINE_OPTIONS_HH

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

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
        explicit Value(const T& val)
            : value(val)
        {
        }
        T value;
    };

    class Argument
    {
    public:
        /// Constructor.
        /// \param name The "identifier" or "name" of the command line argument.
        /// \param values All values given to the argument.
        explicit Argument(const std::string& name, const std::vector<std::shared_ptr<IValue>>& values = {});

        /// Move constructor.
        Argument(Argument&& other);

        /// Copy constructor.
        Argument(const Argument& other);

        /// Copy assignment overload.
        Argument& operator=(const Argument& other);

        /// Move assignment overload.
        Argument& operator=(Argument&& other);

        /// Function to get the value based on index.
        template <typename T>
        T getValue(uint64_t index = 0) const
        {
            auto derived = std::dynamic_pointer_cast<Value<T>>(m_values[index]);
            if (derived)
            {
                return derived->value;
            }
            else
            {
                return T();
            }
        }

        /// Get all the values based on template argument.
        template <typename T>
        std::vector<T> getValues() const
        {
            std::vector<T> result;
            for (const auto& val : m_values)
            {
                auto derived = std::dynamic_pointer_cast<Value<T>>(val);

                if (derived)
                {
                    result.emplace_back(derived->value);
                }
            }
            return result;
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

    /// Copy constructor.
    CmdLineOptions(const CmdLineOptions& other);

    /// Move Constructor.
    CmdLineOptions(CmdLineOptions&& other);

    /// Copy assignment overload.
    CmdLineOptions& operator=(const CmdLineOptions& other);

    /// Move assignment operator.
    CmdLineOptions& operator=(CmdLineOptions&& other);

    /// Get the argument based on any passing name.
    const Argument* getArgument(const std::string& name) const;

    /// Get value for specific argument.
    template <typename T>
    T getValue(const std::string& argumentName, T defaultValue)
    {
        auto arg = getArgument(argumentName);
        return arg ? arg->getValue<T>() : defaultValue;
    }

private:
    std::vector<Argument> m_arguments;
};

} // namespace nexilis

#endif
