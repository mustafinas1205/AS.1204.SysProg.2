#include "stdafx.h"
#include "robotbase.h"
#include <fstream>
#include "math.h"
using namespace std;
extern "C"

void DoStep(stepinfo *Info, step *Step)
{
	/*fstream file;
	CString a;
	if (Info->robots[Info->yourNumber]->name == "robotbase.17.1")
	{
		a = "file1.txt";
	}
	else a = "file2.txt";
	file.open(a, ios_base::app | ios_base::out);*/
	int myNumber = Info->yourNumber;
	int myX = Info->robots[myNumber]->x;
	int myY = Info->robots[myNumber]->y;
	double StepMax = Info->field->Vmax * (double(Info->robots[myNumber]->V) / Info->field->Lmax) * (double(Info->robots[myNumber]->E) / Info->field->Emax);
	double AtakMax = Info->field->Rmax * (double(Info->robots[myNumber]->V) / Info->field->Lmax) * (double(Info->robots[myNumber]->E) / Info->field->Emax);
	int AptNum;
	double AptRast = 100000;
	int TechNum;
	double TechRast = 100000;
	int aim = -10;
	double aimRast = 100000;
	int trup;

	for (int i = 0; i < Info->field->rivals; i++)
	{
		if (pow(pow(abs(myX - Info->robots[i]->x), 2) + pow(abs(myY - Info->robots[i]->y), 2), 0.5) < aimRast &&
			(Info->robots[i]->playerid != Info->robots[myNumber]->playerid) && (Info->robots[i]->E > 0) &&
			(Info->robots[i]->name != "robotbase.02") &&
			(Info->robots[i]->name != "robotbase.02.1") &&
			(Info->robots[i]->name != "robotbase.02.2") &&
			(Info->robots[i]->name != "robotbase.02.3") &&
			(Info->robots[i]->name != "robotbase.02.4") &&
			(Info->robots[i]->name != "robotbase.02.5") &&
			(Info->robots[i]->name != "robotbase.04") &&
			(Info->robots[i]->name != "robotbase.04.1") &&
			(Info->robots[i]->name != "robotbase.04.2") &&
			(Info->robots[i]->name != "robotbase.04.3") &&
			(Info->robots[i]->name != "robotbase.04.4") &&
			(Info->robots[i]->name != "robotbase.04.5")
			)
		{
			aimRast = pow(pow(abs(myX - Info->robots[i]->x), 2) + pow(abs(myY - Info->robots[i]->y), 2), 0.5);
			aim = i;
		}
	}
	
	for (int i = 0; i < Info->field->Ne + Info->field->Nl; i++)
	{
		trup = 0;
		for (int j = 0; j < Info->field->rivals; j++)
		{
			if ((Info->robots[j]->x == Info->objects[i]->x) && (Info->robots[j]->y == Info->objects[i]->y) && !(Info->robots[j]->alive)) trup = 1;
		}

		if (Info->objects[i]->type == OBJ_CHARGER && trup !=1)
		{
			if (pow(pow(abs(myX - Info->objects[i]->x), 2) + pow(abs(myY - Info->objects[i]->y), 2), 0.5) < AptRast)
			{
				AptRast = pow(pow(abs(myX - Info->objects[i]->x), 2) + pow(abs(myY - Info->objects[i]->y), 2), 0.5);
				AptNum = i;
			}
		}
		if (Info->objects[i]->type == OBJ_TECH && trup != 1)
		{
			if (pow(pow(abs(myX - Info->objects[i]->x), 2) + pow(abs(myY - Info->objects[i]->y), 2), 0.5) < TechRast)
			{
				TechRast = pow(pow(abs(myX - Info->objects[i]->x), 2) + pow(abs(myY - Info->objects[i]->y), 2), 0.5);
				TechNum = i;
			}
		}
	}




	if ((Info->objects[TechNum]->x == myX) && (Info->objects[TechNum]->y == myY) && (Info->robots[myNumber]->E >= 0.9*Info->field->Emax) && (Info->robots[myNumber]->L >= 0.9*Info->field->Lmax) && (Info->stepnum < 0.99*Info->field->N) )
	{

		DoAction(Step, ACT_TECH, 0.45*Info->robots[myNumber]->L, 0.2*Info->robots[myNumber]->L, 0.35*Info->robots[myNumber]->L);
		//file << "nt";
	}

	if ((Info->robots[myNumber]->E >= 0.9*Info->field->Emax) && (Info->robots[myNumber]->L >= 0.9*Info->field->Lmax) && aim != -10 && (Info->stepnum < 0.99*Info->field->N) )
	{
		if (Info->robots[myNumber]->A >= 0.4*Info->robots[myNumber]->L)
		{
			if (aimRast < AtakMax)
			{
				//file << "at";
				DoAction(Step, ACT_ATTACK, Info->robots[aim]->x - myX, Info->robots[aim]->y - myY);
			}
			else
			{
				if (aimRast < StepMax && (Info->robots[myNumber]->x != Info->robots[aim]->x && Info->robots[myNumber]->y != Info->robots[aim]->y))
				{
					//file << "atm";
					DoAction(Step, ACT_MOVE, Info->robots[aim]->x - myX, Info->robots[aim]->y - myY);
				}
				else
				{
					//file << "atmo";
					DoAction(Step, ACT_TECH, 0, 0.4*Info->robots[myNumber]->L, 0.6*Info->robots[myNumber]->L);
					int shagx = abs(myX - Info->robots[aim]->x)*StepMax / aimRast;
					int shagy = abs(myY - Info->robots[aim]->y)*StepMax / aimRast;
					(myX - Info->robots[aim]->x < 0) ? shagx = shagx : shagx = -shagx;
					(myY - Info->robots[aim]->y < 0) ? shagy = shagy : shagy = -shagy;
					DoAction(Step, ACT_MOVE, shagx, shagy);
				}
			}
		}
		else
		{
			//file << "tat";
			DoAction(Step, ACT_TECH, 0.45*Info->robots[myNumber]->L, 0.2*Info->robots[myNumber]->L, 0.35*Info->robots[myNumber]->L);
		}
	}
	else
	if (Info->robots[myNumber]->L < 0.9*Info->field->Lmax && Info->robots[myNumber]->E > 0.5*Info->field->Emax)
	{
		if (TechRast > StepMax && StepMax > 0)
		{
			//file << "tt";
			DoAction(Step, ACT_TECH, 0, 0.4*Info->robots[myNumber]->L, 0.6*Info->robots[myNumber]->L);
			int shagx = abs(myX - Info->objects[TechNum]->x)*StepMax / TechRast;
			int shagy = abs(myY - Info->objects[TechNum]->y)*StepMax / TechRast;
			(myX - Info->objects[TechNum]->x < 0) ? shagx = shagx : shagx = -shagx;
			(myY - Info->objects[TechNum]->y < 0) ? shagy = shagy : shagy = -shagy;

			DoAction(Step, ACT_MOVE, shagx, shagy);
		}
		else
		{
			//file << "tm";
			DoAction(Step, ACT_MOVE, Info->objects[TechNum]->x - myX, Info->objects[TechNum]->y - myY);

			if (Info->robots[myNumber]->L <= 0.89*Info->field->Lmax && (Info->objects[TechNum]->x == myX) && (Info->objects[TechNum]->y == myY))
				DoAction(Step, ACT_TECH, 0,Info->robots[myNumber]->L, 0);
			else
				DoAction(Step, ACT_TECH, 0, 0.5*Info->robots[myNumber]->L, 0.5*Info->robots[myNumber]->L);
		}
	}
	else
	if (AptRast > StepMax && StepMax > 0)
	{
		//file << "ate";
		DoAction(Step, ACT_TECH, 0, 0.5*Info->robots[myNumber]->L, 0.5*Info->robots[myNumber]->L);
		int shagx = abs(myX - Info->objects[AptNum]->x)*StepMax / AptRast;
		int shagy = abs(myY - Info->objects[AptNum]->y)*StepMax / AptRast;
		(myX - Info->objects[AptNum]->x < 0) ? shagx = shagx : shagx = -shagx;
		(myY - Info->objects[AptNum]->y < 0) ? shagy = shagy : shagy = -shagy;
		DoAction(Step, ACT_MOVE, shagx, shagy);
	}
	else
	{
		//file << "am";
		DoAction(Step, ACT_MOVE, Info->objects[AptNum]->x - myX, Info->objects[AptNum]->y - myY);
		if (Info->robots[myNumber]->E <= 0.89*Info->field->Emax && (Info->objects[AptNum]->x == myX) && (Info->objects[AptNum]->y == myY))
			DoAction(Step, ACT_TECH, 0, Info->robots[myNumber]->L, 0);
		else
			DoAction(Step, ACT_TECH, 0, 0.4*Info->robots[myNumber]->L, 0.6*Info->robots[myNumber]->L);
	}

	//file << Info->stepnum << "  x=" << Info->robots[myNumber]->x << " y=" << Info->robots[myNumber]->y << " A=" << Info->robots[myNumber]->A << " P=" << Info->robots[myNumber]->P << " V=" << Info->robots[myNumber]->V << " L=" << Info->robots[myNumber]->L << " E=" << Info->robots[myNumber]->E << " maxAtak=" << AtakMax << " max=" << StepMax << " aimRast=" << aimRast << " AP=" << AptRast << " TE=" << TechRast << " xA=" << Info->objects[AptNum]->x << " yA=" << Info->objects[AptNum]->y << " xT=" << Info->objects[TechNum]->x << " yT=" << Info->objects[TechNum]->y << endl;
	//if (aim != -10) file << "he " << Info->stepnum << "  x=" << Info->robots[aim]->x << " y=" << Info->robots[aim]->y << " A=" << Info->robots[aim]->A << " P=" << Info->robots[aim]->P << " V=" << Info->robots[aim]->V << " L=" << Info->robots[aim]->L << " E=" << Info->robots[aim]->E <<  endl;
	//file << "he6 " << Info->stepnum << "  x=" << Info->robots[4]->x << " y=" << Info->robots[4]->y << " A=" << Info->robots[4]->A << " P=" << Info->robots[4]->P << " V=" << Info->robots[4]->V << " L=" << Info->robots[4]->L << " E=" << Info->robots[4]->E <<  endl;
	//file << "he7 " << Info->stepnum << "  x=" << Info->robots[5]->x << " y=" << Info->robots[5]->y << " A=" << Info->robots[5]->A << " P=" << Info->robots[5]->P << " V=" << Info->robots[5]->V << " L=" << Info->robots[5]->L << " E=" << Info->robots[5]->E << endl;
	//file << "he20 " << Info->stepnum << "  x=" << Info->robots[8]->x << " y=" << Info->robots[8]->y << " A=" << Info->robots[8]->A << " P=" << Info->robots[8]->P << " V=" << Info->robots[8]->V << " L=" << Info->robots[8]->L << " E=" << Info->robots[8]->E << endl << endl;

	return;
}