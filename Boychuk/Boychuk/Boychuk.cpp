// Boychuk.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#define _USE_MATH_DEFINES

#include <list>
#include <iostream>
#include <exception>
#include <forward_list>
using namespace std;

#include "Task_classes.h"


int main(){
	const double A = 10.0;
	const double a0 = 10.0;
	const double lambda = 5.0;
	const double x0 = 1.0;
	const double x0_dir = -1.0;
	const double h = 0.1;
	const double end_time = 100;  //in seconds
	const double B = 9;

	try{
		Task_model TM_1(A, a0, lambda, x0);
		Task_model TM_2(A, a0, lambda, x0, B, x0_dir);
		Task_solve TS_1(TM_1, h, end_time);
		Task_solve TS_2(TM_2, h, end_time);
		Task_solution_view TSV_1(TS_1, "gnufile_NonZeroInitCond_1order.plot");
		Task_solution_view TSV_2(TS_2, "gnufile_NonZeroInitCond_2order.plot");
		// put classes into a forward list
		forward_list <Task_base *> models_List;
		models_List.push_front(&TM_1); //put task_model
		models_List.push_front(&TS_1); //put task_solve
		models_List.push_front(&TSV_1); //put solution view class
		models_List.push_front(&TM_2); //put task_model
		models_List.push_front(&TS_2); //put task_solve
		models_List.push_front(&TSV_2); //put solution view class

		//get 2 equation solution
		models_List.front()->print();  //print solution
		models_List.pop_front();  //drop objects before the next solution view object
		models_List.pop_front();
		models_List.pop_front();

		//get 1 equation solution
		models_List.front()->print();  //print solution
		models_List.pop_front();  //drop objects before the next solution view object
		models_List.pop_front();
		models_List.pop_front();

	}
	catch (exception &e){
		cout << e.what() << "\n";
		return -1;
	}

	return 0;
}

