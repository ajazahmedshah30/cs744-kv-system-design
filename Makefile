CXX = g++
CXXFLAGS = -std=c++17 -O2 -pthread -I/opt/homebrew/include
LIBS = -L/opt/homebrew/lib -lsqlite3
SRC = main.cpp
DEPS = cache.hpp db.hpp

all:
	$(CXX) $(SRC) -o kv_server $(CXXFLAGS) $(LIBS)

run: all
	./kv_server
	
# Clean build artifacts
clean:
	rm -f kv_server