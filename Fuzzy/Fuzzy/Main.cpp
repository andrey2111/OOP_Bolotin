// Fuzzy.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "FSet.h"
int _tmain(int argc, _TCHAR* argv[])
{
	string A1 = "0.7/2;1/3;0.6/4";
	string A2 = "0.8/3;1/4;0.5/6";
	FSet F1(A1);
	FSet F2(A2);
	cout << "F1 = {" << F1 << "}" << endl;
	cout << "F2 = {" << F2 << "}" << endl;
	cout << "F1+F2 = {" << F1 + F2 << "}" << endl;
	cout << "F1-F2 = {" << F1 - F2 << "}" << endl;
	cout << "F1*F2 = {" << F1 * F2 << "}" << endl;
	cout << "F1/F2 = {" << F1 / F2 << "}" << endl;
	system("pause");

	return 0;
}


