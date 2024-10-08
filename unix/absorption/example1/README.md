# Example 1

## Setup
We create a straightforward Makefile using the source code in *example_src_a*, we do the following:
- We create two libraries `baselibA` and `baselibB`. They contain an identical function **symbol** `basefunc` that will print out their name.
- `baselibA` will be compiled as **static** and `baselibB` will be compiled as **shared**
- We will also create a **shared** library `somelib` that will have a unique symbol  `somefunc` that calls `basefunc` and as a dependency will use `baselibA`
- Create an app `myapp` that uses `somefunc`and `basefunc` and links to `somelib` and `baselibA`

## Running
`make execute`
```
g++ -c -o libbaselibA.o ../example_src_a/baselibA.cxx
ar rvs libbaselibA.a libbaselibA.o
ar: creating libbaselibA.a
a - libbaselibA.o
g++ -shared -o libsomelib.so ../example_src_a/somelib.cxx -L . -lbaselibA
/usr/bin/ld: ./libbaselibA.a(libbaselibA.o): relocation R_X86_64_32 against `.rodata' can not be used when making a shared object; recompile with -fPIC
/usr/bin/ld: final link failed: Nonrepresentable section on output
collect2: error: ld returned 1 exit status
make: *** [Makefile:19: libsomelib.so] Error 1
```

### Breakdown of the Error Message
- **/usr/bin/ld**:
This is the linker program (ld) being invoked to link the object files and libraries.
- **./libbaselibA.a(libbaselibA.o)**:
This specifies the static library (libbaselibA.a) and the object file within it (libbaselibA.o) that is causing the issue.
- **relocation R_X86_64_32**:
This indicates the type of relocation that is causing the problem. R_X86_64_32 is a relocation type used on x86_64 architectures, which involves a 32-bit absolute address.
Absolute addresses are fixed and cannot be relocated, which is problematic for shared libraries that need to be position-independent.
- **against .rodata**:
This specifies the section of the object file where the relocation is occurring. .rodata is the read-only data section, typically used for storing constants and string literals.
- **can not be used when making a shared object**:
This indicates that the relocation type R_X86_64_32 is not suitable for creating a shared library because shared libraries require position-independent code.
- **recompile with -fPIC**:
The solution suggested by the linker is to recompile the object file with the -fPIC flag. This ensures that the code is position-independent and uses relative addresses instead of absolute ones.
