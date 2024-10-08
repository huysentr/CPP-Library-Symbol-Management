# Example 4

In example 3 we saw that the order of linkage still mattered to the print results of the `myapp` executable. But the correct order was required to get both A and B in the output, this was not robust.

## Setup
Modified setup of example 2:
- add -`--exclude-libs,libbaselibA` to `somelib`

## Running
`make execute`
```
...
./myapp
baselibA
baselibB
```

### Breakdown of the result
The `--exclude-libs` flag prevent symbols from all archive libraries from being exported in a shared library (exports them as hidden) in addition the linker also binds references to the library symbols within the shared library to their definitions within the same shared library as is done with `Bsymbolic`, the result is that regardless of the order of linking we will now get the desired result, as the `basefunc` in `somelib` is no longer exposed and it is resolved already to be used inside `somelib`.
