CFLAGS=-std=c++11 -Wall -pedantic -L/usr/lib -g
OBJS = categorizer.o downloader.o extractor.o file_downloader.o header.o http_request.o http_response.o main.o page_downloader.o spider.o stripper.o thread_pool.o tracker.o url.o

# define header dependencies

ALGORITHM = algorithm.hpp
URL = url.hpp
HEADER = header.hpp
COUNTER = counter.hpp
PATH_UTILITIES = path_utilities.hpp
STRIPPER = stripper.hpp
THREAD_POOL = thread_pool.hpp
TRACKER = tracker.hpp $(URL)
HTTP_RESPONSE = http_response.hpp $(HEADER)
HTTP_REQUEST = http_request.hpp $(HEADER) $(URL)
CATEGORIZER = categorizer.hpp $(URL)
DOWNLOADER = downloader.hpp $(COUNTER) $(HTTP_REQUEST) $(URL)
EXTRACTOR = extractor.hpp $(URL)
FILE_DOWNLOADER = file_downloader.hpp $(DOWNLOADER) $(URL)
PAGE_DOWNLOADER = page_downloader.hpp $(CATEGORIZER) $(DOWNLOADER) $(EXTRACTOR) $(STRIPPER) $(THREAD_POOL) $(TRACKER) $(URL)
SPIDER = spider.hpp $(URL)

all: spider

# binaries

export LD_LIBRARY_PATH=/usr/lib

extractor: spider
	./spider $(SITE) $(DIR)

.PHONY : spider
spider: $(OBJS)
	g++ $(OBJS) -lboost_filesystem -lboost_regex -lboost_system -lboost_thread -lpthread -o spider $(CFLAGS)
	
# object files
	
categorizer.o: categorizer.cpp $(CATEGORIZER) $(URL)
	g++ -c categorizer.cpp $(CFLAGS)
	
downloader.o: downloader.cpp $(COUNTER) $(DOWNLOADER) $(HTTP_REQUEST) $(URL)
	g++ -c downloader.cpp $(CFLAGS)
	
extractor.o: extractor.cpp $(EXTRACTOR) $(URL)
	g++ -c extractor.cpp $(CFLAGS)
	
file_downloader.o: file_downloader.cpp $(DOWNLOADER) $(FILE_DOWNLOADER) $(HTTP_REQUEST) $(HTTP_RESPONSE) $(PATH_UTILITIES) $(URL)
	g++ -c file_downloader.cpp $(CFLAGS)
	
header.o: header.cpp $(HEADER)
	g++ -c header.cpp $(CFLAGS)
	
http_request.o: http_request.cpp $(HEADER) $(HTTP_REQUEST) $(HTTP_RESPONSE) $(URL)
	g++ -c http_request.cpp $(CFLAGS)

http_response.o: http_response.cpp $(ALGORITHM) $(HEADER) $(HTTP_RESPONSE)
	g++ -c http_response.cpp $(CFLAGS)

main.o: main.cpp $(SPIDER) $(URL)
	g++ -c main.cpp $(CFLAGS)
	
page_downloader.o: page_downloader.cpp $(CATEGORIZER) $(COUNTER) $(DOWNLOADER) $(EXTRACTOR) $(FILE_DOWNLOADER) $(HTTP_REQUEST) $(PAGE_DOWNLOADER) $(STRIPPER) $(THREAD_POOL) $(TRACKER) $(URL)
	g++ -c page_downloader.cpp $(CFLAGS)

spider.o: spider.cpp $(CATEGORIZER) $(COUNTER) $(DOWNLOADER) $(EXTRACTOR) $(PAGE_DOWNLOADER) $(SPIDER) $(STRIPPER) $(THREAD_POOL) $(TRACKER) $(URL)
	g++ -c spider.cpp $(CFLAGS)

stripper.o: stripper.cpp $(STRIPPER)
	g++ -c stripper.cpp $(CFLAGS)
	
thread_pool.o: thread_pool.cpp $(THREAD_POOL)
	g++ -c thread_pool.cpp $(CFLAGS)
	
tracker.o: tracker.cpp $(TRACKER) $(URL)
	g++ -c tracker.cpp $(CFLAGS)

url.o: url.cpp $(URL)
	g++ -c url.cpp $(CFLAGS)

.PHONY : clean
clean:
	-rm spider
	-rm *.o
