#ifndef NEXILIS_CONVERT_TO_MAP_HH
#define NEXILIS_CONVERT_TO_MAP_HH

#include <boost/json/object.hpp>
#include <map>

namespace nexilis
{

std::map<std::string, boost::json::value> convert_to_map(const boost::json::object& obj);

}

#endif
