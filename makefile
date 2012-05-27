spider-cpp: main.o
	g++ main.o -lboost_system -lboost_thread -lpthread -o spider-cpp

main.o: main.cpp
	g++ -c main.cpp

http_request.test: http_request_test.o http_request.o
	g++ http_request_test.o http_request.o -o http_request.test

http_request_test.o : http_request_test.cpp http_request.hpp
	g++ -c http_request_test.cpp

http_request.o : http_request.cpp http_request.hpp
	g++ -c http_request.cpp

.PHONY : clean
clean:
	-rm spider-cpp
	-rm http_request.test
	-rm *.o
