#include "stdafx.h"
#include "DNA.h"

DNA::DNA()
{
	lessons = new int[25];
	teachers = new int[25];
	rooms = new int[25];
	for (int i = 0; i < 25; i++)
	{
		lessons[i] = 0;
		teachers[i] = 0;
		rooms[i] = 0;
	}
}

DNA::~DNA()
{
	delete[] lessons;
	delete[] teachers;
	delete[] rooms;
}