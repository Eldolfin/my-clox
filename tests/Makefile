LDLIBS = -lcriterion
SRC = criterion.c
TARGET = criterion
OPTFLAGS = -O3
CFLAGS = -Wall -Wextra $(OPTFLAGS)

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

$(TARGET): $(SRC)
	@ printf "%8s %-40s %s\n" $(CC) $@ "$(CFLAGS)"
	@ $(CC) $(CPPFLAGS) $(CFLAGS) $(SRC) -o $@ $(LDLIBS)

run: $(TARGET)
	./$^

clean:
	@ rm -rf $(TARGET) sandbox-gmon* gmon.out

.PHONY: clean default run
