TARGET := particle_system
SRCS_DIR := ./srcs
OBJS_DIR := ./objs
CC := clang++
OS := $(shell uname)
CFLAGS := -Wall -Wextra -Werror -Wno-missing-braces
ignore-warnings : CFLAGS := -w
OPEN_CL :=
HEADERS := -I./includes

ifeq ($(OS),Darwin)
	OPEN_CL += -framework OpenCL
else
	OPEN_CL += -lOpenCL
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
OBJS := $(patsubst $(SRCS_DIR)%.cpp,$(OBJS_DIR)%.o,$(SRCS))

all: $(OBJS_DIR) $(TARGET)

ignore-warnings: all

$(OBJS_DIR):
	@mkdir -p ./objs

$(TARGET): $(OBJS)
	@$(CC) $(CFLAGS) -o $@ $^ $(OPEN_CL)
	@echo "$(CYAN)Successfully compiled $(TARGET).$(RESET)"

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	@$(CC) $(CFLAGS) -c $^ -o $@ $(HEADERS)

clean:
	@rm -rf objs/

fclean: clean
	@rm -f $(TARGET)

re: fclean
	@make all

.PHONY: all clean fclean re ignore-warnings