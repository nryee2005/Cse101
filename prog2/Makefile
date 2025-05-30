# Makefile for scheduler project

# Target executable
scheduler: preemptive_scheduling.c priority_queue.c test.c
	gcc -g -o prog2 preemptive_scheduling.c priority_queue.c test.c

# Optional pattern rule for individual compilation (not used in main build)
%.o: %.c
	gcc -g -c $< -o $@

# Clean rule to remove the executable
clean:
	rm -f prog2