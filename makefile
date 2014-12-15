#globals
default: build
freshen: clean build
clean:
	rm -rf bin/* obj/* gen/* pkg/*

#vars
includes = -I/usr/local/include/drawstuff -I/usr/local/include/ode
#includes = 
libs = -pthread -lode -ldrawstuff -lm -lstdc++ -lX11 -lGL -lGLU -lGLEW -lglut -lfreeimage
#lib_includes = -L/usr/local/lib/
ode_libfiles = /usr/local/lib/libode.a /usr/local/lib/libdrawstuff.a
options = -g -Wall -O0  -funsigned-char -std=gnu++11
warnings =
#compilation flags
cflags = $(includes) $(options) 
#linking flags
ldflags = $(lib_includes) $(libs) $(warnings)
#ldflags = $(libs) $(warnings)
#other vars
package_file = pkg/kalev_labs.zip
lib_file = bin/lib.a

#includes
include deps.mk
include lists.mk

#compilation definitions
$(objects): obj/%.o : src/%.cpp
	gcc $(cflags) -c $< -o $@
$(binaries): bin/% : obj/%.o $(lib_file)
	gcc $(ldflags) $< $(lib_file) $(ode_libfiles) -o $@

$(lib_file): $(lib_objects)
	ar rc $@ $(lib_objects)
	ranlib $@

$(package_file): $(binaries)
	zip -FSr $(package_file) \
		bin data gen lib obj shdr src \
		deps.mk lists.mk makefile \
		pkg/.gitignore

#commands
build: $(binaries)

package: $(package_file)

#tests
test: test-lab10

test-lab1: bin/lab1
	primusrun $<
test-lab2: bin/lab2
	primusrun $<
test-lab3: bin/lab3
	primusrun $<
test-lab4: bin/lab4
	primusrun $<
test-lab5: bin/lab5
	primusrun $<
test-lab6: bin/lab6
	primusrun $<
test-lab7: bin/lab7
	primusrun $<
test-lab8: bin/lab8
	$<
test-lab9: bin/lab9
	$<
test-lab10: bin/lab10
	primusrun $<
