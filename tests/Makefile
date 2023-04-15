LDLIBS = -lcriterion
SRC = criterion.c
TARGET = criterion
OPTFLAGS = -O3

default: run

$(TARGET): $(SRC)
	@ printf "%8s %-40s %s\n" $(CC) $@ "$(CFLAGS)"
	@ $(CC) $(CPPFLAGS) $(CFLAGS) $(SRC) -o $@ $(LDLIBS)

run: $(TARGET)
	./$^

clean:
	@ rm -rf $(TARGET)

.PHONY: clean default run
