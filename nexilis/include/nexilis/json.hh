#ifndef NEXILIS_JSON_HH
#define NEXILIS_JSON_HH

#include <nexilis/client.hh>
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

    /// Return some data from the server.
    static ::boost::json::object getServerData();

    /// Convert std::vector<uint8_t> to boost::json::object.
    static ::boost::json::object convertToJSON(const std::vector<uint8_t>& bytes);

    /// Print the contents of boost::json::object.
    static void print(const ::boost::json::object& obj);

    /// Write json object to a file.
    static void saveToFile(const ::boost::json::object& obj, const std::string& filePath);

private:
    // Get json data from room vector.
    static ::boost::json::array roomsToJSON(const std::vector<Room>& rooms);

    /// Get json data from client vector.
    static ::boost::json::array clientsToJSON(const std::vector<Client>& clients);
};

} // namespace nexilis

#endif
