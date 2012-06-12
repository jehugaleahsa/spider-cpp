#spider-cpp

##Dependencies
spider-cpp is dependent on **Boost 1.49**. It requires access to the libraries:
* boost asio
* boost regex
* boost system
* boost thread
* boost unit test framework

spider-cpp was compiled on Ubuntu 12 with g++-4.6.3 installed. It was written using VIM and gedit. I used GNU make to automate my builds.

##Purpose
The hope is that given a URL, the code will eventually extract additional URLs from the resultant HTML. From those URLs, more pages will be extracted. This process should continue until all unique URLs are visited (this could take a while).

At first, I will simply make this application a simple command-line application, accepting a single parameter: the URL. Later, I may try using a command-line parser to allow for more complex commands. If I get that far, I will probably try to create a portable UI. A UI will involve a lot of UI synchronization, so it will difficult to say the least.

This project will hopefully provide enough opportunity to teach me the majority of C++11's new features. I want to use this project to demonstrate that I can make something useful with C++. Historically, I always made pointless command-line applications that never talked to the outside world. Particularly, I want to use Boost's asio to connect to a remote server and download content. Also, I want things to run quickly, so I will be playing with Boost's threading.

I am going to take a _very_ iterative approach. At first, I will just see if I can connect to a server and download some HTML. Then, I will see if I can create basic HttpRequest and HttpResponse classes. The `HttpRequest` class will allow me to specify the scheme (http/https), host, port, headers and content body. It will have a `getResponse` method for executing the request. The `getResponse` will allow me to inspect the headers and allow me to grab the response content as it becomes available. The hope is that I will be able to track the percent downloaded thus far.

As my next step, I will see if I can write a class to extract data from the HTML. For instance, URLs and base addresses. This class will take a string (the HTML) and use regular expressions to find the data it needs. It will return objects containing a URL and referrer (the source of the URL). I will be using Boost's regex library; I will need this for parsing URL's as well.

Next, I will write code that can determine whether a URL refers to an HTML page or some type of media (images, videos, etc.). Depending on the type of the content, I will create a Download object that loads the content as a string (for web pages) or saves the content to a file (for media).

Initially, I won't worry about making this process asynchronous. I will get a synchronous version working first, to encapsulate the work being done. Only after I have that working will I wrap the code. I may even implement a command-line interface (plain text or curses) before moving on to threading.

## What I've Learned So Far
I came into this project completely inexperienced writing realistic applications with C++. Previously, I only wrote simple command-line applications that worked within the STL. That means no database access, no network excess and almost no file system access. Already, I have learned a lot about writing realistic C++ applications. It has made me more appreciative of how hard it can be to write large scale software in such a low-level language. Amongst the things I have learned, here is a short list:
* make files are a lot of work.
* separate compilation allows pushing off linking until the last second.
* Boost's unit testing framework is pretty awesome, but relies on macros.
* working with Git on the command-line isn't that bad.
* Boost's asio library is a very low-level abstraction of the underlyiung socket libraries.
* argument checking in C++ is annoying and creating custom exceptions is even more annoying.
* it is hard to tell when to return std::string vs. const char *.
* I find myself reaching for Boost more often than I thought I would.
* having implementation (.cpp) in a separate file than the declaration (.hpp) takes longer and requires more memorization.
* almost never use "using namespace xyz;" (using directive); limit yourself to using declarations within functions.
* template members must be implemented in the header files (since export doesn't work).
* inline functions must be implemented in the header files, too.
* prefer taking a templated destination iterator over a collection to populate.
* always check whether a constructor takes a value or a reference type.
* when you can't figure out the cause of a segmentation fault, use gdb.
* the iostream library is extremely powerful and flexible at the same time.
* fstream only accepts char * for file names.
* fstream doesn't recognize ~ and other special path indicators. No exceptions thrown.
* prior to C++11, default function template arguments weren't allowed.
* template member functions lead to ugly syntax, in many cases.
* inheriting from types in the &lt;functional&gt; header is painful.
