CC=g++
LD=g++
PROG?=a
XEXT=exe

ifneq ($(INPUT),)
FEED = cat $(INPUT) |
endif
ifneq ($(OUTPUT),)
FEEDBACK = > $(OUTPUT)
endif

SRCDIRS=. ..
OBJDIR=./objs
BINDIR=./bin
DEPDIR=$(OBJDIR)

SRCS=
OBJS=$(SRCS:%.cpp=$(OBJDIR)/%.o)
DEPS=$(SRCS:%.cpp=$(DEPDIR)/%.d)
BIN=$(BINDIR)/$(PROG).$(XEXT)

CFLAGS=-std=c++11 -Werror $(DEFINES) -I.. -g -O0
LDFLAGS=$(CFLAGS)

all: build run

build: $(BIN)

$(BIN): $(OBJS) | $(BINDIR)
	@echo [Making $(PROG)] Linking $@...
	$(LD) $(LDFLAGS) $^ -o $@ 2>$@.log
	@-$(RM) $(OBJDIR)/*.log $(BINDIR)/*.log

$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	@echo [Making $(PROG)] Compiling $<...
	$(CC) -c $(CFLAGS) -MMD -MP $< -o $@ 2>$@.log

run:
	@echo [Making $(PROG)] Launching...
	@$(FEED) $(BIN) $(FEEDBACK)

.PHONY: clean
clean: | $(OBJDIR) $(BINDIR)	
	@echo [Making $(PROG)] Cleaning...
	@-$(RM) $(OBJS) $(DEPS) *.stackdump $(OBJDIR)/*.log $(BINDIR)/*.log $(BIN) 2>/dev/null ||:

.PHONY: cleanall
cleanall:
	@echo [Making $(PROG)] Complete cleaning...
	@rm -rf $(OBJDIR) $(BINDIR) *.stackdump

$(OBJDIR):
	mkdir $@

$(BINDIR):
	mkdir $@

test:
	echo $(DEPS)

-include $(DEPS)
vpath %.cpp $(subst $(space),:,$(SRCDIRS))
