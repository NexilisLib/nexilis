# Working on Nexilis

These instructions apply throughout this repository. Read the relevant source and
build configuration before changing behavior; use the documentation below as an
entry point, and update it when behavior changes.

## Repository map

Nexilis is a C++20 client/server library with TCP and UDP transports, rooms,
messaging, and C, C#, and LuaJIT interfaces.

| Path | Purpose |
| --- | --- |
| `nexilis/include/nexilis/`, `nexilis/src/nexilis/` | C++ public headers and implementation |
| `nexilis/include/nexilisc/`, `nexilis/src/nexilisc/` | C API and its implementation |
| `bindings/csharp/`, `bindings/lua/` | Managed and LuaJIT wrappers |
| `tests/nexilis/`, `tests/nexilisc/` | Native GoogleTest suites |
| `tests/CSharpBindings.Tests/`, `tests/lua/`, `tests/premake/` | Binding and build integration tests |
| `examples/` | Runnable examples and downstream usage patterns |
| `scripts/` | Python build, quality, and versioning tools |
| `.github/workflows/` | CI and weekly release automation |

Start with `README.md`, `docs/README.md`, and `docs/dependencies.md`. For protocol
changes, read `docs/server_message.md`; for bindings, read `bindings/README.md`
and `bindings/lua/README.md` as applicable.

## Build and dependencies

Run these commands from the repository root. Native builds need CMake 3.25+,
a C++20 compiler, OpenSSL development files, and Boost. Local builds use the
Boost submodule:

```sh
git submodule update --init --recursive
cmake -S nexilis -B nexilis/build -DCMAKE_BUILD_TYPE=Debug
cmake --build nexilis/build --parallel 2
```

Use a parallelism level appropriate to available memory. Reuse an existing build
when possible. `make install` removes `nexilis/build` before rebuilding and installs
under `nexilis/build/install`; it is unnecessary for every incremental edit.
The install target currently omits public headers, so do not assume its output is
a standalone SDK. Recheck this limitation when changing packaging.

CMake selects system Boost when a CI environment variable such as `CI` or
`GITHUB_ACTIONS` is defined, even if its value is `false`. The Nix environment has
its own detection. Inspect `configure_build_environment` in
`nexilis/CMakeLists.txt` when diagnosing dependency selection; the
`NEXILIS_IS_*` cache values are derived, not reliable user overrides.

For Python tooling, install dependencies into the active virtual environment:

```sh
python -m pip install -e './scripts[dev]'
```

The scripts locate the checkout automatically. If `.env` or the environment sets
`NEXILIS_ROOT`, ensure it points to this checkout, especially in a worktree.

## Choose validation for the change

Build the native library first for native and binding tests. Native suites require
GoogleTest; the C++ suite also finds system Boost.JSON. Consult
`.github/workflows/ci.yml` for the dependencies used in CI.

| Change | Relevant checks from the repository root |
| --- | --- |
| C++ library | `make test-cpp` |
| C API | `make test-c` and affected C++ tests |
| C# wrapper or native interop | `make test-csharp` (test project requires .NET 9) |
| LuaJIT wrapper or native interop | `make test-lua` (requires LuaJIT and a C++ compiler) |
| Examples or public API usage | `make test-examples` |
| Build/source layout | Relevant CMake builds and `make test-premake` (requires Premake 5) |
| Python scripts | `cd scripts && python -m flake8 .`, plus checks of changed behavior |
| Doxygen configuration or API docs | `cd nexilis && doxygen Doxyfile` (requires Doxygen and Graphviz) |

Native test executables support GoogleTest filters, for example
`tests/nexilis/build/nexilis_tests --gtest_filter='SuiteName.*'` after building.
The C# tests copy native `.so` files from `nexilis/build`; Lua tests also use that
directory and start a local server. Report missing dependencies or unavailable
network/socket access separately from test failures.

`make test` runs all suites, including Premake, Lua, and examples. Use it for broad
changes when dependencies are available; documentation-only edits need link/path
and whitespace checks, not a full native rebuild. Add regression coverage for
behavioral fixes. Report the checks actually run and any checks left unverified.

## Editing conventions and compatibility

- Follow `.editorconfig`, `.clang-format`, and nearby code. Native sources normally
  use `.cc` and `.hh`; Python linting uses the 90-character limit in `scripts/.flake8`.
- Run `clang-format --dry-run --Werror` on changed native files to check formatting,
  or `clang-format -i` on those files to apply it. `make format` and the formatting
  stage of the pre-commit scripts can rewrite files across the repository.
- Keep changes scoped to the task and preserve unrelated working-tree edits.
  Avoid committing generated documentation, build outputs, local `.env` files, or
  changes inside `nexilis/third-party/` unless dependency changes are the task.
- When changing public APIs, inspect the C facade, C# P/Invoke declarations, LuaJIT
  FFI declarations, examples, and tests for affected callers. Keep ownership,
  callback lifetimes, error handling, and cross-language type sizes consistent.
- For network changes, check both sending and receiving paths, packet framing,
  authentication, disconnect handling, and thread/object lifetimes. Update the
  protocol documentation and relevant integration tests when semantics change.
- Keep CMake and Premake source selection consistent when adding or moving native
  files. Preserve existing license headers in modified source files.
- Describe security behavior accurately: transport encryption is optional, and
  room encryption has documented limitations. Read `docs/server_message.md`
  before changing encryption behavior or making security claims.

## Versions, releases, and the documentation website

`VERSION.txt` is the source of the library version. Use the existing Python script
for intentional version changes so Doxygen's `PROJECT_NUMBER` stays synchronized:

```sh
python scripts/update_version.py --bump-patch
# Or set an explicit version:
python scripts/update_version.py 0.1.0-unstable
```

Do not bump versions as part of ordinary fixes unless requested. Preserve the
`stable`/`unstable` suffix unless the requested change explicitly changes status.
Avoid hardcoding the current version in prose; link to `VERSION.txt` instead.

`.github/workflows/release.yml` runs weekly CI, increments the patch version,
creates a version commit/tag and GitHub release, and deploys matching Doxygen HTML
to GitHub Pages. Read `docs/releases.md` before modifying release automation.
Doxygen runs from `nexilis/` and produces `nexilis/docs/html/`; the root `docs/`
directory contains maintained Markdown documentation. Preserve release retry
handling and the check against publishing superseded documentation.
