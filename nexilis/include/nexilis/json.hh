#ifndef NEXILIS_JSON_HH
#define NEXILIS_JSON_HH

#include <boost/json/kind.hpp>
#include <nexilis/user.hh>
#include <nexilis/room.hh>

#include <boost/json.hpp>
#include <boost/json/object.hpp>

#include <map>
#include <vector>

namespace nexilis
{
/// Some simple boost abstraction functions.
/// Don't wanna abstract it too much because boost is perfectly fine library,
/// however these things should only be written once.
class Json
{
public:
    /// Default constructor.
    Json() = default;

    // Create JSON data from given key-value pairs
    static ::boost::json::object createJSON(const std::map<std::string, ::boost::json::value>& keyValues);

    /// Read JSON data from file.
    static ::boost::json::value readJSONFromFile(const std::string& filename);

    /// Get data about the rooms in the server.
    static ::boost::json::object getRoomData();

    /// Get the room data with added message data parameters.
    static ::boost::json::object getRoomDataMessage();

    /// Get data about the clients in the server.
    static ::boost::json::object getClientData();

    /// Get the client data with added message data parameters.
    static ::boost::json::object getClientDataMessage();

    /// Return all data from the server.
    static ::boost::json::object getServerData();

    /// Get the nexilis_status.
    /// \note Nexilis status is set to 1 for internal nexilis commands read by the server.
    /// \note Nexilis status is set to 2 for internal nexilis commands not read by the server.
    static ::boost::json::object getNexilisStatus(int status);

    static ::boost::json::object getType(const std::string& typeName);

    /// Convert std::vector<uint8_t> to boost::json::object.
    static ::boost::json::object convertToJSON(const std::vector<uint8_t>& bytes);

    /// Print the contents of boost::json::object.
    static void print(const ::boost::json::object& obj);

    static std::string toString(const ::boost::json::object& obj);

    /// Write json object to a file.
    static void saveToFile(const ::boost::json::object& obj, const std::string& filePath);

    /// Merge second object to the first one.
    static void emplace(::boost::json::object& first, const ::boost::json::object& second);

private:
    // Get json data from room vector.
    static ::boost::json::array roomsToJSON(const std::vector<Room>& rooms);

    /// Get json data from client vector.
    static ::boost::json::array clientsToJSON(const std::vector<User>& clients);
};

} // namespace nexilis

#endif
