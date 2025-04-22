using System;

namespace Nexilis
{

public class NxLogger : IDisposable
{
    private static Logger.Logger _logger = new Logger.Logger();
    private Logger.FunctionHandler _functionHandler;
    private Action<Logger.LogLevel, string>? _logCallback;

    public NxLogger(Action<Logger.LogLevel, string> logFunction)
    {
        _logCallback = logFunction ?? throw new ArgumentNullException(nameof(logFunction));
        var methodInfo = _logCallback.GetMethodInfo();

        // Store the log level from the first parameter of the delegate
        var parameters = _logCallback.Method.GetParameters();
        if (parameters.Length >= 1 && parameters[0].ParameterType == typeof(Logger.LogLevel))
        {
            // Get the default value (if any) or use a default level
            var logLevel = parameters[0].DefaultValue is DBNull ? 
                Logger.LogLevel.Info : 
                (Logger.LogLevel)parameters[0].DefaultValue;
            
            _logger.SetLevel((int)logLevel);
        }
        else
        {
            // Fallback to a default level if we can't determine it
            _logger.SetLevel((int)Logger.LogLevel.Info);
        }

        var logCb = (Logger.LogLevel level, string msg) =>
        {
            _logCallback(level, msg);
        };

        _functionHandler = new Logger.FunctionHandler(logCb);
        _logger.AddFunctionHandler(_functionHandler);
        _logger.SetLevel((int)log_level);
    }

    public void Log(string message)
    {
        _logger.Debug(message);
    }

    public void Dispose()
    {
        _logger?.RemoveHandler(_functionHandler.GetId());
        _functionHandler?.Dispose();
        _logger?.Dispose();
        _logCallback = null;
    }
    public void ClearHandlers()
    {
        _logger?.ClearHandlers();
    }
}

}
