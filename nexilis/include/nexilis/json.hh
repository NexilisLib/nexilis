#ifndef NEXILIS_JSON_HH
#define NEXILIS_JSON_HH

#include <nexilis/log.hh>

#include <boost/json.hpp>

#include <map>

namespace nexilis
{

/// Some simple boost abstraction functions.
/// Don't wanna abstract it too much because boost is perfectly fine library,
/// however these things should be only written once.
class Json
{
public:
    /// Default constructor.
    Json() = default;

    // Create JSON data from given key-value pairs
    static boost::json::object createJSON(const std::map<std::string, boost::json::value>& keyValues);

    /// Read JSON data from file.
    static boost::json::value readJSONFromFile(const std::string& filename);

};

}

#endif
