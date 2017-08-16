###
#	Project name:	USBID-parser
#	Brief:			Converts usbid to vendor name or interface id to interface name
#	Version:		0.0.1
#	Author:			Marek Tamaskovic
#	Email:			tamaskovic.marek@gmail.com
###

CXX=g++
CXXFLAGS=-Wall -pedantic -std=c++11 -O2 -fdiagnostics-color=always -m64
CXXFLAGSFUZZ=-Wall -Wextra -pedantic -std=c++14 -O2 -m64

BIN=USBID-translator
OBJECTS= main.o USBIDs.o params.o

all: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(BIN)

run:
	./$(BIN)

run_id:
	./$(BIN) id 0x1 0x7778 /usr/share/hwdata/usb.ids
	./$(BIN) id 0x1 0x1 < /usr/share/hwdata/usb.ids
	./$(BIN) id 0x2002 0x0 < /usr/share/hwdata/usb.ids

run_inter:
	./$(BIN) interface 0x1 0x1 0x1 < /usr/share/hwdata/usb.ids
	./$(BIN) interface 0x0 0x1 0x1 < /usr/share/hwdata/usb.ids
	./$(BIN) interface 0x2 0x2 0x1 /usr/share/hwdata/usb.ids

run_cov: run_id run_inter
	./$(BIN) --help
	cp content_bad content
	! ./$(BIN) id 0x1 0x7778 < ./content_bad
	! ./$(BIN) interface 0x0 0x1 0x1 < ./content_bad
	cp content_good content

coverage: CXXFLAGS += --coverage
coverage: all run_cov
	lcov --capture --directory ./ --output-file coverage.info
	genhtml coverage.info --output-directory=./html

fuzzy: CXX = afl-g++
fuzzy: CXXFLAGS = $(CXXFLAGSFUZZ)
fuzzy: all

clear: clean clear_coverage

clean:
	rm *.o $(BIN)

clear_coverage:
	rm *.gcda *.gcno coverage.info
	rm -rf ./html
