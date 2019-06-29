# Interface to execute different SVM commands

read -p "Have you first executed 'make' and './main' in src, and 'make clean && make' in libsvm-3.23? (y/n) " makedone

if [[ "$makedone" != "y" ]]; then
exit
fi

read -p "Which dataset are you using? (EUKSIG_13_small / GRAM-SIG_13 / GRAM+SIG_13) " dataset

read -p "Which libsvm kernel do you wish to use? (0/1/2/3/4) " kernel

if [[ "$kernel" == "4" ]]; then
read -p "Identity matrix (identity) or BLOSUM62 (blosum62)? " matrix
else
matrix="default_kernels"
fi

read -p "Do you wish to execute grid.py instead of svm-train and svm-predict? (y/n) " grid_choice

if [[ "$grid_choice" == "y" ]]; then
if [[ "$kernel" == "0" || "$kernel" == "4" ]]; then  # the value of gamma is not relevant, only search different values of C
echo "../libsvm-3.23/tools/grid.py -t $kernel -log2g null -out ../data/outputs/$dataset/t"$kernel"_"$matrix"_grid.out ../data/train/$dataset/$matrix.txt"
../libsvm-3.23/tools/grid.py -t $kernel -log2g null -out ../data/outputs/$dataset/t"$kernel"_"$matrix"_grid.out ../data/train/$dataset/$matrix.txt
exit
else  # search for values of both C and gamma
echo "../libsvm-3.23/tools/grid.py -t $kernel -out ../data/outputs/$dataset/t"$kernel"_"$matrix"_grid.out ../data/train/$dataset/$matrix.txt"
../libsvm-3.23/tools/grid.py -t $kernel -out ../data/outputs/$dataset/t"$kernel"_"$matrix"_grid.out ../data/train/$dataset/$matrix.txt
exit
fi
fi

read -p "Do you wish to set parameters C and g? (y/n) " set_cg

if [[ "$set_cg" == "y" ]]; then
read -p "Choose a value for C: " C
read -p "Choose a value for g: " g
C_opt=" -c $C"  # option
g_opt=" -g $g"
C_ext="_c$C"  # extension in filename
g_ext="_g$g"
else
C_opt=
g_opt=
C_ext=
g_ext=
fi

train_filename=../data/train/$dataset/$matrix.txt
model_filename=../data/models/$dataset/t"$kernel"_$matrix$C_ext$g_ext.model
test_filename=../data/test/$dataset/$matrix.txt
prediction_filename=../data/outputs/$dataset/t"$kernel"_$matrix$C_ext$g_ext.out
score_filename=../data/outputs/$dataset/t"$kernel"_$matrix$C_ext$g_ext.txt

echo "Training file used: $train_filename"
echo "Test file used: $test_filename"
echo ""

echo "The following files will be created:"
echo "Model: $model_filename"
echo "Prediction: $prediction_filename"
echo "Scores: $score_filename"

# Train:
echo ""
echo "Training:"
echo "../libsvm-3.23/svm-train -t $kernel $C_opt $g_opt $train_filename $model_filename"
../libsvm-3.23/svm-train -t $kernel $C_opt $g_opt $train_filename $model_filename

# Predict:
echo ""
echo "Prediction:"
echo "../libsvm-3.23/svm-predict $test_filename $model_filename $prediction_filename > $score_filename"
../libsvm-3.23/svm-predict $test_filename $model_filename $prediction_filename > $score_filename
cat $score_filename