#ifndef NEXILIS_MOVEMENT_MOVEMENT_HH
#define NEXILIS_MOVEMENT_MOVEMENT_HH

#include <nexilis/movement/movement_data.hh>
#include <nexilis/nexilis_constants.hh>
#include <nexilis/nx_data.hh>
#include <nexilis/types/vector2.hh>

#include <functional>

namespace nexilis
{

template <typename T>
class Movement
{
public:
    using MovementFunc = std::function<double(double, double)>;

    enum class Type
    {
        _2D,
        _3D
    };

    /// Constructor.
    explicit Movement(const MovementData& data)
        : m_data(data)
    {
    }

    virtual Type getType() const = 0;
    virtual T getAmount() const = 0;

    uint64_t getObjectId() const
    {
        return m_data.getObjectId();
    }

    float getDeltatime() const
    {
        return m_data.getDeltatime();
    }

    nx_data getMessageData() const
    {
        return m_data.getMessageData();
    }

    uint64_t getMessageId() const
    {
        return m_data.getMessageId();
    }

private:
    MovementData m_data;
};

} // namespace nexilis

#endif
