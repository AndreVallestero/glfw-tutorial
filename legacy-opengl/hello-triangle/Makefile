RM = rm -f
ifdef OS
	RM = del /Q
endif

hello-triangle: main.o
	g++ build/main.o -o bin/hello-triangle

main.o: src/main.cpp
	g++ -c src/main.cpp -o build/main.o

clean:
	$(RM) "build/\*.o"  "bin/\*"