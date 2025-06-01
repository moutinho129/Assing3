TARGET = unitycmd_test

UNITY_DIR = ../unity
SRCS = unitycmd.c mock_cmd.c mock_task.c mock_zephyr.c $(UNITY_DIR)/unity.c

OBJS = $(SRCS:.c=.o)

CC = gcc
CFLAGS = -Wall -Wextra -g -I$(UNITY_DIR)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

