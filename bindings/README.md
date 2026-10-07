# Nexilis bindings

The LuaJIT client binding is documented in [`bindings/lua/README.md`](lua/README.md).

## C#

The C# wrapper is in [`bindings/csharp`](csharp/) and targets .NET Standard 2.0. Build it with a .NET SDK:

```sh
dotnet build bindings/csharp/Nexilis.csproj
```

Run that command from the repository root. The wrapper calls the native `nexilisc` shared library through P/Invoke; the native `nexilis` library and its runtime dependencies must also be available to the dynamic loader. Building the C# assembly alone does not produce a standalone package.

`make install-csharp` runs [`scripts/install_csharp.py`](../scripts/install_csharp.py) to build the native libraries and place the C# assembly and native libraries under `dist/` by default. Run it from the repository root after initializing the Boost submodule. The script currently selects the host platform, so its `--arch` label does not cross-compile binaries.

To run the C# test project after the native libraries are available, use `make test-csharp`.
