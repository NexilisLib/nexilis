#ifndef NEXILIS_OBJECT_HH
#define NEXILIS_OBJECT_HH

#include <string>

template <typename VectorType>
class Object
{
public:
    /// Constructor.
    Object(const VectorType& pos, const VectorType& dim)
        : position(pos), dimensions(dim)
    {
    }

    /// Virtual destructor.
    virtual ~Object() = default;

    const VectorType& getPosition() const
    {
        return position;
    }

    void setPosition(const VectorType& pos)
    {
        position = pos;
    }

    const VectorType& getDimensions() const
    {
        return dimensions;
    }

    void setDimensions(const VectorType& dim)
    {
        dimensions = dim;
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
    VectorType position;
    VectorType dimensions;
    std::string m_filepath;
};

#endif