#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <string>

class File_Reader
{
	public:
		File_Reader(const char * fn)
			:file_name(fn)
		{
			fin.open(file_name, std::ios::in);
			assert(fin.is_open() && "File opening failure!\n");
			
			char d;
			std::string text;
			if(!(fin >> matrix_rows >> d >> matrix_columns >> text))
			{
				std::cout << "Cannot read first row values from file!\n";
	  			exit(1);
			}

			matrix_counter = 0;
		}

		~File_Reader()
		{
			fin.close();
		}

		unsigned int get_rows()
		{
			return matrix_rows;
		}

		unsigned int get_columns()
		{
			return matrix_columns;
		}

		bool int_reading(int& i_out)
		{
			if(fin >> i_out){
				return true;
			}
			return false;
		}

		bool u_int_reading(unsigned int& i_out)
                {
                        if(fin >> i_out){
                                return true;
                        }
                        return false;
                }


		bool char_reading(char& c_out)
		{
			if(fin >> c_out){
				return true;
			}
			return false;
		}

		bool text_reading(std::string& t_out)
		{
			if(fin >> t_out)
			{
				return true;
			}
			return false;
		}

		unsigned int get_matrix_counter()
		{
			return matrix_counter;
		}

		void increment_matrix_counter()
		{
			++matrix_counter;
		}

	private:
		const char * file_name;
		std::ifstream fin;
		unsigned int matrix_rows;
		unsigned int matrix_columns;
		unsigned int matrix_counter;
};


class File_Writer
{
	public:
		File_Writer(const char* fn)
			:file_name(fn)		
		{
			fout.open(file_name, std::ios::out);
			assert(fout.is_open() && "File opening failure!\n");
		}

		~File_Writer()
		{
			fout.close();
		}

		bool float_writing(float f)
		{
			if(fout << std::fixed << std::setprecision(2) << f){
				return true;
			}
			else{
				return false;
			}
		}

		bool int_writing(int i)
		{
			if(fout << i){
				return true;
			}else{
				return false;
			}
		}

		bool char_writing(char c)
		{
			if(fout << c)	{
				return true;
			}else{
				return false;
			}
		}

		bool text_writing(std::string t)
		{
			if(fout << t){
				return true;
			}else{
				return false;
			}
		}

	private:
		const char* file_name;
		std::ofstream fout;
};

class Matrix
{
	public:
		Matrix(unsigned int _number, unsigned int _rows, unsigned int _columns)
			: rows(_rows)
			, columns(_columns)
			, number(_number)
		{
			data = new int[rows * columns];
		}

		~Matrix()
		{
			delete [] data;
		}

		int& operator()(unsigned int i, unsigned int j) 
                {
			return data[i * columns + j];
		}


		int operator()(unsigned int i, unsigned int j) const
		{
			return data[i * columns + j];
		}

		void set_number(unsigned int n)
		{
			number = n;
		}

		unsigned int get_number() const
		{
			return number;
		}

		unsigned int get_rows() const
		{
			return rows;
		}

		unsigned int get_columns() const
		{
			return columns;
		}
			
		
	private:
		unsigned int number;
		unsigned int rows;
		unsigned int columns;
		int * data;
};


bool Read_Matrix(File_Reader& fr, Matrix& M)
{
	char c;
	unsigned int number;
	if(fr.char_reading(c) && fr.u_int_reading(number) && fr.char_reading(c)){
		M.set_number(number);
	}
	else{
		return false;
	}
	
	for(int i = 0; i < fr.get_rows(); ++i)	{
		for(int j = 0; j < fr.get_columns(); ++j){
			if(!(fr.int_reading(M(i, j)))){
				return false;
			}
		}
	}

	fr.increment_matrix_counter();

	return true;
}


bool Compare_Matrix(const Matrix& M1, const Matrix& M2, File_Writer& fw)
{
	assert(M1.get_rows() == M2.get_rows() && " rows incompatibility!\n");
        assert(M1.get_columns() == M2.get_columns() && " columns incompatibility!\n");

	if(M1.get_number() != M2.get_number())	{
		fw.text_writing("Divergence of matrix number: <");
		fw.int_writing (M1.get_number());
		fw.text_writing("> != <");
		fw.int_writing (M2.get_number());
		fw.text_writing(">\n");
	}

	for(int i = 0; i < M1.get_rows(); ++i)	{
		for(int j = 0; j < M1.get_columns(); ++j)	{
			if(M1(i, j) != M2(i, j)){
				fw.text_writing("Divergence of matrices: <");
				fw.int_writing (M1.get_number());
	       			fw.text_writing("> , element(");
				fw.int_writing(i);
				fw.text_writing(", ");
				fw.int_writing(j);
				fw.text_writing(")\n");
				return false;
			}
		}
	}	

	return true;
}


void Summarize(File_Writer& fw, unsigned int matrix_contradiction, unsigned int tested_matrices )
{
	if(tested_matrices == 0){
		if(!fw.text_writing("There is no any data for compare!\n")){
			std::cout << "Cannot write summarized data to file!\n";
			exit(1);
		}
		return;
	}

	if(!(	fw.text_writing("\n=======================================================================\n")
		&& fw.text_writing("                     CHECKED MATRICES: ")
		&& fw.int_writing(tested_matrices)
		&& fw.text_writing("\n=======================================================================\n")

		&& fw.text_writing("Divergence of matrices: quantity = ")
		&& fw.int_writing(matrix_contradiction)
		&& fw.text_writing(", percentage = ")
		&& fw.float_writing(float(matrix_contradiction) / tested_matrices * 100)
        	&& fw.text_writing(" %.\n"))){
			std::cout << "Cannot write summarized data to file!\n";
			exit(1);
	}
}



int main(){

	File_Reader Result_fr("result.txt"),
	    	    Origin_fr("origin.txt");

	File_Writer Test_Result_fw("test_result.txt");

	if(Result_fr.get_rows() != Origin_fr.get_rows() ||
		Result_fr.get_columns() != Origin_fr.get_columns()){
		if(!Test_Result_fw.text_writing("Detected discrepies: Divergence of matrix dimensions (first row)!\n")){
			std::cout<< "Cannot write error description to file! (Divergence of matrices dimension!)\n";
			exit(1);
		}
		std::cout << "The result of test is presented in the \"test_result.txt\" file.\n";
                return 0;
	}

	Matrix RM(Result_fr.get_matrix_counter(), Result_fr.get_rows(), Result_fr.get_columns()),
	       OM(Origin_fr.get_matrix_counter(), Origin_fr.get_rows(), Origin_fr.get_columns());

	unsigned int matrix_contradiction = 0;

	while(Read_Matrix(Result_fr, RM) && Read_Matrix(Origin_fr, OM))	{
		if(!Compare_Matrix(RM, OM, Test_Result_fw)){
			++matrix_contradiction;
		}
	}
	
	Summarize(Test_Result_fw, matrix_contradiction, Result_fr.get_matrix_counter());

	std::cout << "The result of test is presented in the \"test_result.txt\" file.\n";

	return 0;
}
