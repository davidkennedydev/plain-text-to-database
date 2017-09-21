SRC=src
LIB=lib
INCLUDE=include

CXX = g++
DEBUG_FLAGS ?=
FLAGS = -std=c++1y -I $(INCLUDE) $(DEBUG_FLAGS)

all: $(LIB)/plain_text_to_database.o


$(LIB)/plain_text_to_database.o: $(SRC)/processor.cc $(INCLUDE)/region.h
	$(CXX) $(FLAGS) -c $< -o $@ 

clean:
	-@rm -rf $(LIB)/*.o $(LIB)/*.so $(LIB)/*.a
