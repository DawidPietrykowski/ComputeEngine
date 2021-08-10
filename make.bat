@echo off 

set GLFW_ROOT=
set GLEW_ROOT=
set stb_image_ROOT=

cmake -B build -S . -DGLFW_ROOT=%GLFW_ROOT% -DGLEW_ROOT=%GLEW_ROOT% -Dstb_image_ROOT=%stb_image_ROOT%
cmake --build build --config Release