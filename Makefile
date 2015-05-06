INCLUDEPATH := include
SRCPATH := src
BUILDPATH := build

CC := g++

CFLAGS := -c -std=c++11 -Wall -I$(INCLUDEPATH)
LFLAGS := -std=c++11 -Wall

OBJFILES := $(BUILDPATH)/cnf.o				\
			$(BUILDPATH)/clause.o			\
			$(BUILDPATH)/literal.o			\
			$(BUILDPATH)/predicate.o		\
			$(BUILDPATH)/predicatehasher.o 	\
			$(BUILDPATH)/parser.o			\
			$(BUILDPATH)/main.o


# targets

.PHONY: all help clean

all: $(BUILDPATH)/blocks

help:
	@echo "Usage:"
	@echo "  all       -- compile project"
	@echo "  clean     -- clean up object files and executables"
	@echo "  help      -- display this help"

clean:
	rm -r build/


# dependencies

$(BUILDPATH)/blocks: $(OBJFILES)
	$(CC) $(LFLAGS) -o $@ $^

$(BUILDPATH)/%.o: $(SRCPATH)/%.cpp
	@mkdir -p $(BUILDPATH)
	$(CC) $(CFLAGS) -o $@ $<
