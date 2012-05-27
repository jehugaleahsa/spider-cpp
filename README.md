spider-cpp
==========

This project will hopefully provide enough opportunity to teach me the majority of C++11's new features. The hope is that given a URL, the code will extract additional URLs from the resultant HTML. From those URLs, more pages will be extracted. This process should continue until all unique URLs are visited (this could take a while).

At first, I will simply make this application a simple command-line application, accepting a single parameter: the URL. Later, I may try using a command-line parser to allow for more complex commands. If I get that far, I will probably try to create a portable UI. A UI will involve a lot of UI synchronization, so it will difficult to say the least.

I want to use this project to demonstrate that I can make something useful with C++. Historically, I always made pointless command-line applications that never talked to the outside world. Particularly, I want to use Boost.Asio to connect to a remote server and download content. Also, I want things to run quickly, so I will be playing with C++'s new threading types (although I may need to use Boost's for now).

Since I have never worked with Asio before, I am going to take a _very_ iterative approach. At first, I will just see if I can connect to a server and download some HTML. Then, I will see if I can create basic HttpRequest and HttpResponse classes. The HttpRequest class will allow me to specify the scheme (http/https), host, port, headers and content body. It will have a GetResponse method for executing the request. The GetResponse will allow me to inspect the headers and allow me to grab the response content as it becomes available. The hope is that I will be able to track the percent downloaded thus far.

As my next step, I will see if I can write a class to extract data from the HTML. For instance, URLs and base addresses. This class will take a string (the HTML) and use regular expressions to find the data it needs. It will return objects containing a URL and referrer (the source of the URL).

Next, I will write code that can determine whether a URL refers to an HTML page or some type of media (images, videos, etc.). Depending on the type of the content type, I will create a Download object that loads the content as a string (for web pages) or saves the content to a file (for media).

Initially, I won't worry about making this process asynchronous. I will get a synchronous version working first, to encapsulate the work being done. Only after I have that working, will I wrap the code. I may even implement a command-line interface (plain text or curses) before moving on to threading.