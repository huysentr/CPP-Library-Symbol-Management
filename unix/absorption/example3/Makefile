SRC_DIR = ../example_src_a

execute: myapp Makefile
	./myapp

myapp: $(SRC_DIR)/myapp.cxx libsomelib.so libbaselibB.so Makefile
	g++ -o $@ $< -L . -lbaselibB  '-Wl,-rpath,$$ORIGIN'

libbaselibA.o: $(SRC_DIR)/baselibA.cxx $(SRC_DIR)/baselib.h Makefile
	g++ -fPIC -c -o $@ $<

libbaselibA.a: libbaselibA.o Makefile
	ar rvs $@ $<

libbaselibB.so: $(SRC_DIR)/baselibB.cxx $(SRC_DIR)/baselib.h Makefile
	g++ -fPIC -shared -o $@ $<

libsomelib.so: $(SRC_DIR)/somelib.cxx $(SRC_DIR)/somelib.h libbaselibA.a Makefile
	g++ -shared -o $@ $< -L . -lbaselibA -Wl,-Bsymbolic

clean:
	rm -f libbaselibA.o libbaselibA.a libbaselibB.so libsomelib.so myapp
