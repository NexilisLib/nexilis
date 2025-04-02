#if NET6_0_OR_GREATER
using System;

public static class NullCheck
{
    public static void ThrowIfNull(object argument, [System.Runtime.CompilerServices.CallerArgumentExpression("argument")] string paramName = null)
    {
        ArgumentNullException.ThrowIfNull(argument, paramName);
    }
}
#else

// Fallback for .NET Standard 2.1 or older.
using System;

public static class NullCheck
{
    public static void ThrowIfNull(object argument, string paramName)
    {
        if (argument is null)
        {
            throw new ArgumentNullException(paramName);
        }
    }
}
#endif