spider-cpp: main.cpp
	g++ main.cpp -lboost_system -lboost_thread -lpthread --std=c++0x -o spider-cpp

.PHONY : clean
clean:
	rm spider-cpp
