using System;
using System.Runtime.InteropServices;

namespace Nexilis
{
    public static class LoggerNative
    {
        private const string DllName = "nexilis";

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr nexilis_logger_create();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_logger_destroy(IntPtr logger);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_logger_add_console_handler(IntPtr logger, IntPtr handler);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_logger_add_file_handler(IntPtr logger, IntPtr handler);
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_logger_add_function_handler(IntPtr logger, IntPtr handler);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_logger_remove_handler(IntPtr logger, ulong handlerId);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_logger_clear_handlers(IntPtr logger);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int nexilis_logger_no_handlers(IntPtr logger);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_logger_debug(IntPtr logger, string message);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_logger_info(IntPtr logger, string message);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_logger_warning(IntPtr logger, string message);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_logger_error(IntPtr logger, string message);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_logger_critical(IntPtr logger, string message);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool nexilis_logger_unset_level(IntPtr logger, int level);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool nexilis_logger_set_level(IntPtr logger, int level);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool nexilis_logger_get_level(IntPtr logger, int level);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool nexilis_logger_set_minimum_level(IntPtr logger, int level);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_logger_set_log_level(IntPtr logger, byte level);
    }
}
