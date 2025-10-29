#ifndef NEXILIS_OBJECT_HH
#define NEXILIS_OBJECT_HH

#include <nexilis/logger/log.hh>
#include <nexilis/nx_class.hh>
#include <nexilis/nx_util.hh>

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
          m_dimensions(dim),
          m_mutex(std::make_unique<std::mutex>())
    {
    }

    /// Deleted copy constructor.
    Object(const Object&) = delete;

    /// Deleted copy assignment operator.
    Object& operator=(const Object&) = delete;

    /// Move constructor.
    Object(Object&& other)
        : m_id(std::move(other.m_id)),
          m_position(std::move(other.m_position)),
          m_dimensions(std::move(other.m_dimensions)),
          m_filepath(std::move(other.m_filepath)),
          m_mutex(std::move(other.m_mutex))
    {
    }

    /// Move assignment operator.
    Object& operator=(Object&& other) noexcept
    {
        if (this != &other)
        {
            m_id = std::move(other.m_id);
            m_position = std::move(other.m_position);
            m_dimensions = std::move(other.m_dimensions);
            m_filepath = std::move(other.m_filepath);
            m_mutex = std::move(other.m_mutex);
        }
        return *this;
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
        if (!m_mutex)
        {
            m_mutex = std::make_unique<std::mutex>();
        }
        std::lock_guard lock(*m_mutex);
        m_position = pos;
    }

    const VectorType& getDimensions() const
    {
        return m_dimensions;
    }

    void setDimensions(const VectorType& dim)
    {
        if (!m_mutex)
        {
            m_mutex = std::make_unique<std::mutex>();
        }
        std::lock_guard lock(*m_mutex);
        m_dimensions = dim;
    }

    void setFilepath(const std::string& filepath)
    {
        if (!m_mutex)
        {
            m_mutex = std::make_unique<std::mutex>();
        }
        std::lock_guard lock(*m_mutex);
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

    /// Object data.
    uint64_t m_id;
    VectorType m_position;
    VectorType m_dimensions;
    std::string m_filepath;

    /// Mutex for setting data.
    std::unique_ptr<std::mutex> m_mutex;
};

} // namespace nexilis

#endif
