#include <nexilis/convert_to_map.hh>

namespace nexilis
{

std::map<std::string, boost::json::value> convert_to_map(const boost::json::object& obj)
{
    std::map<std::string, boost::json::value> result;

    // Iterate over each key-value pair in the JSON object
    for (const auto& kv : obj)
    {
        // kv.key() returns a string view, kv.value() returns a boost::json::value
        result[std::string(kv.key())] = kv.value();
    }

    return result;
}

} // namespace nexilis
