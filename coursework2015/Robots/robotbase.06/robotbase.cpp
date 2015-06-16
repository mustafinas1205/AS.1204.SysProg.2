// robotbase.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include "robotbase.h"
#include <fstream>

using namespace std;

extern "C" 
void DoStep(stepinfo *Info, step *Step)
{
	if (Info->stepnum == 1)
	{
		ofstream File("../Robots/robotbase.06/Zolotukhin.txt");
		File << 1;
		File.close();
	}

	int num = Info->yourNumber;
	int x = Info->robots[num]->x;
	int y = Info->robots[num]->y;
	int L = Info->robots[num]->L;
	short attack;

	for (int i = Info->field->Ne; i<Info->field->Ne+Info->field->Nl; i++)	
	{
		if (Info->objects[i]->type == OBJ_TECH && Info->objects[i]->x == x && Info->objects[i]->y == y && L < Info->field->Lmax)
		{
			if (Info->field->Lmax - L <= Info->field->dL)
				DoAction(Step, ACT_TECH, 0.3*L, 0.3*L, 0.4*L);
			else
				DoAction(Step, ACT_TECH, 0, L, 0);
			return;
		}
	}

	if (L < Info->field->Lmax)
	{
		int V = Info->field->Vmax;
		if (V > L)
			V = L;
		DoAction(Step, ACT_TECH, 0, L-V, V);
		int lid = Info->field->Ne;
		int lx = Info->objects[lid]->x;
		int ly = Info->objects[lid]->y;
		double mindist = sqrt(pow(lx-x, 2) + pow(ly-y, 2));
		for (int i = Info->field->Ne+1; i<Info->field->Ne + Info->field->Nl; i++)
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
		double maxstep = Info->robots[num]->V*Info->field->Vmax/Info->field->Lmax*Info->robots[num]->E/Info->field->Emax;	
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
			DoAction(Step, ACT_TECH, 0, L, 0);
		}
	}
	else
	{
		ifstream File("../Robots/robotbase.06/Zolotukhin.txt");
		File >> attack;
		File.close();
		if (Info->history[num] && !Info->history[num]->actions[ACT_MOVE] && !attack)	//если в том ходу не шевелились
		{
			DoAction(Step, ACT_TECH, 0.3*L, 0.3*L, 0.4*L);
			ofstream File("../Robots/robotbase.06/Zolotukhin.txt");
			File << 1;
			File.close();
		}
		else
		{
			DoAction(Step, ACT_TECH, 0.3*L, 0.3*L, 0.4*L);
			if (attack)	//ищем жертву
			{
				double dist = Info->field->fieldWidth;
				int n;
				for (int i = 0; i < Info->field->rivals; i++)
				{
					if (Info->robots[i]->alive && Info->robots[i]->playerid != Info->robots[num]->playerid)
					{
						int hisx = Info->robots[i]->x;
						int hisy = Info->robots[i]->y;
						double tmpdist = sqrt(pow(hisx-x,2) + pow(hisy-y,2));
						if (tmpdist < dist)
						{
							dist = tmpdist;
							n = i;
						}
					}
				}
				int rx = Info->robots[n]->x;
				int ry = Info->robots[n]->y;
				double attackdist = Info->field->Rmax*Info->robots[num]->V/Info->field->Lmax*Info->robots[num]->E/Info->field->Emax;
				double step = Info->robots[num]->V*Info->field->Vmax/Info->field->Lmax*Info->robots[num]->E/Info->field->Emax;
				if (dist > attackdist)
				{
					int dx, dy;
					if (dist > step)
					{
						dx = (rx - x)*step/dist;
						dy = (ry - y)*step/dist;
					}
					else
					{
						dx = rx - x;
						dy = ry - y;
						if (rx > x)
							dx-=1;
						else if (rx < x)
							dx+=1;
						else if (ry > y)
							dy-=1;
						else dy+=1;
					}
					DoAction(Step, ACT_MOVE, dx, dy);
				}
				else
				{
					DoAction(Step, ACT_ATTACK, rx-x, ry-y);
					int V = Info->field->Vmax;
					if (V > L)
						V = L;
					DoAction(Step, ACT_TECH, 0, L-V, V);
					ofstream File("../Robots/robotbase.06/Zolotukhin.txt");
					File << 0;
					File.close();
					int eid = 0;
					int ex = Info->objects[eid]->x;
					int ey = Info->objects[eid]->y;
					double mindist = sqrt(pow(ex-x, 2) + pow(ey-y, 2));
					for (int i = 1; i<Info->field->Ne; i++)
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
					ex = Info->objects[eid]->x;
					ey = Info->objects[eid]->y;
					int dx, dy;
					dx = (ex - x)*step/mindist;
					dy = (ey - y)*step/mindist;
					DoAction(Step, ACT_MOVE, dx, dy);
				}
			}
			else  //бежали заряжаться
			{
				int V = Info->field->Vmax;
				if (V > L)
					V = L;
				DoAction(Step, ACT_TECH, 0, L-V, V);
				int eid = 0;
				int ex = Info->objects[eid]->x;
				int ey = Info->objects[eid]->y;
				double mindist = sqrt(pow(ex-x, 2) + pow(ey-y, 2));
				for (int i = 1; i<Info->field->Ne; i++)
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
				double maxstep = Info->robots[num]->V*Info->field->Vmax/Info->field->Lmax*Info->robots[num]->E/Info->field->Emax;	
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
					DoAction(Step, ACT_TECH, 0, L, 0);
				}
			}
		}
	}
	return;
}


