spider-cpp
==========

This project will hopefully provide enough opportunity to teach me the majority of C++11's new features. The hope is that given a URL, the code will extract additional URLs from the resultant HTML. From those URLs, more pages will be extracted. This process should continue until all unique URLs are visited (this could take a while).

At first, I will simply make this application a simple command-line application, accepting a single parameter: the URL. Later, I may try using a command-line parser to allow for more complex commands. If I get that far, I will probably try to create a portable UI. A UI will involve a lot of UI synchronization, so it will difficult to say the least.