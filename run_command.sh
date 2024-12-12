gcc -I./include -o program src/main.c src/lexer.c src/parser.c src/ds.c
source_file="Source.txt"
EC=$?

if [ $EC -eq 0 ]; then
	echo "Compile was successful."
	echo "Starting the test program."
	echo ""
	cat "$source_file" | ./program
else
	echo "Compile Failed"
fi

