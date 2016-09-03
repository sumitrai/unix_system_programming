CFLAGS=-g -Wall
INCLUDES=-I $(TOPDIR)/include

%.o: %.c
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@
