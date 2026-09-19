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

#include <nexilis/nx_class.hh>

namespace nexilis
{

NxClass::NxClass(const std::string& name)
    : m_classname(name),
      m_logHeader("nexilis::" + m_classname + ": ")
{
}

/// Move constructor.
NxClass::NxClass(NxClass&& other)
    : m_classname(std::move(other.m_classname)),
      m_logHeader(std::move(other.m_logHeader))
{
}

/// Move assignment operator.
NxClass& NxClass::operator=(NxClass&& other)
{
    if (this != &other)
    {
        m_classname = std::move(other.m_classname);
        m_logHeader = std::move(other.m_logHeader);
    }
    return *this;
}

} // namespace nexilis
