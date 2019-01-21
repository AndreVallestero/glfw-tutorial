#awesome example here: https://codereview.stackexchange.com/questions/78855/makefile-for-c-opengl-with-glfw-and-glad
#and here: https://stackoverflow.com/questions/5178125/how-to-place-object-files-in-separate-subdirectory

CC = g++

glfw = ../../dependencies/glfw-3.2
glfw_inc = $(glfw)/include
glfw_lib = $(glfw)/lib64

INCLUDES = -I$(glfw_inc)
LIBRARIES = -L$(glfw_lib)

#compile flags and link flags
CFLAGS = -Wall -ggdb3 -Og $(INCLUDES)
LDFLAGS = -lglfw3 -lopengl32 -lgdi32 $(LIBRARIES)

TARGET = main
cpp_files = src/main.cpp
objects = $(patsubst src/%,build/%,$(cpp_files:.cpp=.o))
headers = 

all: $(TARGET)

$(TARGET): $(objects) 
	$(CC) -o bin/$@ $^ $(LDFLAGS) 

$(objects): build/%.o: src/%.cpp $(headers) makefile
	$(CC) $(CFLAGS) -c -o $@ $<