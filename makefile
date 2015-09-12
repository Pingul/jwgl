CPPFLAGS = -Wall -I/opt/local/include/ -std=c++11 -g
FRAMEWORKS = -L/opt/local/lib/ -framework OpenGL -framework Cocoa -lglfw -lGLEW -lfftw3
DEPS = WindowHandler.hpp GraphicsHandler.hpp GL_utilities.hpp VisualizationGraphicsHandler.hpp shaders.hpp models.hpp loadobj.hpp LoadTGA.hpp camera.hpp lighting.hpp misc.hpp terrain.hpp physics.hpp simulation.hpp SIMFileReader.hpp

vis: visualize.o WindowHandler.o GL_utilities.o VisualizationGraphicsHandler.o shaders.cpp models.cpp loadobj.cpp LoadTGA.cpp camera.cpp lighting.cpp terrain.cpp physics.cpp simulation.cpp SIMFileReader.cpp
	clang++ $(CPPFLAGS) $(FRAMEWORKS) -o vis $^

%.o: %.cpp $(DEPS)
	clang++ -c -o $@ $< $(CPPFLAGS)

clean:
	rm -f *.o vis 