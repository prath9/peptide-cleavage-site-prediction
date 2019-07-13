#/usr/bin/env bash

# Create the necessary subdirectories in data/, if they don't already exist.
# To be run from src/.

for subdir1 in models outputs test train; do
    for subdir2 in EUKSIG_13_small GRAM-SIG_13 GRAM+SIG_13; do
        mkdir -p ../data/$subdir1/$subdir2
    done
done