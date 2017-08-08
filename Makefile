###
#	Project name:	USBID-parser
#	Brief:			Converts usbid to vendor name or interface id to interface name
#	Version:		0.0.1
#	Author:			Marek Tamaskovic
#	Email:			tamaskovic.marek@gmail.com
###

CXX=g++
CXXFLAGS=-Wall -Wextra -pedantic -std=c++11 -g3 -fdiagnostics-color=always

BIN=USBID-parser
OBJECTS= main.o USBIDs.o

all: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(BIN)

run:
	./$(BIN)

run_id:
	./$(BIN) id 0x1 0x1

run_inter:
	./$(BIN) interface 0x1 0x1 0x1

run_cov: run_id run_inter
	./$(BIN) --help

coverage: CXXFLAGS += -fprofile-arcs -ftest-coverage
coverage: all run_cov
	lcov --capture --directory ./ --output-file coverage.info
	genhtml coverage.info --output-directory=./html

clean: clear

clear:
	rm *.o $(BIN) *.gcda *.gcno coverage.info
	rm -rf ./html
