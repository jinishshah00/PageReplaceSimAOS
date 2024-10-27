# Makefile for CPU Scheduling Simulation

CC = gcc
CFLAGS = -Wall -Wextra -O2

# Directories
SRC_DIR = src
OUTPUT_DIR = output

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Header files
HDRS = $(wildcard $(SRC_DIR)/*.h)

# Executable name
TARGET = paging

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRCS) $(HDRS)
	@echo "Compiling and linking $@"
	$(CC) $(CFLAGS) -o $@ $(SRCS)

clean:
	@echo "Cleaning up..."
	rm -f $(TARGET)
	rm -rf $(OUTPUT_DIR)/*

