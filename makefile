CFLAGS=-std=c++11 -Wall -pedantic -L/usr/lib -g
OBJS = categorizer.o counter.o downloader.o download_manager.o extractor.o file_downloader.o file_download_factory.o header.o http_request.o http_response.o main.o page_downloader.o page_download_factory.o spider.o stripper.o thread_pool.o tracker.o url.o url_finder.o

# define header dependencies

ALGORITHM = algorithm.hpp
URL = url.hpp
HEADER = header.hpp
COUNTER = counter.hpp
PATH_UTILITIES = path_utilities.hpp
STRIPPER = stripper.hpp
DOWNLOAD_FACTORY = download_factory.hpp $(URL)
THREAD_POOL = thread_pool.hpp $(COUNTER)
TRACKER = tracker.hpp $(URL)
HTTP_RESPONSE = http_response.hpp $(ALGORITHM) $(HEADER)
HTTP_REQUEST = http_request.hpp $(HEADER) $(HTTP_RESPONSE) $(URL)
CATEGORIZER = categorizer.hpp $(URL)
DOWNLOAD_MANAGER = download_manager.hpp $(CATEGORIZER) $(DOWNLOAD_FACTORY) $(THREAD_POOL) $(TRACKER) $(URL)
DOWNLOADER = downloader.hpp $(HTTP_REQUEST) $(URL)
EXTRACTOR = extractor.hpp $(URL)
URL_FINDER = url_finder.hpp $(EXTRACTOR) $(STRIPPER) $(URL)
FILE_DOWNLOADER = file_downloader.hpp $(DOWNLOADER) $(HTTP_REQUEST) $(HTTP_RESPONSE) $(PATH_UTILITIES) $(URL)
FILE_DOWNLOAD_FACTORY = file_download_factory.hpp $(DOWNLOAD_FACTORY) $(FILE_DOWNLOADER) $(URL)
PAGE_DOWNLOADER = page_downloader.hpp $(DOWNLOADER) $(DOWNLOAD_MANAGER) $(HTTP_REQUEST) $(HTTP_RESPONSE) $(URL) $(URL_FINDER)
PAGE_DOWNLOAD_FACTORY = page_download_factory.hpp $(DOWNLOAD_FACTORY) $(DOWNLOAD_MANAGER) $(PAGE_DOWNLOADER) $(URL) $(URL_FINDER)
SPIDER = spider.hpp $(CATEGORIZER) $(COUNTER) $(DOWNLOADER) $(DOWNLOAD_MANAGER) $(EXTRACTOR) $(FILE_DOWNLOAD_FACTORY) $(PAGE_DOWNLOAD_FACTORY) $(STRIPPER) $(THREAD_POOL) $(TRACKER) $(URL) $(URL_FINDER)
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

download_manager.o: download_manager.cpp $(DOWNLOAD_MANAGER)
	g++ -c download_manager.cpp $(CFLAGS)
	
extractor.o: extractor.cpp $(EXTRACTOR)
	g++ -c extractor.cpp $(CFLAGS)
	
file_downloader.o: file_downloader.cpp $(FILE_DOWNLOADER)
	g++ -c file_downloader.cpp $(CFLAGS)

file_download_factory.o: file_download_factory.cpp $(FILE_DOWNLOAD_FACTORY)
	g++ -c file_download_factory.cpp $(CFLAGS)
	
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

page_download_factory.o: page_download_factory.cpp $(PAGE_DOWNLOAD_FACTORY)
	g++ -c page_download_factory.cpp $(CFLAGS)

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
