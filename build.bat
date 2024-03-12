mkdir bin
glslc.exe src/shaders/shader.vert -o bin/vert.spv
glslc.exe src/shaders/shader.frag -o bin/frag.spv

mkdir .build
cd .build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -G "Visual Studio 17 2022" ..
