#globals
default: build
freshen: clean build
clean:
	rm -rf bin/* obj/* gen/* pkg/*

#vars
includes = 
libs = -lm -lstdc++ -lGL -lGLU -lGLEW -lglut -lfreeimage
options = -g -Wall -O0  -funsigned-char
warnings =
#compilation flags
cflags = $(includes) $(options)
#linking flags
ldflags = $(libs) $(warnings)
#other vars
package_file = pkg/kalev_lab6.zip

#includes
include deps.mk
include lists.mk

#compilation definitions
$(objects): obj/%.o : src/%.cpp
	gcc $(cflags) -c $< -o $@
$(binaries): bin/% : obj/%.o
	gcc $(ldflags) $< obj/shaders.o -o $@

#commands
build: $(binaries)
package: $(package_file)
$(package_file): $(binaries)
	zip -FSr $(package_file) \
		bin data gen obj shdr src \
		deps.mk lists.mk makefile \
		pkg/.gitignore

#tests
test: test-lab5

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
	primusrun $<
