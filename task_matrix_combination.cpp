#include <iostream>
#include <fstream>
#include <iomanip>
#include <cassert>
#include <vector>
#include <string>


class Matrix{
public:
	Matrix(unsigned int _rows,unsigned int _columns)
		: rows(_rows)
		, columns(_columns)
	{
		matrix_number = 0;
		mtx.resize(rows, std::vector<int>(columns));
	}

	const int& operator() (unsigned int i, unsigned int j) const {
		return mtx[i][j];
	}

	int& operator() (unsigned int i, unsigned int j) {
                return mtx[i][j];
        }

	unsigned int get_rows_number() const { return rows; }

	unsigned int get_columns_number() const	{ return columns; }

	unsigned int get_matrix_number() const	{ return matrix_number; }

	void set_matrix_number(unsigned int n)	{ matrix_number = n; }

private:
	unsigned int rows;
	unsigned int columns;
	unsigned int matrix_number;

	std::vector<std::vector<int>> mtx;
};


class Matrix_File_Reader
{
public:
	Matrix_File_Reader(const char * fn)
		: file_name(fn)
	{
		fin.open(file_name, std::ios::in);
		assert(fin.is_open() && "File opening failure!\n");
		//reading matrix sizes from first row of file
		char delimiter;
		std::string  text;
		if(!(fin >> matrix_rows >> delimiter >> matrix_columns >> text)){
			std::cout << "Cannot read first row values from file!\n";
			exit(1);
		}
	}

	bool reading(int& n_out){
		if (fin >> n_out){
			return true;
		}
		else{
			return false;
		}
	}

	bool symbol_reading(char& c_out) {
                if (fin >> c_out){
                        return true;  
	       	}
		else  {
                        return false;
                }
        }

	~Matrix_File_Reader(){
		fin.close();
	}

	unsigned int get_rows_value() { return matrix_rows; }

        unsigned int get_columns_value() { return matrix_columns; }

private:
	const char * file_name;
	std::ifstream fin;
	unsigned int matrix_rows;
	unsigned int matrix_columns;
};


class Matrix_File_Writer
{
public:
        Matrix_File_Writer(const char * fn, int sw,
			unsigned int _matrix_rows, unsigned int _matrix_columns)
		: file_name(fn)
		, stream_width(sw)
		, matrix_rows(_matrix_rows)
		, matrix_columns(_matrix_columns)
		, matrix_counter(1)
        {
                fout.open(file_name, std::ios::out);
		assert(fout.is_open() && "File opening failure!\n");
		// writing matrix size to first row of file
		if(!(fout << matrix_rows << " x " << matrix_columns << " (matrix_dimensions)\n"))
		{
                        std::cout << "Cannot write first row values to file!\n";
                        exit(1);
                }
        }

	//writing data without any special width
	bool writing_num(int n) {  
                if (fout << n) {
			return true;
		}
		else{
			return false;
		}
        }

	//writing data with predefined special width
	bool writing(int n){               
		fout << std::setw(stream_width);
		writing_num(n);
	}

	bool symbol_writing(char c) {
		if (fout << c) {
			return true;
		}
		else {
			return false;
		}
	}

	~Matrix_File_Writer()
        {
                fout.close();
        }

	unsigned int get_rows_value() { return matrix_rows; }

	unsigned int get_columns_value() { return matrix_columns; }

	unsigned int get_matrix_counter() { return matrix_counter; }

	void increase_matrix_counter() {
		++matrix_counter;
	}

private:
        const char * file_name;
	std::ofstream fout;
	int stream_width;
	unsigned int matrix_rows;
	unsigned int matrix_columns;
	unsigned int matrix_counter;
};


bool Read_Matrix(Matrix& M, Matrix_File_Reader& fr) {
	assert(M.get_rows_number() == fr.get_rows_value() && " rows incompatibility!\n");
        assert(M.get_columns_number() == fr.get_columns_value() && " columns incompatibility!\n");

	char c;
	int mat_num;

	if(fr.symbol_reading(c) && fr.reading(mat_num) && fr.symbol_reading(c))	{
		M.set_matrix_number(mat_num);
	}
	else {
		return false;
	}

	for(int i = 0; i < M.get_rows_number(); ++i) {
		for(int j = 0; j < M.get_columns_number(); ++j) {
			int n;
			if(fr.reading(n)) {
				M(i,j) = n;
			}
			else {
				return false;
			}
		}
	}

	return true;
}

bool Write_Matrix (const Matrix &M, Matrix_File_Writer& fw) {
	assert(M.get_rows_number() == fw.get_rows_value() && " rows incompatibility!\n");
	assert(M.get_columns_number() == fw.get_columns_value() && " columns incompatibility!\n");

	if (!fw.symbol_writing('<') || !fw.writing_num(fw.get_matrix_counter())
		       	|| !fw.symbol_writing('>') || !fw.symbol_writing('\n'))
	 {
	 	std::cout << "Cannot write matrix number.\n";
	 	return false;
	 }

	for(int i = 0; i < M.get_rows_number(); ++i) {
		for(int j = 0; j < M.get_columns_number(); ++j)	{
			if(!fw.writing(M(i,j)) || !fw.symbol_writing(' ')) {
				return false;
			}
		}
		if(!fw.symbol_writing('\n')) {
			return false;
		}
	}

	fw.symbol_writing('\n');

        std::cout << '<'<<fw.get_matrix_counter() << "> Martix successfully written to file.\n";
        fw.increase_matrix_counter();

	return true;
}


void Sum_Matrix(const Matrix& M_1, const Matrix& M_2, Matrix &R) {
	
	assert(M_1.get_rows_number() == M_2.get_rows_number() && " rows incompatibility!");
	assert(M_1.get_columns_number() == M_2.get_columns_number() && " columns incompatibility!");
	assert(M_1.get_rows_number() == R.get_rows_number() && " rows incompatibility!");
        assert(M_1.get_columns_number() == R.get_columns_number() && " columnss incompatibility    !");

	for(int i = 0; i < M_1.get_rows_number(); ++i) {
                for(int j = 0; j < M_1.get_columns_number(); ++j) {
                       R(i,j) = M_1(i, j) + M_2(i, j);
                }
        }

        std::cout << "Matrices summation succeed!\n";
}


void Mult_Matrix(const Matrix& M_1, const Matrix &M_2, Matrix &R) {
	assert(M_1.get_columns_number() == M_2.get_rows_number() && " Incompatibility of matrix sizes!");
        assert(M_1.get_rows_number() == R.get_rows_number() && " Incompatibility of matrix sizes!");
        assert(M_2.get_columns_number() == R.get_columns_number() && " Incompatibility of matrix sizes!");

        for(int i = 0; i < M_1.get_rows_number(); ++i) {
                for(int j = 0; j < M_2.get_columns_number(); ++j) {
		       int s = 0;
                       for(int k = 0; k < M_2.get_rows_number(); ++k) {
	     		       s += M_1(i, k) * M_2(k, j);
		       }
		       R(i, j) = s;
		}
        }

	std::cout << "Matrices multiplication succeed!\n";
}



int main() {
	Matrix_File_Reader A_fr("a.txt"),
			   B_fr("b.txt"),
			   C_fr("c.txt"),
			   D_fr("d.txt");

	int stream_width = 7; //just for visualisation

	Matrix_File_Writer R_fw("result.txt",
				stream_width,
				A_fr.get_rows_value(),
				D_fr.get_columns_value());

	Matrix A(A_fr.get_rows_value(), A_fr.get_columns_value()),
	       B(B_fr.get_rows_value(), B_fr.get_columns_value()),
	       T1(A_fr.get_rows_value(), A_fr.get_columns_value()),
	       C(C_fr.get_rows_value(), C_fr.get_columns_value()),
	       D(D_fr.get_rows_value(), D_fr.get_columns_value()),
	       T2(C_fr.get_rows_value(), C_fr.get_columns_value()),
	       R(A_fr.get_rows_value(), D_fr.get_columns_value());
	
	unsigned int counter = 1;
	while (Read_Matrix(A, A_fr) && Read_Matrix(B, B_fr) &&
	       Read_Matrix(C, C_fr) && Read_Matrix(D, D_fr))
	{
		std::cout << "<"<< counter <<"> Martices successfully read from files.\n";
		++counter;

		Sum_Matrix(A, B, T1);
		Sum_Matrix(C, D, T2);
	
		Mult_Matrix(T1, T2, R);
		Write_Matrix(R, R_fw);
	}

	return 0;
}

