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

#ifndef NEXILIS_NX_CLASS_HH
#define NEXILIS_NX_CLASS_HH

#include <string>

namespace nexilis
{

/// \note This class has nothing to do with "nx_data" or "nx_util".

class NxClass
{
public:
    /// Constructor.
    /// \param name The classname of the user class.
    explicit NxClass(const std::string& name);

    /// Move constructor.
    NxClass(NxClass&& other);

    /// Move assignment operator.
    NxClass& operator=(NxClass&& other);

    /// Deleted copy constructor.
    NxClass(const NxClass&) = delete;

    /// Deleted copy assignment operator.
    NxClass& operator=(const NxClass&) = delete;

    /// Get the associated name.
    const std::string& classname() const
    {
        return m_classname;
    }

    /// Get the classes name as header for log messages.
    const std::string& header() const
    {
        return m_logHeader;
    }

private:
    std::string m_classname;
    std::string m_logHeader;
};

} // namespace nexilis

#endif
