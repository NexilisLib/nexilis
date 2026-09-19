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

#ifndef NEXILIS_CLIENT_COMMAND_ERROR_HH
#define NEXILIS_CLIENT_COMMAND_ERROR_HH

#include <nexilis/client/base_api_command.hh>

#include <tuple>
#include <type_traits>

namespace nexilis::client
{

template <typename... Args>
class ErrorCommand : public BaseAPICommand
{
public:
    explicit ErrorCommand(ReadResult result, Args&&... args)
        : m_result(result),
          m_args(std::forward<Args>(args)...)
    {
    }

    static std::unique_ptr<ErrorCommand> make_unique(ReadResult result, Args&&... args)
    {
        return std::make_unique<ErrorCommand>(result, std::forward<Args>(args)...);
    }

    ReadResult execute(ClientAPI&, ClientAPI::ClientAPIData&) override
    {
        return m_result;
    }

private:
    ReadResult m_result;
    std::tuple<std::decay_t<Args>...> m_args;
};

} // namespace nexilis::client

#endif
