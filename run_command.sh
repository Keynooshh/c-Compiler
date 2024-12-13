deploy_dir="./deploy"
source_directory="${deploy_dir}/source_files"
source_file=$(find "$source_directory" -type f )
compiler="${deploy_dir}/compiler_program"
asm_file="asm_compiled.asm" 
bin_asm_file="compiled_application"

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
fasm "$asm_file" "$bin_asm_file"
EC=$?

if [ $EC -eq 0 ];then
	chmod +x "$bin_asm_file"
else
	echo "FASM Failed"
	exit 1
fi

echo "\n File has been created -> $bin_asm_file"

echo "\n============================================="
