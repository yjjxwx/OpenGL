#!/bin/bash
#Compile the fold in the file
dir=$1
if [ -d "$dir" ]
then
	echo "Start Comiler"
	cd $dir
	if make clean && make
	then
		echo "Compile End"
		echo "Start execute"
		./$dir.e
		echo "End execute"	
	else
		echo "Compile Error"
	fi
else
	echo "$dir is not a directory, please input a directory name"
fi
