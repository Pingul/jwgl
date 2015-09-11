FLAGS = -Wall -I/opt/local/include/ -I/opt/local/include/freetype2/ -Ifreetype-gl/ -g
CPPFLAGS = -std=c++11
FRAMEWORKS = -L/opt/local/lib/ -framework OpenGL -framework Cocoa -lglfw -lGLEW -lfftw3 -lfreetype
DEPS = WindowHandler.hpp GraphicsHandler.hpp GL_utilities.hpp ProgramGraphics.hpp shaders.hpp models.hpp loadobj.hpp LoadTGA.hpp camera.hpp lighting.hpp misc.hpp terrain.hpp physics.hpp
FREETYPE_OBJ_FILES = shader.o vector.o vertex-buffer.o vertex-attribute.o mat4.o texture-atlas.o texture-font.o font-manager.o

main: main.o WindowHandler.o GL_utilities.o ProgramGraphics.o shaders.o models.o loadobj.o LoadTGA.o camera.o lighting.o terrain.o physics.o
	clang++ $(FLAGS) $(CPPFLAGS) $(FRAMEWORKS) -o main $^

# it feels a little bit ugly to call this make here, but it seems to work decent for now
test: test.o WindowHandler.o GL_utilities.o ProgramGraphics.o shaders.o models.o loadobj.o LoadTGA.o camera.o lighting.o terrain.o physics.o 
	make freetype-obj
	clang++ $(FLAGS) $(CPPFLAGS) $(FRAMEWORKS) $(FREETYPE_OBJ_FILES) -o test $^

freetype-obj:
	clang $(FLAGS) -c freetype-gl/shader.c freetype-gl/vector.c freetype-gl/vertex-buffer.c freetype-gl/vertex-attribute.c freetype-gl/mat4.c freetype-gl/texture-atlas.c freetype-gl/texture-font.c freetype-gl/font-manager.c

%.o: %.cpp $(DEPS)
	clang++ -c -o $@ $< $(FLAGS) $(CPPFLAGS)

clean:
	rm -f *.o main 