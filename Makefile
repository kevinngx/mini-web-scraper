# CC      = gcc
# CFLAGS  = -Wall -Wextra -Wpedantic -Werror -O0 -g
# LIBS	= -lm -lxml2 -lcurl -I/usr/include/libxml2

# # If dcc is available, use it
# ifneq ($(shell which dcc),)
# 	CC = dcc
# endif

# .PHONY: all clean

# all: crawler 

# crawler: crawler.c graph.c list.c
# 	$(CC) $(CFLAGS) $(POSIX) -o $@ $^ $(LIBS)

# clean:
# 	rm -f crawler
# 	rm -rf *.dSYM

CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -std=c11

##################### Test List #####################

# testList : testList.o list.o
# 	$(CC) $(CFLAGS) -o testList testList.o list.o

# testList.o : testList.c list.h
# 	$(CC) $(CFLAGS) -c testList.c


##################### Test Graph #####################

testGraph : testGraph.o graph.o list.o
	$(CC) $(CFLAGS) -o testGraph testGraph.o graph.o list.o

testGraph.o : testGraph.c graph.h
	$(CC) $(CFLAGS) -c testGraph.c

##################### ADTs #####################

graph.o : graph.c graph.h list.o
	$(CC) $(CFLAGS) -c graph.c

list.o : list.c list.h
	$(CC) $(CFLAGS) -c list.c

clean:
	rm -f *.o, testGraph, testList