g++ -O3 main.cpp shader.cpp cube.cpp player.cpp scene.cpp rectangle_model.cpp image.cpp glad.c -o main.exe -Iinclude  -Wl,--subsystem,windows -L. -lglfw3

pause