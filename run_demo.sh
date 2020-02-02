
g++ -I./include src/hello.cpp src/glad.c \
    -lglfw3 -ldl -lX11 -lpthread \
    && ./a.out
