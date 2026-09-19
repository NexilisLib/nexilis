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

#ifndef NEXILIS_CONNECTION_MANAGER_HH
#define NEXILIS_CONNECTION_MANAGER_HH

#include <nexilis/protocol.hh>

#include <type_traits>
#include <vector>

namespace nexilis
{

namespace detail
{

template <typename T, typename = void>
struct has_get_type : std::false_type
{
};

template <typename T>
struct has_get_type<T, std::void_t<decltype(std::declval<T>().getType())>>
    : std::is_same<decltype(std::declval<T>().getType()), Protocol::Type>
{
};

} // namespace detail

class ProtocolManager
{
public:
    class ProtocolData
    {
    public:
        /// Constructor.
        explicit ProtocolData(Protocol::Type type);

        /// Copy constructor.
        ProtocolData(const ProtocolData& other);

        /// Move constructor.
        ProtocolData(ProtocolData&& other);

        /// Copy assignment operator.
        ProtocolData& operator=(const ProtocolData& other);

        /// Move assignment operator.
        ProtocolData& operator=(ProtocolData&& other);

        /// Get the Protocol::Type.
        Protocol::Type getType() const
        {
            return m_type;
        }

        /// Get the id of the protocol.
        size_t getId() const
        {
            return m_id;
        }

    private:
        Protocol::Type m_type;
        size_t m_id;
    };

    /// Create a new protocol and register it.
    /// The type T must either derive from Protocol or have a getType() method
    /// that returns Protocol::Type.
    template <typename T, typename... Args>
    T createProtocol(Args&&... args)
    {
        static_assert(detail::has_get_type<T>::value,
                      "Type must derive from nexilis::Protocol or have a "
                      "getType() method returning Protocol::Type");

        // This is little hacky. I'd much rather prefer if this was std::move call.
        // However this would require that the parameters cannot be references.
        // So this is technically always move call and it works and the api is nice.
        //
        // It's also important to note that this gives compile-time error
        // if the construction fails.
        auto protocol = T(std::forward<Args>(args)...);
        m_items.emplace_back(ProtocolData(protocol.getType()));

        return protocol;
    }

    /// Get the number of registered protocols.
    size_t getProtocolCount() const
    {
        return m_items.size();
    }

    /// Check if a protocol with the given id exists.
    bool hasProtocol(size_t id) const;

    /// Check if a protocol with the given type exists.
    bool hasProtocolType(Protocol::Type type) const;

    /// Find a protocol data by id. Returns nullptr if not found.
    const ProtocolData* findById(size_t id) const;

    /// Find all protocol data entries matching the given type.
    std::vector<const ProtocolData*> findByType(Protocol::Type type) const;

    std::vector<ProtocolData>::const_iterator begin() const
    {
        return m_items.cbegin();
    }

    std::vector<ProtocolData>::const_iterator end() const
    {
        return m_items.cend();
    }

private:
    std::vector<ProtocolData> m_items;
};

} // namespace nexilis

#endif
