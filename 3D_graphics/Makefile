
Inc_Dir = .././include
СXX?= g++
CXXFLAGS?= -Wall -std=c++2a -g -I $(Inc_Dir)

export CXX CXXFLAGS


.PHONY: clean


subsystem:
	cd src && $(MAKE)

clean:
	cd src && $(MAKE) $@