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
