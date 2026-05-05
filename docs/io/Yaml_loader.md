# `Yaml_loader`

Source files:

- [`include/io/Yaml_loader.h`](../../include/io/Yaml_loader.h)
- [`source/io/Yaml_loader.cpp`](../../source/io/Yaml_loader.cpp)

## Purpose

`Yaml_loader` provides the generic YAML-to-settings binding layer used throughout the runtime configuration path.

## Main Ideas

- A settings type supplies a `Yaml_loader::SettingsAdapter<T>` specialization.
- `Apply(...)` maps YAML keys into the struct.
- `Validate(...)` enforces post-load constraints.
- Generic helpers handle file loading, section selection, key lookup, and error reporting.

## Common Entry Points

- `LoadFromYamlFile<T>(path, section)`
- `LoadFromYamlFiles<T>(paths, section)`
- `LoadFromYamlSpecs<T>(specs)`
- `ApplyYamlFileToSettings<T>(path, settings, section)`

## Where It Is Used

The current repo uses it for:

- sample-chain loading in [`include/io/analysis_samples.h`](../../include/io/analysis_samples.h)
- event-selection settings
- analysis-cut settings
- acceptance-map settings
- momentum-resolution settings

## Constraints

- `LoadFrom...` helpers require a default-constructible target type.
- Validation errors are surfaced as `std::runtime_error`.
- `CONFIG_DIR` comes from a CMake-defined macro.
