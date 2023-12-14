CC = g++
CFLAGS = -std=c++11

TARGET = cholesky_decomposition

all: $(TARGET)

$(TARGET): cholesky_decomposition.cpp
	$(CC) $(CFLAGS) -o $(TARGET) cholesky_decomposition.cpp

clean:
	rm -f $(TARGET)