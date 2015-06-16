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
		ofstream File("../Robots/robotbase.20/Nech");
		File << 0;
		File.close();
	}

	int myid = Info->yourNumber;
	int mypl = Info->robots[myid]->playerid;
	int myx = Info->robots[myid]->x;
	int myy = Info->robots[myid]->y;
	int L = Info->robots[myid]->L;
	int E = Info->robots[myid]->E;

	for (int i = 0; i<Info->field->Ne+Info->field->Nl; i++)	//если заряжаемся
	{
		if (Info->objects[i]->type == OBJ_CHARGER && Info->objects[i]->x == myx && Info->objects[i]->y == myy && E < 0.95*Info->field->Emax)
		{
			DoAction(Step, ACT_TECH, 0, L, 0);
			return;
		}
		if (Info->objects[i]->type == OBJ_TECH && Info->objects[i]->x == myx && Info->objects[i]->y == myy && L < Info->field->Lmax)
		{
			DoAction(Step, ACT_TECH, 0, L, 0);
			return;
		}
	}

	if (Info->robots[myid]->E < 0.45*Info->field->Emax)
	{
		int V = Info->field->Vmax;
		if (V > L)
			V = L;
		DoAction(Step, ACT_TECH, 0, L-V, V);
		int eid = 0;
		int ex = Info->objects[eid]->x;
		int ey = Info->objects[eid]->y;
		double mindist = sqrt(pow(ex-myx, 2) + pow(ey-myy, 2));
		for (int i = 1; i<Info->field->Ne; i++)
		{
			ex = Info->objects[i]->x;
			ey = Info->objects[i]->y;
			double curdist = sqrt(pow(ex-myx, 2) + pow(ey-myy, 2));
			if (curdist < mindist)
			{
				mindist = curdist;
				eid = i;
			}
		}
		double maxstep = Info->robots[myid]->V*Info->field->Vmax/Info->field->Lmax*Info->robots[myid]->E/Info->field->Emax;	
		ex = Info->objects[eid]->x;
		ey = Info->objects[eid]->y;
		if (maxstep < mindist)
		{
			int dx, dy;
			dx = (ex - myx)*maxstep/mindist;
			dy = (ey - myy)*maxstep/mindist;
			DoAction(Step, ACT_MOVE, dx, dy);
		}
		else
		{
			int dx, dy;
			dx = ex - myx;
			dy = ey - myy;
			DoAction(Step, ACT_MOVE, dx, dy);
			DoAction(Step, ACT_TECH, 0, L, 0);
		}
	}
	else
	{
		if (Info->robots[myid]->L == Info->field->Lmax)
		{
			int victim;
			ifstream File("../Robots/robotbase.20/Nech");
			File >> victim;
			File.close();
			
			DoAction(Step, ACT_TECH, 0.75*L, 0, 0.25*L); 
			int hisx = Info->robots[victim]->x;
			int hisy = Info->robots[victim]->y;
			double distance = sqrt(pow(hisx-myx,2) + pow(hisy-myy,2));
			double attack = Info->field->Rmax*Info->robots[myid]->V/Info->field->Lmax*Info->robots[myid]->E/Info->field->Emax;
			if (distance <= attack)
			{
				DoAction(Step, ACT_ATTACK, hisx-myx, hisy-myy);
				do
				{
					victim++;
					if (victim == Info->field->rivals)
						victim = 0;
				}
				while (Info->robots[victim]->playerid == mypl || !Info->robots[victim]->alive);
				ofstream File("../Robots/robotbase.20/Nech");
				File << victim;
				File.close();
			}
			else
			{
				double maxstep = Info->robots[myid]->V*Info->field->Vmax/Info->field->Lmax*Info->robots[myid]->E/Info->field->Emax;
				double curdist = sqrt(pow(hisx-myx, 2) + pow(hisy-myy, 2));
				int dx, dy;
				if (curdist > maxstep)
				{
					dx = (hisx - myx)*maxstep/distance;
					dy = (hisy - myy)*maxstep/distance;
				}
				else
				{
					dx = hisx - myx;
					dy = hisy - myy;
					if (hisx > myx)
						dx-=1;
					else if (hisx < myx)
						dx+=1;
					else if (hisy > myy)
						dy-=1;
					else dy+=1;
				}
				DoAction(Step, ACT_MOVE, dx, dy);
			}
		}
		else
		{
			int V = Info->field->Vmax;
			if (V > L)
				V = L;
			DoAction(Step, ACT_TECH, 0, L-V, V);
			int lid = Info->field->Ne;
			int lx = Info->objects[lid]->x;
			int ly = Info->objects[lid]->y;
			double mindist = sqrt(pow(lx-myx, 2) + pow(ly-myy, 2));
			for (int i = Info->field->Ne+1; i<Info->field->Ne + Info->field->Nl; i++)
			{
				lx = Info->objects[i]->x;
				ly = Info->objects[i]->y;
				double curdist = sqrt(pow(lx-myx, 2) + pow(ly-myy, 2));
				if (curdist < mindist)
				{
					mindist = curdist;
					lid = i;
				}
			}
			double maxstep = Info->robots[myid]->V*Info->field->Vmax/Info->field->Lmax*Info->robots[myid]->E/Info->field->Emax;	
			lx = Info->objects[lid]->x;
			ly = Info->objects[lid]->y;
			if (maxstep < mindist)
			{
				int dx, dy;
				dx = (lx - myx)*maxstep/mindist;
				dy = (ly - myy)*maxstep/mindist;
				DoAction(Step, ACT_MOVE, dx, dy);
			}
			else
			{
				int dx, dy;
				dx = lx - myx;
				dy = ly - myy;
				DoAction(Step, ACT_MOVE, dx, dy);
				DoAction(Step, ACT_TECH, 0, L, 0);
			}
		}
	}
	return;
}


