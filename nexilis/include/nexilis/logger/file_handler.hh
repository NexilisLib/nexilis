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

#ifndef NEXILIS_LOGGER_FILE_HANDLER_HH
#define NEXILIS_LOGGER_FILE_HANDLER_HH

#include <nexilis/logger/base_handler.hh>

#include <fstream>

namespace nexilis::logger
{

class FileHandler : public BaseHandler
{
public:
    /// Constructor.
    /// \param filename The file where the messages will be written.
    explicit FileHandler(const std::string& filename)
        : ofs(filename, std::ios::app)
    {
    }

    /// Write messages to the given file.
    /// \param data The data of the message.
    void emit(LogLevel /*logLevel*/, const std::string& data) override
    {
        ofs << data << std::endl;
    }

    // Overloading the equality operator.
    bool operator==(const BaseHandler& other) const override
    {
        return this == &other;
    }

private:
    std::ofstream ofs;
};

} // namespace nexilis::logger

#endif
