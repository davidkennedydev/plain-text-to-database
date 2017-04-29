SRC=../../src

CXX=c++
CXXFLAGS=--std=c++11 -I $(SRC) -g
LIBS=$$(pkg-config --cflags --libs libmongocxx) -Wl,-rpath /usr/local/lib

all:

%.out:%.cpp plain_text_to_database.o
	$(CXX) $(CXXFLAGS) $^ $| $(LIBS) -o $@

plain_text_to_database.o: $(SRC)/plain_text_to_database/processor.cc
	$(CXX) $(CXXFLAGS) -c $^ -o $@

clean:
	rm -rf *.out *.o

