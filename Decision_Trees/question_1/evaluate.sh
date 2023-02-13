file1="./output/prediction.txt"
file2="./example_output/prediction.txt"
# store the output of diff command in a variable
missed=$(diff -y --suppress-common-lines $file1 $file2 | wc -l)
# remove space in missed variable
missed=$(echo $missed | tr -d ' ')
# multiply diff output with 5
missed_points=$(echo "scale=2; $missed * 5" | bc)
# score is 100 - missed
score=$(echo "scale=2; 40 - $missed_points" | bc)

if cmp -s -- "$file1" "$file2"; then
  echo "All Passed"
  echo "Score of: ${score}"
  
else
  echo "Failed: ${missed} test cases"
  echo "Score of: ${score}"
fi
