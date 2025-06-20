using System;
using System.IO;
using System.Threading;

public static class FileLog
{
    private static readonly object _lock = new object();
    private static readonly string _logFilePath = "/tmp/nexilis/csharp-bindings-log.txt";

    static FileLog()
    {
        // Ensure the directory exists
        Directory.CreateDirectory(Path.GetDirectoryName(_logFilePath));

        // Initialize log file (or clear existing)
        lock (_lock)
        {
            File.WriteAllText(_logFilePath, $"[{DateTime.UtcNow:yyyy-MM-dd HH:mm:ss.fff}] LOG INITIALIZED\n");
        }
    }

    public static void Log(string message)
    {
        lock (_lock)
        {
            try
            {
                string logEntry = $"[{DateTime.UtcNow:yyyy-MM-dd HH:mm:ss.fff}] {message}\n";
                File.AppendAllText(_logFilePath, logEntry);
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine($"LOGGER FAILED: {ex.Message}");
            }
        }
    }
}
