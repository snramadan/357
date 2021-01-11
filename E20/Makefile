CC      = gcc
CCFLAGS = -std=c89 -pedantic -Wall -Werror -D NDEBUG
LDFLAGS = -lm 
PARENT_SOURCES  = parent.c
PARENT_OBJECTS  = parent.o
PARENT_INCLUDES =
CHILD_SOURCES   = child.c
CHILD_OBJECTS   = child.o
CHILD_INCLUDE   =

all:parent child

parent:$(PARENT_OBJECTS) 
	$(CC) -o a.out $(CCFLAGS) $(LDFLAGS) $(PARENT_OBJECTS)

$(PARENT_OBJECTS):$(PARENT_SOURCES) $(PARENT_INCLUDES)
	$(CC) -c $(CCFLAGS) $(PARENT_SOURCES)

child:$(CHILD_OBJECTS) 
	$(CC) -o child $(CCFLAGS) $(LDFLAGS) $(CHILD_OBJECTS)

$(CHILD_OBJECTS):$(CHILD_SOURCES) $(CHILD_INCLUDES)
	$(CC) -c $(CCFLAGS) $(CHILD_SOURCES)

clean:
	rm -f a.out child *.o 
