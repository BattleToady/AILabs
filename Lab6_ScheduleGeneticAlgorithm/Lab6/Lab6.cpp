// Lab6.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"
#include "string.h"
#include "DNA.h"
#include <iomanip>
using namespace std;

float CalculateSolution(DNA* Sol, int lenght)
{
	float problems = 0;
	int windows = 0;
	//different classes have different classleaders -- very easy
	if (Sol[0].ClassLeader == Sol[1].ClassLeader) problems++;
	if (Sol[1].ClassLeader == Sol[2].ClassLeader) problems++;
	if (Sol[0].ClassLeader == Sol[2].ClassLeader) problems++;
	//check windows -- very easy
	for(int i=0;i<lenght;i++)
		for(int j=0;j<5;j++)
		{
			for (int k = 0; k < 4; k++)
			{
				if (Sol[i].lessons[j * 5 + k] == 0 && Sol[i].lessons[j * 5 + k] != 0)
					windows++;
			}
		}
	problems += float(windows) / float(lenght);
	//rooms have one lesson in one time -- not hard
	int roomUses = 0;
	for (int i = 0; i<lenght; i++)
		for(int k=0;k<lenght;k++)
			for (int j = 0; j<25; j++)
			{
				if (Sol[i].rooms[j] == Sol[k].rooms[j] && i != k)
					roomUses++;
			}
	problems += float(roomUses) / float(lenght);
	//one teacher have one lesson in one time -- not hard
	int teachUses = 0;
	for (int i = 0; i<lenght; i++)
		for (int k = 0; k<lenght; k++)
			for (int j = 0; j<25; j++)
			{
				if (Sol[i].teachers[j] == Sol[k].teachers[j] && i != k)
					teachUses++;
			}
	problems += float(teachUses) / float(lenght);
	
	//all 8 lessons habe - not hard
	int DifferentLessons;
	int* Lessons = new int[8];
	int tim;
	for (int i = 0; i < lenght; i++)
	{
		DifferentLessons = 0;
		tim = 0;
		for (int j = 0; j < 8; j++)
			Lessons[j] = -1;
		for (int j = 0; j < 25; j++)
		{
			bool check = true;
			for (int l = 0; l < 8; l++)
				if (Sol[i].lessons[j] == Lessons[l])
				{
					check = false;
					break;
				}
			if (check)
			{
				Lessons[tim++] = Sol[i].lessons[j];
			}

		}
		for (int j = 0; j < 8; j++)
			if (Lessons[j] == -1)problems++;
	}
	delete[] Lessons;
	return problems;
}

/*void OutputTimetable(DNA* Sol, int lenght)
{
	for(int i=0;i<5;i++)
		for(int j=0;j<5;i++)
		{ 
			for(int k=0;k<lenght;k++)
			{ 
				cout << Lessons[Sol[k].lessons[i * 5 + j]] << "___";
			}
			cout << endl;
		}
}*/


char** Lessons = new char*[8];
int TeachersNum = 8;
int* Teachers = new int[TeachersNum];
int Groups = 3;
int* Rooms = new int[Groups + 2];// type of rooms, 0 - simple classroom

bool Brand()
{
	return(rand()%10);
}

void RandSolution(DNA* Solution)
{
	for (int i = 0; i < 3; i++)
		Solution[i].ClassLeader = rand() % 3;
	for (int i = 0; i<3; i++)
		for (int j = 0; j < 25; j++)
		{
			Solution[i].teachers[j] = rand() % TeachersNum;
			
			Solution[i].lessons[j] = Teachers[Solution[i].teachers[j]];

			if (Lessons[Solution[i].lessons[j]] == "Physical training") Solution[i].rooms[j] = 1;
			else
				if (Lessons[Solution[i].lessons[j]] == "Music") Solution[i].rooms[j] = 2;
				else
					Solution[i].rooms[j] = 0;
		}
}

int* FindBest(float* Ev, int lenght, int numOfBest)
{
	int* Best = new int[numOfBest];
	for (int i = 0; i < numOfBest; i++)
		Best[i] = -1;
	int curent;
	for(int j=0;j<numOfBest;j++)
	{ 
		curent = 0;
		for (int i = 0; i < lenght; i++)
		{
			bool repeat = false;
			for (int k = 0; k < numOfBest; k++)
				if (Ev[Best[k]] == Ev[i])
				{
					repeat = true;
				}
			//cout << Ev[i] << " " << Ev[curent] << " " << Ev[Best[0]] << endl;
			if (Ev[i] < Ev[curent] && !repeat)curent = i;
		}
		Best[j] = curent;
	}
	return Best;
}

DNA** Mutate(DNA* Father, int numOfChilds)
{
	DNA** Child = new DNA*[numOfChilds];
	for (int i = 0; i < numOfChilds; i++)
		Child[i] = new DNA[3];
	for (int i = 0; i < numOfChilds; i++)
	{
			for(int k=0;k<3;k++)
			{ 
				if (Brand())
				{ 
					Child[i][k].ClassLeader = Father[k].ClassLeader;
				}
			else
				Child[i][k].ClassLeader = rand() % 3;
				for (int j = 0; j < 25; j++)
				{
					if (Brand())
						Child[i][k].teachers[j] = Father[k].teachers[j];
					else
						Child[i][k].teachers[j] = rand() % 8;
					Child[i][k].lessons[j] = Teachers[Child[i][k].teachers[j]];
					if (Lessons[Child[i][k].lessons[j]] == "Physical training") Child[i][k].rooms[j] = 1;
					else
						if (Lessons[Child[i][k].lessons[j]] == "Music") Child[i][k].rooms[j] = 2;
						else
							Child[i][k].rooms[j] = 0;
				}
			}
	}
	return Child;
}

DNA** Crossover(DNA* Father, DNA* Mother, int numOfChilds)
{
	DNA** Child = new DNA*[numOfChilds];
	for (int i = 0; i < numOfChilds; i++)
		Child[i] = new DNA[3];
	for (int i = 0; i < numOfChilds; i++)
	{
		for (int k = 0; k<3; k++)
		{
			if (Brand())
			{
				Child[i][k].ClassLeader = Father[k].ClassLeader;
			}
			else
				Child[i][k].ClassLeader = Mother[k].ClassLeader;
			for (int j = 0; j < 25; j++)
			{
				if (Brand())
					Child[i][k].teachers[j] = Father[k].teachers[j];
				else
					Child[i][k].teachers[j] = Mother[k].ClassLeader;
				Child[i][k].lessons[j] = Teachers[Child[i][k].teachers[j]];
				if (Lessons[Child[i][k].lessons[j]] == "Physical training") Child[i][k].rooms[j] = 1;
				else
					if (Lessons[Child[i][k].lessons[j]] == "Music") Child[i][k].rooms[j] = 2;
					else
						Child[i][k].rooms[j] = 0;
			}
		}
	}
	return Child;
}

int main()
{
	Lessons[0] = (char*)"Math";
	Lessons[1] = (char*)"Ukrainian";
	Lessons[2] = (char*)"English";
	Lessons[3] = (char*)"Physics";
	Lessons[4] = (char*)"Physical training";
	Lessons[5] = (char*)"Music";
	Lessons[6] = (char*)"Literature";
	Lessons[7] = (char*)"History";
	for (int i = 0; i < 8; i++)
		Teachers[i] = i;
	for (int i = 0; i < Groups + 2; i++)
		Rooms[i] = 0;
	Rooms[0] = 1;//training room
	Rooms[1] = 2;//music room
	int LenghtOfPopulation = 250;	//input dates
	int NumOfBest = 50;
	int* Best;
	int maxSteps = 7000;
	DNA** Solution = new DNA*[LenghtOfPopulation];
	float* Evolution = new float[LenghtOfPopulation];
	//Generate Solution
	for (int i = 0; i < LenghtOfPopulation; i++)
	{
		Solution[i] = new DNA[3];
		RandSolution(Solution[i]);
		Evolution[i] = CalculateSolution(Solution[i], 3);
	}
	//General Code
	int i = 0;
	do
	{ 
		//Check
		for (int i = 0; i < LenghtOfPopulation; i++)
			Evolution[i] = CalculateSolution(Solution[i], 3);
		Best = FindBest(Evolution, LenghtOfPopulation, NumOfBest);
		if (Evolution[Best[0]] == 0)break;
		//Selection
		DNA** Copy = new DNA*[NumOfBest];
		for (int i = 0; i < NumOfBest; i++)
			Copy[i] = Solution[Best[i]];
		for (int i = 0; i < LenghtOfPopulation; i++)
		{
			bool repeat = false;
			for (int j = 0; j < NumOfBest; j++)
				if (Solution[i] == Copy[j])repeat = true;
		}
		for (int i = 0; i < NumOfBest; i++)
			Solution[i] = Copy[i];
		//Mutation
		DNA** MutationChilds;
		for (int i = 0; i < NumOfBest; i++)
		{
			MutationChilds = Mutate(Solution[i], 2);
			for (int j = 0; j < 2; j++)
			{ 
				Solution[i * 2 + j + 20] = MutationChilds[j];
			}
		}
		//Crossover
		DNA** CrossoverChilds;
		for (int i = 0; i < NumOfBest; i++)
		{
			CrossoverChilds = Crossover(Solution[2*i], Solution[2*i+1], 2);
			for (int j = 0; j < 2; j++)
			{
				Solution[i * 2 + j + 60] = CrossoverChilds[j];
			}
		}
		i++;

		//Output
		cout << "Population Number: " << setw(4) << i << " Adaptation: " << Evolution[Best[0]] << endl;
	} while (i<maxSteps);//loop

	//Output Solution
	cout << "|-----ClassLeaders-----|" << endl;;
	for (int i = 0; i < 3; i++)
		cout << i << "-class have " << Solution[Best[0]][i].ClassLeader << " teacher" << endl;
	cout << "|-----Lessons-----|" << endl;
	for (int i = 0; i<5; i++)
	{
		for (int j = 0; j<5; j++)
		{
			for (int k = 0; k<3; k++)
			{
				cout << setw(18) << left << Lessons[Solution[Best[0]][k].lessons[i * 5 + j]];
			}
			cout << endl;
		}
	}
	cout << "|-----Teachers-----|" << endl;
	for (int i = 0; i<5; i++)
	{
		for (int j = 0; j<5; j++)
		{
			for (int k = 0; k<3; k++)
			{
				cout << setw(18) << left << Teachers[Solution[Best[0]][k].teachers[i * 5 + j]];
			}
			cout << endl;
		}
	}
	cout << "|-----Rooms-----|" << endl;
	for (int i = 0; i<5; i++)
	{
		for (int j = 0; j<5; j++)
		{
			for (int k = 0; k<3; k++)
			{
				cout << setw(18) << left << Rooms[Solution[Best[0]][k].rooms[i * 5 + j]];
			}
			cout << endl;
		}
	}
	delete[] Solution;
	delete[] Lessons;
	delete[] Teachers;
	delete[] Rooms;
    return 0;
}

