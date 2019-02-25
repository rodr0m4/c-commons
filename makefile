CC=gcc
CFLAGS=-Wall
TESTFLAGS=
SRCDIR=src
TESTDIR=tests
BUILDDIR=target
PROGNAME=c-commons
ALLBESIDESMAIN=$(shell find $(SRCDIR) -iname "*.c" -not -iname "$(PROGNAME).c" | tr '\n' ' ')

build:
	$(CC) $(CFLAGS) -g -o $(BUILDDIR)/$(PROGNAME) -I$(SRCDIR) $(SRCDIR)/*.c 
build-test:
	$(CC) -DSNOW_ENABLED -Wall -g -I$(SRCDIR) -I$(TESTDIR) -o $(BUILDDIR)/tests $(ALLBESIDESMAIN) $(TESTDIR)/*.c
run:
	./$(BUILDDIR)/$(PROGNAME)
run-valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 ./$(BUILDDIR)/$(PROGNAME)
run-test:
	./$(BUILDDIR)/tests
run-test-valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 ./$(BUILDDIR)/tests
test: clean build-test run-test
testv: clean build-test run-test-valgrind
clean:
	rm -rf $(BUILDDIR)/*
