#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <algorithm>
using namespace std;

class FSet {
public:
	FSet(string str) { // ����������� �� ������
		workmap = new map<double, double>(); //�������� ������� map, � ��� ����� �������� ������ ��� ������
		string sub;
		int semi = str.find(";"); //����� ������� ����� ";"
		sub = str.substr(0, semi);
		str = str.substr(semi + 1, str.size() - semi);
		while (!sub.empty())
		{
			workmap->insert(pair<double, double>(stod(sub.substr(sub.find("/") + 1)), stod(sub.substr(0, sub.find("/"))))); //� map ���������� ���� (�������, ������� ��������������)
			semi = str.find(";");
			if (semi == string::npos) break;
			sub = str.substr(0, semi);
			str = str.substr(semi + 1, str.size() - semi);
		}
		workmap->insert(pair<double, double>(stod(str.substr(str.find("/") + 1)), stod(str.substr(0, str.find("/")))));

	}

	//����������� ���������� �������������� ��������
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

friend ostream& operator<<(ostream& os, const FSet& fs); //���������� �������������� ������ ��������� ������

private:
	FSet(map<double, double>* workmap) : workmap(workmap)  {}//�����������, ����������� map, ��������� ��� ������ ����������

	FSet operation(FSet& second, int operationtype) { 	//��������� �����, ������������ ���������, � ����������� �� �������-���������
		map <double, double>* opermap = new map <double, double>();
		for (auto& itemfirst : *workmap) {
			for (auto& itemsecond : *second.workmap) {
				double current; //������� ����� ����
				double minvalue = min(itemfirst.second, itemsecond.second); //������� ����������� ������� ��������������

				switch (operationtype) //����� �������� ������� �� �������� ���������
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

				if (opermap->count(current)) //��������, ���� �� ��� � map ������� ����� ����
					opermap->at(current) = max(opermap->at(current), minvalue); //���� ����, ���� �������� �� ����������� �������� � �������� ��������
				else
					opermap->insert(pair<double, double>(current, minvalue)); //���� � ���, ��������� ����� ���� � map
			}
		}
		FSet* result = new FSet(opermap); //�������� ���������� ������ - ����������
		return *result;
	}

	map <double, double>* workmap; //��������� ���� ������� map
};

ostream& operator <<(ostream& os, const FSet& fs) //���������� ��������� ������ ��� ������ ������
{
	ostringstream oss;
	for (auto& item : *fs.workmap) {
		oss << item.second << '/' << item.first << ";";
	}
	os << oss.str().substr(0, oss.str().size() - 1);
	return os;
}
