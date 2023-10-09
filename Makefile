src=$(wildcard src/*.cpp)
-fsanitize=thread

server:
	g++ -std=c++11 -pthread -g \
	$(src) \
	server.cpp \
	-o servers
	
# client:
# 	g++ src/util.cpp src/Buffer.cpp src/Socket.cpp client.cpp -o client

# th:
# 	g++ -pthread src/ThreadPool.cpp ThreadPoolTest.cpp -o ThreadPoolTest

test:
	g++ src/util.cpp src/Buffer.cpp src/InetAddress.cpp src/Socket.cpp src/ThreadPool.cpp \
	-pthread \
	test.cpp -o test

clean:
	rm server && rm client && rm test