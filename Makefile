# For building EECS 211 Homework 4
# You don’t need to edit this file, and you probably shouldn’t.

# This Makefile will automatically link against the libvc.so shared
# library if src/libvc.c doesn’t exist.
#
# However, you can also enable libvc.so temporarily without renaming
# or deleting src/libvc.c by passing defining a Make variable named
# USE_SHARED_LIBVC. You can define Make variables when running Make
# as extra arguments, like this:
#
#   % make test USE_SHARED_LIBVC=1
#
# Then Make will use the shared libvc.so in place of any src/libvc.c
# that might be available. Be careful, though, because the option won’t
# take effect until the programs are relinked. You can force a relink
# (and more) by running `make clean`.
ifeq ($(wildcard src/libvc.c),)
  USE_SHARED_LIBVC=1
endif

# If we’ve been asked to use the shared library, add it to the linker
# flags; otherwise, ask it to build a libvc.o and use that.
ifdef USE_SHARED_LIBVC
  LDFLAGS   += -lvc
  LIBVC.o    =
else
  LIBVC.o    = src/libvc.o
endif

# C compiler to use:
CC         ?= cc
# Extra options for catching bad stuff:
SANFLAGS   += -fsanitize=address,undefined
# Flags for compiling individual files:
CFLAGS     += -g -std=c11 -pedantic-errors @.Wflags $(SANFLAGS)
# Uncomment/modify the next line to change the candidate capacity:
# CFLAGS    += -DMAX_CANDIDATES=11
# Flags for linking the final program:
LDFLAGS    += -l211 $(SANFLAGS)


# Test programs we can build:
TEST_EXES  = test_ballot test_ballot_box

# All programs we can build:
ALL_EXES   = $(TEST_EXES) irv

# Object files that we want to link into everything:
COMMON.o   = src/ballot.o \
             src/ballot_box.o \
             src/helpers.o \
             $(LIBVC.o)


# Build everything.
all: $(ALL_EXES)

# Build and run all tests.
test: $(TEST_EXES)
	@for prog in $^; do \
	    echo "*** Running $$prog ***"; \
	    ./$$prog || echo "Error exit: $$?" ; \
	    echo; \
	done

irv: src/irv.o $(COMMON.o)
	$(CC) -o $@ $^ $(LDFLAGS)

test_ballot: test/test_ballot.o $(COMMON.o)
	$(CC) -o $@ $^ $(LDFLAGS)

test_ballot_box: test/test_ballot_box.o $(COMMON.o)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) $(CPPFLAGS)

# When compiling C files in test/, `#include "..."` should look
# for header files in src/.
test/%.o: CPPFLAGS += -iquote src

# Remove all build products so the next build will be from scratch.
# Useful if things get weird.
clean:
	$(RM) */*.o $(ALL_EXES)

# Tells make that these are fake targets, not actually files that we can
# build.
.PHONY: all test clean

# Dependencies (the files on the left get rebuilt from .c files when the
# .h files on the right change).
src/ballot.o: src/ballot.h src/helpers.h src/libvc.h
src/ballot_box.o: src/ballot.h src/ballot_box.h src/helpers.h src/libvc.h
src/helpers.o: src/helpers.h
src/irv.o: src/ballot.h src/ballot_box.h src/libvc.h
src/libvc.o: src/libvc.h
test/test_ballot.o: src/libvc.h src/ballot.h src/helpers.h
test/test_ballot_box.o: src/libvc.h src/helpers.h
test/test_ballot_box.o: src/ballot.h src/ballot_box.h

# Ensure we have the correct C compiler version.
CSHOST = .northwestern.edu
ifeq ($(DEV211)$(CSHOST),$(findstring $(CSHOST),$(shell hostname)))
  $(error You need to run ‘dev’ first)
endif
