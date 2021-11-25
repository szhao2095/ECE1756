all:
	g++ -Wall -c -std=c++17 -o main.o main.cpp
	g++ -Wall -c -std=c++17 -o Mapper.o Mapper.cpp
	g++ -Wall -c -std=c++17 -o Circuit.o Circuit.cpp
	g++ -Wall -c -std=c++17 -o LogicalRam.o LogicalRam.cpp
	g++ -Wall -c -std=c++17 -o PhysicalRam.o PhysicalRam.cpp
	g++ -Wall -c -std=c++17 -o MappedRam.o MappedRam.cpp
	g++ -Wall -c -std=c++17 -o Arch.o Arch.cpp
	g++ -Wall -c -std=c++17 -o helper.o helper.cpp
	g++ -o mapper main.o Mapper.o Circuit.o LogicalRam.o PhysicalRam.o MappedRam.o Arch.o helper.o
	
clean:
	rm -f main.o Mapper.o Circuit.o LogicalRam.o PhysicalRam.o MappedRam.o Arch.o helper.o
	rm -f mapper