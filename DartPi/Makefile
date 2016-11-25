CC	= g++
CFLAGS	= -I/usr/include/opencv -pedantic -std=c++0x
LDFLAGS = -lopencv_core -lopencv_highgui -lopencv_imgproc

PROGRAM = dartpi
OBJ	= Camera.o DartDetector.o DartPi.o

all: $(OBJ)
	$(CC) $(CFLAGS) -o $(PROGRAM) $(OBJ) $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJ) $(PROGRAM)
