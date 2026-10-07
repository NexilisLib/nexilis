# Contributing to Nexilis

Contributions are welcome. Bug reports, documentation improvements, tests, and code changes all help the project.

## Before you submit a change

Read the [README](README.md), [documentation index](docs/README.md), and [dependencies](docs/dependencies.md). Follow the conventions in [AGENTS.md](AGENTS.md) and keep changes focused.

For a local native build, initialize the Boost submodule and build from the repository root:

```sh
git submodule update --init --recursive
cmake -S nexilis -B nexilis/build -DCMAKE_BUILD_TYPE=Debug
cmake --build nexilis/build --parallel 2
```

## Tests and CI

Add or update unit tests for every code change. Bug fixes need a test that reproduces the issue, and new behavior needs tests for its expected results. Put tests in the relevant suite under `tests/nexilis/`, `tests/nexilisc/`, or `tests/CSharpBindings.Tests/`. Run the affected tests locally before opening a pull request:

| Area changed | Check |
| --- | --- |
| C++ library | `make test-cpp` |
| C API | `make test-c` and affected C++ tests |
| C# bindings | `make test-csharp` |
| LuaJIT bindings | `make test-lua` |
| Examples or public API usage | `make test-examples` |
| Build configuration | Relevant CMake build and `make test-premake` |
| Python scripts | `cd scripts && python -m flake8 .` |

The native library must be built before native and binding tests. See [AGENTS.md](AGENTS.md#choose-validation-for-the-change) for test dependencies and additional checks. Run `make test` when your change affects several areas and the required tools are available.

All applicable [CI checks](.github/workflows/ci.yml) must pass before a contribution can be merged. If a local check cannot run, say which check was skipped and why in the pull request; CI still needs to pass.

## Pull requests

Explain what changed and why, link any related issue, and list the tests you ran. Update documentation when behavior or public APIs change. Keep generated build outputs and local configuration files out of the pull request.
