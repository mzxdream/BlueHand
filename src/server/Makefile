OUTPUT_OBJ = ../../obj
TARGET = server
SOURCES = $(wildcard *.cpp ../kernal/*.cpp ../msg/*.cpp)
DIR_OBJS = $(patsubst %.cpp,%.o,$(SOURCES))
OBJS_NAME = $(notdir $(patsubst %.cpp,%.o,$(SOURCES)))
OBJS = $(addprefix $(OUTPUT_OBJ)/,$(notdir $(patsubst %.cpp,%.o,$(SOURCES))))
INCPATH		= -I. -I../kernal -I../msg
LIBPATH		= -L. -lboost_thread -lboost_signals -lboost_filesystem -lboost_system -lboost_serialization
LFLAGS          = -g -Wall
CFLAGS		= -g -Wall -std=c++0x
CC     	= g++

all:$(TARGET)
$(TARGET):$(OBJS)
	$(CC) $(OBJS) $(LFLAGS) -o $(TARGET) $(LIBPATH)
$(OBJS):$(DIR_OBJS)
$(DIR_OBJS):%.o : %.cpp
	$(CC) $(CFLAGS) -o $@ -c  $<
	mv $@ $(OUTPUT_OBJ)
.PHONY:clean
clean:
	rm -rf $(OUTPUT_OBJ)/* $(TARGET)
