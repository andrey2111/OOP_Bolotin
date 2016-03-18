#ifndef _TASK_CLASSES_
#define _TASK_CLASSES_
#define _USE_MATH_DEFINES
#pragma warning(disable: 4996)


#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <math.h>
#include <exception>
#include <assert.h>

using namespace std;

class Task_base{//base class for the others
public:
	Task_base(){}
	virtual ~Task_base(){}
	virtual int solve() = 0;
	virtual int print() = 0;
};

class Task_model : public Task_base{//comprise of model parameters for especial kind of equation (look p.134) 
private:
	double A, a0, l, x0, B, x0_dir;
	string f_name;

	int solve(){ throw invalid_argument("No equation to solve"); }
	int print(){ throw invalid_argument("No data to print"); }
public:
	Task_model() :Task_base(), A(1), a0(1), l(0), x0(0), B(0), x0_dir(0){}
	Task_model(double A) :Task_base(), A(A), a0(1), l(0), x0(0), B(0), x0_dir(0){
		if (A <= 0) throw invalid_argument("non-positive value of the parameter A");
	}
	Task_model(double A, double a0) :Task_base(), A(A), a0(a0), l(0), x0(0), B(0), x0_dir(0){
		if (A <= 0) throw invalid_argument("non-positive value of the parameter A");
	}
	Task_model(double A, double a0, double l, double x0) : Task_base(), A(A), a0(a0), l(l), x0(x0), B(0), x0_dir(0){
		if (A <= 0) throw invalid_argument("non-positive value of the parameter A");
	}
	Task_model(double A, double a0, double l, double x0, double B) : Task_base(), A(A), a0(a0), l(l), x0(x0), B(B), x0_dir(0){
		if (A <= 0) throw invalid_argument("non-positive value of the parameter A");
		//condition on constants A and B
		else if (B <= 0) throw invalid_argument("non-positive value of the parameter A");
		else if (A <= 2 * sqrt(B)) throw invalid_argument("Condition A>2sqrt(B) isn't executed");
	}
	Task_model(double A, double a0, double l, double x0, double B, double x0_dir) : Task_base(), A(A), a0(a0), l(l), x0(x0), B(B), x0_dir(x0_dir){
		if (A <= 0) throw invalid_argument("non-positive value of the parameter A");
		//condition on constants A and B
		else if (B <= 0) throw invalid_argument("non-positive value of the parameter A");
		else if (A <= 2 * sqrt(B)) throw invalid_argument("Condition A>2sqrt(B) isn't executed");
	}
	Task_model(double A, double a0, double l, double x0, double B, double x0_dir, const string &f_name) :Task_base(), A(A), a0(a0), l(l), x0(x0), B(B), x0_dir(x0_dir), f_name(f_name){
		if (A <= 0) throw invalid_argument("non-positive value of the parameter A");
		else if (B <= 0) throw invalid_argument("non-positive value of the parameter A");
		else if (A <= 2 * sqrt(B)) throw invalid_argument("Condition A>2sqrt(B) isn't executed");
		else if (f_name != "sin(t)" && f_name != "ln(1+t)" && f_name != "1/(1+t)" && f_name != ""){
			string message;
			message.append("Incorrect perturbed function ").append(f_name);
			throw invalid_argument(message);
		}
	}
	~Task_model(){}

	//extern force
	double f(double t) const{
		//set predefined function if the user doesn't specify it himself
		if (f_name.empty()){
			return 0;
		}
		else if (f_name == "sin(t)"){
			return sin(t);
		}
		else if (f_name == "ln(t+1)"){
			return log(t + 1);
		}
		else if (f_name == "1 / (t + 1)"){
			return 1 / (t + 1);
		}
		else throw invalid_argument("Inknown type of paturbated function\nUsage: supported functions: sin(x), ln(x+1)\nby default: 1/(t+1)");
	}

	double f_dir(const double t, const double step) const{
		return (f(t + step) - f(t)) / step;
	}
	//double f_dir(double t,double step) const;

	//get
	double getA()const{ return A; }
	double getB()const{ return B; }
	double geta0()const{ return a0; }
	double getl()const{ return l; }
	double getx0()const{ return x0; }
	double getx0_dir()const{ return x0_dir; }
	string getF_name()const{ return f_name; }

};

class Task_solve : public Task_base{
private:
	const Task_model * TM;  //data
	double h; //Method step
	double end_time;  //end time of integration

	//data structures for results
	vector <double> u_rez;
	vector <double> x_rez;
	vector <double> t_rez;

	int solve(){ //solve the equation
		if (TM->getB() == 0){
			//1 order functional
			if (solveoderungekutta()) throw invalid_argument("Integration was failed");
		}
		else{
			//2 order functional
			const double step_dir = 1e-10;
			const double A = TM->getA();
			const double B = TM->getB();

			const double l1 = -sqrt((A + sqrt(pow(A, 2) - 4 * B)) / (2 * B));
			const double l2 = -sqrt((A - sqrt(pow(A, 2) - 4 * B)) / (2 * B));
			const double C1 = (TM->f_dir(0, step_dir) - TM->getx0_dir() - l2*(TM->f(0) - TM->getx0_dir())) / (l1 - l2);
			const double C2 = (-TM->f_dir(0, step_dir) + TM->getx0_dir() + l1*(TM->f(0) - TM->getx0_dir())) / (l1 - l2);

			double x_current = 0;
			double u_current = 0;
			for (double epoch_time = 0; epoch_time < end_time; epoch_time += h){
				t_rez.push_back(epoch_time);
				x_current = TM->f(epoch_time) - C1*exp(l1*epoch_time) - C2*exp(l2*epoch_time);
				x_rez.push_back(x_current);
				u_current = TM->geta0()*(TM->f_dir(epoch_time, step_dir) - C1*l1*exp(l1*epoch_time) - C2*l2*exp(l2*epoch_time)) + exp(-TM->getl()*epoch_time)*x_current;
				u_rez.push_back(u_current);
			}
		}
		return 0;
	}
	int print(){ throw invalid_argument("This kind of class can not display data"); }

	double f_B(const double t, const double x){  //specify view of the right part in dx/dt=f(t,x)
		const double step = 1e-10; //derivate step
		//specify Boychyk function
		return TM->f_dir(t, step) + (TM->f(t) - x) / sqrt(TM->getA());
	}

	int solveoderungekutta(){
		//solver for R1 dimension
		//solver for Rn dimension
		double t = 0;

		//initial value x0
		double x_prev = TM->getx0();  //first x value
		double x = x_prev;//new value
		double u = TM->geta0()* (TM->f_dir(0, 1e-10) + (TM->f(0) - x) / sqrt(TM->getA())) + x;
		x_rez.push_back(x);
		t_rez.push_back(t);
		u_rez.push_back(u);

		double k1 = 0;
		double k2 = 0;
		double k3 = 0;
		//standart RK 4 order + filling u(t)
		for (double epoch = 0; epoch < end_time; epoch += h){
			t = epoch;

			k1 = h* f_B(t, x);
			t += 0.5*h;
			x = x_prev + 0.5*k1;
			k2 = h* f_B(t, x);
			x = x_prev + 0.5*k2;
			k3 = h*f_B(t, x);
			t += 0.5*h;
			x = x_prev + k3;
			x = x_prev + (k1 + 2 * k2 + 2 * k3 + h*f_B(t, x)) / 6.0;

			x_rez.push_back(x);
			t_rez.push_back(t);

			u = (TM->geta0()) * (TM->f_dir(t, 1e-10) + (TM->f(t) - x) / sqrt(TM->getA())) + exp(-(TM->getl()) *t)*x;  //u(t)=a0dx/dt +exp(-lt)x
			u_rez.push_back(u);
			x_prev = x;
		}
		return 0;
	}
public:
	explicit Task_solve(const Task_model &TM) :Task_base(), TM(&TM), h(0.1), end_time(10){
		solve();
	};
	Task_solve(const Task_model &TM, double h) :Task_base(), TM(&TM), h(h), end_time(10) {
		if (h <= 0) throw invalid_argument("non-positive value of the parameter h (step of integration)");
		else solve();
	};
	Task_solve(const Task_model &TM, double h, double end_time) :TM(&TM), h(h), end_time(end_time){
		if (h <= 0) throw invalid_argument("non-positive value of the parameter h (step of integration)");
		if (end_time <= 0) throw invalid_argument("non-positive time of the end of integration)");
		else  solve();
	}
	~Task_solve(){};
	//get
	double getU(int iter)const{
		return u_rez[iter];
	}
	double getX(int iter)const{
		return x_rez[iter];
	}
	double getT(int iter)const{
		return t_rez[iter];
	}
	double getf(int t)const{
		return TM->f(t);
	}
	double getEnd_time()const{
		return end_time;
	}
	double get_h()const{
		return h;
	}
};

class Task_solution_view : public Task_base{
private:
	const Task_solve * TS;
	FILE * gnuFile;
	int solve(){ throw invalid_argument("This kind of class can not solve equation"); };
public:
	explicit Task_solution_view(const Task_solve & TS, const string FileName) :TS(&TS){
		gnuFile = fopen(FileName.c_str(), "w");
		if (gnuFile == nullptr){
			string message;
			message.append("Can not open the file").append(FileName);
			throw invalid_argument(message);
		}
	}

	~Task_solution_view(){
		fclose(gnuFile);
	}

	int print(){
		//fill gnuplot file with the next format:
		// | time of integration | x (state function values) | f (perturbed force values)| u (control function values) |
		for (int i = 0; i < (TS->getEnd_time()) / TS->get_h(); i++){
			fprintf(gnuFile, "%+.15lf %+.15lf %+.15lf %+.15lf\n", TS->getT(i), TS->getX(i), TS->getf(i), TS->getU(i));
		}
		return 0;
	}
};

//vector <double> operator*(const double C, const vector <double>& a){
//  vector < double > rez;
//  rez.reserve(a.size());
//  for (int i = 0; i < a.size(); ++i){
//    rez.push_back(C*a[i]);
//  }
//  return rez;
//}
//
//vector <double> operator+(const vector <double> &a, const vector<double > &b){
//  assert(a.size() == b.size());
//  vector <double> rez;
//  rez.reserve(a.size());
//  for (int i = 0; i < a.size(); ++i){
//    rez.push_back(a[i] + b[i]);
//  }
//  return rez;
//}

#endif
