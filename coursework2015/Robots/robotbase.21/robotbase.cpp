// robotbase.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include "robotbase.h"
#include <fstream>

using namespace std;

extern "C" 

int n = 0;
int id;
int x;
int y;
int E;
int L;

void DoStep(stepinfo *Info, step *Step)
{
	n = Info->yourNumber;
	id = Info->robots[n]->playerid;
	x = Info->robots[n]->x;
	y = Info->robots[n]->y;
	E = Info->robots[n]->E;
	L = Info->robots[n]->L;

	if (Info->stepnum == 1)
	{
		ofstream fStep("../Robots/robotbase.21/Yuri.txt");
		fStep << 1;
		fStep.close();
	}

	if (L < 0.8 * Info->field->Lmax)
	{
		ofstream fStep("../Robots/robotbase.21/Yuri.txt");
		fStep << 4;
		fStep.close();
	}

	if (E < 0.4 * Info->field->Emax)
	{
		ofstream fStep("../Robots/robotbase.21/Yuri.txt");
		fStep << 2;
		fStep.close();
	}

	int step;
	ifstream fStep("../Robots/robotbase.21/Yuri.txt");
	fStep >> step;
	fStep.close();

	switch(step)
	{
	case 1:
		{
			DoAction(Step, ACT_TECH, 0.4*L, 0.4*L, 0.2*L);
			double dist = Info->field->fieldWidth;
			int v;
			for (int i = 0; i < Info->field->rivals; i++)
			{
				if (Info->robots[i]->alive && Info->robots[i]->playerid != id)
				{
					int vx = Info->robots[i]->x;
					int vy = Info->robots[i]->y;
					double tmpdist = sqrt(pow(vx-x,2) + pow(vy-y,2));
					if (tmpdist < dist)
					{
						dist = tmpdist;
						v = i;
					}
				}
			}
			int vx = Info->robots[v]->x;
			int vy = Info->robots[v]->y;
			double attackdist = Info->field->Rmax*Info->robots[n]->V/Info->field->Lmax*Info->robots[n]->E/Info->field->Emax;
			double step = Info->robots[n]->V*Info->field->Vmax/Info->field->Lmax*Info->robots[n]->E/Info->field->Emax;
			if (dist > attackdist)
			{
				int dx, dy;
				if (dist > step)
				{
					dx = (vx - x)*step/dist;
					dy = (vy - y)*step/dist;
				}
				else
				{
					dx = vx - x;
					dy = vy - y;
					if (vx > x)
						dx-=1;
					else if (vx < x)
						dx+=1;
					else if (vy > y)
						dy-=1;
					else dy+=1;
				}
				DoAction(Step, ACT_MOVE, dx, dy);
			}
			else
			{
				double A = Info->robots[n]->A * Info->robots[n]->E / Info->field->Emax;
				double P = Info->robots[v]->P * Info->robots[v]->E / Info->field->Emax;
				int dx, dy;
				dx = Info->robots[v]->x - Info->robots[n]->x;
				dy = Info->robots[v]->y - Info->robots[n]->y;
				if (A>=P)
					DoAction(Step, ACT_ATTACK, dx, dy);
				ofstream fStep("../Robots/robotbase.21/Yuri.txt");
				fStep << 2;
				fStep.close();
			}
			break;
		}
	case 2:
		{
			DoAction(Step, ACT_TECH, 0.4*L, 0.1*L, 0.5*L);
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
			double maxstep = Info->robots[n]->V*Info->field->Vmax/Info->field->Lmax*E/Info->field->Emax;	
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
				DoAction(Step, ACT_TECH, 0.4*L, 0.3*L, 0.3*L);
				ofstream fStep("../Robots/robotbase.21/Yuri.txt");
				fStep << 3;
				fStep.close();
			}

			bool found = false;
			int v;
			double A = Info->robots[n]->A * Info->robots[n]->E / Info->field->Emax;
			double attackdist = Info->field->Rmax*Info->robots[n]->V/Info->field->Lmax*Info->robots[n]->E/Info->field->Emax;
			for (int i = 0; i < Info->field->rivals; i++)
			{
				if (Info->robots[i]->alive && Info->robots[i]->playerid != id)
				{
					int vx = Info->robots[i]->x;
					int vy = Info->robots[i]->y;
					double robodist = sqrt(pow(vx-x,2) + pow(vy-y,2));
					if (robodist < attackdist)
					{
						double vP = Info->robots[i]->P * Info->robots[i]->E / Info->field->Emax;
						if (vP < A)
						{
							v = i;
							found = true;
							break;
						}
					}
				}
			}
			if (found)
			{
				int vx = Info->robots[v]->x;
				int vy = Info->robots[v]->y;
				int dx = vx - x;
				int dy = vy - y;
				DoAction(Step, ACT_ATTACK, dx, dy);
			}
			break;
		}
	case 3:
		{
			DoAction(Step, ACT_TECH, 0.4*L, 0.3*L, 0.3*L);
			if (E < Info->field->Emax * 0.99)
			{
				bool found = false;
				int v;
				double A = Info->robots[n]->A * Info->robots[n]->E / Info->field->Emax;
				double attackdist = Info->field->Rmax*Info->robots[n]->V/Info->field->Lmax*Info->robots[n]->E/Info->field->Emax;
				for (int i = 0; i < Info->field->rivals; i++)
				{
					if (Info->robots[i]->playerid != id)
					{
						int vx = Info->robots[i]->x;
						int vy = Info->robots[i]->y;
						double robodist = sqrt(pow(vx-x,2) + pow(vy-y,2));
						if (robodist < attackdist)
						{
							double vP = Info->robots[i]->P * Info->robots[i]->E / Info->field->Emax;
							if (vP < A)
							{
								v = i;
								found = true;
								break;
							}
						}
					}
				}
				if (found)
				{
					int vx = Info->robots[v]->x;
					int vy = Info->robots[v]->y;
					int dx = vx - x;
					int dy = vy - y;
					DoAction(Step, ACT_ATTACK, dx, dy);
				}
			}
			else
			{
				DoAction(Step, ACT_TECH, 0.4*L, 0.1*L, 0.5*L);
				ofstream fStep("../Robots/robotbase.21/Yuri.txt");
				fStep << 4;
				fStep.close();
			}
			break;
		}
	case 4:
		{
			DoAction(Step, ACT_TECH, 0.4*L, 0.1*L, 0.5*L);
			int lid = Info->field->Ne;
			int lx = Info->objects[lid]->x;
			int ly = Info->objects[lid]->y;
			double mindist = sqrt(pow(lx-x, 2) + pow(ly-y, 2));
			for (int i = Info->field->Ne+1; i<Info->field->Ne+Info->field->Nl; i++)
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
			double maxstep = Info->robots[n]->V*Info->field->Vmax/Info->field->Lmax*E/Info->field->Emax;	
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
				DoAction(Step, ACT_TECH, 0.4*L, 0.3*L, 0.3*L);
				ofstream fStep("../Robots/robotbase.21/Yuri.txt");
				fStep << 5;
				fStep.close();
			}

			bool found = false;
			int v;
			double A = Info->robots[n]->A * Info->robots[n]->E / Info->field->Emax;
			double attackdist = Info->field->Rmax*Info->robots[n]->V/Info->field->Lmax*Info->robots[n]->E/Info->field->Emax;
			for (int i = 0; i < Info->field->rivals; i++)
			{
				if (Info->robots[i]->alive && Info->robots[i]->playerid != id)
				{
					int vx = Info->robots[i]->x;
					int vy = Info->robots[i]->y;
					double robodist = sqrt(pow(vx-x,2) + pow(vy-y,2));
					if (robodist < attackdist)
					{
						double vP = Info->robots[i]->P * Info->robots[i]->E / Info->field->Emax;
						if (vP < A)
						{
							v = i;
							found = true;
							break;
						}
					}
				}
			}
			if (found)
			{
				int vx = Info->robots[v]->x;
				int vy = Info->robots[v]->y;
				int dx = vx - x;
				int dy = vy - y;
				DoAction(Step, ACT_ATTACK, dx, dy);
			}
			break;
		}
	case 5:
		{
			DoAction(Step, ACT_TECH, 0.4*L, 0.3*L, 0.3*L);
			if (L < Info->field->Lmax)
			{
				bool found = false;
				int v;
				double A = Info->robots[n]->A * Info->robots[n]->E / Info->field->Emax;
				double attackdist = Info->field->Rmax*Info->robots[n]->V/Info->field->Lmax*Info->robots[n]->E/Info->field->Emax;
				for (int i = 0; i < Info->field->rivals; i++)
				{
					if (Info->robots[i]->alive && Info->robots[i]->playerid != id)
					{
						int vx = Info->robots[i]->x;
						int vy = Info->robots[i]->y;
						double robodist = sqrt(pow(vx-x,2) + pow(vy-y,2));
						if (robodist < attackdist)
						{
							double vP = Info->robots[i]->P * Info->robots[i]->E / Info->field->Emax;
							if (vP < A)
							{
								v = i;
								found = true;
								break;
							}
						}
					}
				}
				if (found)
				{
					int vx = Info->robots[v]->x;
					int vy = Info->robots[v]->y;
					int dx = vx - x;
					int dy = vy - y;
					DoAction(Step, ACT_ATTACK, dx, dy);
				}
			}
			else
			{
				if (E > 0.4 * Info->field->Emax)
				{
					bool found = false;
					double dist = Info->field->fieldWidth;
					int v;
					for (int i = 0; i < Info->field->rivals; i++)
					{
						if (Info->robots[i]->alive && Info->robots[i]->playerid != id)
						{
							double A = Info->robots[n]->A * Info->robots[n]->E / Info->field->Emax;
							double P = Info->robots[i]->P * Info->robots[i]->E / Info->field->Emax;
							if (A>=P)
							{
								int vx = Info->robots[i]->x;
								int vy = Info->robots[i]->y;
								double tmpdist = sqrt(pow(vx-x,2) + pow(vy-y,2));
								if (tmpdist < dist)
								{
									dist = tmpdist;
									v = i;
									found = true;
								}
							}
						}
					}
					if (found)
					{
						int vx = Info->robots[v]->x;
						int vy = Info->robots[v]->y;
						double attackdist = Info->field->Rmax*Info->robots[n]->V/Info->field->Lmax*Info->robots[n]->E/Info->field->Emax;
						double step = Info->robots[n]->V*Info->field->Vmax/Info->field->Lmax*Info->robots[n]->E/Info->field->Emax;
						if (dist > attackdist)
						{
							int dx, dy;
							if (dist > step)
							{
								dx = (vx - x)*step/dist;
								dy = (vy - y)*step/dist;
							}
							else
							{
								dx = vx - x;
								dy = vy - y;
								if (vx > x)
									dx-=1;
								else if (vx < x)
									dx+=1;
								else if (vy > y)
									dy-=1;
								else dy+=1;
							}
							DoAction(Step, ACT_MOVE, dx, dy);
						}
						else
						{
							double A = Info->robots[n]->A * Info->robots[n]->E / Info->field->Emax;
							double P = Info->robots[v]->P * Info->robots[v]->E / Info->field->Emax;
							int dx, dy;
							dx = Info->robots[v]->x - Info->robots[n]->x;
							dy = Info->robots[v]->y - Info->robots[n]->y;
							if (A>=P)
								DoAction(Step, ACT_ATTACK, dx, dy);
						}
					}
					else
					{
						DoAction(Step, ACT_TECH, 0.6*L, 0, 0.4*L);
						ofstream fStep("../Robots/robotbase.21/Yuri.txt");
						fStep << 2;
						fStep.close();
					}
				}
				else
				{
					DoAction(Step, ACT_TECH, 0.6*L, 0, 0.4*L);
					ofstream fStep("../Robots/robotbase.21/Yuri.txt");
					fStep << 2;
					fStep.close();
				}
			}
			break;
		}
	}
	return;
}


