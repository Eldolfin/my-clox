TARGET = clox

CC := gcc

CFLAGS = -Wall -Wextra
OPTFLAGS = -O3
CPPFLAGS = -MMD
LDFLAGS :=

# Debug mode can be enabled by executing `make DEBUG=1 rule_name`
DEBUG := 0

# Find all subdirectories
SRC_DIRS := $(shell find ./src -type d -not -path "*_build*")

BUILD_DIR := ./target

CFILES := $(shell find ./src -type f \( -iname "*.c" \))
OBJS := $(CFILES:%.c=$(BUILD_DIR)/%.o)

ifneq ($(DEBUG), 0)
	OPTFLAGS = -O0
	CFLAGS += -g -pg -fsanitize=address,undefined
	LDFLAGS += -fsanitize=address,undefined
	CPPFLAGS += -DDEBUG # define DEBUG like `#define DEBUG` in all C files
default: clean run
else
	CPPFLAGS += -DNDEBUG
default: run
endif

$(TARGET): $(OBJS)
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.c $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

$(BUILD_DIR):
	mkdir -p $(SRC_DIRS:%=$(BUILD_DIR)/%)


run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
	rm -rf gmon.out

.PHONY: clean test
