#list of all the directories, in the project
INC_DIR=./inc
OBJ_DIR=./obj
LIB_DIR=./lib
SRC_DIR=./src
BIN_DIR=./bin

CC=gcc
RM=rm -f

# figure out all the sources in the project
SOURCES:=${shell find $(SRC_DIR) -name '*.c'}
# and the required objects ot be built
OBJECTS:=$(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.o,${SOURCES})
# the traget library
TARGET_LIB:=${LIB_DIR}/libcashed.a
# the traget cashed caching server
TARGET:=${BIN_DIR}/cashed.out

# compiler flags
CFLAGS=-Wall -O3 -I${INC_DIR}
# linker flags
LFLAGS=-lcashed -lrwlock -lconnman -lpthread -lboompar -lcutlery

# rule to make the object directory
${OBJ_DIR} :
	mkdir -p $@

# rule to make the directory for library
${LIB_DIR} :
	mkdir -p $@

# rule to make the directory for binary
${BIN_DIR} :
	mkdir -p $@

# generic rule to build any object file
${OBJ_DIR}/%.o : ${SRC_DIR}/%.c ${INC_DIR}/%.h | ${OBJ_DIR}
	${CC} ${CFLAGS} -c $< -o $@

# generic rule to make a library target
$(TARGET_LIB) : ${OBJECTS} | ${LIB_DIR}
	ar rcs $@ ${OBJECTS}

# generic rule to compile main with the library
$(TARGET) : ./main.c $(TARGET_LIB) | ${BIN_DIR}
	${CC} ${CFLAGS} $< ${LFLAGS} -o $@

# just build the target
all : $(TARGET)

# clean all the build, in this directory
# does not remove the existing installation
clean :
	$(RM) -r $(BIN_DIR) $(LIB_DIR) $(OBJ_DIR)

# install the library and the server, from this directory to user environment path
install : all
	cp ${INC_DIR}/* /usr/local/include
	cp ${LIB_DIR}/* /usr/local/lib
	cp ${BIN_DIR}/* /usr/local/bin