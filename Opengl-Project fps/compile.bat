g++ -O3 main.cpp shader.cpp cube.cpp player.cpp scene.cpp image.cpp material.cpp engine.cpp game_app.cpp obj_loader.cpp obj_mesh.cpp glad.c -o main.exe -Iinclude  -Wl,--subsystem,windows -L. -lglfw3

pause