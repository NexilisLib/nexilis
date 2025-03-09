namespace Nexilis
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
            if (consoleHandler == null)
            {
                throw new ArgumentNullException(nameof(consoleHandler));
            }
            LoggerNative.nexilis_logger_add_console_handler(_loggerPtr, consoleHandler.HandlerPtr);
        }

        public void AddFileHandler(FileHandler fileHandler)
        {
            if (fileHandler == null)
            {
                throw new ArgumentNullException(nameof(fileHandler));
            }
            LoggerNative.nexilis_logger_add_file_handler(_loggerPtr, fileHandler.HandlerPtr);
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
