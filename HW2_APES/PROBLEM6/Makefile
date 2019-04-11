CC = arm-linux-gcc
CFLAGS = -Wall -g
SRCS = cron.c
OBJS = $(SRCS:.c=.o)
TARGET = cron
all: cron

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

.c.o:
	$(CC) -c  $(SRCS)

clean:
	-rm -f $(TARGET) *.txt *.o
