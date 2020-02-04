
g++ -I./include src/hello.cpp src/glad.c \
    src/shader.cpp \
    -lglfw3 -ldl -lX11 -lpthread \
    && ./a.out
