deploy_dir="./build"
source_directory="${deploy_dir}/sources"
source_file=$(find "$source_directory" -type f )
compiler="${deploy_dir}/compiler"
asm_file="application.asm" 
bin_asm_file="compiled_app"


if ! command -v gcc &> /dev/null; then
    	echo "Error: GCC is not installed or not in $PATH."
    	echo "Please install GCC to compile the source files."
    	exit 1
fi

if ! command -v fasm &> /dev/null; then
	echo "Error: FASM is not installed or not in $PATH."
	echo "Please install Flat Assembler (FASM) to assemble the output."
	exit 1
fi



echo "=============================================\n"
gcc -I ./include -o "$compiler" src/*
EC=$?

if [ "$asm_file" ];then
	rm -rf "${deploy_dir}/${asm_file}"
	
fi

if [ "$bin_asm_file" ];then
	rm -rf "${deploy_dir}/${bin_asm_file}"
fi
if [ $EC -eq 0 ]; then
	echo "Compile was successful -> ./program"
	echo "The source file is as:"
	echo "--------------------------------------------\n"
	cat "$source_file"
	echo "\n\n---------------------------------------------\n"
	cat "$source_file" | "$compiler" | tee "${deploy_dir}/${asm_file}"
else
	echo "Compile Failed"
	exit 1
fi

echo "\n---------------------------------------------\n"


echo "Using Flat Assmebler (Fasm) V$(fasm | awk '{print $4;exit;}'):"
cd "$deploy_dir"
fasm "$asm_file" "$bin_asm_file" > /dev/null
EC=$?

if [ $EC -eq 0 ];then
	chmod +x "$bin_asm_file"
else
	echo "FASM Failed"
	exit 1
fi

echo "\n  File has been created -> ${deploy_dir}/${bin_asm_file}"

echo "\n============================================="
