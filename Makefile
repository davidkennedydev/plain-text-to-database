SRC=src
LIB=lib

CXX = g++
DEBUG_FLAGS ?=
HEADERS = $(SRC)
FLAGS = -std=c++1y -I $(HEADERS) $(DEBUG_FLAGS)

all: $(LIB)/plain_text_to_database.o


$(LIB)/plain_text_to_database.o: $(SRC)/plain_text_to_database/processor.cc $(SRC)/plain_text_to_database/region.h
	$(CXX) $(FLAGS) -c $< -o $@ 

clean:
	-@rm -rf $(LIB)/*.o $(LIB)/*.so $(LIB)/*.a
