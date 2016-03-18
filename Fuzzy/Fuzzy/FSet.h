#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <algorithm>
using namespace std;

class FSet {
public:
	FSet(string str) { // конструктор от строки
		workmap = new map<double, double>(); //создание рабочей map, в ней будут хранитс€ данные дл€ класса
		string sub;
		int semi = str.find(";"); //поиск первого знака ";"
		sub = str.substr(0, semi);
		str = str.substr(semi + 1, str.size() - semi);
		while (!sub.empty())
		{
			workmap->insert(pair<double, double>(stod(sub.substr(sub.find("/") + 1)), stod(sub.substr(0, sub.find("/"))))); //в map помещаетс€ пара (элемент, степень принадлежности)
			semi = str.find(";");
			if (semi == string::npos) break;
			sub = str.substr(0, semi);
			str = str.substr(semi + 1, str.size() - semi);
		}
		workmap->insert(pair<double, double>(stod(str.substr(str.find("/") + 1)), stod(str.substr(0, str.find("/")))));

	}

	//определение операторов арифмитических действий
	FSet operator + (FSet& second) {
		return operation(second, 0);
	}

	FSet operator - (FSet& second) {
		return operation(second, 1);
	}

	FSet operator / (FSet& second) {
		return operation(second, 2);
	}

	FSet operator * (FSet& second) {
		return operation(second, 3);
	}

friend ostream& operator<<(ostream& os, const FSet& fs); //объ€вление дружественного класса оператора вывода

private:
	FSet(map<double, double>* workmap) : workmap(workmap)  {}//конструктор, принимающий map, необходим дл€ работы операторов

	FSet operation(FSet& second, int operationtype) { 	//приватный метод, возвращающий результат, в зависимости от функции-параметра
		map <double, double>* opermap = new map <double, double>();
		for (auto& itemfirst : *workmap) {
			for (auto& itemsecond : *second.workmap) {
				double current; //текущий новый ключ
				double minvalue = min(itemfirst.second, itemsecond.second); //текуща€ минимальна€ степень принадлежности

				switch (operationtype) //выбор операции зависит от значени€ параметра
				{
				case 0:
					current = itemfirst.first + itemsecond.first;
					break;
				case 1:
					current = itemfirst.first - itemsecond.first;
					break;
				case 2:
					current = itemfirst.first / itemsecond.first;
					break;
				case 3:
					current = itemfirst.first * itemsecond.first;
					break;
				}

				if (opermap->count(current)) //проверка, есть ли уже в map текущий новый ключ
					opermap->at(current) = max(opermap->at(current), minvalue); //если есть, берЄм максимум из записанного значени€ и текущего минимума
				else
					opermap->insert(pair<double, double>(current, minvalue)); //если еЄ нет, добавл€ем новую пару в map
			}
		}
		FSet* result = new FSet(opermap); //создание экземпл€ра класса - результата
		return *result;
	}

	map <double, double>* workmap; //приватное поле рабочей map
};

ostream& operator <<(ostream& os, const FSet& fs) //перегрузка оператора вывода дл€ нашего класса
{
	ostringstream oss;
	for (auto& item : *fs.workmap) {
		oss << item.second << '/' << item.first << ";";
	}
	os << oss.str().substr(0, oss.str().size() - 1);
	return os;
}
