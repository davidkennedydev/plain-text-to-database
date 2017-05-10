SRC=src
LIB=lib

CXX = g++
FLAGS = -std=c++1y -I $(SRC) -g
LIBRARY_PATH = /usr/local/lib
LIBRARIES = $$(pkg-config --cflags --libs libmongocxx) -Wl,-rpath $(LIBRARY_PATH)

all: $(LIB)/plain_text_to_database.o


$(LIB)/plain_text_to_database.o: $(SRC)/plain_text_to_database/processor.cc | $(SRC)/plain_text_to_database/region.h
	$(CXX) $(FLAGS) -c $^ -o $@ 

clean:
	-@rm -rf $(LIB)/*.o $(LIB)/*.so $(LIB)/*.a
