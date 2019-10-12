INC_DIR=./inc
OBJ_DIR=./obj
LIB_DIR=./lib
SRC_DIR=./src
BIN_DIR=./bin

CC=gcc
RM=rm -f

TARGET=roaredb

CFLAGS=-I${INC_DIR} -I${CUTLERY_PATH}/inc -I${BOOMPAR_PATH}/inc -I${CONNMAN_PATH}/inc

LIB_FL=-L${CUTLERY_PATH}/bin -L${BOOMPAR_PATH}/bin -L${CONNMAN_PATH}/bin

${OBJ_DIR}/%.o : ${SRC_DIR}/%.c ${INC_DIR}/%.h ${CUTLERY_PATH}/inc ${BOOMPAR_PATH}/inc ${CONNMAN_PATH}/inc
	${CC} ${CFLAGS} -c $< -o $@

${BIN_DIR}/$(TARGET) : ${OBJ_DIR}/main.o ${OBJ_DIR}/jenkinshash.o ${OBJ_DIR}/query.o ${OBJ_DIR}/data.o ${OBJ_DIR}/command.o
	gcc $@ ${OBJ_DIR}/*.o ${CFLAGS} ${LIB_FL} -lcutlery -lboompar -lconnman

all: ${BIN_DIR}/$(TARGET)

clean :
	$(RM) $(BIN_DIR)/* $(OBJ_DIR)/*
