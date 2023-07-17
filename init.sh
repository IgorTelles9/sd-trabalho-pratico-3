#!/bin/bash

# Define the range of integers for the loop
start=1
end=128

# Loop over the range and run the command
for ((i=start; i<=end; i++))
do
    ./process 10 2 "$i"&
done
wait
