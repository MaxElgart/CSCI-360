g++ -std=c++14 -o decision_tree decision_tree.cpp
if [ $? != 0 ]
then
    printf "compile error"
else
    ./decision_tree
fi

