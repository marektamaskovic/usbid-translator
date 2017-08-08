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
	./$(BIN) id 0x1 0x7778
	./$(BIN) id 0x1 0x1
	./$(BIN) id 0x2002 0x0

run_inter:
	./$(BIN) interface 0x1 0x1 0x1
	./$(BIN) interface 0x0 0x1 0x1
	./$(BIN) interface 0x2 0x2 0x1

run_usage:
	./$(BIN) usage 0x00 0x000
	./$(BIN) usage 0x01 0x001
	./$(BIN) usage 0xfe 0x001

run_cov: run_id run_inter run_usage
	./$(BIN) --help

coverage: CXXFLAGS += --coverage
coverage: all run_cov
	lcov --capture --directory ./ --output-file coverage.info
	genhtml coverage.info --output-directory=./html

clean: clear

clear:
	rm *.o $(BIN) *.gcda *.gcno coverage.info
	rm -rf ./html
