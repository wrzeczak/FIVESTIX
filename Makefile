TARGET := app

SOURCES := $(wildcard src/*.c) $(wildcard src/*.cpp)
LIBS := -L"C:\\vcpkg\\packages\\raylib_x64-windows\\bin" -lraylib -lGL -lm
OBJECTS := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))
DEPENDS := $(patsubst %.c,%.d,$(patsubst %.cpp,%.d,$(SOURCES)))

CFLAGS = -IC:\raylib\src -O2 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -std=c++2a

.PHONY: build run clean

build: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET).elf $(OBJECTS) $(LIBS)

run: build
	@./$(TARGET).elf

clean:
	$(RM) $(OBJECTS) $(DEPENDS)

-include $(DEPENDS)

%.o: %.c Makefile
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

%.o: %.cpp Makefile
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@