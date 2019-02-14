PROG?=a
XEXT=exe

ifneq ($(INPUT),)
FEED = cat $(INPUT) |
endif
ifneq ($(OUTPUT),)
FEEDBACK = > $(OUTPUT)
endif

all: build run

build:
	g++ -std=c++11 -I.. -g -O0 $(PROG) -o $(subst cpp,$(XEXT),$(PROG))
	
run:
	$(FEED) ./$(subst cpp,$(XEXT),$(PROG)) $(FEEDBACK)
