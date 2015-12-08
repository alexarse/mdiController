UNAME := $(shell uname)
## MAC
ifeq ($(UNAME), Darwin)
CC = clang++
CC_FLAGS = -std=c++11 -Wall
CC_LINK = -shared -undefined dynamic_lookup
INCLUDE_SRC = -Iinclude -I../../include/ -I/usr/local/include/freetype2/ -I/usr/local/include/ -I../include/
LINKER_FLAG = -lpng -laxLibCore -lfreetype -laxExec

else 
#ifeq ($(UNAME), Linux)

CC = g++-4.9
CC_FLAGS = -std=c++14 -DANDROID -DGL_GLEXT_PROTOTYPES=1 -fpermissive -fPIC
CC_LINK = -shared
INCLUDE_SRC = -Iinclude/ -I../../include/ -I/usr/include/freetype2/ -I/usr/local/include/ -I/opt/vc/include/ -I/opt/vc/include/interface/vcos/pthreads -I /opt/vc/include/interface/vmcs_host/linux -I/usr/include/

LINKER_INCLUDE =  -L/usr/local/lib/ -L/opt/vc/lib/ -L/usr/lib/arm-linux-gnueabihf/
LINKER_FLAGS = -lpng -laxLibCore -lfreetype -lEGL -lGLESv2 -lpthread -lopenmaxil -lbcm_host -ldl -lutil
endif

PLUGIN_NAME = Button

OBJ_DIR = build
SRC_DIR = .
INC_DIR = .

CPP_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(addprefix $(OBJ_DIR)/,$(notdir $(CPP_FILES:.cpp=.o)))


all: create_dir $(OBJ_FILES)
	$(CC) $(CC_LINK) $(LINKER_INCLUDE) $(LINKER_FLAGS) -o ../../plugin/Button/$(PLUGIN_NAME).ctrl $(OBJ_FILES)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CC_FLAGS) $(INCLUDE_SRC) -c -o $@ $<

create_dir:
	@mkdir -p $(OBJ_DIR)

clean:
	rm -f $(OBJ_DIR)/*.o
