#ifndef NEXILIS_NX_CLASS_HH
#define NEXILIS_NX_CLASS_HH

#include <string>

namespace nexilis
{

class NxClass
{
public:
    /// Constructor.
    /// \param name The classname of the user, derived class.
    /// \param file The source file of the class.
    NxClass(const std::string& name, const char* file);

    /// Move constructor.
    NxClass(NxClass&& other);

    /// Move assignment operator.
    NxClass& operator=(NxClass&& other);

    /// Deleted copy constructor.
    NxClass(const NxClass&) = delete;

    /// Deleted copy assignment operator.
    NxClass& operator=(const NxClass&) = delete;

    /// Get the associated name.
    const std::string& getLogName() const
    {
        return m_classname;
    }

    /// Get the classes name as header for log messages.
    const std::string& logHeader() const
    {
        return m_logHeader;
    }

    /// Get the file name.
    const std::string& getFile() const
    {
        return m_file;
    }

private:
    std::string m_classname;
    std::string m_file;
    std::string m_logHeader;
};

}

#endif
