#ifndef NEXILIS_NX_CLASS_HH
#define NEXILIS_NX_CLASS_HH

#include <string>

namespace nexilis
{

/// \note This class has nothing to do with "nx_data" or "nx_util".

class NxClass
{
public:
    /// Constructor.
    /// \param name The classname of the user class.
    explicit NxClass(const std::string& name);

    /// Move constructor.
    NxClass(NxClass&& other);

    /// Move assignment operator.
    NxClass& operator=(NxClass&& other);

    /// Deleted copy constructor.
    NxClass(const NxClass&) = delete;

    /// Deleted copy assignment operator.
    NxClass& operator=(const NxClass&) = delete;

    /// Get the associated name.
    const std::string& classname() const
    {
        return m_classname;
    }

    /// Get the classes name as header for log messages.
    const std::string& header() const
    {
        return m_logHeader;
    }

private:
    std::string m_classname;
    std::string m_logHeader;
};

} // namespace nexilis

#endif
