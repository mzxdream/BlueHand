TARGET   	= ../../bin/server
OBJ_DIR 	= ../../obj
SRC		= $(wildcard *.cpp ../kernal/*.cpp ../msg/*.cpp) 
OBJ	    	= $(patsubst %.cpp,%.o,$(SRC))
INCPATH		= -I. -I../kernal -I../msg
LIBPATH		= -L. -lboost_thread -lboost_signals -lboost_filesystem -lboost_system -lboost_serialization
LFLAGS          = -g -Wall
CFLAGS		= -c -g -Wall
CC     	= g++

$(TARGET) : $(OBJ)
	$(CC) $(LFLAGS) $(OBJ) -o $@ @< $(LIBPATH)
$(OBJ) : %.o : %.cpp
	$(CC) $(CFLAGS) $< -o $@ $(INCPATH) -std=c++0x
clean:
	rm -f $(OBJ)
	rm -f $(TARGET)
