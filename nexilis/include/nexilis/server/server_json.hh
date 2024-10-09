#ifndef NEXILIS_SERVER_JSON_HH
#define NEXILIS_SERVER_JSON_HH

#include <nexilis/server/room.hh>
#include <nexilis/server/user.hh>

#include <boost/json/object.hpp>

#include <map>
#include <vector>

namespace nexilis
{
/// Boost abstraction for serverside clients and rooms.
class ServerJson
{
public:
    /// Default constructor.
    ServerJson() = default;

    /// Get data about the rooms in the server.
    static boost::json::object getRoomData();

    /// Get data about the clients in the server.
    static boost::json::object getClientData();

    /// Return all data from the server.
    static boost::json::object getServerData();

private:
    // Get json data from room vector.
    static boost::json::array roomsToJSON(const std::vector<Room>& rooms);

    /// Get json data from client vector.
    static boost::json::array clientsToJSON(const std::vector<User>& clients);
};

} // namespace nexilis

#endif
