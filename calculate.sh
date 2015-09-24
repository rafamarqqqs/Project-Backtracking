#!/bin/bash

for VAR in 0 1 2 3
do
    for i in {0..10}
    do
        ./main $VAR < brasil.in >> brasil.out
        ./main $VAR < usa.in >> usa.out
        ./main $VAR < europe.in >> europe.out
    done
done
