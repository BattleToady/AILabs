// Lab4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"
#include "cmath"
#include "windows.h"

using namespace std;

float function(float arg)
{
	//return(exp(pow((arg),2)));//e^x^2
	//return(pow(arg, 2));//x^2
	return(sin(arg));//sin(x)
	//return(pow(2,arg));//2^x
}
float pX = 500.;
float pY = 400.;
float Scale = 80;
float FindMax(float minx, float maxx)
{
	float max, curent;
	curent = max = function(minx);
	for (float i = minx; i <= maxx; i += 0.001)
	{
		curent = function(i);
		if (curent > max) { max = curent; };
	}
	return max;
}

float FindMin(float minx, float maxx)
{
	float min, curent;
	curent = min = function(minx);
	for (float i = minx; i <= maxx; i += 0.001)
	{
		curent = function(i);
		if (curent < min) { min = curent; };
	}
	return min;
}

float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

void PaintDates(float a, float b, int steps, COLORREF Color)
{
	HDC hDC = GetDC(GetConsoleWindow());
	HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
	//Create Ox and Oy
	SelectObject(hDC, Pen);
	MoveToEx(hDC, pX - 100, pY, NULL);
	LineTo(hDC, pX + 10, pY);
	MoveToEx(hDC, pX, 0, NULL);
	LineTo(hDC, pX, pY + 65);
	//graph
	HPEN GraphPen = CreatePen(PS_SOLID, 2, Color);
	SelectObject(hDC, GraphPen);
	for (float i = a; i < b; i+=0.01)
	{
		MoveToEx(hDC, pX + i * Scale, pY, NULL);
		LineTo(hDC, pX + i * Scale, pY - function(i) * Scale);
	}
}

float Integral(float minx, float maxx)
{
	double sum = 0;
	float lit = 0.00001;
	for (float i = minx; i < maxx; i+=lit)
	{ 
		sum += function(i);
	}
	return (sum*lit);
}

float Monte_Karlo(float a, float b, int steps, COLORREF Color)
{
	HDC hDC = GetDC(GetConsoleWindow());
	HPEN Pen = CreatePen(PS_SOLID, 2, Color);
	SelectObject(hDC, Pen);
	float Y, YN;
	Y = 0; YN = 0;
	int l = 0;
	for (int i = 0; i < steps; i++)
	{
		float min, max;
		max = FindMax(a, b);
		min = FindMin(a, b);
		float x = RandomFloat(a, b);
		if (function(x) >= 0)
			min = 0;
		if (function(x) < 0)
			max = 0;
	
		float y = RandomFloat(min, max);
	
		if (y <= function(x) and y>=0 and function(x)>=0)
		{ 
			Y++;
		}
		if (y >= function(x) and y <= 0 and function(x) < 0)
			YN++;
		else
			if (y > function(x))l++;
		MoveToEx(hDC, pX+x*Scale, pY-y*Scale, NULL);
		LineTo(hDC, pX+(x + 0.01)* Scale, pY-(y + 0.01)*Scale);
	}
	return (((b - a) * (FindMax(a, b) - FindMin(a, b)) * (Y / steps)) - ((b - a) * (FindMax(a, b) - FindMin(a, b)) * (YN / steps)));
}

int main()
{
	float a, b;
	a = 0;
	b = 3.14;

	cout << "Min of Function:" << FindMin(a, b) << endl;
	cout << "Max of Function:" << FindMax(a, b) << endl;
	int Steps = 1000;
	PaintDates(a, b, Steps, RGB(255, 255, 255));
	cout << "Integral by Monte-Karlo:" << Monte_Karlo(a, b, Steps, RGB(255, 0, 0)) << endl;
	cout << "Integral:" << Integral(a, b);
	cout << endl <<  function(3.14 / 2) << endl;
    return 0;
}

