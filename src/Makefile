ODIR		= obj
LDIR 		= ../lib

CC 			= g++
CFLAGS 		:= -I$(LDIR) `pkg-config --cflags glfw3`
VPATH 		= tests

LIBFLAGS 	:= `pkg-config --static --libs glfw3`
IMGUISRC 	= $(wildcard $(LDIR)/imgui/*.cpp)
IMGUI 		= $(patsubst %.cpp, %.o, $(IMGUISRC))
_LIBS 		= glad/glad.o
LIBS 		= $(patsubst %,$(LDIR)/%,$(_LIBS)) $(IMGUI)

SOURCES 	= $(wildcard *.cpp) $(wildcard tests/*.cpp)
OBJ 		= $(patsubst %.cpp, $(ODIR)/%.o, $(SOURCES))
#HEADERS 	= $(wildcard *.h) $(wildcard tests/*.h)
DEPENDS 	:= $(patsubst %.cpp,%.d,$(SOURCES))

-include $(DEPENDS)

TARGET 		= App

.PHONY: all

all: $(TARGET)

$(ODIR)/%.o: %.cpp Makefile
	$(CC) -c -o $@ $< $(CFLAGS) -MMD -MP

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LIBS) $(CFLAGS) $(LIBFLAGS)

.PHONY: libs

libs: $(IMGUI)

$(LDIR)/imgui/%.o: $(LDIR)/imgui/%.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	find $(ODIR) -type f -name '*.o' -delete
	find $(ODIR) -type f -name '*.d' -delete
	find . -type f -name '*~' -delete

.PHONY: echo
echo:
	echo $(DEPS)
	echo $(SOURCES)
	echo $(OBJ)
	echo $(LIBFLAGS)
	echo $(CFLAGS)
