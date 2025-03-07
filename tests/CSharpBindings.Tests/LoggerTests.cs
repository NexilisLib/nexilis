using Nexilis;

namespace CSharpBindings.Tests;

public class LoggerTests
{
    [Fact]
    public void TestTest()
    {
        var logger = new Logger();
        logger.Debug("Debug message");
    }
}
