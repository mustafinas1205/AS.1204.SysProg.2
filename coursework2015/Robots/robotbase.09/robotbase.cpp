// robotbase.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include "robotbase.h"

using namespace std;

extern "C" 
	void DoStep(stepinfo *Info, step *Step)
{
	int myNumber, myX, myY, rivals,Ne,Nl;

	double  myL, myE, myV, myA, myP, 
		Lmax,Emax,Rmax,Vmax,fieldHeight,fieldWidth,RNDmin,RNDmax; 

	int x,y; 
	double L,E,A,P,V;       

	double length_of_step;
	CString Myname, name;

	myNumber = Info->yourNumber;
	Info->robots[myNumber]->E ;
	Info->robots[myNumber]->L ;
	myX = Info->robots[myNumber]->x;
	myY = Info->robots[myNumber]->y;
	myL = Info->robots[myNumber]->L;
	myE = Info->robots[myNumber]->E;
	myV = Info->robots[myNumber]->V;
	myA = Info->robots[myNumber]->A;
	myP = Info->robots[myNumber]->P;
	Myname = Info->robots[myNumber]->name;

	rivals = Info->field->rivals;
	Emax = Info->field->Emax;
	Lmax = Info->field->Lmax;
	Vmax = Info->field->Vmax;
	Rmax = Info->field->Rmax;
	Nl = Info->field->Nl;
	Ne = Info->field->Ne;
	fieldHeight = Info->field->fieldHeight;
	fieldWidth = Info->field->fieldWidth;
	RNDmin = Info->field->rndmin;
	RNDmax = Info->field->rndmax;
	double expect_value=(RNDmin+RNDmax)/2;

	length_of_step = (Rmax*myV*myE) / (Emax*Lmax);

	int number_of_nearest=-1;
	double local_dist;
	double min_dist = fieldHeight + fieldWidth;
	for (int i = 0; i < rivals; i++)
	{
		name = Info->robots[i]->name;
		x = Info->robots[i]->x;
		y = Info->robots[i]->y;
		if (name != Myname)
		{
			local_dist = sqrt(pow(x-myX, 2)+pow(y-myY,2));
			if (local_dist < min_dist) { min_dist = local_dist; number_of_nearest = i;}
		}
	}

	int number_of_nearest_e=-1;
	min_dist=fieldHeight+fieldWidth;
	for (int i = 0; i < Ne+Nl; i++)
	{
		int type=Info->objects[i]->type;
		if ((type == OBJ_CHARGER)||(type == -2))
		{
			double x=Info->objects[i]->x;
			double y=Info->objects[i]->y;
			double dist = sqrt((x-myX)*(x-myX)+(y-myY)*(y-myY));
			if (dist < min_dist) { min_dist = dist; number_of_nearest_e = i; }
		}
	}

	int number_of_nearest_l=-1;
	min_dist=fieldHeight+fieldWidth;
	for (int i = 0; i < Ne+Nl; i++)
	{
		int type=Info->objects[i]->type;
		if ((type == OBJ_TECH)||(type == -3))
		{
			double x=Info->objects[i]->x;
			double y=Info->objects[i]->y;
			double dist = sqrt((x-myX)*(x-myX)+(y-myY)*(y-myY));
			if (dist < min_dist) { min_dist = dist; number_of_nearest_l = i; }
		}
	}

	if (myE<0.85*Emax)
	{
		double new_x=Info->objects[number_of_nearest_e]->x;
		double new_y=Info->objects[number_of_nearest_e]->y;
		double dist = sqrt((new_x-myX)*(new_x-myX)+(new_y-myY)*(new_y-myY));
		double local_V = (dist*Lmax*Emax)/(myE*Vmax);
		DoAction(Step,ACT_TECH,myL-local_V,0,local_V);
		DoAction(Step,ACT_MOVE,new_x-myX,new_y-myY);
	}

	else {
		if (myE<0.65*Emax)
		{
			DoAction(Step,ACT_TECH,0,0.6*myL,0.4*myL);
			int min = 1, max = fieldWidth/rivals;
			int rnd	 = rand();
			int dx = min + rnd%(max-min);
			rnd	 = rand();
			int dy = min + rnd%(max-min);
			DoAction(Step,ACT_MOVE,dx,dy);
		} else
		{
if (length_of_step>min_dist)
		{
			int his_P=Info->robots[number_of_nearest]->P;
			if (myA*expect_value>his_P*(1-expect_value))
			{
				double new_x=Info->robots[number_of_nearest]->x;
				double new_y=Info->robots[number_of_nearest]->y;
				double dx = new_x-myX;
				double dy = new_y-myY;
				if ((myX!=new_x)&&(myY!=new_y))
				{
					DoAction(Step,ACT_TECH,0.8*myL, 0, 0.2*myL);
					DoAction(Step,ACT_ATTACK,dx,dy);
				}					
			}	else {
				DoAction(Step,ACT_TECH,0,0.6*myL,0.4*myL);
				int min = 1, max = fieldWidth/rivals;
				int rnd	 = rand();
				int dx = min + rnd%(max-min);
				rnd	 = rand();
				int dy = min + rnd%(max-min);
				DoAction(Step,ACT_MOVE,dx,dy);
			}
		}
		else { 
			int his_P=Info->robots[number_of_nearest]->P;
			if (myA*expect_value>his_P*(1-expect_value))
			{
				double new_x=Info->robots[number_of_nearest]->x;
				double new_y=Info->robots[number_of_nearest]->y;
				double dist = sqrt((new_x-myX)*(new_x-myX)+(new_y-myY)*(new_y-myY));

				double AM=dist-length_of_step;
				double MB=length_of_step;
				double koef=AM/MB;
				double Xm=(myX+koef*new_x)/(koef+1);
				double Ym=(myY+koef*new_y)/(koef+1);
				if ((myX!=new_x)&&(myY!=new_y))
				{
					DoAction(Step,ACT_MOVE,Xm-myX,Ym-myY);
					DoAction(Step,ACT_ATTACK,new_x-myX,new_y-myY);
				} 
				else {
					DoAction(Step,ACT_TECH,0,0.6*myL,0.4*myL);
					int min = 1, max = fieldWidth/rivals;
					int rnd	 = rand();
					if (max == min) max = min + 1;
					int dx = min + rnd%(max-min);
					rnd	 = rand();
					int dy = min + rnd%(max-min);
					DoAction(Step,ACT_MOVE,dx,dy);
				}
			}
		}
		}
		
	}
	return;
}


