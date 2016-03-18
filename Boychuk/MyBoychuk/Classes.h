#include <string>
#include <vector>
#include <forward_list>
#pragma warning(disable: 4996)

using namespace std;

class Task {//абстрактный базовый класс
public:
	Task(){}
	virtual ~Task(){}
	virtual void work() = 0;
};
class Model;
class Solution;
class Output;

class Model : public Task {
public:
	Model(double A, double a0, double l, double x0) : A(A), a0(a0), l(l), x0(x0) {} //конструктор
	void work(){};
	friend Solution;

private:
	double A, a0, l, x0; //параметры модели

};

class Solution : public Task {
public:
	Solution(Model &model, double h, double end, double psi) : model(&model), h(h), end(end), psi(psi) {
		this->work();
	}
	friend Output;

private:
	Model *model;

	double psi;
	double h;
	double end;

	//векторы результатов расчётов
	vector<double> u_res;
	vector<double> x_res;
	vector<double> t_res;

	double F(double x) { //подынтегральная функция
		return (psi - x) / sqrt(model->A);
	}
	void work() { //метод решения
		double x0 = model->x0;
		double a0 = model->a0;
		double l = model->l;

		double t = 0;
		double x_prev = x0;
		double x = x0;
		double u = a0 * F(x) + x;
		x_res.push_back(x);
		t_res.push_back(t);
		u_res.push_back(u);
		double k1 = 0;
		double k2 = 0;
		double k3 = 0;
		//интегрирование с помощью метода Рунге-Кутты 4-го порядка
		for (double i = 0; i < end; i += h){
			t = i;

			k1 = h * F(x);
			t += 0.5 * h;
			x = x_prev + 0.5*k1;
			k2 = h * F(x);
			x = x_prev + 0.5 * k2;
			k3 = h * F(x);
			t += 0.5 * h;
			x = x_prev + k3;
			x = x_prev + (k1 + 2 * k2 + 2 * k3 + h * F(x)) / 6.0;
			u = a0 * F(x) + exp(-l * t) * x;
			x_prev = x;

			u_res.push_back(u);
			x_res.push_back(x);
			t_res.push_back(t);
		}

	}

};

class Output : public Task {
private:
	Solution * solution;
	string filex, filepsi, fileu, filet;

public:
	Output(Solution &solution, string filex, string filepsi, string fileu, string filet) : solution(&solution),
		filex(filex), filepsi(filepsi), fileu(fileu), filet(filet) {
		this->work();
	};

	void work() { //функция вывода данных в файлы
		FILE * File1;
		FILE * File2;
		FILE * File3;
		FILE * File4;
		//открытие файлов
		File1 = fopen(filex.c_str(), "w");
		File2 = fopen(filepsi.c_str(), "w");
		File3 = fopen(fileu.c_str(), "w");
		File4 = fopen(filet.c_str(), "w");
		//запись
		for (int i = 0; i < solution->end / solution->h; i++){
			fprintf(File1, "%+.5f\n", solution->x_res[i]);
			fprintf(File2, "%+.5f\n", solution->psi);
			fprintf(File3, "%+.5f\n", solution->u_res[i]);
			fprintf(File4, "%+.5f\n", solution->t_res[i]);
		}
		//закрытие файлов
		fclose(File1);
		fclose(File2);
		fclose(File3);
		fclose(File4);
	}
};

class Task_list {
public:
	Task_list(Task &task) {
		list.push_front(&task);
	}
	void push_front(Task &task){
		list.push_front(&task);
	}
private:
	forward_list <Task *> list;
};