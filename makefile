all: spider-cpp http_request.test url.test

spider-cpp: main.o
	g++ main.o -lboost_system -lboost_thread -lpthread -o spider-cpp

test: http_request.test url.test
	./http_request.test
	./url.test

main.o: main.cpp
	g++ -c main.cpp

http_request.test: http_request_test.o http_request.o
	g++ http_request_test.o http_request.o -o http_request.test

http_request_test.o: http_request_test.cpp http_request.hpp
	g++ -c http_request_test.cpp

http_request.o: http_request.cpp http_request.hpp
	g++ -c http_request.cpp

url.test: url_test.o url.o bad_url_exception.o
	g++ url_test.o url.o bad_url_exception.o -lboost_unit_test_framework -lboost_regex -o url.test

url_test.o: url_test.cpp url.hpp bad_url_exception.hpp
	g++ -c url_test.cpp

url.o: url.cpp url.hpp bad_url_exception.hpp
	g++ -c url.cpp

bad_url_exception.o : bad_url_exception.cpp bad_url_exception.hpp
	g++ -c bad_url_exception.cpp

.PHONY : clean
clean:
	-rm spider-cpp
	-rm *.test
	-rm *.o
