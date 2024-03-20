#ifndef NEXILIS_JSON_HH
#define NEXILIS_JSON_HH

#include <nexilis/room.hh>
#include <nexilis/client.hh>

#include <boost/json/object.hpp>
#include <boost/json.hpp>

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
    static boost::json::object createJSON(const std::map<std::string, boost::json::value>& keyValues);

    /// Read JSON data from file.
    static boost::json::value readJSONFromFile(const std::string& filename);

    /// Return some data from the server.
    static boost::json::object getServerData();

private:
    // Get json data from room vector.
    static boost::json::array roomsToJSON(const std::vector<Room>& rooms);

    /// Get json data from client vector.
    static boost::json::array clientsToJSON(const std::vector<Client>& clients);
};

} // namespace nexilis

#endif
