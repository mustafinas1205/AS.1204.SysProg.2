// robotbase.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include "robotbase.h"
#include <fstream>

using namespace std;

extern "C" 
void DoStep(stepinfo *Info, step *Step)
{
	int me = Info->yourNumber;
	int id = Info->robots[me]->playerid;
	int cx = Info->robots[me]->x;
	int cy = Info->robots[me]->y;
	int cL = Info->robots[me]->L;
	int cE = Info->robots[me]->E;
	int charge;

	if (cE == Info->field->Emax)
	{
		ofstream File("../Robots/robotbase.23/charge.txt");
		File << 0;
		File.close();
	}

	ifstream File("../Robots/robotbase.23/charge.txt");
	File >> charge;
	File.close();

	if (cL < Info->field->Lmax)
	{
		for (int i = Info->field->Ne; i < Info->field->Ne+Info->field->Nl; i++)
		{
			if (Info->objects[i]->x == cx && Info->objects[i]->y == cy)
			{
				DoAction(Step, ACT_TECH, 0, cL, 0);
				return;
			}
		}
		int V = Info->field->Vmax;
		if (V > cL)
			V = cL;
		DoAction(Step, ACT_TECH, 0, cL-V, V);
		int lid = Info->field->Ne;
		int lx = Info->objects[lid]->x;
		int ly = Info->objects[lid]->y;
		double mindist = sqrt(pow(lx-cx, 2) + pow(ly-cy, 2));
		for (int i = Info->field->Ne+1; i<Info->field->Ne+Info->field->Nl; i++)
		{
			lx = Info->objects[i]->x;
			ly = Info->objects[i]->y;
			double curdist = sqrt(pow(lx-cx, 2) + pow(ly-cy, 2));
			if (curdist < mindist)
			{
				mindist = curdist;
				lid = i;
			}
		}
		double maxstep = Info->robots[me]->V*Info->field->Vmax/Info->field->Lmax*cE/Info->field->Emax;	
		lx = Info->objects[lid]->x;
		ly = Info->objects[lid]->y;
		int dx, dy;
		if (maxstep < mindist)
		{
			dx = (lx - cx)*maxstep/mindist;
			dy = (ly - cy)*maxstep/mindist;
		}
		else
		{
			dx = lx - cx;
			dy = ly - cy;
			DoAction(Step, ACT_TECH, 0, cL, 0);
			ofstream File("../Robots/robotbase.23/charge.txt");
			File << 1;
			File.close();
		}
		DoAction(Step, ACT_MOVE, dx, dy);
		return;
	}
	else if (cE < 0.75 * Info->field->Emax || charge)
	{
		for (int i = 0; i < Info->field->Ne; i++)
		{
			if (Info->objects[i]->x == cx && Info->objects[i]->y == cy)
			{
				DoAction(Step, ACT_TECH, 0, cL, 0);
				return;
			}
			int V = Info->field->Vmax;
			if (V > cL)
				V = cL;
			DoAction(Step, ACT_TECH, 0, cL-V, V);
			int eid = 0;
			int ex = Info->objects[eid]->x;
			int ey = Info->objects[eid]->y;
			double mindist = sqrt(pow(ex-cx, 2) + pow(ey-cy, 2));
			for (int i = 1; i<Info->field->Ne; i++)
			{
				ex = Info->objects[i]->x;
				ey = Info->objects[i]->y;
				double curdist = sqrt(pow(ex-cx, 2) + pow(ey-cy, 2));
				if (curdist < mindist)
				{
					mindist = curdist;
					eid = i;
				}
			}
			double maxstep = Info->robots[me]->V*Info->field->Vmax/Info->field->Lmax*Info->robots[me]->E/Info->field->Emax;	
			ex = Info->objects[eid]->x;
			ey = Info->objects[eid]->y;
			int dx, dy;
			if (maxstep < mindist)
			{
				dx = (ex - cx)*maxstep/mindist;
				dy = (ey - cy)*maxstep/mindist;
			}
			else
			{
				dx = ex - cx;
				dy = ey - cy;
				DoAction(Step, ACT_TECH, 0, cL, 0);
				ofstream File("../Robots/robotbase.23/charge.txt");
				File << 0;
				File.close();
			}
			DoAction(Step, ACT_MOVE, dx, dy);
		}
		return;
	}


	double mindist = 123456;
	int aim = -1;
	for (int i = 0; i < Info->field->rivals; i++)	//ищем ближайшего
	{
		if (Info->robots[i]->alive && Info->robots[i]->playerid != id)
		{
			int hx = Info->robots[i]->x;
			int hy = Info->robots[i]->y;
			double dist = sqrt(pow(hx-cx,2) + pow(hy-cy,2));
			if (dist < mindist)
			{
				mindist = dist;
				aim = i;
			}
		}
	}

	if (aim == -1)
		return;

	int ax = Info->robots[aim]->x;
	int ay = Info->robots[aim]->y;
	if (ax > cx)
		ax-=1;
	else if (ax < cx)
		ax+=1;
	else if (ay > cy)
		ay-=1;
	else
		ay+=1;

	if (Info->robots[aim]->P == Info->robots[aim]->L)	//если враг в полной защите
	{
		if (cx != ax || cy != ay)	//если еще не дошли
		{
			double maxstep = Info->robots[me]->V*Info->field->Vmax/Info->field->Lmax*cE/Info->field->Emax;
			int dx, dy;
			if (maxstep < mindist)
			{
				dx = (ax - cx)*maxstep/mindist;
				dy = (ay - cy)*maxstep/mindist;
			}
			else
			{
				dx = ax - cx;
				dy = ay - cy;
				DoAction(Step, ACT_TECH, 0.89*cL, 0.01*cL, 0.1*cL);
			}
			DoAction(Step, ACT_MOVE, dx, dy);
		}
		else	//у врага
		{
			if (Info->robots[me]->A > 0)
			{
				int dx = Info->robots[aim]->x - cx;
				int dy = Info->robots[aim]->y - cy;
				DoAction(Step, ACT_ATTACK, dx, dy);
			}
		}
	}
	else	//если враг не в дефе
	{
		double maxattack = Info->field->Rmax*0.2*cL/Info->field->Lmax*cE/Info->field->Emax;
		if (mindist > maxattack)
		{
			int V = Info->field->Vmax;
			if (V > cL)
				V = cL;
			DoAction(Step, ACT_TECH, 0, cL-V, V);
			double maxstep = Info->robots[me]->V*Info->field->Vmax/Info->field->Lmax*cE/Info->field->Emax;
			int dx, dy;
			if (maxstep < mindist)
			{
				dx = (ax - cx)*maxstep/mindist;
				dy = (ay - cy)*maxstep/mindist;
			}
			else
			{
				dx = ax - cx;
				dy = ay - cy;
				DoAction(Step, ACT_TECH, 0.79*cL, 0.01*cL, 0.2*cL);
			}
			DoAction(Step, ACT_MOVE, dx, dy);
		}
		else
		{
			DoAction(Step, ACT_TECH, 0.79*cL, 0.01*cL, 0.2*cL);
			if (Info->robots[me]->A > 0)
			{
				int dx = Info->robots[aim]->x - cx;
				int dy = Info->robots[aim]->y - cy;
				DoAction(Step, ACT_ATTACK, dx, dy);
			}
		}
	}
	return;
}


