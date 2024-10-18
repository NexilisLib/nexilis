#ifndef NEXILIS_OBJECT_HH
#define NEXILIS_OBJECT_HH

#include <nexilis/util.hh>

namespace nexilis
{

template <typename VectorType>
class Object
{
public:
    /// Constructor.
    Object(const VectorType& pos, const VectorType& dim)
        : m_position(pos), m_dimensions(dim)
    {
    }

    /// Virtual destructor.
    virtual ~Object() = default;

    uint64_t getId() const
    {
        return m_id;
    }

    const VectorType& getPosition() const
    {
        return m_position;
    }

    void setPosition(const VectorType& pos)
    {
        m_position = pos;
    }

    const VectorType& getDimensions() const
    {
        return m_dimensions;
    }

    void setDimensions(const VectorType& dim)
    {
        m_dimensions = dim;
    }

    void setFilepath(const std::string& filepath)
    {
        m_filepath = filepath;
    }

    std::string getFilepath() const
    {
        return m_filepath;
    }

protected:
    uint64_t m_id = Util::getRandomUint64();
    VectorType m_position;
    VectorType m_dimensions;
    std::string m_filepath;
};

}

#endif