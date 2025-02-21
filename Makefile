CXX = g++
SRC = main.cpp
OUT = mygame
CXXFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

all:
	mkdir -p build
	$(CXX) -o $(OUT) $(SRC) $(CXXFLAGS)

clean:
	rm -rf build

