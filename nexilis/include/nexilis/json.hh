/* Copyright (C) 2026 Valtteri Viirret
   This file is part of the Nexilis Project.

   This file is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This file is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this file.  If not, see <https://gnu.org>. */

#ifndef NEXILIS_JSON_HH
#define NEXILIS_JSON_HH

#include <boost/json/object.hpp>
#include <nexilis/nexilis_constants.hh>
#include <nexilis/nx_data.hh>

#include <map>

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

    /// Convert nx_data to boost::json::object.
    static boost::json::object convertToJSON(const nx_data& bytes);

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
