# `clas12_include.h`

Source file:

- [`include/CLAS12/clas12_include.h`](../../include/CLAS12/clas12_include.h)

## Purpose

`clas12_include.h` is a small convenience wrapper that centralizes the core CLAS12ROOT includes used across the project:

- `HipoChain.h`
- `clas12reader.h`
- `region_particle.h`

It also exposes `using namespace clas12;`, so including this header makes the common CLAS12 types directly available in the including translation unit.

## Notes

- This is a convenience header, not a subsystem on its own.
- Because it introduces a namespace import at header scope, it is useful for legacy compatibility but increases the chance of symbol collisions in new code.
