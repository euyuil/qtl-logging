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

## Improvement Plans

### Priority 1: Async Logging Support

**Status**: 📋 Planned

**Description:**
Add support for asynchronous logging using spdlog's async features.

**Configuration:**
```cpp
struct LoggingParams {
    bool asyncLogging = false;
    size_t asyncQueueSize = 8192;
    size_t asyncThreads = 1;
    // ... existing fields
};
```

**Benefits:**
- Non-blocking log calls (better performance for hot paths)
- Separate thread handles I/O
- Configurable queue size

**Implementation Notes:**
- Use spdlog::async_factory
- Ensure proper shutdown (flush queue)
- Document trade-offs (memory usage, ordering guarantees)

---

### Priority 2: Compile-time Log Level Filtering

**Status**: 📋 Planned

**Description:**
Add compile-time macros to completely eliminate low-level logs in release builds.

**Implementation:**
```cpp
// In loggable.h
#ifndef QTL_MIN_LOG_LEVEL
#define QTL_MIN_LOG_LEVEL QTL_LEVEL_TRACE
#endif

#if QTL_MIN_LOG_LEVEL <= QTL_LEVEL_TRACE
#define QTL_TRACE(...) logger().trace(__VA_ARGS__)
#else
#define QTL_TRACE(...) ((void)0)
#endif
```

**CMake Integration:**
```cmake
# Release builds
target_compile_definitions(your_target PRIVATE
    QTL_MIN_LOG_LEVEL=QTL_LEVEL_INFO
)
```

**Benefits:**
- Zero runtime overhead for disabled log levels
- Compiler can optimize away the code completely
- Useful for release builds

---

### Priority 3: Enhanced Documentation

**Status**: 📋 Planned

**Items:**
1. **Doxygen Comments**: Add to all public APIs
2. **API Reference**: Generate with Doxygen
3. **More Examples**:
   - Rotation example
   - Custom pattern example
   - Multi-component application
   - Performance benchmark
4. **Architecture Diagram**: Visual overview of components
5. **Migration Guide**: From direct spdlog usage to qtl-logging

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
- `QTL_LOGGING_BUILD_TESTS` (default: ON)
- `QTL_LOGGING_BUILD_EXAMPLES` (default: ON)

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

---

Last Updated: 2025-11-08 (cleaned up completed priorities)
