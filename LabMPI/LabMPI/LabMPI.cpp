#include <iostream>
#include <mpi.h>
#include <vector>
#include <iomanip>

using namespace std;
void max_sum(int*, int*, int*, MPI_Datatype*);

const int CONST_N = 5; // Заданное значение


void max_sum(int* in, int* inout, int* len, MPI_Datatype* datatype) {

	for (int i = 0; i < *len; i++) {
		if (in[i] != CONST_N) {
			inout[i] = inout[i] + in[i];
		}
		else {
			if (inout[i] == CONST_N) {
				inout[i] = 0;
			}
			else {
				inout[i] = inout[i];
			}
		}
	}
}
int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);

	int rank, size; // rank - номер процесса, size - количество процессов 

	MPI_Comm_rank(MPI_COMM_WORLD, &rank); // помещаем в переменную rank - номер процесса
	MPI_Comm_size(MPI_COMM_WORLD, &size); // а в переменную size - количество процессов 

	int const n = 10; // Размер массива, который мы будем задавать

	vector<int> local_array(n); // создаем вектор, размером n
	vector<int> result(n); // и вектор с будущими резуальтатом, размером n

	srand(time(NULL) + rank); // Используем разные seed для каждого процесса

	for (int i = 0; i < n; i++) { // Заполняем массив случайными числами
		local_array[i] = rand() % 10 - 3;
		result[i] = 0; // все элементы результирующего вектора инициализируются нулями.
	}

	for (int y : local_array) { // выводим полученный массив, для визуализации созданных данных
		cout << setw(7) << y;
	}
	cout << endl;


	MPI_Op max_sum_op; // моя MPI функция будет в этой переменной
	MPI_Op_create((MPI_User_function*)max_sum, 1, &max_sum_op); //создание пользовательской функции, при помощи преобраования max_even в функцию MPI


	MPI_Reduce(local_array.data(), result.data(), n, MPI_INT, max_sum_op, 0, MPI_COMM_WORLD);

	MPI_Op_free(&max_sum_op); //освобождение ресурса max_even_op

	if (rank == 0) {
		printf("Sum other %i:\n", CONST_N);
		for (int y : result) {
			cout << setw(7) << y;
		}
		cout << endl;
	}

	MPI_Finalize();
	return 0;
}