// Lab1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"
#include <random>
#include "cmath"
#include "cstdlib"
#include "ctime"
using namespace std;

void OutputDesk(int* Desk, int Num)
{
	for (int i = 0; i < Num + 1; i++)
		cout << i;
	cout << endl;
	for (int i = 0; i < Num; i++)
	{
		cout << char('a' + i);
		for (int j = 0; j < Num; j++)
		{
			bool pr = false;
			for (int k = 0; k < Num; k++)
				if (k == i && Desk[k] == j)pr = true;
			if (pr) cout << "Q";
			else
				cout << ".";
		}
		cout << endl;
	}
}

int CalculateSolution(int* Desk, int Num)
{
	int sol = 0;
	int fer;
	for (int i = 0; i < Num; i++)
	{
		fer = 0;
		for (int j = 0; j < Num-i; j++)
		{
			if (Desk[j] == j + i)fer++;
		}
		if (fer > 1) { sol += fer - 1; };

		fer = 0;
		if(i!=0)
		for (int j = 0; j < Num - i; j++)
		{
			if (Desk[j+i] == j)fer++;
		}
		if (fer > 1) { sol += fer - 1; };

		fer = 0;
		for (int j = 0; j < Num - i; j++)
		{
			if (Desk[Num - j - 1 - i] == j)fer++;
		}
		if (fer > 1) { sol += fer - 1; };

		fer = 0;
		if(i!=0)
		for (int j = 0; j < Num - i; j++)
		{
			if (Desk[Num - (Num - j - 1 - i) -1] == Num - j - 1)fer++;
		}
		if (fer > 1) { sol += fer - 1; };
	}
	return sol;
}

float Mrand()
{
	float r;
	for (int i = 0; i<20; i++)
	{
		r = rand();
	}
	return r;
}

void InizSolution(int* Desk, int Num)
{
	for (int i = 0; i < Num; i++)
		Desk[i] = i;
}

void GenerateSolution(int* Desk, int Num)
{
	int temp, x, y;
	do
	{
		
		x = int(Mrand()) % Num;
		y = int(Mrand()) % Num;
	} while (x==y);
	temp = Desk[x];
	Desk[x] = Desk[y];
	Desk[y] = temp;
}

void CopySolution(int* First, int* Second, int Num)
{
	for (int i = 0; i < Num; i++)
		First[i] = Second[i];
}


int main()
{
	int Num;
	cout << "Input Num of Queens:\n";
	do
	{
		cin >> Num;
		if (Num <= 1)cout << "Uncorect input. Enter again\n";
	} while (Num<=1);
	int step, solution = 0, useNew, steps = 100;
	float temperature = 30, Alpfa = 0.98;
	int* Desk = new int[Num];
	int* Solution = new int[Num];
	int* Best = new int[Num];
	int newSolution = 0, genSolution = 0;
	InizSolution(Desk, Num);
	OutputDesk(Desk, Num);
	CopySolution(Solution, Desk, Num);
	CopySolution(Best, Desk, Num);
	do
	{
		for (step = 0; step < steps; step++)
		{
			useNew = 0;
			CopySolution(Solution, Desk, Num);
			GenerateSolution(Solution, Num);
			newSolution = CalculateSolution(Solution, Num);
			genSolution = CalculateSolution(Desk, Num);
			if (newSolution <= genSolution)
			{
				useNew = 1;
			}
			else
			{
				useNew = 0;
				float delta;
				delta = newSolution - genSolution;
				float r = Mrand();
				float p = exp(-delta / temperature);
				if (r < p) { useNew = 1; }
			}
			if (useNew==1)
			{
				useNew = 0;
				CopySolution(Desk, Solution, Num);
				if (newSolution<CalculateSolution(Best,Num))
				{
					CopySolution(Best, Solution, Num);
				}
			}
			else
			{
				CopySolution(Solution, Desk, Num);
			}
		}
		temperature *= Alpfa;
	} while (temperature>0.5);
	OutputDesk(Solution, Num);
	cout << "Solution:" << CalculateSolution(Solution, Num) << endl;
	return 0;
}