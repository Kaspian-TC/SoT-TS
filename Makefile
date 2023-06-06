CC = gcc
CFLAGS = -Wall -g 
SRCS = main.c TS.c
OBJS = $(SRCS:.c=.o)
# Define the main target
SoT-TS.exe: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Define the pattern rule for compiling .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
TS: TS.o
	$(CC) $(CFLAGS) TS.c -o TS.exe

clean:
	del SoT-TS.exe
	del *.o
	