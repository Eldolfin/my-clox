LDLIBS = -lcriterion
SRC = criterion.c
TARGET = criterion
OPTFLAGS = -O3
CFLAGS = -Wall -Wextra $(OPTFLAGS)
CPPFLAGS = -MMD -I ../src/

BUILD_DIR := ../target
CFILES := $(shell find ../src -type f \( -iname "*.c" ! -iname "main.c" \))
OBJS := $(CFILES:../%.c=$(BUILD_DIR)/%.o)

DEBUG := 0

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

$(TARGET): $(OBJS) $(SRC)
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ $(LDLIBS)

$(OBJS): MainBuild

MainBuild: $(CFILES)
	@ $(MAKE) -C ../ clox -s

run: $(TARGET)
	./$^

clean:
	@ rm -rf $(TARGET) sandbox-gmon* gmon.out *.d __pycache__

.PHONY: clean default run MainBuild
