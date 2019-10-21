CC=g++
LD=g++
PROG?=a
OPT_LEVEL?=O0
XEXT=exe

ifneq ($(INPUT),)
FEED = cat $(INPUT) |
endif
ifneq ($(OUTPUT),)
FEEDBACK = > $(OUTPUT)
endif

SRCDIRS=. .. ../..
ifneq ($(SRCDIRS_PLUS),)
SRCDIRS+=$(SRCDIRS_PLUS)
endif
INCDIRS_=.. ../..
ifneq ($(INCDIRS_PLUS),)
INCDIRS_+=$(INCDIRS_PLUS)
endif
INCDIRS=$(addprefix -I,$(INCDIRS_))
OBJDIR=./objs
BINDIR=./bin
DEPDIR=$(OBJDIR)

SRCS=
OBJS=$(SRCS:%.cpp=$(OBJDIR)/%.o)
DEPS=$(SRCS:%.cpp=$(DEPDIR)/%.d)
BIN=$(BINDIR)/$(PROG).$(XEXT)

CFLAGS=-std=c++17 -Wall $(DEFINES) $(INCDIRS)  -g -$(OPT_LEVEL)

ifeq ($(DEBUG_MACROS),1)
CFLAGS+=-D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC
endif

ifeq ($(SANITIZE),1)
CFLAGS+=-fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -fno-optimize-sibling-calls
endif

LDFLAGS=$(CFLAGS)

all: build run

build: $(BIN)

$(BIN): $(OBJS) | $(BINDIR)
	@echo [Making $(PROG)] Linking $@...
	$(LD) $(LDFLAGS) $^ -o $@ 2> ./$@.log
	@-$(RM) $(OBJDIR)/*.log $(BINDIR)/*.log

$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	@echo [Making $(PROG)] Compiling $<...
	$(CC) -c $(CFLAGS) -MMD -MP $< -o $@ 2> ./$@.log

run:
	@echo [Making $(PROG)] Launching...
	@echo -----------------------
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
