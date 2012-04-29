#!/bin/bash

for i in {1..50}
do
    ./random_input 100
    java NumberPartition
done

