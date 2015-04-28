CPPFLAGS = -Wall -I/opt/local/include/ -std=c++11 -g
FRAMEWORKS = -L/opt/local/lib/ -framework OpenGL -framework Cocoa -lglfw -lGLEW -lfftw3
DEPS = WindowHandler.hpp GraphicsHandler.hpp GL_utilities.hpp ProgramGraphics.hpp shaders.hpp models.hpp loadobj.hpp LoadTGA.hpp camera.hpp lighting.hpp misc.hpp terrain.hpp physics.hpp

main: main.o WindowHandler.o GL_utilities.o ProgramGraphics.o shaders.cpp models.cpp loadobj.cpp LoadTGA.cpp camera.cpp lighting.cpp terrain.cpp physics.cpp
	clang++ $(CPPFLAGS) $(FRAMEWORKS) -o main $^

%.o: %.cpp $(DEPS)
	clang++ -c -o $@ $< $(CPPFLAGS)

clean:
	rm -f *.o main 