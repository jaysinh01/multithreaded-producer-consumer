CXXFLAGS =	-O0 -g -Wall -fmessage-length=0 -std=c++11 -pthread

OBJS =	testAndRun.o

TARGET =	testAndRun

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS)


all:	$(TARGET) 
	./$(TARGET)

debug: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)