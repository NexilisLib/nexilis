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

using System;
using System.Runtime.InteropServices;

namespace Nexilis.Logger
{
    public class Logger : IDisposable
    {
        private IntPtr _loggerPtr;

        public Logger()
        {
            _loggerPtr = LoggerNative.nexilis_logger_create();
        }

        public void Dispose()
        {
            if (_loggerPtr != IntPtr.Zero)
            {
                LoggerNative.nexilis_logger_destroy(_loggerPtr);
                _loggerPtr = IntPtr.Zero;
            }
        }

        public void AddConsoleHandler(ConsoleHandler consoleHandler)
        {
            NullCheck.ThrowIfNull(consoleHandler, nameof(consoleHandler));
            LoggerNative.nexilis_logger_add_console_handler(_loggerPtr, consoleHandler.HandlerPtr);
        }

        public void AddFileHandler(FileHandler fileHandler)
        {
            NullCheck.ThrowIfNull(fileHandler, nameof(fileHandler));
            LoggerNative.nexilis_logger_add_file_handler(_loggerPtr, fileHandler.HandlerPtr);
        }

        public void AddFunctionHandler(FunctionHandler functionHandler)
        {
            NullCheck.ThrowIfNull(functionHandler, nameof(functionHandler));
            LoggerNative.nexilis_logger_add_function_handler(_loggerPtr, functionHandler.HandlerPtr);
        }

        public void RemoveHandler(ulong handlerId)
        {
            LoggerNative.nexilis_logger_remove_handler(_loggerPtr, handlerId);
        }

        public void ClearHandlers()
        {
            LoggerNative.nexilis_logger_clear_handlers(_loggerPtr);
        }

        public bool NoHandlers()
        {
            return LoggerNative.nexilis_logger_no_handlers(_loggerPtr) != 0;
        }

        public void Debug(string message)
        {
            LoggerNative.nexilis_logger_debug(_loggerPtr, message);
        }

        public void Info(string message)
        {
            LoggerNative.nexilis_logger_info(_loggerPtr, message);
        }

        public void Warning(string message)
        {
            LoggerNative.nexilis_logger_warning(_loggerPtr, message);
        }

        public void Error(string message)
        {
            LoggerNative.nexilis_logger_error(_loggerPtr, message);
        }

        public void Critical(string message)
        {
            LoggerNative.nexilis_logger_critical(_loggerPtr, message);
        }

        public bool UnsetLevel(int level)
        {
            return LoggerNative.nexilis_logger_unset_level(_loggerPtr, level);
        }

        public bool SetLevel(int level)
        {
            return LoggerNative.nexilis_logger_set_level(_loggerPtr, level);
        }

        public bool GetLevel(int level)
        {
            return LoggerNative.nexilis_logger_get_level(_loggerPtr, level);
        }

        public bool SetMinimumLevel(int level)
        {
            return LoggerNative.nexilis_logger_set_minimum_level(_loggerPtr, level);
        }

        public void SetLogLevel(byte level)
        {
            LoggerNative.nexilis_logger_set_log_level(_loggerPtr, level);
        }
    }
}
