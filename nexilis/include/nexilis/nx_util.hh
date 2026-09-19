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

#ifndef NEXILIS_NX_CREATE_HH
#define NEXILIS_NX_CREATE_HH

#include <nexilis/util.hh>

namespace nexilis
{

template <typename... Args>
nx_data nx_create(Args&&... args)
{
    // Calculate total size needed.
    size_t total_size = 0;
    (void)std::initializer_list<int>{
            (total_size += std::forward<Args>(args).size(), 0)...};

    // Create result vector with enough capacity.
    std::vector<uint8_t> result;
    result.reserve(total_size);

    // Append all vectors using move semantics where possible.
    (void)std::initializer_list<int>{
            (result.insert(result.end(),
                           std::make_move_iterator(std::forward<Args>(args).begin()),
                           std::make_move_iterator(std::forward<Args>(args).end())),
             0)...};

    return result;
}

template <typename... Args>
static void nx_emplace(nx_data& originalData, Args&&... args)
{
    ([&originalData](const auto& data)
     {
         const auto& byteVector = Util::convertToByteVector(data);
         originalData.reserve(originalData.size() + byteVector.size());
         std::copy(byteVector.begin(), byteVector.end(), std::back_inserter(originalData)); }(std::forward<Args>(args)),
     ...);
}

} // namespace nexilis

#endif
