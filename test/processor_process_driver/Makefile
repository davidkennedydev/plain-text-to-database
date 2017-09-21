ROOT=../..
SRC=$(ROOT)/src
LIB=$(ROOT)/lib

CXX = g++
DEBUG_FLAGS ?= -g
HEADERS = $(SRC)
FLAGS = -std=c++1y -I $(HEADERS) $(DEBUG_FLAGS)
LIBRARY_PATH = /usr/local/lib
LIBRARIES = $$(PKG_CONFIG_PATH="$(LIBRARY_PATH)/pkgconfig" pkg-config --cflags --libs libmongocxx) -Wl,-rpath $(LIBRARY_PATH)

BIN_PATH = bin
DRIVERS := $(shell ls *.cpp)
RUNABLE := $(patsubst %.cpp, bin/%, $(DRIVERS))

.PHONY: $(LIB)/plain_text_to_database.o 

all: $(RUNABLE) 


$(BIN_PATH):
	mkdir -p $@

$(LIB)/plain_text_to_database.o:
	cd $(ROOT) && DEBUG_FLAGS="$(DEBUG_FLAGS)" $(MAKE)

bin/%: %.cpp $(LIB)/plain_text_to_database.o | $(BIN_PATH) 
	$(CXX) $(FLAGS) $^ -o $@ $(LIBRARIES)

test: $(RUNABLE)
	for file in `ls bin/*`; do ./$$file; done

clean:
	-@rm -rf $(BIN_PATH) *.o 2> /dev/null || true
