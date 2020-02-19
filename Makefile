test: test.cpp
	c++ -std=c++17 -Wall -Wextra $^ -o $@

clean:
	rm -f test
