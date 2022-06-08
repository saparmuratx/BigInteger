src = $(wildcard *.cpp)
hdr = $(wildcard *.hpp)

CXXFLAGS = -g -std=c++11 -Wall -Wextra -Wshadow -pedantic -D_GLIBCXX_DEBUG -fsanitize=address -fsanitize=undefined
CXXRLSFLAGS = -O2 -std=c++11 -Wall -Wextra -Wshadow -pedantic


main: $(src) $(hdr)
	$(CXX) -o main $(CXXFLAGS) $(src) $(include)

.PHONY: release
release:
	$(CXX) -o main $(CXXRLSFLAGS) $(src) $(include)

.PHONY: clean
clean:
	rm -f main
