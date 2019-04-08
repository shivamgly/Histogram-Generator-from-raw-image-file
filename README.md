# Histogram-Generator-from-raw-image-file
The program will generate histogram from a Raw12 image file.

## Usage and Description of files
The histogram.c is the main C file, which will take a Raw12 image as argument and would print color histogram from it.

The profile_time.sh is a bash script which will run gprof on histogram.c and 
would generate a report "time_profile.txt". You can test this by
providing a Raw12 file as a argument to this.

The Makefile is the standard Makefile:
Use `make` to generate a histogram file. You can execute this by ./histogram
use `make clean` to clean the file 
