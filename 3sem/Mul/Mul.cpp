#include "stdafx.h" //! Ýòî äëÿ âèæàëêè

#include <stdio.h>
#include <tchar.h>
#include <fstream>
#include <vector>
#include <time.h>
#include <omp.h>
#include <iostream>

/*
 * Нормально, засчитано, но...
 * 1) Не здорово писать using namespace std;
 * если из std вам необходима пара сущностей.
 * Лучше их перечислить явно:
 * using std::ifstream;
 * usgin std::ofstream;
 * ...
 * 2) Желательно глазами увидеть хоть какой-то выигрыш от использования многопоточности.
 * Готов спорить, что для матриц размером 10 на 10 его нет, т.к. дополнительные расходы на создание нитей и т.д. сравнимы с перемножением 10 чисел.
 * Возьмите хотя бы 1000 на 1000. Для измерения времени работы именно wall time интересно, как фактически прошедшнее.
 * Мне кто-то из группы показывал ускорение 2 на 4х ядрах, что, конечно, не идеал, но хоть что-то :)
 */

using namespace std;

struct squared_matrix
{
	int** Data_;

	int size_;
};

//! Primary settings
void matrix_crt   (squared_matrix *Matrix, int matrix_size);
void matrix_drt   (squared_matrix *Matrix);

//! Multiplication
void matrixes_mult (squared_matrix *A, squared_matrix *B, squared_matrix *result, int threads_number);
int  get_sum       (squared_matrix *A, squared_matrix *B, int i, int j, int size);

//! Streams
void get_matrix    (squared_matrix *Matrix, int matrix_size, ifstream &infile);
void print_matrix  (squared_matrix *Matrix, ofstream &outfile);



//! Main
//!==================================================================================
int main()
{
	int matrix_size, threads_number;
	clock_t clock_timer;
	double wall_timer;

	ifstream infile;
	infile.open("in.txt");

	ofstream outfile;
	outfile.open("out.txt");

	infile >> matrix_size >> threads_number;

	squared_matrix* A      = (squared_matrix*) calloc (1, sizeof (squared_matrix));
	squared_matrix* B      = (squared_matrix*) calloc (1, sizeof (squared_matrix));
	squared_matrix* result = (squared_matrix*) calloc (1, sizeof (squared_matrix));

	//! Инициализация
	matrix_crt (A, matrix_size);
	matrix_crt (B, matrix_size);
	matrix_crt (result, matrix_size);

	//! Считываем матрицы
	get_matrix (A, matrix_size, infile);
	get_matrix (B, matrix_size, infile);

	//! Счётчик
	clock_timer = clock();
	wall_timer = omp_get_wtime();

	//! То, ради чего мы всё это писали
	matrixes_mult (A, B, result, threads_number);

	//! Вывод ркзультатов
	outfile << "threads: " << threads_number <<  " time on clock(): " << 
            (double) (clock() - clock_timer) / CLOCKS_PER_SEC
           << " time on wall: " <<  omp_get_wtime() - wall_timer << endl;


	print_matrix (result, outfile);

	//! Очистка
	infile.close();
	outfile.close();

	matrix_drt (A);
	matrix_drt (B);
	matrix_drt (result);

	free (A);
	free (B);
	free (result);

	return 0;
}
//!==================================================================================


//! Primary settings
//! @{
//!==================================================================================
void matrix_crt   (squared_matrix *Matrix, int matrix_size)
{
	Matrix -> size_ = matrix_size;

	Matrix -> Data_ = (int**) calloc (matrix_size, sizeof (int*));

	for (int i = 0; i < matrix_size; i++)
		Matrix -> Data_[i] = (int*) calloc (matrix_size, sizeof (int));
}
//!----------------------------------------------------------------------------------
void matrix_drt   (squared_matrix *Matrix)
{
	for (int i = 0; i < Matrix -> size_; ++i)
		free (Matrix -> Data_[i]);
	
	free (Matrix -> Data_);
}
//!==================================================================================
//! @}


//! Multiplication
//! @{
//!==================================================================================
void matrixes_mult (squared_matrix *A, squared_matrix *B, squared_matrix *result, int threads_number)
{
	int matrix_size = result -> size_;

	#pragma omp parallel for num_threads (n_threads)

	for (int i = 0; i < matrix_size; ++i)
		for (int j = 0; j < matrix_size; ++j)
			result -> Data_[i][j] = get_sum(A, B, i, j, matrix_size);
}
//!----------------------------------------------------------------------------------
int  get_sum       (squared_matrix *A, squared_matrix *B, int i, int j, int size)
{
	int result = 0;

	for (int k = 0; k < size; ++k)
		result += (A -> Data_[i][k]) * (B -> Data_[k][j]);

	return result;
}
//!==================================================================================
//! @}


//! Streams
//! @{
//!==================================================================================
void get_matrix    (squared_matrix *Matrix, int matrix_size, ifstream &infile)
{
	for (int i = 0; i < matrix_size; ++i)
		for (int j = 0; j < matrix_size; ++j)
			infile >> Matrix -> Data_[i][j];
}
//!----------------------------------------------------------------------------------
void print_matrix  (squared_matrix *Matrix, ofstream &outfile)
{
	int matrix_size = Matrix -> size_;

	for (int i = 0; i < matrix_size; ++i)
	{
		for (int j = 0; j < matrix_size; ++j)
			outfile << Matrix -> Data_[i][j] << " ";
		outfile << endl;
	}
}
//!==================================================================================

