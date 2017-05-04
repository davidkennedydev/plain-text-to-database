SRC=../src

CXX = g++
FLAGS = -std=c++1y -I $(SRC) -g
LIBRARY_PATH = /usr/local/lib
LIBRARIES = $$(pkg-config --cflags --libs libmongocxx) -Wl,-rpath $(LIBRARY_PATH)

BIN_PATH = bin
DRIVERS := $(shell ls *.cpp)
RUNABLE := $(patsubst %.cpp, bin/%, $(DRIVERS))

all: $(RUNABLE)


$(BIN_PATH):
	mkdir -p $@

bin/build_bson_driver: ./build_bson_driver.cpp | $(BIN_PATH)
	$(CXX) $(FLAGS) $^ -o $@ $(LIBRARIES)

bin/%: %.cpp plain_text_to_database.o | $(BIN_PATH)
	$(CXX) $(FLAGS) $^ -o $@ $(LIBRARIES)

plain_text_to_database.o: $(SRC)/plain_text_to_database/processor.cc | $(SRC)/plain_text_to_database/region.h
	$(CXX) $(FLAGS) -c $^ -o $@ 

test: $(RUNABLE)
	for file in `ls bin/*`; do ./$$file; done

clean:
	-@rm -rf $(BIN_PATH) *.o 2> /dev/null || true
