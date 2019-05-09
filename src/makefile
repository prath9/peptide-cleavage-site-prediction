main: main.cpp main.hpp predictor.cpp predictor.hpp PSSM.cpp PSSM.hpp sequence.cpp sequence.hpp train_test_split.cpp train_test_split.hpp
	g++ -Wall -Wextra -Werror -c PSSM.cpp
	g++ -Wall -Wextra -Werror -c sequence.cpp
	g++ -Wall -Wextra -Werror -c train_test_split.cpp
	g++ -Wall -Wextra -Werror -o main main.cpp PSSM.o sequence.o train_test_split.o

clean:
	rm -f main
	rm -f PSSM.o
	rm -f sequence.o
	rm -f train_test_split.o