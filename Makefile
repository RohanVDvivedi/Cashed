INC_DIR=./inc
OBJ_DIR=./obj
LIB_DIR=./lib
SRC_DIR=./src
BIN_DIR=./bin

CUTLERY_PATH=../cutlery
BOOMPAR_PATH=../BoomPar
CONNMAN_PATH=../ConnMan

CC=gcc
RM=rm -f

TARGET=libroarmem.a

CFLAGS=-I${INC_DIR} -I${CUTLERY_PATH}/inc -I${BOOMPAR_PATH}/inc -I${CONNMAN_PATH}/inc

${OBJ_DIR}/%.o : ${SRC_DIR}/%.c ${INC_DIR}/%.h ${CUTLERY_PATH}/inc ${BOOMPAR_PATH}/inc ${CONNMAN_PATH}/inc
	${CC} ${CFLAGS} -c $< -o $@

${BIN_DIR}/$(TARGET) : ${OBJ_DIR}/query.o ${OBJ_DIR}/datatypes.o ${OBJ_DIR}/datastructures.o
	ar rcs $@ ${OBJ_DIR}/*.o 

all: ${BIN_DIR}/$(TARGET)

clean :
	$(RM) $(BIN_DIR)/* $(OBJ_DIR)/*
