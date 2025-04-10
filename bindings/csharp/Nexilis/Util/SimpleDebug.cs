using System;

namespace Nexilis
{

public class SimpleDebug : IDisposable
{
    private static Logger.Logger _logger = new Logger.Logger();
    private Logger.FunctionHandler _functionHandler;
    private Action<string>? _logCallback;

    public SimpleDebug(Action<string> logFunction)
    {
        _logCallback = logFunction ?? throw new ArgumentNullException(nameof(logFunction));

        Action<Logger.LogLevel, string> logCb = (Logger.LogLevel level, string msg) =>
        {
            _logCallback(msg);
        };

        _functionHandler = new Logger.FunctionHandler(logCb);
        _logger.AddFunctionHandler(_functionHandler);
        _logger.SetLevel((int)Logger.LogLevel.DEBUG);
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
