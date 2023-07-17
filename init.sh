#!/bin/bash

# Define the range of integers for the loop
start=1
end=128 # n 

# Loop over the range and run the command
for ((i=start; i<=end; i++))
do
#             r k  id
    ./process 3 0 "$i"&
done
wait
