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

#include <nexilis/client/protocol_status.hh>

namespace nexilis::client
{

std::string ProtocolUtils::getProtocolStatusAsString(ProtocolStatus protocolStatus)
{
    switch (protocolStatus)
    {
        case ProtocolStatus::connected:
            return "connected";
        case ProtocolStatus::switching_ports:
            return "switching_ports";
        case ProtocolStatus::connecting:
            return "connecting";
        case ProtocolStatus::error:
            return "error";
        case ProtocolStatus::undefined:
            return "undefined";
    }
    return "";
}

} // namespace nexilis::client
