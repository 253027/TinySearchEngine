# 根目录，项目中所有代码的根路径
ROOT_DIR := .

# 根目录的源文件
ROOT_SRC := server.cpp

# 包含源文件的子目录列表
SUBDIRS := src

# 构造源文件的目录路径
SRC_DIRS := $(addprefix $(ROOT_DIR)/, $(SUBDIRS))

# 搜索源文件并为每个文件构造完整的路径
SRC := $(ROOT_SRC) \
       $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.cpp))

# 对象文件目录
OBJ_DIR := object

# 将源文件名转化为目标对象文件路径（无论源文件位于何处）
OBJ := $(addprefix $(OBJ_DIR)/, $(notdir $(patsubst %.cpp, %.o, $(SRC))))

# 确保目标文件夹存在
$(shell mkdir -p $(OBJ_DIR))

# 编译器和编译选项
CC := g++
CFLAGS := -g #-lpthread #-fsanitize=address
ifdef _DEBUG
    CFLAGS += -D _DEBUG
endif

# 为链接添加所需的库
LDLIBS := -lcrypt

# 链接生成最终的可执行文件
server: $(OBJ)
	$(CC) $^ -o $@ $(CFLAGS) $(LDLIBS)

# 增加一个vpath，会在当前目录和所有子目录中查找.cpp文件
vpath %.cpp $(SRC_DIRS) .

# 模式规则，从%.cpp生成%.o
$(OBJ_DIR)/%.o : %.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

# 清理规则，删除生成的二进制文件和对象文件
.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) server