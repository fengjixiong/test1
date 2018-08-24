
DIR_INC  = -I./inc \
		   -I./ref/kdtree
DIR_LIB  = ./ref/kdtree
DIR_SRC = ./src
DIR_OBJ = ./obj
DIR_BIN = ./bin

SRC = $(wildcard ${DIR_SRC}/*.c)  
OBJ = $(patsubst %.c,${DIR_OBJ}/%.o,$(notdir ${SRC})) 
LIB_FILES = libkdtree.a

TARGET = main

BIN_TARGET = ${DIR_BIN}/${TARGET}

CC = gcc
CFLAGS = -g -w ${DIR_INC} -L${DIR_LIB} -l${LIB_FILES}

${BIN_TARGET}:${OBJ}
	    $(CC) $(OBJ)  -o $@
			    
${DIR_OBJ}/%.o:${DIR_SRC}/%.c
	    $(CC) $(CFLAGS) -c  $< -o $@

.PHONY:clean

clean:
	    rm ${DIR_OBJ}/*.o
