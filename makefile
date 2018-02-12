all:
	g++ -std=c++11 client.cpp -o client -lrt
	g++ -std=c++11 server.cpp -o server -lrt
