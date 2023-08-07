CC = gcc
CFLAGS = -Wall -g
SRCS = main.c TS.c
OBJS = $(SRCS:.c=.o)
# Define the main target
SoT-TS.exe: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
ifeq ($(shell uname),)
	del /Q /S SoT-TS.exe
	del /Q /S *.o
else
	rm -rf SoT-TS.exe
	rm -rf *.o
endif
	