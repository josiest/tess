# Compiler
CC = g++

# Compiler flags
CFLAGS = -g -Wall

# Create sources list
SRCDIR = src
SRCEXT = cpp
SOURCES = $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))

# Create objects list
BUILDDIR = build
OBJECTS = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

# include all headers from the include folder
INC = -I include

# default make
TARGET = hexes
LIBDIR = lib

$(TARGET): $(OBJECTS)
	@mkdir -p $(LIBDIR)
	@echo " ar rcs $(LIBDIR)/libhexes.a $(OBJECTS)"; \
		ar rcs $(LIBDIR)/libhexes.a $(OBJECTS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; \
		$(CC) $(CFLAGS) $(INC) -c -o $@ $<

.PHONY: clean test

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(LIBDIR)"; $(RM) -r $(BUILDDIR) $(LIBDIR)

test: $(OBJECTS)
	$(CC) $(CFLAGS) test/test.cpp $(SOURCES) $(INC) -o bin/test
