#include <nexilis/json.hh>

#include <fstream>

namespace nexilis
{

// Create JSON data from given key-value pairs
boost::json::object Json::createJSON(const std::map<std::string, boost::json::value>& keyValues)
{
    boost::json::object json_obj;
    for (const auto& kv : keyValues)
    {
        json_obj[kv.first] = kv.second;
    }
    return json_obj;
}

/// Read JSON data from file.
boost::json::value Json::readJSONFromFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        Log::error("Error: Unable to open file ", filename);
        return boost::json::value();
    }

    std::string json_str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    boost::json::error_code ec;
    boost::json::value json_value = boost::json::parse(json_str, ec);
    if (ec)
    {
        Log::error("Error parsing JSON: ", ec.message());
        return boost::json::value();
    }
    return json_value;
}

}
