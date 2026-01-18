CC		:= gcc
CFLAGS	:= -Wall -Wextra -Iinclude
DBGFLAG	:= -g -O0
RELFLAG	:= -O2

SRCS	:= \
		src/file_info.c \
		src/list.c \
		src/filter.c \
		src/scanner.c \
		src/filter_registry.c \
		src/options.c \
		src/main.c

OBJS	:= $(SRCS:.c=.o)

TARGET	:=file_scanner

.PHONY: all debug release clean

debug: CFLAGS += $(DBGFLAG)
debug: $(TARGET)

release: CFLAGS += $(RELFLAG)
release: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)