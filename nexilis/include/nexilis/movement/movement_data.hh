#ifndef NEXILIS_MOVEMENT_DATA_HH
#define NEXILIS_MOVEMENT_DATA_HH

#include <nexilis/nx_data.hh>

namespace nexilis
{

class MovementData
{
public:
    /// Constructor.
    explicit MovementData(uint64_t object_id, float delta_time, const nx_data& message_data, uint64_t message_id);

    uint64_t getObjectId() const
    {
        return m_objectId;
    }

    float getDeltatime() const
    {
        return m_deltaTime;
    }

    nx_data getMessageData() const
    {
        return m_messageData;
    }

    uint64_t getMessageId() const
    {
        return m_messageId;
    }

private:
    uint64_t m_objectId;
    float m_deltaTime;
    nx_data m_messageData;
    uint64_t m_messageId;
};

} // namespace nexilis

#endif
