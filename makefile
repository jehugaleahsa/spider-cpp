CFLAGS=-std=c++11 -Wall -pedantic -L/usr/lib -g
OBJS = categorizer.o counter.o downloader.o download_manager.o environment.o extractor.o file_downloader.o file_download_factory.o header.o http_request.o http_response.o main.o page_downloader.o page_download_factory.o path.o spider.o stripper.o task_pool.o tracker.o url.o url_finder.o

# define header dependencies

ALGORITHM = algorithm.hpp
ENVIRONMENT = environment.hpp
URL = url.hpp
HEADER = header.hpp
COUNTER = counter.hpp
CHUNKED_STREAM = chunked_stream.hpp
PATH_ = path.hpp
STRIPPER = stripper.hpp
DOWNLOAD_FACTORY = download_factory.hpp $(URL)
TASK_POOL = task_pool.hpp $(COUNTER)
TRACKER = tracker.hpp $(URL)
HTTP_RESPONSE = http_response.hpp $(ALGORITHM) $(HEADER) $(CHUNKED_STREAM)
HTTP_REQUEST = http_request.hpp $(HEADER) $(HTTP_RESPONSE) $(URL)
CATEGORIZER = categorizer.hpp $(URL)
DOWNLOAD_MANAGER = download_manager.hpp $(CATEGORIZER) $(DOWNLOAD_FACTORY) $(TASK_POOL) $(TRACKER) $(URL)
DOWNLOADER = downloader.hpp $(HTTP_REQUEST) $(URL)
EXTRACTOR = extractor.hpp $(URL)
URL_FINDER = url_finder.hpp $(EXTRACTOR) $(STRIPPER) $(URL)
FILE_DOWNLOADER = file_downloader.hpp $(DOWNLOADER) $(HTTP_REQUEST) $(HTTP_RESPONSE) $(PATH_) $(URL)
FILE_DOWNLOAD_FACTORY = file_download_factory.hpp $(DOWNLOAD_FACTORY) $(FILE_DOWNLOADER) $(URL)
PAGE_DOWNLOADER = page_downloader.hpp $(DOWNLOADER) $(DOWNLOAD_MANAGER) $(HTTP_REQUEST) $(HTTP_RESPONSE) $(URL) $(URL_FINDER)
PAGE_DOWNLOAD_FACTORY = page_download_factory.hpp $(DOWNLOAD_FACTORY) $(DOWNLOAD_MANAGER) $(PAGE_DOWNLOADER) $(URL) $(URL_FINDER)
SPIDER = spider.hpp $(CATEGORIZER) $(DOWNLOADER) $(DOWNLOAD_MANAGER) $(EXTRACTOR) $(FILE_DOWNLOAD_FACTORY) $(PAGE_DOWNLOAD_FACTORY) $(STRIPPER) $(TASK_POOL) $(TRACKER) $(URL) $(URL_FINDER)
MAIN = $(CATEGORIZER) $(ENVIRONMENT) $(PATH_) $(SPIDER) $(URL)

all: spider

extractor: spider
	LD_LIBRARY_PATH=/usr/lib:/usr/lib64	./spider --url=$(SITE) --directory=$(DIR)

.PHONY : spider
spider: $(OBJS)
	g++ $(OBJS) -lboost_filesystem -lboost_regex -lboost_system -lpthread -lboost_program_options -o spider $(CFLAGS)
	
# object files
	
categorizer.o: categorizer.cpp $(CATEGORIZER)
	g++ -c categorizer.cpp $(CFLAGS)
	
counter.o: counter.cpp $(COUNTER)
	g++ -c counter.cpp $(CFLAGS)

downloader.o: downloader.cpp $(DOWNLOADER)
	g++ -c downloader.cpp $(CFLAGS)

download_manager.o: download_manager.cpp $(DOWNLOAD_MANAGER)
	g++ -c download_manager.cpp $(CFLAGS)

environment.o: environment.cpp $(ENVIRONMENT)
	g++ -c environment.cpp $(CFLAGS)
	
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

path.o: path.cpp $(PATH_)
	g++ -c path.cpp $(CFLAGS)

spider.o: spider.cpp $(SPIDER)
	g++ -c spider.cpp $(CFLAGS)

stripper.o: stripper.cpp $(STRIPPER)
	g++ -c stripper.cpp $(CFLAGS)
	
task_pool.o: task_pool.cpp $(TASK_POOL)
	g++ -c task_pool.cpp $(CFLAGS)
	
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
