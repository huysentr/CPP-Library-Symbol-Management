# Example 5

In example 4 managed hide and resolve all symbols from the static dependency, however what if we want to be selective?

## Setup
We now use the source in *example_src_b*:

Modified setup of example 2:
- add `__attribute__((visibility("hidden")))` is added as prefix to the basefunc1; this has the same effect as the `--exclude-libs` but only for this specific symbol.

## Running
`make execute`
```
...
./myapp
baselibA
baselibB
```

### Breakdown of the result
`__attribute__((visibility("hidden")))` has the same effect as the `--exclude-libs` but only for this specific symbol. Additionally we can hide symbols on the level of object file of `baselibA`.
```
libbaselibA.o: baselibA.cxx baselib.h Makefile
	g++ -fPIC -c -o $@ $< -fvisibility=hidden
```
Now all symbols are hidden we no longer need to exclude libs, we can turn on specific symbols by using: `__attribute__((visibility("default")))`