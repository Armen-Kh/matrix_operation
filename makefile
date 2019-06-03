.PHONY : task, clean 

task :
	@echo "===== (A + B) * (C + D) matrix composition make commands ====="
	@echo "make generate: generation of the executable file as a creator of matrix files (a.txt, b.txt, c.txt, d.txt)"
	@echo "make computer: generation of the executable file as a matrix computer"
	@echo "make clean: for cleaning some generated files"


generate : generate.exe

generate.exe : matrix_generator.cpp
	g++ -o $@ $<
	@echo "After running generate.exe,\
		the created files will be in the same directory as makefile."

computer : computer.exe

computer.exe : task_matrix_combination.cpp
	g++ -o $@ $<
	@echo "After running computere.exe,\
		the results will be displayed in the result.txt."


clean :
	rm -fv *.exe
