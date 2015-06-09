#include "stdafx.h"
#include "robotbase.h"
#include <fstream>
#include "math.h"
using namespace std;
extern "C"

void DoStep(stepinfo *Info, step *Step)
{
	int myNumber = Info->yourNumber;
	int myX = Info->robots[myNumber]->x;
	int myY = Info->robots[myNumber]->y;
	//fstream file;
	//file.open("file2.txt", ios_base::app | ios_base::out );
	//file << Info->robots[myNumber]->E << " _ " << Info->robots[myNumber]->P << " _ " << Info->robots[myNumber]->name << " _ " << endl;
	int StepMax = Info->field->Vmax * Info->robots[myNumber]->V * Info->robots[myNumber]->E / Info->field->Lmax / Info->field->Emax;
	int AptNum;
	int AptRast = 100000;
	int TechNum;
	int TechRast = 100000;

	if (Info->objects[Info->field->Ne - 2]->type == OBJ_CHARGER)
	{
		for (int i = 0; i < Info->field->Ne; i++)
		{
			if (pow(pow(abs(myX - Info->objects[i]->x), 2) + pow(abs(myY - Info->objects[i]->y), 2), 0.5) < AptRast)
			{
				AptRast = pow(pow(abs(myX - Info->objects[i]->x), 2) + pow(abs(myY - Info->objects[i]->y), 2), 0.5);
				AptNum = i;
			}
			//file << AptRast << "__" << AptNum << "_" << i << "_" << Info->objects[i]->x << "_" << Info->objects[i]->y << endl;
		}

		for (int i = Info->field->Ne; i < Info->field->Ne + Info->field->Nl; i++)
		{
			if (pow(pow(abs(myX - Info->objects[i]->x), 2) + pow(abs(myY - Info->objects[i]->y), 2), 0.5) < TechRast)
			{
				TechRast = pow(pow(abs(myX - Info->objects[i]->x), 2) + pow(abs(myY - Info->objects[i]->y), 2), 0.5);
				TechNum = i;
			}
			//file << TechRast << "__" << TechNum << "_" << i << "_" << Info->objects[i]->x << "_" << Info->objects[i]->y << endl;
		}
	}
	else
	{
		for (int i = 0; i < Info->field->Nl; i++)
		{
			if (pow(pow(abs(myX - Info->objects[i]->x), 2) + pow(abs(myY - Info->objects[i]->y), 2), 0.5) < AptRast)
			{
				TechRast = pow(pow(abs(myX - Info->objects[i]->x), 2) + pow(abs(myY - Info->objects[i]->y), 2), 0.5);
				TechNum = i;
			}
			//file << AptRast << "__" << AptNum << "_" << i << "_" << Info->objects[i]->x << "_" << Info->objects[i]->y << endl;
		}

		for (int i = Info->field->Nl; i < Info->field->Ne + Info->field->Nl; i++)
		{
			if (pow(pow(abs(myX - Info->objects[i]->x), 2) + pow(abs(myY - Info->objects[i]->y), 2), 0.5) < TechRast)
			{
				AptRast = pow(pow(abs(myX - Info->objects[i]->x), 2) + pow(abs(myY - Info->objects[i]->y), 2), 0.5);
				AptNum = i;
			}
			//file << TechRast << "__" << TechNum << "_" << i << "_" << Info->objects[i]->x << "_" << Info->objects[i]->y << endl;
		}
	}


	if (Info->robots[myNumber]->L < 0.6*Info->field->Lmax && Info->robots[myNumber]->E > 0.5*Info->field->Emax && Info->stepnum < 0.9*Info->field->N)
	{
		if (TechRast > StepMax)
		{
			DoAction(Step, ACT_TECH, 0, 0.5*Info->robots[myNumber]->L, 0.5*Info->robots[myNumber]->L);
			int shagx, shagy;
			int shag = pow(StepMax / 2, 0.5);
			(myX - Info->objects[TechNum]->x < 0) ? shagx = shag : shagx = -shag;
			(myY - Info->objects[TechNum]->y < 0) ? shagy = shag : shagy = -shag;
			DoAction(Step, ACT_MOVE, shagx, shagy);
		}
		else
		{
			DoAction(Step, ACT_MOVE, Info->objects[TechNum]->x - myX, Info->objects[TechNum]->y - myY);
		}
	}
	else
	if (myX - Info->objects[AptNum]->x == 0 && myY - Info->objects[AptNum]->y == 0)
	{
		DoAction(Step, ACT_TECH, 0, Info->robots[myNumber]->L, 0);
	}
	else
	if (AptRast > StepMax)
	{
		DoAction(Step, ACT_TECH, 0, 0.5*Info->robots[myNumber]->L, 0.5*Info->robots[myNumber]->L);
		int shagx, shagy;
		int shag = pow(StepMax / 2, 0.5);
		(myX - Info->objects[AptNum]->x < 0) ? shagx = shag : shagx = -shag;
		(myY - Info->objects[AptNum]->y < 0) ? shagy = shag : shagy = -shag;
		DoAction(Step, ACT_MOVE, shagx, shagy);
	}
	else
	{
		DoAction(Step, ACT_MOVE, Info->objects[AptNum]->x - myX, Info->objects[AptNum]->y - myY);
	}

	return;
}