spider-cpp: main.cpp
	g++ main.cpp -o spider-cpp

.PHONY : clean
clean:
	rm spider-cpp
