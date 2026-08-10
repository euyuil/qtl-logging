# qtl-logging Development Notes

This document contains development plans, decisions, and notes for future Claude Code instances.

## Project Overview

qtl-logging is a lightweight, standalone C++20 logging library extracted from the trading-engine project. It's built on top of spdlog and provides a clean, dependency-free interface.

**Design Principles:**
- Lightweight: No heavy dependencies on DI frameworks or configuration parsers
- Flexible: Support both direct construction and global registry patterns
- Modern: C++20, clean API, type-safe
- Fast: Built on spdlog's high-performance core

## Coding Style

- **Indentation**: 4 spaces
- **Methods**: camelCase (e.g., `getLogger()`)
- **Members**: mCamelCase (e.g., `mLogger`)
- **No [[nodiscard]]** on simple getters
- **Namespaces**: `qtl::logging` and `qtl::util`

## Git Commit Guidelines

- **NO Claude Code credits**: Do not add "Generated with Claude Code" or "Co-Authored-By: Claude" to commit messages
- **Keep commits clean**: Only include relevant technical information
- **Use conventional format**: Clear subject line, detailed body when needed

## Planned Improvements

**For Claude Code agents**: All planned features and improvements are tracked as GitHub issues.
Use the GitHub MCP tools to interact with issues:
- `mcp__github__list_issues` - List all open issues
- `mcp__github__issue_read` - Read issue details
- `mcp__github__issue_write` - Create or update issues

**For humans**: View issues at https://github.com/euyuil/qtl-logging/issues

Current priorities:
- **#1**: Async logging support (high priority)
- **#2**: Compile-time log level filtering
- **#3**: Enhanced documentation
- **#4**: More usage examples
- **#5**: Performance benchmarks

---

## Design Decisions

### Why Not Header-Only?

**Considered**: Making the library header-only for easier integration

**Decision**: Keep as compiled library

**Reasoning:**
- Faster compile times for users (implementation hidden)
- spdlog is already header-only, so we're not adding much
- Better for ABI stability
- Users can still use it easily with CMake's find_package

### Why Support Both Direct and Registry Patterns?

**Direct Construction**:
```cpp
Loggable(params, consoleSink, fileSink, "MyLogger")
```

**Registry Pattern**:
```cpp
Loggable("MyLogger")  // Uses global registry
```

**Reasoning:**
- Direct: Better for libraries (no global state)
- Registry: Better for applications (convenience)
- Users choose based on their needs

### Why Not Abstract spdlog Away Completely?

**Current**: spdlog types visible in public API (`spdlog::logger`, `spdlog::sinks::sink`)

**Alternative**: Wrap everything in our own types

**Decision**: Keep spdlog visible

**Reasoning:**
- spdlog is a well-known, stable API
- Wrapping would add complexity with little benefit
- Users can use spdlog features directly if needed
- Our library config is abstracted (LoggingParams), which is what matters

### Full Control Over spdlog Configuration

**Decision**: qtl-logging maintains complete control over spdlog configuration

**Implementation:**
- All spdlog configuration happens in `spdlog_config.h`
- Strict compile-time check: `#error` if `SPDLOG_ACTIVE_LEVEL` is already defined
- Users cannot override via CMake flags or source definitions

**Reasoning:**
- **Single source of truth**: Prevents configuration conflicts and surprises
- **Predictable behavior**: Same behavior across all users and build systems
- **Future flexibility**: If compile-time optimization is needed later, we'll add our own macros (e.g., `QTL_MIN_LOG_LEVEL`) rather than exposing spdlog's internals
- **Encapsulation**: spdlog is an implementation detail; our API should control it

**Future Compile-Time Optimization:**
When Priority 2 is implemented, use qtl-logging's own flags:
```cmake
# Correct way (future)
target_compile_definitions(your_target PRIVATE QTL_MIN_LOG_LEVEL=QTL_LEVEL_INFO)

# Wrong way (never do this)
target_compile_definitions(your_target PRIVATE SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO)
```

---

## Testing Strategy

**Unit Tests**: 21 tests covering all components
- LoggingParams factory methods and builder pattern
- ConsoleSinkManager enabled/disabled states
- FileSinkManager rotation and flushing
- Loggable direct and registry construction
- LoggerRegistry initialization and logger retrieval

**Test Framework**: Google Test

**Test Coverage Areas**:
- Configuration (LoggingParams)
- Sink creation and lifecycle
- Logger creation and naming
- Macro usage
- Registry singleton behavior

**Future Tests Needed**:
- Async logging tests
- Performance benchmarks
- Thread safety tests
- Log rotation verification (file count, size)

---

## Build Configuration

**CMake Version**: 3.15+
**C++ Standard**: C++20
**Dependencies**:
- spdlog 1.13.0+
- Google Test (for tests)

**CMake Options**:
- `BUILD_TESTING` is the global CMake test switch. The top-level project owns it.
- `QTL_LOGGING_BUILD_TESTS` defaults to ON for a standalone build and OFF when used as a subproject. Tests require both this option and `BUILD_TESTING` to be ON.
- `QTL_LOGGING_BUILD_EXAMPLES` defaults to ON for a standalone build and OFF when used as a subproject.

**Build Commands**:
```bash
mkdir build && cd build
cmake ..
cmake --build .
ctest --output-on-failure
```

---

## Future Considerations

### Potential Features

1. **Custom Sinks**: Support for user-defined sinks
2. **Network Logging**: TCP/UDP sink support
3. **Structured Logging**: JSON output format
4. **Log Filtering**: Runtime filters by component/level
5. **Statistics**: Log count, rate, size metrics
6. **Integration Examples**: How to use with popular frameworks

### Performance Targets

- Log call overhead: < 100ns (synchronous)
- Async queue latency: < 1us (p99)
- Zero allocations in hot path (when using format string caching)

---

## Notes for Future Development

- **Backward Compatibility**: Try to maintain API compatibility
- **Version Scheme**: Use semantic versioning (currently 0.1.0)
- **Breaking Changes**: Document clearly in CHANGELOG
- **Testing**: Always add tests for new features
- **Examples**: Update examples to show new features

---

## Questions to Resolve

1. Should we support C++17 for wider compatibility?
2. Do we need a LoggingParams validator (check invalid combinations)?
3. Should we add a default logger (without calling initialize)?
4. Support for custom formatters beyond patterns?
