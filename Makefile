#Makefile
TARGET=pa2
OBJECTS=PA2.o
CXX=gcc

$(TARGET):$(OBJECTS)
	$(CXX) -o $(TARGET) $(OBJECTS)

$(OBJECTS):PA2.c
	$(CXX) -c PA2.c

clean:
	rm $(TARGET) $(OBJECTS)
