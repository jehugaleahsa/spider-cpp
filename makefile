CFLAGS=-std=c++11 -Wall -pedantic -L/usr/lib -g
OBJS = categorizer.o counter.o downloader.o extractor.o file_downloader.o header.o http_request.o http_response.o main.o page_downloader.o spider.o stripper.o thread_pool.o tracker.o url.o url_finder.o

# define header dependencies

ALGORITHM = algorithm.hpp
URL = url.hpp
HEADER = header.hpp
COUNTER = counter.hpp
PATH_UTILITIES = path_utilities.hpp
STRIPPER = stripper.hpp
THREAD_POOL = thread_pool.hpp $(COUNTER)
TRACKER = tracker.hpp $(URL)
HTTP_RESPONSE = http_response.hpp $(ALGORITHM) $(HEADER)
HTTP_REQUEST = http_request.hpp $(HEADER) $(HTTP_RESPONSE) $(URL)
CATEGORIZER = categorizer.hpp $(URL)
DOWNLOADER = downloader.hpp $(HTTP_REQUEST) $(URL)
EXTRACTOR = extractor.hpp $(URL)
URL_FINDER = url_finder.hpp $(EXTRACTOR) $(STRIPPER) $(URL)
FILE_DOWNLOADER = file_downloader.hpp $(DOWNLOADER) $(HTTP_REQUEST) $(HTTP_RESPONSE) $(PATH_UTILITIES) $(URL)
PAGE_DOWNLOADER = page_downloader.hpp $(CATEGORIZER) $(DOWNLOADER) $(EXTRACTOR) $(FILE_DOWNLOADER) $(HTTP_REQUEST) $(STRIPPER) $(THREAD_POOL) $(TRACKER) $(URL)
SPIDER = spider.hpp $(CATEGORIZER) $(COUNTER) $(DOWNLOADER) $(EXTRACTOR) $(PAGE_DOWNLOADER) $(STRIPPER) $(THREAD_POOL) $(TRACKER) $(URL)
MAIN = $(SPIDER) $(URL)

all: spider

# binaries

export LD_LIBRARY_PATH=/usr/lib:/usr/lib64

extractor: spider
	./spider $(SITE) $(DIR)

.PHONY : spider
spider: $(OBJS)
	g++ $(OBJS) -lboost_filesystem -lboost_regex -lboost_system -lpthread -o spider $(CFLAGS)
	
# object files
	
categorizer.o: categorizer.cpp $(CATEGORIZER)
	g++ -c categorizer.cpp $(CFLAGS)
	
counter.o: counter.cpp $(COUNTER)
	g++ -c counter.cpp $(CFLAGS)

downloader.o: downloader.cpp $(DOWNLOADER)
	g++ -c downloader.cpp $(CFLAGS)
	
extractor.o: extractor.cpp $(EXTRACTOR)
	g++ -c extractor.cpp $(CFLAGS)
	
file_downloader.o: file_downloader.cpp $(FILE_DOWNLOADER)
	g++ -c file_downloader.cpp $(CFLAGS)
	
header.o: header.cpp $(HEADER)
	g++ -c header.cpp $(CFLAGS)
	
http_request.o: http_request.cpp $(HTTP_REQUEST)
	g++ -c http_request.cpp $(CFLAGS)

http_response.o: http_response.cpp $(HTTP_RESPONSE)
	g++ -c http_response.cpp $(CFLAGS)

main.o: main.cpp $(MAIN)
	g++ -c main.cpp $(CFLAGS)
	
page_downloader.o: page_downloader.cpp $(PAGE_DOWNLOADER)
	g++ -c page_downloader.cpp $(CFLAGS)

spider.o: spider.cpp $(SPIDER)
	g++ -c spider.cpp $(CFLAGS)

stripper.o: stripper.cpp $(STRIPPER)
	g++ -c stripper.cpp $(CFLAGS)
	
thread_pool.o: thread_pool.cpp $(THREAD_POOL)
	g++ -c thread_pool.cpp $(CFLAGS)
	
tracker.o: tracker.cpp $(TRACKER)
	g++ -c tracker.cpp $(CFLAGS)

url.o: url.cpp $(URL)
	g++ -c url.cpp $(CFLAGS)

url_finder.o: url_finder.cpp $(URL_FINDER)
	g++ -c url_finder.cpp $(CFLAGS)

.PHONY : clean
clean:
	-rm spider
	-rm *.o
