# 根目录，项目中所有代码的根路径
ROOT_DIR := .

# 根目录的源文件，确保包括
ROOT_SRC := server.cpp

# 包含源文件的子目录列表
SUBDIRS := src
TPL_DIRS := 

# 构造源文件的目录路径
SRC_DIRS := $(addprefix $(ROOT_DIR)/, $(SUBDIRS))
TEMPLATE_DIRS := $(addprefix $(ROOT_DIR)/../, $(TPL_DIRS))

# 搜索源文件并为每个文件构造完整的路径
SRC := $(ROOT_SRC) \
       $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.cpp)) \
       $(foreach dir, $(TEMPLATE_DIRS), $(wildcard $(dir)/*.cpp))

# 将源文件路径转为对象文件路径
OBJ := $(SRC:.cpp=.o)

# 编译器和编译选项
CC := g++
CFLAGS := #-g -lpthread #-fsanitize=address
ifdef _DEBUG
    CFLAGS += -D _DEBUG
endif

# 为链接添加所需的库
LDLIBS := -lcrypt

# 链接生成最终的server可执行文件
server: $(OBJ)
	$(CC) $^ -o $@ $(CFLAGS) $(LDLIBS)

# 模式规则，从%.c生成%.o
%.o: %.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

# 清理规则，删除生成的二进制文件和对象文件
.PHONY: clean
clean:
	$(RM) $(OBJ)