# Example 2

## Setup
Modified setup of example 1:
- **ADD FPIC**: we add `fpic` to `baselibB` and `baselibA`, and not to `somelib.so` because it already implicitly uses `fpic` as it depends on `baselibA` which uses `fpic`.

## Running
`make execute`
```
...
./myapp
baselibA
baselibA
```

### Breakdown of the result
Read the static object file from libA:
`readelf -r libbaselibA.o`
```
Relocation section '.rela.text' at offset 0x540 contains 11 entries:
  Offset          Info           Type           Sym. Value    Sym. Name + Addend
000000000007  000500000002 R_X86_64_PC32     0000000000000000 .rodata - 3
...
```
For the non fpic:
```
Relocation section '.rela.text' at offset 0x4f0 contains 11 entries:
  Offset          Info           Type           Sym. Value    Sym. Name + Addend
000000000005  00050000000a R_X86_64_32       0000000000000000 .rodata + 1
...
```
Notice that the relocation type is different, allowing the library to be linked with a shared library.
We can see that the output is `"baselibA"` twice. Looking at our app we expected `"baselibA"` and `"baselibB"`. 
To explain this lets look at the loaded dynamic dependencies of myapp. Run `ldd myapp`
```
linux-vdso.so.1 (0x00007ffdaed71000)
libsomelib.so => cpp-library-symbol-management/unix/absorption/example2/./libsomelib.so (0x00007f3c7e30a000)
libbaselibB.so => cpp-library-symbol-management/unix/absorption/example2/./libbaselibB.so (0x00007f3c7e108000)
libstdc++.so.6 => /lib64/libstdc++.so.6 (0x00007f3c7dd73000)
libm.so.6 => /lib64/libm.so.6 (0x00007f3c7d9f1000)
libgcc_s.so.1 => /lib64/libgcc_s.so.1 (0x00007f3c7d7d9000)
libc.so.6 => /lib64/libc.so.6 (0x00007f3c7d403000)
/lib64/ld-linux-x86-64.so.2 (0x00007f3c7e50c000)
```
The relevant shared objects are  `libsomelib.so` and `libbaselibB.so`. The latter is loaded last and this is the reason `"baselibB"` is not part of the output. This is because both libs contain the same symbol, the symbol from `baselibA` is exposed in `somelib`!

Run `objdump -T libsomelib.so`:
```
libsomelib.so:     file format elf64-x86-64

DYNAMIC SYMBOL TABLE:
0000000000000000  w   DF *UND*  0000000000000000  GLIBC_2.2.5 __cxa_finalize
0000000000000000      DF *UND*  0000000000000000  GLIBCXX_3.4 _ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_
0000000000000000      DF *UND*  0000000000000000  GLIBC_2.2.5 __cxa_atexit
0000000000000000      DF *UND*  0000000000000000  GLIBCXX_3.4 _ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
0000000000000000      DF *UND*  0000000000000000  GLIBCXX_3.4 _ZNSolsEPFRSoS_E
0000000000000000      DO *UND*  0000000000000000  GLIBCXX_3.4 _ZSt4cout
0000000000000000      DF *UND*  0000000000000000  GLIBCXX_3.4 _ZNSt8ios_base4InitC1Ev
0000000000000000  w   D  *UND*  0000000000000000  Base        _ITM_deregisterTMCloneTable
0000000000000000  w   D  *UND*  0000000000000000  Base        __gmon_start__
0000000000000000  w   D  *UND*  0000000000000000  Base        _ITM_registerTMCloneTable
0000000000000000      DF *UND*  0000000000000000  GLIBCXX_3.4 _ZNSt8ios_base4InitD1Ev
0000000000201048 g    D  .got.plt       0000000000000000  Base        _edata
0000000000000899 g    DF .text  000000000000000c  Base        somefunc
0000000000201050 g    D  .bss   0000000000000000  Base        _end
0000000000201048 g    D  .bss   0000000000000000  Base        __bss_start
00000000000008a5 g    DF .text  0000000000000032  Base        basefunc
```
We can see the `basefunc` symbol (at the end) of shared object. The dynamic linker will go in order through all loaded libraries when looking up a symbol, each time it will start the process at the start of the list. It will go through `somelib` first and always find that symbol before ever reaching `baselibB`. Changing the linkin order will also change which library is atop of the list.

--------------------
*As an exercise switch the position of linking of `somelib` and `baselibB` in:
`g++ -o $@ $< -L . -lsomelib -lbaselibB '-Wl,-rpath,$$ORIGIN'`
to
`g++ -o $@ $< -L . -lbaselibB -lsomelib '-Wl,-rpath,$$ORIGIN'`*

`make execute`
```
...
./myapp
baselibB
baselibB
```
----------------------