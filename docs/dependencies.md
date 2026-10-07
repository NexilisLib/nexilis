# Dependencies

The native library requires CMake 3.25 or newer, a C++20 compiler, OpenSSL 1.1.1 or newer, and Boost. `nexilis/CMakeLists.txt` builds against the Boost submodule for local builds. In CI it uses a system Boost installation with the JSON component (Boost 1.75 or newer).

OpenSSL is a required build dependency even when the optional encryption features are disabled. The native libraries link to `OpenSSL::SSL`. Boost.JSON is a compiled dependency; the local build creates it from the submodule.

For a local checkout, initialize the submodule:

```sh
git submodule update --init --recursive
```

On Ubuntu, install the basic build tools and OpenSSL development package:

```sh
sudo apt install cmake g++ libssl-dev
```

CI or another build using system Boost also needs `libboost-json-dev`. On Arch Linux, the corresponding packages include `cmake`, `gcc`, `openssl`, and `boost`.

The C# wrapper targets .NET Standard 2.0 and needs a .NET SDK to build. It calls the native `nexilisc` shared library at runtime; deploying only `Nexilis.dll` is insufficient. Native tests require GoogleTest, and the Premake test requires Premake 5.
