all: test spider-cpp

spider-cpp: main.o http_request.o http_response.o header.o url.o page_downloader.o file_downloader.o download_queue.o
	g++ main.o http_request.o http_response.o header.o url.o page_downloader.o file_downloader.o download_queue.o -lboost_system -lboost_thread -lpthread -lboost_regex -o spider-cpp

test: algorithm.test download_queue.test path_utilities.test url.test
	./algorithm.test;\
	./download_queue.test;\
	./path_utilities.test;\
	./url.test

main.o: main.cpp
	g++ -g -c main.cpp

http_request.o: http_request.cpp http_request.hpp http_response.hpp header.hpp
	g++ -c http_request.cpp

http_response.o: http_response.cpp http_response.hpp algorithm.hpp header.hpp
	g++ -c http_response.cpp

header.o: header.cpp header.hpp
	g++ -c header.cpp

url.o: url.cpp url.hpp
	g++ -c url.cpp

page_downloader.o: page_downloader.cpp page_downloader.hpp url.hpp http_request.hpp http_response.hpp
	g++ -c page_downloader.cpp

file_downloader.o: file_downloader.cpp file_downloader.hpp url.hpp http_request.hpp http_response.hpp path_utilities.hpp
	g++ -c file_downloader.cpp

download_queue.o: download_queue.cpp download_queue.hpp url.hpp
	g++ -c download_queue.cpp

# Tests
algorithm.test: algorithm_test.o
	g++ algorithm_test.o -lboost_unit_test_framework -o algorithm.test

algorithm_test.o: algorithm_test.cpp algorithm.hpp
	g++ -c algorithm_test.cpp

download_queue.test: download_queue_test.o download_queue.o url.o
	g++ download_queue_test.o download_queue.o url.o -lboost_regex -lboost_unit_test_framework -o download_queue.test

download_queue_test.o: download_queue_test.cpp download_queue.hpp
	g++ -c download_queue_test.cpp

path_utilities.test: path_utilities_test.o
	g++ path_utilities_test.o -lboost_unit_test_framework -o path_utilities.test

path_utilities_test.o: path_utilities_test.cpp path_utilities.hpp
	g++ -c path_utilities_test.cpp

url.test: url_test.o url.o
	g++ url_test.o url.o -lboost_unit_test_framework -lboost_regex -o url.test

url_test.o: url_test.cpp url.hpp
	g++ -c url_test.cpp

.PHONY : clean
clean:
	-rm spider-cpp
	-rm *.test
	-rm *.o
