main: main.cpp main.hpp PSSM.cpp PSSM.hpp sequence.cpp sequence.hpp train_test_split.cpp train_test_split.hpp
	g++ -c PSSM.cpp
	g++ -c sequence.cpp
	g++ -c train_test_split.cpp
	g++ -o main main.cpp PSSM.o sequence.o train_test_split.o

sequence: sequence.cpp sequence.hpp
	g++ -o sequence sequence.cpp

train_test_split: train_test_split.cpp train_test_split.hpp
	g++ -o train_test_split train_test_split.cpp

clean:
	rm -f main
	rm -f PSSM.o
	rm -f sequence.o
	rm -f train_test_split.o