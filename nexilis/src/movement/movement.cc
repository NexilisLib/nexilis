#include <nexilis/movement/movement.hh>

namespace nexilis
{

Movement::Movement(const Movement::Data& data)
  : m_data(data)
{
}

Movement::Data::Data(uint64_t object_id, float delta_time, const nx_data& message_data, uint64_t message_id)
  : m_objectId(object_id),
    m_deltaTime(delta_time),
    m_messageData(message_data),
    m_messageId(message_id)
{
}

}
