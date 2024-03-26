#ifndef NEXILIS_CLIENT_HH
#define NEXILIS_CLIENT_HH

#include <boost/asio.hpp>

#include <cstdint>
#include <memory>
#include <nexilis/common/util.hh>
#include <nexilis/nexilis_macros.hh>

namespace nexilis
{

// This class acts as an abstraction for different clients.

class Client
{
public:
    using sendFunctionType = std::shared_ptr<std::function<void(std::vector<uint8_t>)>>;

    /// Constructor.
    Client(std::string ip_address) noexcept;

    /// Move constructor.
    Client(Client&& other);

    /// Move assignment operator.
    Client& operator=(Client&& other);

    /// Deleted copy constructor.
    Client(const Client& other) = delete;

    /// Deleted copy assignment operator.
    Client& operator=(const Client& other) = delete;

    /// Destructor.
    ~Client()
    {
    }

    /// Operator overload for comparison operator.
    bool operator==(const Client& other) const
    {
        return m_ip_address == other.getIPAddress();
    }

    /// Getter for the ip address.
    /// \return The ip address of the connection.
    std::string getIPAddress() const
    {
        return m_ip_address;
    }

    void setRootAccess(bool hasAccess)
    {
        m_hasRootAccess = hasAccess;
    }

    bool hasRootAccess() const
    {
        return m_hasRootAccess;
    }

    void setCommonAccess(bool hasAccess)
    {
        m_hasCommonAccess = hasAccess;
    }

    bool hasCommonAccess() const
    {
        return m_hasCommonAccess;
    }

    uint64_t getId() const
    {
        return m_id;
    }

    void setRoomId(uint64_t roomId)
    {
        m_roomId = roomId;
    }

    uint64_t getRoomId() const
    {
        return m_roomId;
    }

    void setUsername(const std::string& username)
    {
        m_username = username;
    }

    std::string getUsername() const
    {
        return m_username;
    }

    // Protocol specific stuff.
    void setBoostTCPSend(sendFunctionType sendFunction)
    {
        m_boostTCPSendToClient = sendFunction;
    }

    // Send data using boost TCP.
    bool boostTCPSend(std::vector<uint8_t> data);

private:
    // General
    std::string m_ip_address;
    std::string m_username;
    uint64_t m_id = Util::getRandomUint64();
    uint64_t m_roomId = 0;

private:
    sendFunctionType m_boostTCPSendToClient = nullptr;

private:
    /// Access area.
    bool m_hasRootAccess = false;
    bool m_hasCommonAccess = false;
};

} // namespace nexilis

#endif
