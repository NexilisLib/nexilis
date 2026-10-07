# Nexilis documentation

Nexilis is a C++20 client/server library with C, C#, and LuaJIT interfaces. Start with the [repository README](../README.md) for a local build and a runnable example.

## Project structure

| Path | Contents |
| --- | --- |
| `nexilis/` | Native C++ library and C API |
| `bindings/csharp/` | C# wrapper for the native library |
| `bindings/lua/` | LuaJIT client wrapper for the C API |
| `examples/` | Standalone C++ examples |
| `tests/` | Native, C API, and C# tests |
| `scripts/` | Build and quality-check scripts |

See [dependencies](dependencies.md), the [message protocol](server_message.md), and the [bindings guide](../bindings/README.md) for details.

## Testing

The native test suites need GoogleTest and a configured Nexilis build. After `make install`, run `make test-cpp` and `make test-c`. C# tests also require the .NET SDK; run `make test-csharp`. `make test-lua` requires LuaJIT and runs a local TCP/UDP server. `make test-examples` builds and runs the examples. `make test` runs all suites, including Premake, which requires Premake 5.

The CI workflow in [`.github/workflows/ci.yml`](../.github/workflows/ci.yml) shows the dependencies used for each check.

## Before a public release

- The CMake install target exports libraries and package files but omits public headers. Its package config also records absolute Boost include paths from the source checkout when submodules are used. Downstream consumers cannot use the installed package alone or reliably move it to another machine.
- Enabling room message encryption does not guarantee confidentiality: the server knows the password used to derive the key, and an encryption error currently falls back to sending the plaintext payload. See [security notes](server_message.md#security-notes).
- The package is marked `unstable` in [VERSION.txt](../VERSION.txt). Confirm supported platforms, compatibility, and release packaging before advertising a stable version.
