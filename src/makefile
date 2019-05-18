main: main.cpp main.hpp predictor.cpp predictor.hpp PSSM.cpp PSSM.hpp sequence.cpp sequence.hpp train_test_split.cpp train_test_split.hpp
	g++ -Wall -Wextra -Werror -c predictor.cpp
	g++ -Wall -Wextra -Werror -c PSSM.cpp
	g++ -Wall -Wextra -Werror -c sequence.cpp
	g++ -Wall -Wextra -Werror -c train_test_split.cpp
	g++ -Wall -Wextra -Werror -o main main.cpp predictor.o PSSM.o sequence.o train_test_split.o 

svm-linear:
	../libsvm-3.23/svm-train -t 0 ../data/train_svm.txt ../data/train_svm_linear.txt.model
	../libsvm-3.23/svm-predict ../data/test_svm.txt ../data/train_svm_linear.txt.model ../data/test_svm_linear.out

svm-RBF:
	../libsvm-3.23/svm-train -t 2 ../data/train_svm.txt ../data/train_svm_RBF.txt.model
	../libsvm-3.23/svm-predict ../data/test_svm.txt ../data/train_svm_RBF.txt.model ../data/test_svm_RBF.out


clean:
	rm -f main
	rm -f predictor.o
	rm -f PSSM.o
	rm -f sequence.o
	rm -f train_test_split.o