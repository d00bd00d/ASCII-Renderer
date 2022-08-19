CXX := g++ 
CPPFLAGS := -Wall 

ASCII: main.o canvas.o triangle.o
	$(CXX) $(CPPFLAGS) -o ASCII -lncurses triangle.o canvas.o main.o 

main.o: main.cpp canvas.h
	$(CXX) $(CPPFLAGS) -c main.cpp 

canvas.o: canvas.cpp canvas.h 
	$(CXX) $(CPPFLAGS) -c canvas.cpp

triangle.o: triangle.cpp triangle.h vector.h rotater.h
	$(CXX) $(CPPFLAGS) -c triangle.cpp
