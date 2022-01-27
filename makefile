# project name
PROJECT_NAME:=cashed

# this is the place where we download in your system
DOWNLOAD_DIR:=/usr/local

# we may download all the public headers

# list of public api headers (only these headers will be installed)
PUBLIC_HEADERS:=cashed_client.h cashed_hashtable.h cashed_query.h cashed_command.h cashed_result.h
# the library, which we will create
LIBRARY:=lib${PROJECT_NAME}.a
# the binary, which will use the created library
BINARY_SERVER:=${PROJECT_NAME}
BINARY_CLIENT:=${PROJECT_NAME}_shell_cli

# list of all the directories, in the project
INC_DIR:=./inc
OBJ_DIR:=./obj
LIB_DIR:=./lib
SRC_DIR:=./src
BIN_DIR:=./bin

# compiler
CC:=gcc
# compiler flags
CFLAGS:=-Wall -O3 -I${INC_DIR}
# linker flags, this will used to compile the binary
LFLAGS:=-L${LIB_DIR} -l${PROJECT_NAME} -lm -lslabby -lconnman -lboompar -lrwlock -lpthread -lcutlery
# Archiver
AR:=ar rcs

# utility
RM:=rm -f
MK:=mkdir -p
CP:=cp

# sources and objects must be evaluated every time you use them
# figure out all the sources in the project
SOURCES=$(shell find ${SRC_DIR} -name '*.c')
# and the required objects to be built, as intermediary
OBJECTS=$(patsubst ${SRC_DIR}/%.c, ${OBJ_DIR}/%.o, ${SOURCES})

# rule to make the directory for storing object files, that we create
${OBJ_DIR} :
	${MK} $@

# generic rule to build any object file
${OBJ_DIR}/%.o : ${SRC_DIR}/%.c | ${OBJ_DIR}
	${CC} ${CFLAGS} -c $< -o $@

# rule to make the directory for storing libraries, that we create
${LIB_DIR} :
	${MK} $@

# generic rule to make a library
${LIB_DIR}/${LIBRARY} : ${OBJECTS} | ${LIB_DIR}
	${AR} $@ ${OBJECTS}

# rule to make the directory for storing binaries, that we create
${BIN_DIR} :
	${MK} $@

# rule to make a binary cashed server
${BIN_DIR}/${BINARY_SERVER} : ./${BINARY_SERVER}.c ${LIB_DIR}/${LIBRARY} | ${BIN_DIR}
	${CC} ${CFLAGS} $< ${LFLAGS} -o $@

# rule to make a binary cashed shell client
${BIN_DIR}/${BINARY_CLIENT} : ./${BINARY_CLIENT}.c ${LIB_DIR}/${LIBRARY} | ${BIN_DIR}
	${CC} ${CFLAGS} $< ${LFLAGS} -o $@

# to build all the binaries along with the library
all : ${BIN_DIR}/${BINARY_SERVER} ${BIN_DIR}/${BINARY_CLIENT}


# clean all the build, in this directory
clean :
	${RM} -r ${BIN_DIR} ${LIB_DIR} ${OBJ_DIR}

# -----------------------------------------------------
# INSTALLING and UNINSTALLING system wide
# -----------------------------------------------------

PUBLIC_HEADERS_TO_INSTALL=$(patsubst %.h, ${INC_DIR}/%.h, ${PUBLIC_HEADERS})

# install the library, from this directory to user environment path
# you must uninstall current installation before making a new installation
install : uninstall all
	${MK} ${DOWNLOAD_DIR}/include
	${CP} ${PUBLIC_HEADERS_TO_INSTALL} ${DOWNLOAD_DIR}/include
	${MK} ${DOWNLOAD_DIR}/lib
	${CP} ${LIB_DIR}/${LIBRARY} ${DOWNLOAD_DIR}/lib
	${MK} ${DOWNLOAD_DIR}/bin
	${CP} ${BIN_DIR}/* ${DOWNLOAD_DIR}/bin

PUBLIC_HEADERS_TO_UNINSTALL=$(patsubst %.h, ${DOWNLOAD_DIR}/include/%.h, ${PUBLIC_HEADERS})

# ** assumption is that all your public headers, libraries and binaries used 
# ** will always have your project name in them
# and this is how we figure out what to remove from the 
uninstall : 
	${RM} ${PUBLIC_HEADERS_TO_UNINSTALL}
	${RM} ${DOWNLOAD_DIR}/lib/${LIBRARY}
	${RM} ${DOWNLOAD_DIR}/bin/${BINARY_SERVER} ${DOWNLOAD_DIR}/bin/${BINARY_CLIENT}