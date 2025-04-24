using System;
using System.Threading;

namespace Nexilis
{

public class NxLogger : IDisposable
{
    // Nexilis logger.
    private static Logger.Logger _logger = new Logger.Logger();

    // Nexilis logger function handler.
    private Logger.FunctionHandler _functionHandler;

    // Logging callback.
    private Action<Logger.LogLevel, string>? _logCallback;
    
    private readonly ReaderWriterLockSlim _lock = new ReaderWriterLockSlim();
    private bool _disposed = false;
    private string _logContext;

    public NxLogger(string logContext)
    {
        _logContext = logContext;
        _functionHandler = new Logger.FunctionHandler(Logger.FunctionHandler.EmptyFunction);
    }

    public void AddHandler(Action<Logger.LogLevel, string> logFunction)
    {
        _logCallback = logFunction ?? throw new ArgumentNullException(nameof(logFunction));

        try
        {
            // Create and register the function handler.
            Action<Logger.LogLevel, string> logCb = (level, msg) =>
            {
                SafeInvokeLogCallback(level, msg);
            };
            _functionHandler = new Logger.FunctionHandler(logCb);
            _logger.AddFunctionHandler(_functionHandler);
        }
        catch
        {
            _functionHandler?.Dispose();
            throw;
        }
    }

    public void Debug(string message)
    {
        _logger.Debug(AddLogContext(message));
    }

    public void Info(string message)
    {
        _logger.Info(AddLogContext(message));
    }

    public void Warning(string message)
    {
        _logger.Warning(AddLogContext(message));
    }

    public void Error(string message)
    {
        _logger.Error(AddLogContext(message));
    }

    public void Critical(string message)
    {
        _logger.Critical(AddLogContext(message));
    }

    private string AddLogContext(string message)
    {
        if (_disposed) throw new ObjectDisposedException(nameof(NxLogger));
        if (_functionHandler.isEmpty) throw new InvalidOperationException("Function handler is empty.");
        if (!string.IsNullOrEmpty(_logContext))
        {
            return $"{_logContext}: {message}";
        }
        return message;
    }

    public void ClearHandlers()
    {
        if (_disposed) throw new ObjectDisposedException(nameof(NxLogger));
        _logger?.ClearHandlers();
    }

    public void RemoveHandler(ulong handlerId)
    {
        if (_disposed) throw new ObjectDisposedException(nameof(NxLogger));
        _logger?.RemoveHandler(handlerId);
    }

    public bool NoHandlers()
    {
        if (_disposed) throw new ObjectDisposedException(nameof(NxLogger));
        return _logger?.NoHandlers() ?? false;
    }

    public void SetMinimumLevel(Logger.LogLevel level)
    {
        if (_disposed) throw new ObjectDisposedException(nameof(NxLogger));
        _logger.SetMinimumLevel((int)level);
    }

    public bool UnsetLevel(int level)
    {
        if (_disposed) throw new ObjectDisposedException(nameof(NxLogger));
        return _logger?.UnsetLevel(level) ?? false;
    }

    public bool SetLevel(int level)
    {
        if (_disposed) throw new ObjectDisposedException(nameof(NxLogger));
        return _logger?.SetLevel(level) ?? false;
    }

    public bool GetLevel(int level)
    {
        if (_disposed) throw new ObjectDisposedException(nameof(NxLogger));
        return _logger?.GetLevel(level) ?? false;
    }

    public void SetLogLevel(byte level)
    {
        if (_disposed) throw new ObjectDisposedException(nameof(NxLogger));
        _logger?.SetLogLevel(level);
    }

    public void Dispose()
    {
        if (_disposed) return;

        _logger?.RemoveHandler(_functionHandler.GetId());
        _functionHandler?.Dispose();
        _logger?.Dispose();
        _logCallback = null;
    }

    private void SafeInvokeLogCallback(Logger.LogLevel level, string message)
    {
        if (_disposed) throw new ObjectDisposedException(nameof(NxLogger));

        _lock.EnterReadLock();
        try
        {
            _logCallback?.Invoke(level, message);
        }
        finally
        {
            _lock.ExitReadLock();
        }
    }
}

}
