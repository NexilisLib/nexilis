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

public static class FileLog
{
    static readonly object _lock = new object();
    static readonly string _logFilePath = "/tmp/nexilis/csharp-bindings-log.txt";

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
