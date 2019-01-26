TARGET := particle_system
SRCS_DIR := ./srcs
OBJS_DIR := ./objs
CC := clang++
OS := $(shell uname)
CFLAGS := -Wall -Wextra -Werror -Wno-missing-braces
ignore-warnings : CFLAGS := -w
LIBS :=
HEADERS := -I./includes/ -I./libs/includes/

ifeq ($(OS),Darwin)
	LIBS += -framework OpenCL
else
	LIBS += -lOpenCL -lsfml-graphics -lsfml-window -lsfml-system -lGL -lglfw -ldl
	HEADERS += -I/opt/amdgpu-pro/include/
endif

# Colors
RESET := \033[0m
CYAN := \033[36;01m
GREEN := \033[32;01m
ERROR := \033[31;01m
WARN := \033[33;01m
# Formatting
CLEAR_LINE := \033[2K
MOVE_CURSOR_UP := \033[1A

SRCS := $(shell find $(SRCS_DIR) -name *.cpp)
SRCS += ./srcs/glad/glad.cpp
OBJS := $(patsubst $(SRCS_DIR)%.cpp,$(OBJS_DIR)%.o,$(SRCS))

all: $(OBJS_DIR) $(TARGET)

ignore-warnings: all

$(OBJS_DIR):
	@mkdir -p ./objs
	@mkdir -p ./objs/glad

$(TARGET): $(OBJS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	@echo "$(CYAN)Successfully compiled $(TARGET).$(RESET)"

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	@$(CC) $(CFLAGS) -c $^ -o $@ $(HEADERS)

install:
	./install.sh

uninstall:
	./uninstall.sh

clean:
	@rm -rf objs/

fclean: clean
	@rm -f $(TARGET)

re: fclean
	@make all

.PHONY: all clean fclean re ignore-warnings install uninstall
