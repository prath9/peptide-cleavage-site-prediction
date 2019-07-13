# Peptide cleavage site prediction

## Running
* set the ```build_libsvm_datasets``` at the beginning of the ```src/main``` function to ```true```
* run ```make``` and ```./main``` inside the ```src``` directory and ```make``` inside the ```libsvm-3.23``` directory
* enter ```./main.sh``` in your terminal shell and follow the instructions there.

## Structure of our code

### Preprocessing
In this section, we describe how the raw data is transformed in order to be exploitable by our learning models:


* The function ```CreateTrainTestSets``` in class ```train_test_split``` takes the original dataset (throughout the project, we worked with ```EUKSIG_13.red```) and builds two files ```train.red``` and ```test.red```, in respective proportions 4/5 and 1/5 using a random seed.

* The class ```Sequence``` encodes a sequence of amino acids. Each instance has three attributes: the integers ```length``` and ```cleavage_site``` contain the number of amino acids in the sequence, and the position of the cleavage site. ```aa_sequence``` is a string containing the sequence of amino acids.

* ```FileToSequenceArray``` converts ```.red``` files (typically ```train.red``` and ```test.red```) into arrays of instances of ```Sequence```. As we shall see below, these arrays will be used directly for training in the case of the PSSM method, or converted into ```libsvm```-compatible files in the case of SVM.


### The classes ```Predictor``` and ```PSSM```

* The abstract class ```Predictor``` contains the functions used to predict the location of the cleavage site. It is extended by the class ```PSSM```, which implements these functions in the case of the PSSM approach.

* ```train``` takes as argument a training set (an array of instances of ```Sequence```) and the number of instances it contains, and performs training (in the ```PSSM``` implementation, it simply builds the Position Specific Scoring Matrix). Once training has been completed, the following functions can be used:

* ```WindowScore``` gives the ```PSSM``` score of a given window in a sequence.

* ```FindCleavageMax``` returns the position in a sequence such that the window surrounding it maximizes the value of ```WindowScore```.

* ```FindCleavagesThreshold``` returns all the positions in a sequence such that the score of the surrounding windows exceeds some specified threshold.

* Given an array of instances of ```Sequence```, the function ```SequenceAccuracy``` returns the fraction of sequences such that ```FindCleavageMax``` returns the true position of the cleavage site of the sequence.


### Support Vector Machines
We used the library ```libsvm``` to perform SVM on the training examples. This entailed preprocessing the data so as to convert it in a format accessible by ```libsvm```. Note that at this point, the format of the data is an array of sequences, whereas the actual dataset on which we wish to train Support Vector Machines is the set of windows (rather than the set of sequences).

Hence the following functions, written directly inside ```main.cpp```, which convert the data from an array of sequences into a vector of windows (and their associated labels):

* ```SequenceToWindows``` converts the array of instances of ```Sequence``` into a vector of strings, such that each string corresponds to a window of size ```p + q```.
* ```SequenceToLabels``` converts the same array into a vector of strings, such that each string is the label of the corresponding window ("+1" or "-1" depending on whether the window's position is that of the sequence's cleavage site). 


The two following functions use the vectors created above to create ```libsvm```-compatible training and testing files:


* ```SequenceSVMTranslate``` takes as argument the vectors built by the two functions above and uses them to write into a ```.txt``` file (the name of which is also an argument) in the ```libsvm-compatible ``` ```<label> <index1>:<value1> <index2>:<value2> ...``` format

* ```SequenceSVMTranslateSimilarityMatrix``` also writes into a file (the name of which is also an argument) in a ```libsvm```-compatible data format. However, in this case the file should allow ```libsvm``` to exploit a precomputed kernel, specified in the argument ```similarity_matrix```: the format is ```<label> 0:i 1:K(xi,x1) ... L:K(xi,xL)```. We noticed that the resulting files can quickly become very heavy (> 500 MB with 267 training instances), since the size grows quadratically with the number of instances.



We wrote a bash script ```src/main.sh``` in order to facilitate training and prediction using ```libsvm```, making it easier to choose kernels and hyperparameters. Once you have run ```make``` and ```./main``` inside the ```src``` directory (with the ```build_libsvm_datasets``` flag set to true at the beginning of the ```main```, and ```make``` inside the ```libsvm-3.23``` directory, you can enter ```./main.sh``` in your terminal shell and follow the instructions there.

### Directories

The project comprises three directories: ```src```, ```data```, and ```libsvm-3.23```. ```src``` is the source directory: it contains all the code we wrote, as detailed above.

The directory ```data``` contains all our data. To be more precise:

* ```data/datasets``` contains all the ```.red``` files, including ```train.red``` and ```test.red```.

* ```data/matrices``` contains the Identity and BLOSUM62 matrices in ```.txt``` format.
* ```data/models``` contains the models created by ```svm-train``` during training.

* ```data/outputs``` contains the output files created when running ```svm-predict```, both predictions for each instance and the associated scores (f-score, precision, recall, accuracy).

* ```data/test``` contains the test sets, in a format compatible with ```libsvm``` specifications.

* ```data/train``` contains the training sets, in a format compatible with ```libsvm``` specifications.



```libsvm-3.23``` is where the ```libsvm``` functions are stored. We modified it slightly, exactly as advised [here](https://www.csie.ntu.edu.tw/~cjlin/libsvmtools/eval/index.html), in order to validate our model based on better evaluation functions than the accuracy.
