// MyBoychuk.cpp: определ€ет точку входа дл€ консольного приложени€.
//

#include "stdafx.h"
#include "Classes.h"

int main()
{
	//«адание параметров модели
	double A = 7; //весова€ константа
	double a0 = -3; //параметр модели a0
	double l = 3; //параметр модели л€мбда
	double x0 = 2; //начальное значение x
	double psi = 1; //функци€ единичного возмущени€
	//«адание шага и предела интегрировани€
	double h = 0.1;
	double end = 50;

	Model model(A, a0, l, x0); //создание модели
	Task_list task_list(model); //создание односв€зного списка на основе модели

	Solution solution(model, h, end, psi);//решение (включа€ интегрирование)
	task_list.push_front(solution); //добавление решени€ в список
	
	Output output(solution, "x.dat", "psi.dat", "u.dat", "t.dat"); //вывод рассчитаных значений в файлы
	task_list.push_front(output); //добавление вывода в список

	return 0;
}

