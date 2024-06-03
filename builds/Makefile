CXX = g++ 
INC_PATH = ././includes .././includes/freetypeinc .././includes/freetypeinc/freetype
APP_NAME = ./Exe/test
OBJ_DIR = ./obj/
LINKER_MAC = .././lib/mac/libglfw.3.3.dylib .././lib/mac/libfreetype.6.dylib
LINKER_WIN = .././lib/win/libglfw3dll.a .././lib/win/libfreetype.dll.a

CFLAGS = --std=c++11 -Wall -Wextra $(foreach D, $(INC_PATH), -I$(D))
PLATFORM_DIR_OFF = ../Platform/OpenGL/
SANDBOX_DIR_OFF = ../Sandbox/
UNAME_S := $(shell uname -s)

# src files inside the sandbox
SRC_FILES = glad main

CPP_FILE_LIST = $(wildcard $(PLATFORM_DIR_OFF)*.cpp)
H_FILE_LIST = $(wildcard $(PLATFORM_DIR_OFF)*.h)

PLATFORM_OBJ = $(patsubst $(PLATFORM_DIR_OFF)%.cpp, $(OBJ_DIR)%.obj, $(CPP_FILE_LIST)) 
SRC_OBJ = $(patsubst %.cpp, $(OBJ_DIR)%.obj, $(foreach D, $(SRC_FILES), $(D).cpp))

# Short hand for combining all the object file into one executable form
WIN_COMMAND = ${CXX} ${CFLAGS} $(foreach D, $(LINKER_WIN), -L. $(D))
MAC_COMMAND = ${CXX} ${CFLAGS} $(foreach D, $(LINKER_MAC), -L. $(D))

build_m: $(APP_NAME)

$(APP_NAME) : $(SRC_OBJ) $(PLATFORM_OBJ) 
ifeq (${UNAME_S}, Darwin)
	${MAC_COMMAND} -o $@ $^ 
else
	$(WIN_COMMAND) -o $@ $^ 
endif

$(OBJ_DIR)%.obj: $(PLATFORM_DIR_OFF)%.cpp 
	$(CXX) $(CFLAGS) -c -o $@ $<
	
$(OBJ_DIR)%.obj: $(SANDBOX_DIR_OFF)%.cpp 
	$(CXX) $(CFLAGS) -c -o $@ $<

clean_build: clean build_m

clean:
ifeq (${UNAME_S}, Darwin)
	rm -f $(OBJ_DIR)*.obj
else
	del -f $(OBJ_DIR)*.obj
endif