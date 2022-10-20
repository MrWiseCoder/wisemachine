include ./.env

cc=gcc

build_dir=./build
sdir=./other
odir=./objects
idir=/usr/include
adir=./assembly

source=$(wildcard *.c)
target=$(PROJECT_NAME)
headers=$(wildcard *.h)

prog:=${source:.c=}
objs:=${source:.c=.o}
asms:=${source:.c=.s}
obj=$(patsubst %,$(odir)/%,$(objs))
asf=$(patsubst %,$(adir)/%,$(asms))

pager=bat
tagFile=./.tags

inc=-I$(idir)
libs=-lm

opts=-g -std=c99 -pedantic -Wall -Wextra -Werror -Wshadow -Wconversion -Wunreachable-code
# TODO: use $(compile) in the rules
compile=$(cc) $(opts) $(inc) $(libs)

release_flags=-o
debug_flags=-Wall -pedantic -g -o
oFlags=-c -o
aFlags=-S -o

release_dir=$(build_dir)/release
debug_dir=$(build_dir)/debug

release_target=$(release_dir)/$(target)
debug_target=$(debug_dir)/$(target)

targets=$(release_target) $(debug_target)

all: $(targets)	## Builds all targets

$(release_target): $(source) $(release_dir) ## Builds release binary
	$(cc) $(release_flags) $@ $<

$(odir)/%.o: %.c $(odir) $(debug_dir)	## Builds debug objects
	$(cc) $(oFlags) $@ $<

$(debug_target): $(obj)	## Builds debug binary
	$(cc) $(debug_flags) $@ $^ $(libs) 
	# echo $(obj)

# $(debug_target): $(odir)/$(objs)	## Builds debug binary
	# $(cc) $(debug_flags) $@ $?
	# $(cc) $(debug_flags) $@ $^
	# $(cc) $(debug_flags) $@ $+

.PHONY: asm

asm: $(asf)	## Check contents of asm files
	@bat $(asf)

$(adir)/%.s: %.c $(adir)	## Creates assembly files
	$(cc) $(aFlags) $@ $<

debug: $(debug_target)	## Create debug binary

release: $(release_target)	## Create release binary

$(release_dir): $(build_dir)
	@mkdir $(release_dir)

$(debug_dir): $(build_dir)
	@mkdir $(debug_dir)

$(build_dir):
	@mkdir $(build_dir)

$(odir):
	@mkdir $(odir)

$(adir):
	@mkdir $(adir)

run:	## Runs released version of the binary
	$(release_target)

t: clean debug tests	## Shortcut to clean-debug-tests

tests:	## Runs several tests in the main entrypoint
	$(debug_target)

checkMemoryLeaks:	## Checks memory leaks via Valgrind
	@valgrind $(debug_target)

gdb:	## Starts debugger (gdb) for analysis
	@gdb $(debug_target)

repl: ## Starts C interpreter for interactive analysis of the source files
	@cint $(source)

dump:	## Dump binary details to stdout
	@objdump -s -g $(debug_target) | $(pager)

hex:	## Show hex values of the debug-binary
	@hexdump $(debug_target) | $(pager)

elf:	## Show executable linkable file details
	@readelf -a $(debug_target)

mem:	## Show colorful memory layout of the binary
	@nm $(debug_target) | $(pager)

rad:	## Search binary details
	@radare2 $(debug_target)

scope: $(sdir)	## Searches labels and symbols in the code
	@cscope $(target) -f $(sdir)/cscope.out

tag: $(sdir) ## Creates a tag file for IDEs to jump between symbols
	@ctags -f $(sdir)/$(tagFile) -I -R . --languages=C

style:	## Re-style the source file
	@astyle *.c *.h

$(sdir):
	@mkdir $(sdir)

clean:
	rm -rf $(build_dir)
	rm -rf $(sdir)
	rm -rf $(odir)
	rm -rf $(adir)
	rm -f *.orig

.PHONY: help phonies 

help: ## Prints help for targets with comments
	@cat $(MAKEFILE_LIST) | grep -E '^[a-zA-Z_-]+:.*?## .*$$' | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

readf:	## Show .env file contents
	@echo $(PROJECT_NAME)
	@bat .env

