all: driver

%: %.cpp
	g++ -std=c++11 $< -o $@
