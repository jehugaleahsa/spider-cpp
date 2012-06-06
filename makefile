all: spider-cpp http_request.test url.test

spider-cpp: main.o http_request.o http_response.o url.o bad_url_exception.o page_downloader.o
	g++ main.o http_request.o http_response.o url.o bad_url_exception.o page_downloader.o -lboost_system -lboost_thread -lpthread -lboost_regex -o spider-cpp

test: http_request.test url.test
	./http_request.test
	./url.test

main.o: main.cpp
	g++ -g -c main.cpp

http_request.test: http_request_test.o http_request.o http_response.o url.o bad_url_exception.o
	g++ http_request_test.o http_request.o http_response.o url.o bad_url_exception.o -lboost_system -lpthread -lboost_regex -lboost_unit_test_framework -o http_request.test

http_request_test.o: http_request_test.cpp http_request.hpp
	g++ -c http_request_test.cpp

http_request.o: http_request.cpp http_request.hpp http_response.hpp
	g++ -c http_request.cpp

http_response.o: http_response.cpp http_response.hpp
	g++ -c http_response.cpp

url.test: url_test.o url.o bad_url_exception.o
	g++ url_test.o url.o bad_url_exception.o -lboost_unit_test_framework -lboost_regex -o url.test

url_test.o: url_test.cpp url.hpp bad_url_exception.hpp
	g++ -c url_test.cpp

url.o: url.cpp url.hpp bad_url_exception.hpp
	g++ -c url.cpp

bad_url_exception.o : bad_url_exception.cpp bad_url_exception.hpp
	g++ -c bad_url_exception.cpp

page_downloader.o: page_downloader.cpp page_downloader.hpp url.hpp http_request.hpp http_response.hpp
	g++ -c page_downloader.cpp

.PHONY : clean
clean:
	-rm spider-cpp
	-rm *.test
	-rm *.o
