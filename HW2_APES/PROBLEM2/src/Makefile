CC = arm-linux-gcc
CFLAGS = -Wall -g
SRCS = myfileIO.c
OBJS = $(SRCS:.c=.o)
TARGET = myfileIO
all: myfileIO

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

.c.o:
	$(CC) -c  $(SRCS)

clean:
	-rm -f $(TARGET) *.txt *.o
