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
