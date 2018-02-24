#OBJS specifies which files to compile as part of the project
OBJS = keypress.c

#CC specifies which compiler we're using
CC = gcc

#COMPILER_FLAGS specifies the additional compilation options were using
# -w suppresses all warnings
COMPILER_FLAGS = -g -Wall

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = program

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
