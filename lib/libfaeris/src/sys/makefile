# Normal Information 
CC=g++ 
FLAGS= -g -Wall -Werror -DLINUX
FILE_EXT=cc

#Project infor 
PROJECT_PATH=../../build/linux/
PROJECT=fsys

# User Config Information 
LIB_DIR=$(PROJECT_PATH)lib
OBJ_DIR=$(PROJECT_PATH)obj
SRC_DIR=./
INCLUDE_DIR=  -I./ -I../

#Source File 
SRC= FsFile.cc \
     FsSysFile.cc \
     FsVFS.cc \
	 FsSys.cc \
	 FsTimer.cc \
	 FsEventListener.cc \
	 FsFrame.cc  \
	 FsWindow.cc \
	 FsEventDispatcher.cc 
	

#full src name 
SRC_NAME_WITH_PATH=$(addprefix $(SRC_DIR)/,$(SRC))

#obj name 
OBJ_NAME=$(SRC:%.$(FILE_EXT)=%.o) 
OBJ_NAME_WITH_PATH=$(addprefix $(OBJ_DIR)/,$(OBJ_NAME))




main:$(LIB_DIR)/lib$(PROJECT).a

$(LIB_DIR)/lib$(PROJECT).a: $(OBJ_NAME_WITH_PATH) 
	ar cr $@ $(OBJ_NAME_WITH_PATH)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.$(FILE_EXT)
	$(CC) $(FLAGS) -c -o $@ $< $(INCLUDE_DIR) 


