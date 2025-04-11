#ifndef NEXILIS_MOVEMENT_MOVEMENT_HH
#define NEXILIS_MOVEMENT_MOVEMENT_HH

#include <nexilis/nexilis_constants.hh>
#include <nexilis/nx_data.hh>
#include <nexilis/types/vector2.hh>

namespace nexilis
{

class Movement
{
public:
    class Data
    {
    public:
        /// Constructor.
        explicit Data(uint64_t m_objectId, float m_deltaTime, const nx_data& m_messageData, uint64_t m_messageId);

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

    /// Constructor.
    explicit Movement(const Data& data);

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
    Data m_data;
};

} // namespace nexilis

#endif
