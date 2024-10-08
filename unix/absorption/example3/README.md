# Example 3

In example 2 we saw that the order of linkage mattered to the print results of the `myapp` executable. We got either A twice or B twice, but never what we wanted which was A and B.

## Setup
Modified setup of example 2:
- add -`BSymbolic` to `somelib`
- place `baselibB` first when linking to `myapp`

## Running
`make execute`
```
...
./myapp
baselibA
baselibB
```

### Breakdown of the result
The `-Bsymbolic` flag instructs the linker to bind references to global symbols within the shared library to their definitions within the same shared library. This means that if a shared library contains a reference to a global symbol that is also defined within the same library, the reference will be resolved to the internal definition rather than an external one. This can also improve performance by reducing the amount of relocations needed at runtime. However this is not as robust as one might hope, as demonstrated below we switch the linking order so `somelib` is first:

----------------------
`g++ -o $@ $< -L . -lsomelib -lbaselibB '-Wl,-rpath,$$ORIGIN'`
`make execute`
```
...
./myapp
baselibA
baselibA
```
----------------------
While we fixed the call within `somelib` we cannot do so for `myapp`, as it cannot discriminate between the symbol from `somelib` and `baselibB`. 