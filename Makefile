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
	./$(BIN) id 0 0

run_inter:
	./$(BIN) interface 0 0 0

clean: clear

clear:
	rm *.o $(BIN)
