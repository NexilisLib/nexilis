#ifndef NEXILIS_OBJECT_HH
#define NEXILIS_OBJECT_HH

#include <nexilis/nx_emplace.hh>
#include <nexilis/util.hh>

namespace nexilis
{

template <typename VectorType>
class Object
{
public:
    /// Constructor.
    Object(uint64_t id, const VectorType& pos, const VectorType& dim)
        : m_id(id),
          m_position(pos),
          m_dimensions(dim)
    {
    }

    /// Virtual destructor.
    virtual ~Object() = default;

    virtual nx_data getData() = 0;

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

    const std::string& getFilepath() const
    {
        return m_filepath;
    }

protected:
    nx_data baseData()
    {
        nx_data startingData;
        nx_emplace(startingData, m_id, m_position, m_dimensions, m_filepath);
        return startingData;
    }

    uint64_t m_id;
    VectorType m_position;
    VectorType m_dimensions;
    std::string m_filepath;
};

} // namespace nexilis

#endif
