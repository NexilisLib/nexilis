#if !NET6_0_OR_GREATER && !UNITY_2021_2_OR_NEWER
// Define the attribute manually for older .NET/Unity versions
namespace System.Runtime.CompilerServices
{
    [AttributeUsage(AttributeTargets.Parameter, AllowMultiple = false)]
    public sealed class CallerArgumentExpressionAttribute : Attribute
    {
        public string ParameterName { get; }
        public CallerArgumentExpressionAttribute(string parameterName)
        {
            ParameterName = parameterName;
        }
    }
}
#endif

public static class NullCheck
{
    public static void ThrowIfNull(
        object argument,
        [System.Runtime.CompilerServices.CallerArgumentExpression("argument")] string? paramName = null)
    {
#if NET6_0_OR_GREATER || UNITY_2021_2_OR_NEWER
        ArgumentNullException.ThrowIfNull(argument, paramName);
#else
        if (argument is null)
            throw new ArgumentNullException(paramName ?? nameof(argument));
#endif
    }
}
