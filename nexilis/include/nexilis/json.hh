#ifndef NEXILIS_JSON_HH
#define NEXILIS_JSON_HH

#include <boost/json/object.hpp>

#include <map>
#include <vector>

namespace nexilis
{

/// Boost json abstraction layer.
class Json
{
public:
    /// Default constructor.
    Json() = default;

    // Create JSON data from given key-value pairs
    static boost::json::object createJSON(const std::map<std::string, boost::json::value>& keyValues);

    /// Read JSON data from file.
    static boost::json::value readJSONFromFile(const std::string& filename);

    /// Convert std::vector<uint8_t> to boost::json::object.
    static boost::json::object convertToJSON(const std::vector<uint8_t>& bytes);

    /// Print the contents of boost::json::object.
    static void print(const boost::json::object& obj);

    /// Convert json object to string.
    static std::string toString(const boost::json::object& obj);

    /// Write json object to a file.
    static void saveToFile(const boost::json::object& obj, const std::string& filePath);

    /// Merge second object to the first one.
    static void emplace(boost::json::object& first, const boost::json::object& second);
};

} // namespace nexilis

#endif

