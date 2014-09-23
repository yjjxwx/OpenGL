#!/bin/bash
#Compile the fold in the file
#g++ main.cpp -o main.e -lglut -lGLU -lGLEW -lGL
dir="`pwd`/$1"
if [ -n "$dir" ]
then
	echo "Start Comile file in $dir fold"
	files=""
	for file in $dir/*.cpp $dir/*.c
	do
		if [ -f "$file" ]
		then
			files="$files $file " 
		fi
	done
	echo "All file in $dir contains $files"
	echo "Start compile"
	g++ $files -o main.e -lglut -lGLU -lGLEW -lGL
	echo "End compile"
	echo "Start execute"
	./main.e
	echo "End execute"
else
	echo "$dir is not a directory, please input a directory name"
fi
