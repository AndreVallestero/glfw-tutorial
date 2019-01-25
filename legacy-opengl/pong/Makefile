#awesome example here: https://codereview.stackexchange.com/questions/78855/makefile-for-c-opengl-with-glfw-and-glad
#and here: https://stackoverflow.com/questions/5178125/how-to-place-object-files-in-separate-subdirectory

CC = g++

# Block below for Windows only
#glfw = ../../dependencies/glfw-3.2
#glfw_inc = $(glfw)/include
#glfw_lib = $(glfw)/lib64

# Block below for Linux only
glfw_inc = /usr/include
glfw_lib = /usr/lib

INCLUDES = -I$(glfw_inc)
LIBRARIES = -L$(glfw_lib)

CPPFLAGS = -Wall -ggdb3 -Og $(INCLUDES)

# Line below for Windows only
#LDFLAGS = -lglfw3 -lopengl32 -lgdi32 $(LIBRARIES)

# Line below for Linux only
LDFLAGS = -lglfw -lGL $(LIBRARIES)

TARGET = pong
cpp_files = src/main.cpp
objects = $(patsubst src/%,build/%,$(cpp_files:.cpp=.o))
headers = 

all: $(TARGET)

$(TARGET): $(objects) 
	$(CC) -o bin/$@ $^ $(LDFLAGS) 

$(objects): build/%.o: src/%.cpp $(headers) Makefile
	$(CC) $(CPPFLAGS) -c -o $@ $<

clean:
	$(RM) bin/$(TARGET) $(objects)