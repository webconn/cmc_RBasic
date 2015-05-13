TARGET=rbasic

OBJS = main.o lex.o syntax.o rbasic.o variable.o value.o elem.o

CXX = g++
CXX_FLAGS = -g -Wall
LD_FLAGS = -lm

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LD_FLAGS)

%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXX_FLAGS)

clean:
	rm $(OBJS) $(TARGET) -rf

.PHONY: clean
