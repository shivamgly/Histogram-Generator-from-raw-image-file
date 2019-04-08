#!/usr/bin/env bash
if [ "$1" == '-h' ]; then
  echo "Usage: ./profile_time.sh [ arguments shoould be provided to the histogram program ]"
  exit 0
fi
if [[ -d "histogram.c" ]]; then
  echo "Error: No histogram.c is present in the current dir"
  exit 0
fi
echo -e "Compileing and executing\n"
gcc -Wall -pg histogram.c -o test
./test $1
gprof test gmon.out > time_profile.txt
rm test gmon.out
echo -e "\nDone, the report is saved in time_profile.txt\n\n"