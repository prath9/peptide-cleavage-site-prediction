# defaults
small=
t=0
gc=

main: main.cpp main.hpp predictor.cpp predictor.hpp PSSM.cpp PSSM.hpp sequence.cpp sequence.hpp train_test_split.cpp train_test_split.hpp
	g++ -Wall -Wextra -Werror -c predictor.cpp
	g++ -Wall -Wextra -Werror -c PSSM.cpp
	g++ -Wall -Wextra -Werror -c sequence.cpp
	g++ -Wall -Wextra -Werror -c train_test_split.cpp
	g++ -Wall -Wextra -Werror -o main main.cpp predictor.o PSSM.o sequence.o train_test_split.o 

# Usage example: $ make svm t=0 small=_small gc="-c 512 -g 0.004"
# small=_small can be omitted: $ make svm t=0 gc="-c 512 -g 0.004" (small will default to "")
# gc=... can also be omitted: $ make svm t=0 (c will default to 10 and g to 1/num_features)
# t=... can also be omitted: $ make svm (t will default to 0)
svm:
	../libsvm-3.23/svm-train $(gc) -t $(t) ../data/train_svm$(small).txt ../data/train_svm$(small)_t$(t).txt.model
	../libsvm-3.23/svm-predict ../data/test_svm$(small).txt ../data/train_svm$(small)_t$(t).txt.model ../data/test_svm$(small)_t$(t).out

# Same usage as target svm, except that g and c can't be set.
svm-grid:
	../libsvm-3.23/tools/grid.py -t $(t) -out ../data/train_svm$(small).txt.out ../data/train_svm$(small).txt

clean:
	rm -f main
	rm -f predictor.o
	rm -f PSSM.o
	rm -f sequence.o
	rm -f train_test_split.o