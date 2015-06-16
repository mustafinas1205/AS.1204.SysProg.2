// robotbase.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include "robotbase.h"

using namespace std;

extern "C" 
void DoStep(stepinfo *Info, step *Step)
{
	int me = Info->yourNumber;
	int x = Info->robots[me]->x;
	int y = Info->robots[me]->y;
	int L = Info->robots[me]->L;
	int E = Info->robots[me]->E;

	bool onobj = false;
	for (int i = 0; i < Info->field->Ne+Info->field->Nl; i++)
	{
		int ox = Info->objects[i]->x;
		int oy = Info->objects[i]->y;
		if (ox == x && oy == y)
		{
			onobj = true;
			break;
		}
	}

	bool needcharge = false;
	bool needtech = false;

	if (E < 0.5 * Info->field->Emax)
		needcharge = true;
	if (L < 0.85 * Info->field->Lmax)
		needtech = true;

	if (onobj)	//если на зарядке
	{
		DoAction(Step, ACT_TECH, 0.3*L, 0.3*L, 0.4*L);
		double attackdist = Info->field->Rmax*Info->robots[me]->V/Info->field->Lmax*E/Info->field->Emax;
		for (int i = 0; i < Info->field->rivals; i++)
		{
			if (Info->robots[i]->playerid != Info->robots[me]->playerid)
			{
				int hisx = Info->robots[i]->x;
				int hisy = Info->robots[i]->y;
				double disttorobot = sqrt(pow(hisx-x,2) + pow(hisy-y,2));
				if (disttorobot < attackdist)
				{
					DoAction(Step,ACT_ATTACK,hisx-x,hisy-y);
					break;
				}
			}
		}

		if (needcharge)
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
			double maxstep = Info->robots[me]->V*Info->field->Vmax/Info->field->Lmax*Info->robots[me]->E/Info->field->Emax;	
			ex = Info->objects[eid]->x;
			ey = Info->objects[eid]->y;
			int dx, dy;
			if (maxstep < mindist)
			{
				dx = (ex - x)*maxstep/mindist;
				dy = (ey - y)*maxstep/mindist;
			}
			else
			{
				dx = ex - x;
				dy = ey - y;
				DoAction(Step, ACT_TECH, 0.3*L, 0.3*L, 0.4*L);
			}
			DoAction(Step, ACT_MOVE, dx, dy);
		}
		else if (needtech)
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
			double maxstep = Info->robots[me]->V*Info->field->Vmax/Info->field->Lmax*Info->robots[me]->E/Info->field->Emax;	
			lx = Info->objects[lid]->x;
			ly = Info->objects[lid]->y;
			int dx, dy;
			if (maxstep < mindist)
			{
				dx = (lx - x)*maxstep/mindist;
				dy = (ly - y)*maxstep/mindist;
			}
			else
			{
				dx = lx - x;
				dy = ly - y;
				DoAction(Step, ACT_TECH, 0.3*L, 0.3*L, 0.4*L);
			}
			DoAction(Step, ACT_MOVE, dx, dy);
		}
	}
	else  //если не на зарядке
	{
		if (needtech)
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
			double maxstep = Info->robots[me]->V*Info->field->Vmax/Info->field->Lmax*Info->robots[me]->E/Info->field->Emax;	
			lx = Info->objects[lid]->x;
			ly = Info->objects[lid]->y;
			int dx, dy;
			if (maxstep < mindist)
			{
				dx = (lx - x)*maxstep/mindist;
				dy = (ly - y)*maxstep/mindist;
			}
			else
			{
				dx = lx - x;
				dy = ly - y;
				DoAction(Step, ACT_TECH, 0.3*L, 0.3*L, 0.4*L);
			}
			DoAction(Step, ACT_MOVE, dx, dy);
		}
		else
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
			double maxstep = Info->robots[me]->V*Info->field->Vmax/Info->field->Lmax*Info->robots[me]->E/Info->field->Emax;	
			ex = Info->objects[eid]->x;
			ey = Info->objects[eid]->y;
			int dx, dy;
			if (maxstep < mindist)
			{
				dx = (ex - x)*maxstep/mindist;
				dy = (ey - y)*maxstep/mindist;
			}
			else
			{
				dx = ex - x;
				dy = ey - y;
				DoAction(Step, ACT_TECH, 0.3*L, 0.3*L, 0.4*L);
			}
			DoAction(Step, ACT_MOVE, dx, dy);
		}
	}
	return;
}


