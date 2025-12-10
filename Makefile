CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = linver
SRC = linver.cpp

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $< -lncurses

clean:
	rm -f $(TARGET)

install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/

.PHONY: clean install