

Lsys: src/*.cpp
	g++ -o Lsys src/main.cpp src/glad.c src/ShaderObject.cpp src/ShaderProgram.cpp src/logging.cpp src/lindenmayer.cpp -Iinclude -lGL -lglfw -ldl
