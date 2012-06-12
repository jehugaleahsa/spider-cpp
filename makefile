all: spider-cpp url.test

spider-cpp: main.o http_request.o http_response.o url.o page_downloader.o file_downloader.o download_queue.o
	g++ main.o http_request.o http_response.o url.o page_downloader.o file_downloader.o download_queue.o -lboost_system -lboost_thread -lpthread -lboost_regex -o spider-cpp

test: url.test
	./url.test

main.o: main.cpp
	g++ -g -c main.cpp

http_request.o: http_request.cpp http_request.hpp http_response.hpp
	g++ -c http_request.cpp

http_response.o: http_response.cpp http_response.hpp
	g++ -c http_response.cpp

url.test: url_test.o url.o
	g++ url_test.o url.o -lboost_unit_test_framework -lboost_regex -o url.test

url_test.o: url_test.cpp url.hpp
	g++ -c url_test.cpp

url.o: url.cpp url.hpp
	g++ -c url.cpp

page_downloader.o: page_downloader.cpp page_downloader.hpp url.hpp http_request.hpp http_response.hpp
	g++ -c page_downloader.cpp

file_downloader.o: file_downloader.cpp file_downloader.hpp url.hpp http_request.hpp http_response.hpp
	g++ -c file_downloader.cpp

download_queue.o: download_queue.cpp download_queue.hpp url.hpp
	g++ -c download_queue.cpp

.PHONY : clean
clean:
	-rm spider-cpp
	-rm *.test
	-rm *.o
