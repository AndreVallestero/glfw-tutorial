#awesome example here: https://codereview.stackexchange.com/questions/78855/makefile-for-c-opengl-with-glfw-and-glad
#and here: https://stackoverflow.com/questions/5178125/how-to-place-object-files-in-separate-subdirectory

CC = g++

#glfw = ../../dependencies/glfw-3.2	# windows
#glfw_inc = $(glfw)/include			# windows
#glfw_lib = $(glfw)/lib64			# windows

glfw_inc = /usr/include				# linux
glfw_lib = /usr/lib					# linux

INCLUDES = -I$(glfw_inc)
LIBRARIES = -L$(glfw_lib)

CPPFLAGS = -Wall -ggdb3 -Og $(INCLUDES)
#LDFLAGS = -lglfw3 -lopengl32 -lgdi32 $(LIBRARIES)	# window
LDFLAGS = -lglfw -lGL $(LIBRARIES)					# linux

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