TARGET=main
all: *.c++
	g++  -w -Wfatal-errors 681assign2.c++ lcgrand.c -o $(TARGET) -I .
	./$(TARGET) < input.txt >output300.txt

clean:
	rm $(TARGET)arr