#ifndef NEXILIS_CLIENT_SERVER_DATA_HH
#define NEXILIS_CLIENT_SERVER_DATA_HH

#include <cstdint>
#include <string>

namespace nexilis::client
{

class ServerData
{
public:
    /// Default constructor.
    ServerData() = default;

    /// Move constructor.
    ServerData(ServerData&& other);

    /// Move assignment operator.
    ServerData& operator=(ServerData&& other);

    /// Copy constructor.
    ServerData(const ServerData& other);

    /// Copy assignment operator.
    ServerData& operator=(const ServerData& other);

    const std::string& getPassword() const
    {
        return m_password;
    }

    void setPassword(const std::string& password)
    {
        m_password = password;
    }

    /// af_inet UDP
    const std::string& getInetUDPServerAddress() const
    {
        return m_inetUDPServerAddress;
    }

    void setInetUDP(const std::string& serverAddress)
    {
        m_inetUDPServerAddress = serverAddress;
    }

    uint16_t getInetUDPServerPort() const
    {
        return m_inetUDPServerPort;
    }

    void setInetUDPServerPort(uint16_t port)
    {
        m_inetUDPServerPort = port;
    }

    /// af_inet TCP
    const std::string& getInetTCPServerAddress() const
    {
        return m_inetTCPServerAddress;
    }

    void setInetTCP(const std::string& serverAddress)
    {
        m_inetTCPServerAddress = serverAddress;
    }

    uint16_t getInetTCPServerPort() const
    {
        return m_inetTCPServerPort;
    }

    void setInetTCPServerPort(uint16_t port)
    {
        m_inetTCPServerPort = port;
    }

    /// boost TCP.
    const std::string& getBoostTCPServerAddress() const
    {
        return m_boostTCPServerAddress;
    }

    void setBoostTCPAddress(const std::string& serverAddress)
    {
        m_boostTCPServerAddress = serverAddress;
    }

    uint16_t getBoostTCPServerPortNumber() const
    {
        return m_boostTCPServerPort;
    }

    void setBoostTCPPortNumber(uint16_t port)
    {
        m_boostTCPServerPort = port;
    }

    /// boost UDP.
    const std::string& getBoostUDPServerAddress() const
    {
        return m_boostUDPServerAddress;
    }

    void setBoostUDP(const std::string& serverAddress)
    {
        m_boostUDPServerAddress = serverAddress;
    }

    uint16_t getBoostUDPServerPort() const
    {
        return m_boostUDPServerPort;
    }

    void setBoostUDPServerPort(uint16_t port)
    {
        m_boostUDPServerPort = port;
    }

    /// af_unix DGRAM
    const std::string& getUnixDgramServerPath() const
    {
        return m_unixDgramServerPath;
    }

    void setUnixDgramServerPath(const std::string& socketPath)
    {
        m_unixDgramServerPath = socketPath;
    }

    /// af_unix STREAM
    const std::string& getUnixStreamServerPath() const
    {
        return m_unixStreamServerPath;
    }

    void setUnixStreamServerPath(const std::string& socketPath)
    {
        m_unixStreamServerPath = socketPath;
    }

    void setProtocolPort(const std::string& protocol, uint16_t port)
    {
        if (protocol == "boost_tcp")
            m_boostTCPServerPort = port;
        else if (protocol == "boost_udp")
            m_boostUDPServerPort = port;
        else if (protocol == "inet_tcp")
            m_inetTCPServerPort = port;
        else if (protocol == "inet_udp")
            m_inetUDPServerPort = port;
    }

private:
    /// The server password.
    std::string m_password;

    /// af_inet UDP
    std::string m_inetUDPServerAddress;
    uint16_t m_inetUDPServerPort = 0;

    /// af_inet TCP
    std::string m_inetTCPServerAddress;
    uint16_t m_inetTCPServerPort = 0;

    /// boost TCP
    std::string m_boostTCPServerAddress;
    uint16_t m_boostTCPServerPort = 0xFF;

    /// boost UDP
    std::string m_boostUDPServerAddress;
    uint16_t m_boostUDPServerPort = 0;

    /// af_unix DGRAM
    std::string m_unixDgramServerPath;

    /// af_unix STREAM
    std::string m_unixStreamServerPath;
};

} // namespace nexilis::client

#endif
