# CSC308 - CPU Scheduling Simulator
# Usage:
#   make        → compile
#   make run    → compile and run
#   make clean  → remove compiled files

CC      = gcc
CFLAGS  = -Wall -Wextra -lm
TARGET  = simulator
SRCS    = main.c process.c workload.c metrics.c \
          scheduler_fcfs.c scheduler_sjf.c scheduler_rr.c \
          scheduler_priority.c scheduler_advanced1.c scheduler_advanced2.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) -o $(TARGET) $(SRCS) $(CFLAGS)
	@echo "✅ Compiled successfully. Run with: ./simulator"

run: all
	@mkdir -p results
	./$(TARGET)

clean:
	rm -f $(TARGET)
	rm -f results/results.csv
