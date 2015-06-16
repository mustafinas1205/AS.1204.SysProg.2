// robotbase.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include "robotbase.h"

using namespace std;

extern "C" 
void DoStep(stepinfo *Info, step *Step)
{
	int id = Info->yourNumber;
	int L = Info->robots[id]->L;
	int E = Info->robots[id]->E;
	int V = Info->robots[id]->V;
	int x = Info->robots[id]->x;
	int y = Info->robots[id]->y;

	int bros = Info->robots[id]->playerid;
	int egor = 0;
	for (int i = 0; i<Info->field->rivals; i++)
	{
		if (Info->robots[i]->name.GetString() == "robotbase.19" || Info->robots[i]->name.GetString() == "robotbase.19.1")
		{
			egor = Info->robots[i]->playerid;
			break;
		}
	}
	
	for (int i = 0; i<Info->field->Ne; i++)	//если заряжаемся
	{
		if (Info->objects[i]->x == x && Info->objects[i]->y == y && E < 0.95*Info->field->Emax)
		{
			DoAction(Step, ACT_TECH, 0, L, 0);
			return;
		}
	}

	if (E < 0.7*Info->field->Emax)	//если мало энергии
	{
		DoAction(Step, ACT_TECH, 0, L*3/4, L/4);
		int eid = 0;
		int ex = Info->objects[eid]->x;
		int ey = Info->objects[eid]->y;
		double mindist = sqrt(pow(ex-x, 2) + pow(ey-y, 2));
		for (int i = 1; i<Info->field->Ne; i++)	//ищем ближайшую зарядку
		{
			ex = Info->objects[i]->x;
			ey = Info->objects[i]->y;
			double curdist = sqrt(pow(ex-x, 2) + pow(ey-y, 2));
			if (curdist < mindist)
			{
				mindist = curdist;
				eid = i;
			}
		}
		double maxstep = V*Info->field->Vmax/Info->field->Lmax*Info->robots[id]->E/Info->field->Emax;	
		ex = Info->objects[eid]->x;
		ey = Info->objects[eid]->y;
		if (maxstep < mindist)
		{
			int dx, dy;
			dx = (ex - x)*maxstep/mindist;
			dy = (ey - y)*maxstep/mindist;
			DoAction(Step, ACT_MOVE, dx, dy);
		}
		else
		{
			int dx, dy;
			dx = ex - x;
			dy = ey - y;
			DoAction(Step, ACT_MOVE, dx, dy);
		}
	}

	else if (Info->stepnum < 100)
		DoAction(Step, ACT_TECH, 0, L, 0);

	else
	{
		DoAction(Step, ACT_TECH, L/2, L/4, L/4);
		int lid = Info->field->Ne;
		int lx = Info->objects[lid]->x;
		int ly = Info->objects[lid]->y;
		double mindist = sqrt(pow(lx-x, 2) + pow(ly-y, 2));
		for (int i = Info->field->Ne+1; i<Info->field->Ne + Info->field->Nl; i++)	//ищем ближайшую станцию
		{
			lx = Info->objects[i]->x;
			ly = Info->objects[i]->y;
			double curdist = sqrt(pow(lx-x, 2) + pow(ly-y, 2));
			if (curdist < mindist)
			{
				mindist = curdist;
				lid = i;
			}
		}
		double maxstep = V*Info->field->Vmax/Info->field->Lmax*Info->robots[id]->E/Info->field->Emax;	
		lx = Info->objects[lid]->x;
		ly = Info->objects[lid]->y;
		if (maxstep < mindist)
		{
			int dx, dy;
			dx = (lx - x)*maxstep/mindist;
			dy = (ly - y)*maxstep/mindist;
			DoAction(Step, ACT_MOVE, dx, dy);
		}
		else
		{
			int dx, dy;
			dx = lx - x;
			dy = ly - y;
			DoAction(Step, ACT_MOVE, dx, dy);
		}

		bool found = false;
		int target;
		double A = Info->robots[id]->A * Info->robots[id]->E / Info->field->Emax;
		double maxattack = Info->field->Rmax*V/Info->field->Lmax*E/Info->field->Emax;
		for (int i = 0; i < Info->field->rivals; i++)
		{
			if (Info->robots[i]->playerid != bros && Info->robots[i]->playerid != egor)
			{
				int hisx = Info->robots[i]->x;
				int hisy = Info->robots[i]->y;
				double robodist = sqrt(pow(hisx-x,2) + pow(hisy-y,2));
				if (robodist < maxattack)
				{
					double hisP = Info->robots[i]->A * Info->robots[i]->E / Info->field->Emax;
					if (hisP < A)
					{
						target = i;
						found = true;
						break;
					}
				}
			}
		}
		if (found)
		{
			int hisx = Info->robots[target]->x;
			int hisy = Info->robots[target]->y;
			int dx = hisx - x;
			int dy = hisy - y;
			DoAction(Step, ACT_ATTACK, dx, dy);
		}
	}
	return;
}


