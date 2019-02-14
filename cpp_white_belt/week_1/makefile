PROG?=a
XEXT=exe

all: build run

build:
	g++ -std=c++11 -g -O0 $(PROG) -o $(subst cpp,$(XEXT),$(PROG))
	
run:	
	$(subst cpp,$(XEXT),$(PROG))