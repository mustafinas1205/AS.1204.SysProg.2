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
	int pid = Info->robots[me]->playerid;
	int x = Info->robots[me]->x;
	int y = Info->robots[me]->y;
	int L = Info->robots[me]->L;
	int E = Info->robots[me]->E;
	int coalition = 0;
	int coalitionid[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int coalitionx[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int coalitiony[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	for (int i = 0; i < Info->field->rivals; i++)
	{
		if (Info->robots[i]->playerid == pid){
			coalitionid[coalition] = i;
			coalitionx[coalition] = Info->robots[i]->x;
			coalitiony[coalition] = Info->robots[i]->y;
			coalition++;

		}
	}

	for (int i = 0; i<Info->field->Ne + Info->field->Nl; i++)
	{
		if (Info->objects[i]->type == OBJ_CHARGER && Info->objects[i]->x == x && Info->objects[i]->y == y && E < 0.95*Info->field->Emax)
		{
			DoAction(Step, ACT_TECH, 0, L, 0);
			return;
		}
		if (Info->objects[i]->type == OBJ_TECH && Info->objects[i]->x == x && Info->objects[i]->y == y && L < 0.80*Info->field->Lmax)
		{
			DoAction(Step, ACT_TECH, 0, L, 0);
			return;
		}
	}

	if (Info->stepnum == 1)
	{
		ofstream File("../Robots/robotbase.14/Koshechkin");
		File << 0;
		File.close();
	}
	//if (E < 0.75*Info->field->Emax || (Info->robots[me]->kills >= 3 && coalition == 1))
	if (E < 0.90*Info->field->Emax)
	{
		int V = Info->field->Vmax;
		if (V > L)
			V = L;
		DoAction(Step, ACT_TECH, 0, L - V, V);
		int eid = 0;
		int ex = Info->objects[eid]->x;
		int ey = Info->objects[eid]->y;
		double mindist = sqrt(pow(ex - x, 2) + pow(ey - y, 2));
		for (int i = 1; i<Info->field->Ne; i++)
		{
			ex = Info->objects[i]->x;
			ey = Info->objects[i]->y;
			double curdist = sqrt(pow(ex - x, 2) + pow(ey - y, 2));
			if (curdist < mindist)
			{
				mindist = curdist;
				eid = i;
			}
		}
		double maxstep = Info->robots[me]->V*Info->field->Vmax / Info->field->Lmax*Info->robots[me]->E / Info->field->Emax;
		ex = Info->objects[eid]->x;
		ey = Info->objects[eid]->y;
		if (maxstep < mindist)
		{
			int dx, dy;
			dx = (ex - x)*maxstep / mindist;
			dy = (ey - y)*maxstep / mindist;
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
	else
	{
		if (L == Info->field->Lmax)
		{

			

			/*if (coalition > 1 && me != coalitionid[0]){

			double distancetoc = sqrt(pow(coalitionx[0] - x, 2) + pow(coalitiony[0] - y, 2));
			double maxstep = Info->robots[me]->V*Info->field->Vmax / Info->field->Lmax*Info->robots[me]->E / Info->field->Emax;
			if (maxstep < distancetoc)
			{
			int dx, dy;
			dx = (coalitionx[0] - x)*maxstep / distancetoc;
			dy = (coalitiony[0] - y)*maxstep / distancetoc;
			DoAction(Step, ACT_MOVE, dx, dy);
			}
			else
			{
			int dx, dy;
			dx = coalitionx[0] - x;
			dy = coalitiony[0] - y;
			DoAction(Step, ACT_MOVE, dx, dy);
			}


			}*/


			int kill;
			ifstream File("../Robots/robotbase.14/Koshechkin");
			File >> kill;
			File.close();

			DoAction(Step, ACT_TECH, 0.50*L, 0, 0.50*L);
			int enx = Info->robots[kill]->x;
			int eny = Info->robots[kill]->y;
			double distance = sqrt(pow(enx - x, 2) + pow(eny - y, 2));
			double attack = Info->field->Rmax*Info->robots[me]->V / Info->field->Lmax*Info->robots[me]->E / Info->field->Emax;


			if ((distance <= attack))
			{
				DoAction(Step, ACT_ATTACK, enx - x, eny - y);
				do
				{
					kill++;
					if (kill == Info->field->rivals)
						kill = 0;
				} while (Info->robots[kill]->playerid == pid || !Info->robots[kill]->alive);
				ofstream File("../Robots/robotbase.14/Koshechkin");
				File << kill;
				File.close();
			}
			else
			{
				double maxstep = Info->robots[me]->V*Info->field->Vmax / Info->field->Lmax*Info->robots[me]->E / Info->field->Emax;
				double curdist = sqrt(pow(enx - x, 2) + pow(eny - y, 2));
				int dx, dy;
				if (curdist > maxstep)
				{
					dx = (enx - x)*maxstep / distance;
					dy = (eny - y)*maxstep / distance;
				}
				else
				{
					dx = enx - x;
					dy = eny - y;
					if (enx > x)
						dx -= 1;
					else if (enx < x)
						dx += 1;
					else if (eny > y)
						dy -= 1;
					else dy += 1;
				}
				DoAction(Step, ACT_MOVE, dx, dy);
			}
		}
		else
		{
			int V = Info->field->Vmax;
			if (V > L)
				V = L;
			DoAction(Step, ACT_TECH, 0, L - V, V);
			int lid = Info->field->Ne;
			int lx = Info->objects[lid]->x;
			int ly = Info->objects[lid]->y;
			double mindist = sqrt(pow(lx - x, 2) + pow(ly - y, 2));
			for (int i = Info->field->Ne + 1; i<Info->field->Ne + Info->field->Nl; i++)
			{
				lx = Info->objects[i]->x;
				ly = Info->objects[i]->y;
				double curdist = sqrt(pow(lx - x, 2) + pow(ly - y, 2));
				if (curdist < mindist)
				{
					mindist = curdist;
					lid = i;
				}
			}
			double maxstep = Info->robots[me]->V*Info->field->Vmax / Info->field->Lmax*Info->robots[me]->E / Info->field->Emax;
			lx = Info->objects[lid]->x;
			ly = Info->objects[lid]->y;
			if (maxstep < mindist)
			{
				int dx, dy;
				dx = (lx - x)*maxstep / mindist;
				dy = (ly - y)*maxstep / mindist;
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
	}
	return;
}


