#ifndef NEXILIS_CORE_HH
#define NEXILIS_CORE_HH

#include <functional>
#include <iostream>
#include <string>

namespace nexilis
{

class Core
{

public:

    enum class CoreFunction
    {
        none,
        print,
    };


public:
    Core() = default;

    void setPrint(std::function<void(std::string)> print)
    {
        m_print = print;
    }

    void print(const std::string& text)
    {
        m_print(text);
    }

    template<typename T>
    void callFunction(CoreFunction implFunc, T param)
    {
        switch(implFunc)
        {
            case CoreFunction::none: break;
            case CoreFunction::print:
            {
                if constexpr (std::is_same_v<T, std::string>)
                {
                    print(param);
                }
                else if constexpr (std::is_convertible_v<T, std::string>)
                {
                    print(static_cast<std::string>(param));
                }
                else
                {
                    std::cout << "ERROR: Core::callFunction template argument is not convertible to string" << std::endl;
                }
                break;
            }
        }
    }

private:
    std::function<void(const std::string&)> m_print = [](const std::string& text)
    {
        std::cout << text << std::endl;
    };
};

}

#endif
