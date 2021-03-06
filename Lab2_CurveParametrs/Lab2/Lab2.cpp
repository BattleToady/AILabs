#include "stdafx.h"
#include "iostream"
#include <Windows.h>
#include <stdlib.h>
#include <math.h>
using namespace std;
const float pi = 3.14;
float posX = 500;//pos painted graph
float posY = 185;
float Scale = 40;// scale painted graphs
//Scale = 200 - to see jumps in test progression
float t = 100;

float Arg(int x)
{
	//2*pi/t
	return (2*pi*x/t);//function help scale graph
}

float ArifMean(float* prog, int num, int start)
{
	double sum = 0;
	for (int i = start; i < start + num; i++)
	{ 
		sum += prog[i];
	}
	return (sum/num);
}

float GeoMean(float* prog, int num, int start)
{
	long double sum = prog[start];
	int Rnum = num;
	for (int i = start + 1; i < start + num; i++)
	{ 
		if(prog[i]!=0)
		{ 
			sum *= prog[i];
		}
		else
		{
			Rnum -= 1;
		}
	}
	if (sum < 0) sum *= -1;
	return (pow(sum, 1 / double(Rnum)));
}

float HarMean(float* prog, int num, int start)
{
	long double sum = 0;
	for (int i = start; i < start + num; i++)
	{ 
		sum += 1 / prog[i];
	}
	return (num / sum);
}

void PaintDates(float* dates, float Aplt, int Num, COLORREF Color, float pX, float pY)
{
	HDC hDC = GetDC(GetConsoleWindow());
	HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
	//Create Ox and Oy
	SelectObject(hDC, Pen);
	MoveToEx(hDC, pX - 100, pY, NULL);
	LineTo(hDC, pX + 10 * Scale, pY);
	MoveToEx(hDC, pX, 0, NULL);
	LineTo(hDC, pX, pY + 65);
	//graph
	HPEN GraphPen = CreatePen(PS_SOLID, 2, Color);
	SelectObject(hDC, GraphPen);
	for (int i = 0; i < Num-1; i++)//paint togles of progression
	{
		MoveToEx(hDC, Scale * Arg(i) + pX, -10 * dates[i] + pY, NULL);
		LineTo(hDC, Scale * Arg(i) + pX, -10 * dates[i+1] + pY);
	}
}

float SimpleMean(float* prog, int num, int start)
{
	return ArifMean(prog, num, start);
}

float WeightedMean(float* prog, int num, float* weights, int start)
{
	double sum = 0;
	for (int i = start; i < start + num; i++)
	{ 
		sum += prog[i] * weights[i - start];
	}
	double wSum = 0;
	for (int i = start; i < start + num; i++)
		wSum += weights[i - start];
	return (sum / wSum);
}

float ExpMean(float* prog, int num, int start)
{
	float EMA = 0;
	float prevEMA = 0;
	for (int i = start; i < start + num; i++)
	{
		EMA = (prevEMA * (1 - 0.2) + 0.2 * prog[i]);
		prevEMA = EMA;
	}
	return EMA;
}

float Check(float* prog, float* sProg, int elm)
{
	return((prog[elm] - sProg[elm]));
}

float* ExpSmothing(float* prog, int num, float koef)
{
	float* SmoothedProg = new float[num];
	SmoothedProg[0] = prog[0];
	for (int i = 1; i < num; i++)
		SmoothedProg[i] = SmoothedProg[i - 1] + koef * (prog[i] - SmoothedProg[i - 1]);
	return SmoothedProg;
}

float DeltaRel(float* fProg, float* sProg, int num)
{
	long double sum = 0;
	for (int i = 0; i < num; i++)
	{
		sum += pow(pow((fProg[i] - sProg[i]), 2), (1 / 2)) / abs(fProg[i]);
	}
	return(sum / num);
}

float DeltaAbs(float* fProg, float* sProg, int num)
{
	long double sum = 0;
	for (int i = 0; i < num; i++)
	{
		sum += pow((fProg[i] - sProg[i]), 2);
	}
	return(sum / num);
}

void Wait()
{
	cout << "To go forward press Enter two times\n";
	system("pause>null");
	for (int i = 0; i < 30; i++)
		cout << endl;
	system("pause>null");
}

int main(void)
{
	//input dates
	int Num = 1000;
	float u = pi / 3;
	float A = 3;
	//Generate sin - trueprogression and rand-sin - progressin
	float* progression = new float[Num];
	float* trueProgression = new float[Num];
	for (int i = 0; i < Num; i++)
	{ 
		progression[i] = A * sin(Arg(i) + u) + (A * 0.01 *((rand() % 25)-12.5)) +4;
		trueProgression[i] = A * sin(Arg(i) + u) +4;
	}
	//Find Means
	int MeansLenght = 40;
		//arif
	float* ArifMeanProg = new float[Num - MeansLenght];
	float* ArifMeanTrueProg = new float[Num - MeansLenght];
	for (int i = MeansLenght / 2; i < Num - MeansLenght/2; i++)
	{ 
		ArifMeanProg[i - MeansLenght / 2] = ArifMean(progression, MeansLenght, i - MeansLenght / 2);
		ArifMeanTrueProg[i - MeansLenght / 2] = ArifMean(trueProgression, MeansLenght, i - MeansLenght / 2);
	}
		//geometric
	float* GeoMeanProg = new float[Num - MeansLenght];
	float* GeoMeanTrueProg = new float[Num - MeansLenght];
	for (int i = MeansLenght / 2; i < Num -MeansLenght/2; i++)
	{ 
		GeoMeanProg[i - MeansLenght / 2] = GeoMean(progression, MeansLenght, i - MeansLenght / 2);
		GeoMeanTrueProg[i - MeansLenght / 2] = GeoMean(trueProgression, MeansLenght, i - MeansLenght / 2);
	}
		//harmony
	float* HarMeanProg = new float[Num - MeansLenght];
	float* HarMeanTrueProg = new float[Num - MeansLenght];
	for (int i = MeansLenght / 2; i < Num -MeansLenght / 2; i++)
	{ 
		HarMeanProg[i - MeansLenght / 2] = HarMean(progression, MeansLenght, i - MeansLenght / 2);
		HarMeanTrueProg[i - MeansLenght / 2] = HarMean(trueProgression, MeansLenght, i - MeansLenght / 2);
	}
		//simple
	float* SimpleMeanProg = new float[Num - MeansLenght];
	float* SimpleMeanTrueProg = new float[Num - MeansLenght];
	for (int i = MeansLenght / 2; i < Num - MeansLenght / 2; i++)
	{ 
		SimpleMeanProg[i - MeansLenght / 2] = SimpleMean(progression, MeansLenght, i - MeansLenght / 2);
		SimpleMeanTrueProg[i - MeansLenght / 2] = SimpleMean(trueProgression, MeansLenght, i - MeansLenght / 2);
	}
		//weighted
	float* WeightedMeanProg = new float[Num - MeansLenght];
	float* WeightedMeanTrueProg = new float[Num - MeansLenght];
	float* weights = new float[MeansLenght];
	for (int i = 0; i < MeansLenght; i++)
	{ 
		weights[i] = rand() % 3 + 1;
	}
	for (int i = MeansLenght / 2; i < Num - MeansLenght / 2; i++)
	{ 
		WeightedMeanProg[i - MeansLenght / 2] = WeightedMean(progression, MeansLenght, weights, i - MeansLenght / 2);
		WeightedMeanTrueProg[i - MeansLenght / 2] = WeightedMean(trueProgression, MeansLenght, weights, i - MeansLenght / 2);
		
	}
		//exponent
	float* ExpMeanProg = new float[Num - MeansLenght];
	float* ExpMeanTrueProg = new float[Num - MeansLenght];
	for (int i = MeansLenght / 2; i < Num - MeansLenght / 2; i++)
	{
		ExpMeanProg[i - MeansLenght / 2] = ExpMean(progression, MeansLenght, i - MeansLenght / 2);
		ExpMeanTrueProg[i - MeansLenght / 2] = ExpMean(trueProgression, MeansLenght, i - MeansLenght / 2);
	}
	// Ouput Progressions
	cout << "Our graphs\n Red - Test Progression\n Blue - Real Progression\n";
	PaintDates(progression, A, Num, RGB(255, 0, 0), posX, posY);
	PaintDates(trueProgression, A, Num, RGB(0, 0, 255), posX - u, posY);
	Wait();
	//Output Means
	cout << "Means: Red - ArifMean; Blue - GeoMean; Green - HarmMean\n";
	PaintDates(ArifMeanProg, A, Num - MeansLenght, RGB(255, 0, 0), posX, posY);
	PaintDates(GeoMeanProg, A, Num - MeansLenght, RGB(0, 0, 255), posX, posY);
	PaintDates(HarMeanProg, A, Num - MeansLenght, RGB(0, 255, 0), posX, posY);
	Wait();
	cout << "More Means: Red - SimpleMean, Blue - WeightedMean, Green - ExponentMean\n";
	PaintDates(SimpleMeanProg, A, Num - MeansLenght, RGB(255, 0, 0), posX, posY);
	PaintDates(WeightedMeanProg, A, Num - MeansLenght, RGB(0, 0, 255), posX, posY);
	PaintDates(ExpMeanProg, A, Num - MeansLenght, RGB(0, 255, 0), posX, posY);
	Wait();
	cout << "Delta Abs:\n";
	cout << "Arif:" << DeltaAbs(ArifMeanProg, ArifMeanTrueProg, Num - MeansLenght) << endl;
	cout << "Geo:" << DeltaAbs(GeoMeanProg, GeoMeanTrueProg, Num - MeansLenght) << endl;
	cout << "Har:" << DeltaAbs(HarMeanProg, HarMeanTrueProg, Num - MeansLenght) << endl;
	cout << "Simple:" << DeltaAbs(SimpleMeanProg, SimpleMeanTrueProg, Num - MeansLenght) << endl;
	cout << "Weighted:" << DeltaAbs(WeightedMeanProg, WeightedMeanTrueProg, Num - MeansLenght) << endl;
	cout << "Exp:" << DeltaAbs(ExpMeanProg, ExpMeanTrueProg, Num - MeansLenght) << endl;
	cout << "Delta Rel:\n";
	cout << "Arif:" << DeltaRel(ArifMeanTrueProg, ArifMeanProg, Num - MeansLenght) << endl;
	cout << "Geo:" << DeltaRel(GeoMeanTrueProg, GeoMeanProg, Num - MeansLenght) << endl;
	cout << "Har:" << DeltaRel(HarMeanTrueProg, HarMeanProg, Num - MeansLenght) << endl;
	cout << "Simple:" << DeltaRel(SimpleMeanTrueProg, SimpleMeanProg, Num - MeansLenght) << endl;
	cout << "Weighted:" << DeltaRel(WeightedMeanTrueProg, WeightedMeanProg, Num - MeansLenght) << endl;
	cout << "Exp:" << DeltaRel(ExpMeanTrueProg, ExpMeanProg, Num - MeansLenght) << endl;
	Wait();
	//Smoothing
	cout << "Smoothing Progression:\n";
	float* ExpSmoothedProg = ExpSmothing(progression, Num, 0.2);
	//0.1 - зміщення вправо,
	//0.25 - великий розкид
	//0.2 - майже без зміщень і +- мінімальний розкид
	//0.002 - не подібна до sin(x)
	PaintDates(trueProgression, A, Num, RGB(255, 0, 0), posX, posY + 100 * A);
	PaintDates(ExpSmoothedProg, A, Num, RGB(0, 0, 255), posX, posY + 100*A);
	float min = 0, max = 0;
	float curent;
	//Max and min of smoothing
	for (int i = 0; i < Num; i++)
	{
		curent = Check(trueProgression, ExpSmoothedProg, i);
		if (curent < min)min = curent;
		if (curent > max)max = curent;
	}
	cout << "mistakes:    min:" << min << " max:" << max << endl;
	delete progression;
	delete trueProgression;
	delete ArifMeanProg;
	delete ArifMeanTrueProg;
	delete GeoMeanProg;
	delete GeoMeanTrueProg;
	delete HarMeanProg;
	delete HarMeanTrueProg;
	delete SimpleMeanProg;
	delete SimpleMeanTrueProg;
	delete WeightedMeanProg;
	delete WeightedMeanTrueProg;
	delete weights;
	delete ExpMeanProg;
	delete ExpMeanTrueProg;
	delete ExpSmoothedProg;
	system("pause");
	return 0;
}