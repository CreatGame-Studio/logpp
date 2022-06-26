CXX = g++
OUT = prog.exe
CXX_FLAGS = -std=c++20 -Wall -Wextra -pedantic -Wno-unused-parameter
LD_FLAGS = 


SRC = $(wildcard src/*.cpp)
OBJ = ${SRC:.cpp=.o}


all : $(OUT)

obj : $(OBJ)

$(OUT) : $(OBJ)
	$(CXX) -o $@ $^ $(LD_FLAGS)

src/%.o : src/%.cpp
	$(CXX) -o $@ $< $(CXX_FLAGS) -c