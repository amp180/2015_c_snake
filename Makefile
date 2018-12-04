#!/usr/bin/make -f
NCURSESW_CFLAGS:=$(shell pkg-config ncursesw --cflags)
NCURSESW_LDFLAGS:=$(shell pkg-config ncursesw --libs)
VERSION:=0.0.1
SOURCES:=$(shell find src/lib/**.c)
OBJECTS:= $(patsubst src/lib/%, build/lib/%, $(SOURCES:.c=.o))
LIBSOUT:=$(foreach obj, $(subst build/lib/,  , $(OBJECTS:.o=)), -l$(obj) )
CFLAGS:= $(NCURSESW_CFLAGS) -Ibuild/include
LDFLAGS:=$(NCURSESW_LDFLAGS) -Lbuild/lib

all: build/bin/csnek build/csnek.pc

build/bin/csnek: $(OBJECTS) src/csnek.c | build
	cc -Wall $(CFLAGS) $^ -o $@ $(LDFLAGS) 

$(OBJECTS): build/lib/%.o: src/lib/%.c | src/include/%.h build  # Wildcard rule on static target.
	cc -Wall $(CFLAGS) -c $< -o $@ 

build: 
	mkdir -p $@/bin/
	mkdir -p $@/lib/
	cp -rp src/include/ $@/include

build/csnek.pc: $(OBJECTS)
	echo 'prefix=$(shell pwd)/build' >$@
	echo 'exec_prefix=$${prefix}/build/bin'>>$@
	echo 'includedir=$${prefix}/build/include'>>$@
	echo 'libdir=$${prefix}/build/lib'>>$@
	echo >>$@
	echo 'Name: csnek'>>$@
	echo 'Description: A simple snake game.'>>$@
	echo 'Version: $(VERSION)' >>$@
	echo 'Requires: ncursesw' >>$@
	echo 'Cflags: -I$$(includedir)'>>$@
	echo 'Libs: -L$${libdir} $(LIBSOUT)'>>$@

.PHONY: clean
clean:
	rm -rf build

## Notes:
# Makefile job syntax is:
# - `target : prereqs (| order-only prereqs)? \n(\t job\n)*`
# - `=` is var assignment
# - `:=` is var assignment, immediately evaluates, no recursive evaluation.
# - `$$` is an escaped $
# - `$()` is variable substitution/function call
# - `|` in prereqs means everything after is an order-only prereq.
# - `%` is a wildcard that's captured, used for wildcard rules and substitutions.
# Targets only run if there's no file with the same name, unless .PHONY depends on them.
# Prereqs that are targets or filenames are built first.
# Automatic variables are:
# - `$@` is the target.
# - `$<` is the first prereq.
# - `$^` is all prereqs.
