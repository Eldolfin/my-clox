TARGET = clox

CC := gcc

CFLAGS = -Wall -Wextra $(OPTFLAGS)
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
default: clean docker
else
	CPPFLAGS += -DNDEBUG
default: docker
endif

$(TARGET): $(OBJS)
	@ printf "%8s %-40s %s\n" $(CC) $@ "$(CFLAGS)"
	@ $(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.c $(BUILD_DIR)
	@ printf "%8s %-40s %s\n" $(CC) $< "$(CFLAGS)"
	@ $(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

$(BUILD_DIR):
	@ mkdir -p $(SRC_DIRS:%=$(BUILD_DIR)/%)


run: $(TARGET)
	./$(TARGET)

clean:
	@ rm -rf $(BUILD_DIR) $(TARGET)
	@ rm -rf gmon.out
	@ $(MAKE) -C tests -s clean

check: $(TARGET)
	@ # @ $(MAKE) -C tests -s
	@ python3.10 tests/tests.py

docker:
	docker build -t clox . 
	docker run -it --rm --name=clox --mount type=bind,source=${PWD},target=/clox clox || true
	# generate html test report
	./venv/bin/junit2html target/test_results.xml target/test_results.html


.PHONY: clean test default check
