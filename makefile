BOOST_PATH=/home/travis/Projects/local/boost_1_50_0
CFLAGS=-Wall -I$(BOOST_PATH) -L$(BOOST_PATH)/stage/lib
OBJS = main.o spider.o http_request.o http_response.o header.o extractor.o stripper.o categorizer.o url.o page_downloader.o file_downloader.o downloader.o tracker.o

export LD_LIBRARY_PATH=/home/travis/Projects/local/boost_1_50_0/stage/lib

all: spider

.PHONY : spider
spider: $(OBJS)
	g++ $(OBJS) -lboost_system -lboost_thread -lpthread -lboost_regex -o spider $(CFLAGS)

main.o: main.cpp spider.hpp url.hpp thread_pool.hpp counter.hpp
	g++ -c main.cpp $(CFLAGS)

spider.o: spider.cpp categorizer.hpp tracker.hpp extractor.hpp file_downloader.hpp page_downloader.hpp spider.hpp stripper.hpp url.hpp
	g++ -c spider.cpp $(CFLAGS)

http_request.o: http_request.cpp header.hpp http_request.hpp http_response.hpp url.hpp
	g++ -c http_request.cpp $(CFLAGS)

http_response.o: http_response.cpp algorithm.hpp header.hpp http_response.hpp
	g++ -c http_response.cpp $(CFLAGS)

header.o: header.cpp header.hpp
	g++ -c header.cpp $(CFLAGS)

extractor.o: extractor.cpp extractor.hpp url.hpp
	g++ -c extractor.cpp $(CFLAGS)

stripper.o: stripper.cpp stripper.hpp
	g++ -c stripper.cpp $(CFLAGS)

categorizer.o: categorizer.cpp categorizer.hpp url.hpp
	g++ -c categorizer.cpp $(CFLAGS)

url.o: url.cpp url.hpp
	g++ -c url.cpp $(CFLAGS)

downloader.o: downloader.cpp downloader.hpp http_request.hpp url.hpp
	g++ -c downloader.cpp $(CFLAGS)

page_downloader.o: page_downloader.cpp downloader.hpp http_request.hpp page_downloader.hpp url.hpp
	g++ -c page_downloader.cpp $(CFLAGS)

file_downloader.o: file_downloader.cpp downloader.hpp file_downloader.hpp http_request.hpp http_response.hpp path_utilities.hpp url.hpp
	g++ -c file_downloader.cpp $(CFLAGS)

tracker.o: tracker.cpp tracker.hpp url.hpp
	g++ -c tracker.cpp $(CFLAGS)

extractor: spider
	./spider $(SITE)

.PHONY : clean
clean:
	-rm spider
	-rm *.test
	-rm *.o
