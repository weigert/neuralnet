CC = g++ -std=c++17
CFLAGS = -Wfatal-errors -O
LFLAGS = -I/usr/local/include -L/usr/local/lib
#LINK = -ldl -lX11 -lXfixes -lXrender -lGL -lGLEW -lboost_system -lboost_filesystem

all: main.cpp
	@$(CC) main.cpp $(CFLAGS) $(LFLAGS) -O3 -o main
	#@mv splash /usr/local/bin/splash
	#@echo "splash compiled. moved to /usr/local/bin"
	#@echo "test with: splash fidget"
