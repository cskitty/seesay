CC = g++
CFLAGS = -std=c++11 -Wall

# Source files
SRCS := common.cpp common-ggml.cpp common-sdl.cpp gpt-2.cpp grammar-parser.cpp talk.cpp voice.cpp chat.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

LIBS := libwhisper.a

CC_SDL := $(shell sdl2-config --libs) 

# Executable name
TARGET = talk

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@ $(CC_SDL) $(LDFLAGS) -lcurl -lmosquitto

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $< $(shell sdl2-config --cflags)

clean:
	rm -f $(OBJS) $(TARGET)

