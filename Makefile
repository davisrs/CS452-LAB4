run: main.cpp
	g++ main.cpp -lglut -lGLU -lGL -lGLEW -g
	
clean: 
	rm -f *.out *~ run
