
g++ -I./include src/hello.cpp src/glad.c \
    src/shader.cpp src/stb_image.cpp \
    -lglfw -ldl -lX11 -lpthread \
    && ./a.out
