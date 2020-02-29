

CXXFLAGS =	-O0 -g -Wall -fmessage-length=0 -std=c++11 -pthread

OBJS =	testAndRun.o printing.o

TARGET =	testAndRun

$(TARGET):	$(OBJS)
	g++ $(CXXFLAGS) $(OBJS) -o $(TARGET)


all:	$(TARGET) 
	./$(TARGET) 3 2 < input.txt

debug: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)