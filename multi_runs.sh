#!/bin/bash

command="./corrector -c tealeaf -f freq"

n=0;
max=1000
for ((c=1; c<=$max; c++))
do
  t=$($command | grep "corrected instruction" | awk 'n = 0; {if ($1 ~ "Successfuly") {n++} else {} } END {print n+0}')
  let n=n+t
  if (( $c % 10 == 0 ))
  then
	percent=$(awk "BEGIN { pc=100*${n}/${c}; print pc}")
	echo "Progress: successfuly recovered $n out of $c ($percent%)"
  fi
done

percent=$(awk "BEGIN { pc=100*${n}/${max}; print pc}")

echo "Successfuly recovered $n out of $max ($percent%)"