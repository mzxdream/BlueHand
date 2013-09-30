CC = g++ -Wall -g
CFLAGS = `pkg-config --cflags --libs gtkmm-3.0` -std=c++11
LIBS = -lboost_signals
OBJS = main.o BlueHandApp.o LoginWnd.o

all : main
main : $(OBJS)
	$(CC) -o main $(OBJS) $(CFLAGS) $(LIBS)
$(OBJS) : %.o : %.cpp
	$(CC) -c $< -o $@ $(CFLAGS) $(LIBS)

.PHONY : clean
clean :
	rm $(OBJS) main



