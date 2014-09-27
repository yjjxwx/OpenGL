SRCPATH = ./
SHAREDPATH = ../GLTools/src/
SHAREDINCPATH = ..//GLTools/include
LIBDIRS = -L/usr/X11R6/lib -L/usr/X11R6/lib64 -L/usr/local/lib
INCDIRS = -I$(SHAREDINCPATH)  -I$(SHAREDINCPATH)/GL \
	-I/usr/include -I/usr/local/include -I/usr/include/GL

CC = g++
CFLAGS = $(COMPILERFLAGS) -g $(INCDIRS)
LIBS = -lX11 -lglut -lGL -lGLU -lGLEW -lm

prog : $(MAIN)
glew.o    : $(SHAREDPATH)glew.c
GLTools.o    : $(SHAREDPATH)GLTools.cpp
GLBatch.o    : $(SHAREDPATH)GLBatch.cpp
GLTriangleBatch.o    : $(SHAREDPATH)GLTriangleBatch.cpp
GLShaderManager.o    : $(SHAREDPATH)GLShaderManager.cpp
$(MAIN).o : $(SRCPATH)$(MAIN).cpp
math3d.o    : $(SHAREDPATH)math3d.cpp
$(MAIN) : $(MAIN).o glew.o
	$(CC) $(CFLAGS) -o $(MAIN).e $(LIBDIRS) $(SRCPATH)$(MAIN).cpp \
	$(SHAREDPATH)glew.c $(SHAREDPATH)GLTools.cpp $(SHAREDPATH)GLBatch.cpp $(SHAREDPATH)GLTriangleBatch.cpp $(SHAREDPATH)GLShaderManager.cpp $(SHAREDPATH)math3d.cpp $(LIBS)
clean:
	rm -f *.o