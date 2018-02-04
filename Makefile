all:
	${CXX} -std=c++11 main.cpp heap.cpp
clean:
	rm -f a.out *~
