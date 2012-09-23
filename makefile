BOOST_PATH=/home/travis/Projects/local/boost_1_50_0
CFLAGS=-I$(BOOST_PATH) -L$(BOOST_PATH)/stage/lib
export LD_LIBRARY_PATH=/home/travis/Projects/local/boost_1_50_0/stage/lib

all: test spider-cpp

spider-cpp: main.o http_request.o http_response.o header.o url.o page_downloader.o file_downloader.o download_queue.o
	g++ main.o http_request.o http_response.o header.o url.o page_downloader.o file_downloader.o download_queue.o -lboost_system -lboost_thread -lpthread -lboost_regex -o spider-cpp $(CFLAGS)

test: algorithm.test download_queue.test path_utilities.test url.test
	./algorithm.test;\
	./download_queue.test;\
	./path_utilities.test;\
	./url.test

main.o: main.cpp
	g++ -g -c main.cpp $(CFLAGS)

http_request.o: http_request.cpp http_request.hpp http_response.hpp header.hpp
	g++ -c http_request.cpp $(CFLAGS)

http_response.o: http_response.cpp http_response.hpp algorithm.hpp header.hpp
	g++ -c http_response.cpp $(CFLAGS)

header.o: header.cpp header.hpp
	g++ -c header.cpp $(CFLAGS)

url.o: url.cpp url.hpp
	g++ -c url.cpp $(CFLAGS)

page_downloader.o: page_downloader.cpp page_downloader.hpp url.hpp http_request.hpp http_response.hpp
	g++ -c page_downloader.cpp $(CFLAGS)

file_downloader.o: file_downloader.cpp file_downloader.hpp url.hpp http_request.hpp http_response.hpp path_utilities.hpp
	g++ -c file_downloader.cpp $(CFLAGS)

download_queue.o: download_queue.cpp download_queue.hpp url.hpp
	g++ -c download_queue.cpp $(CFLAGS)

# Tests
algorithm.test: algorithm_test.o
	g++ algorithm_test.o -lboost_unit_test_framework -o algorithm.test $(CFLAGS)

algorithm_test.o: algorithm_test.cpp algorithm.hpp
	g++ -c algorithm_test.cpp $(CFLAGS)

download_queue.test: download_queue_test.o download_queue.o url.o
	g++ download_queue_test.o download_queue.o url.o -lboost_regex -lboost_unit_test_framework -o download_queue.test $(CFLAGS)

download_queue_test.o: download_queue_test.cpp download_queue.hpp
	g++ -c download_queue_test.cpp $(CFLAGS)

path_utilities.test: path_utilities_test.o
	g++ path_utilities_test.o -lboost_unit_test_framework -o path_utilities.test $(CFLAGS)

path_utilities_test.o: path_utilities_test.cpp path_utilities.hpp
	g++ -c path_utilities_test.cpp $(CFLAGS)

url.test: url_test.o url.o
	g++ url_test.o url.o -lboost_unit_test_framework -lboost_regex -o url.test $(CFLAGS)

url_test.o: url_test.cpp url.hpp
	g++ -c url_test.cpp $(CFLAGS)

.PHONY : clean
clean:
	-rm spider-cpp
	-rm *.test
	-rm *.o
