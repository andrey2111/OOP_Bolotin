// MyBoychuk.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include "Classes.h"

int main()
{
	//������� ���������� ������
	double A = 7; //������� ���������
	double a0 = -3; //�������� ������ a0
	double l = 3; //�������� ������ ������
	double x0 = 2; //��������� �������� x
	double psi = 1; //������� ���������� ����������
	//������� ���� � ������� ��������������
	double h = 0.1;
	double end = 50;

	Model model(A, a0, l, x0); //�������� ������
	Task_list task_list(model); //�������� ������������ ������ �� ������ ������

	Solution solution(model, h, end, psi);//������� (������� ��������������)
	task_list.push_front(solution); //���������� ������� � ������
	
	Output output(solution, "x.dat", "psi.dat", "u.dat", "t.dat"); //����� ����������� �������� � �����
	task_list.push_front(output); //���������� ������ � ������

	return 0;
}

