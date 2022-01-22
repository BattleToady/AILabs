
#include <iostream>
#include "windows.h"
using namespace std;

float random_float(float min, float max) {

	return ((float)rand() / RAND_MAX) * (max - min) + min;

}

float** Generation(int num, int min, int max)
{
	float** Progression = new float* [num];

	for (int i = 0; i < num; i++)
	{
		Progression[i] = new float[2];
		Progression[i][0] = random_float(min, max);
		Progression[i][1] = random_float(min, max);
	}
	return Progression;
}

void ShowProgression(float** Progresion, int num)
{
	for (int i = 0; i < num; i++)
		cout << Progresion[i][0] << " " << Progresion[i][1] << endl;
}

float Distance(float* first, float* second)
{
	return pow(float(pow((first[0] - second[0]), 2) + pow((first[1] - second[1]), 2)), (1. / 2.));
}

int main()
{
	int N = 1000;
	int K = 5;
	float** Prog = Generation(N, -1000, 1000);
	float** Centers = new float* [K];
	Centers = Generation(K, -1000, 1000);
	int* Klasters = new int[N];
	bool pr = true;
	int l = 0;
	while(pr)
	{ 
		pr = false;
		//Приписування точок до кластерів
		for (int i = 0; i < N; i++)
		{
			float minDistance = Distance(Prog[i],Centers[0]);
			float minCenter = 0;
			for (int j = 1; j < K; j++)
			{
				if (Distance(Prog[i], Centers[j]) < minDistance)
				{
					minCenter = j;
					minDistance = Distance(Prog[i], Centers[j]);
				}
			}
			Klasters[i] = minCenter;
		}
		//Розрахування нових кластерів
		for (int j = 0; j < K; j++)
		{
			double sum1 = 0;
			double sum2 = 0;
			float size = 0;
			for (int i = 0; i < N; i++)
			{
				if (Klasters[i] == j)
				{
					sum1 += Prog[i][0];
					sum2 += Prog[i][1];
					size++;
				}
			}
			float Test[2] = { (sum1 / size),(sum2 / size) };
			if (Distance(Centers[j],Test)>0.00001)//Перевірка закінчення циклу
				pr = true;
			Centers[j][0] = sum1 / size;
			Centers[j][1] = sum2 / size;
		}
		l++;
	}
	cout << "Centers:\n";
	ShowProgression(Centers, K);
	cout << "Num of iterations: " << l << endl;

	float pX, pY;
	pX = 500;
	pY = 400;
	float Scale = 0.1;
	
	HDC hDC = GetDC(GetConsoleWindow());
	HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
	//Create Ox and Oy
	SelectObject(hDC, Pen);
	MoveToEx(hDC, pX - 100, pY, NULL);
	LineTo(hDC, pX + 10, pY);
	MoveToEx(hDC, pX, 0, NULL);
	LineTo(hDC, pX, pY + 65);
	//graph
	COLORREF Color1 = RGB(0, 255, 0);
	COLORREF Color2 = RGB(255, 0, 0);
	COLORREF Color3 = RGB(0, 0, 255);
	COLORREF Color4 = RGB(255, 255, 0);
	COLORREF Color5 = RGB(0, 255, 255);
	for (int i = 0; i < N; i++)//paint togles of progression
	{
		COLORREF Color;
		if(Klasters[i]==0)
		{
			Color = Color1;
		}
		else
			if (Klasters[i] == 1)
			{
				Color = Color2;
			}
			else
				if (Klasters[i] == 2)
				{
					Color = Color3;
				}
				else
					if (Klasters[i] == 4)
					{
						Color = Color5;
					}
					else
						Color = RGB(255, 255, 255);
		HPEN GraphPen = CreatePen(PS_SOLID, 2, Color);
		SelectObject(hDC, GraphPen);
		MoveToEx(hDC, Scale * Prog[i][0] + pX, Scale * Prog[i][1] + pY, NULL);
		LineTo(hDC, Scale*Prog[i][0] + pX + 1, Scale * Prog[i][1] + pY + 1);
	}

}