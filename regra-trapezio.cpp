/*
Gustavo Rodrigues Sousa - DCC/UFLA - 2020
Programação Paralela e Concorrente
Programa para calcular a área de uma integral usando
a regra do trapézio e multithreading e também mostrar o tempo de execução
nota: passar a flag -lpthread ao compilar
*/

#include <iostream>
#include <cmath>
#include <pthread.h>
#include <chrono> 

using namespace std;

const int NUM_THREADS = 8;
int iterador = 0;
int n;
double integral, a, b, h, x1, x2;

double f(double x){
	double return_val;
	return_val = x*sqrt(x*x+9); //insira sua função aqui
	return return_val;
}

// calcula a área do trapézio entre x1 e x2
void* calcula_area_trapezio(void* arg) {
	integral += abs((f(x1) + f(x2)) * h / 2.0);
	x1 += h;
	x2 += h;
	iterador++;
	pthread_exit(NULL);
}

void input() {
	while (1) {
		cout << "Entre com os valores de a, b, n" << endl;
		cin >> a >> b >> n;
		if (b == a) cout << "b não pode ser igual a a" << endl;
		if (n <= 0) cout << "n deve ser maior ou igual a zero" << endl;
		else if (b < a) {
			int c = a;
			a = b;
			b = c;
		} else break;
	}
}

int main() {
	input();
	// grava tempo de início da execução
	auto start_time = chrono::high_resolution_clock::now();
	
	h = (b-a)/n;
	x1 = a;
	x2 = a+h;
	
	// criar as threads iniciais e designar as tarefas a elas
	pthread_t threads[NUM_THREADS];
	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_create(&threads[i], NULL, calcula_area_trapezio, (void*)NULL);
	}
	for (int i = NUM_THREADS; i < n; i++) {
		// escolher a thread com base no módulo de i para garantir alternância entre elas
		int thread_responsavel = i % NUM_THREADS;
		// esperar a thread alocada naquele espaço do array terminar
		pthread_join(threads[thread_responsavel], NULL);
		pthread_create(&threads[thread_responsavel], NULL, calcula_area_trapezio, (void*)NULL);
	}
	
	// grava tempo de término da execução e sua duração total
	auto stop_time = chrono::high_resolution_clock::now();
	auto execution_time = chrono::duration_cast<chrono::milliseconds>(stop_time - start_time);
	long int tempo = execution_time.count();
	
	cout << "Valor da integral: " << integral << endl;
	cout << "Tempo de execução: " << tempo << "ms" << endl;
	cout << "threads: " << NUM_THREADS << endl;
	
	return 0;
}
