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

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; \
		$(CC) $(CFLAGS) $(INC) -c -o $@ $<

.PHONY: clean test

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) "; $(RM) -r $(BUILDDIR)

test: $(OBJECTS)
	$(CC) $(CFLAGS) test/test.cpp $(SOURCES) $(INC) -o bin/test
