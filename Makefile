UNAME := $(shell uname)

CC = g++-4.9
CC_FLAGS = -std=c++14 -DANDROID -DGL_GLEXT_PROTOTYPES=1 -fpermissive
CC_LINK = -shared
INCLUDE_SRC = -Iinclude/ -I/usr/include/freetype2/ -I/usr/local/include/ -I/opt/vc/include/ -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux -I/usr/include/

INCLUDE_LINKER =  -L/usr/local/lib/ -L/opt/vc/lib/ -L/usr/lib/arm-linux-gnueabihf/
LINKER_FLAG = -lpng -laxLibCore -lfreetype -lEGL -lGLESv2 -lpthread -lopenmaxil -lbcm_host -ldl -lutil

OBJ_DIR = build
SRC_DIR = source

CPP_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(addprefix $(OBJ_DIR)/,$(notdir $(CPP_FILES:.cpp=.o)))

all: create_dir $(OBJ_FILES)
	$(CC) $(CC_FLAGS) $(INCLUDE_SRC) $(INCLUDE_LINKER) $(OBJ_FILES) $(LINKER_FLAG) -o mdiCtrl

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CC_FLAGS) $(INCLUDE_SRC) -c -o $@ $<

create_dir:
	@mkdir -p build

clean:
	rm -f build/*.o
