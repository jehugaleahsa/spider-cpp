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

## TODO
* Add the ability to specify the referrer in the page/file downloaders.
* Write class to detect URLs in HTML pages.
    * Track the URL of the current page for relative paths and fragment identifiers.
    * Override current URL with last base address, if is exists.
    * Skip JavaScript URLs.
    * Convert back slashes to forward slashes. \ -> /
* Write class for classifying URLs.
    * Determine if URL refers to web page (.html, .htm, .asp, .php, .chm, etc.).
    * Determine whether the URL refers to a desired content type (.jpeg, .mpg, etc.)
* Write code to store extracted URLs in download queue/download files.
* Spit out progress to command line as streaming text.

## What I've Learned So Far
I came into this project completely inexperienced writing realistic applications with C++. Previously, I only wrote simple command-line applications that worked within the STL. That means no database access, no network access and almost no file system access. Already, I have learned a lot about writing realistic C++ applications. It has made me more appreciative of how hard it can be to write large scale software in such a low-level language. Amongst the things I have learned, here is a short list:
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
* inheriting from types in the &lt;functional&gt; header is painful. Use ptr_fun, etc. when possible.