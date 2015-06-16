// robotbase.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include "robotbase.h"
#include "cmath"

using namespace std;


extern "C"
void DoStep(stepinfo *Info, step *Step)
{
	int p = 0;
	int q = 0;
	int a = 0;
	int j = 0;
	int h = 0;
	int NeX1, NeY1, NlX1, NlY1, xk1, yk1, k1;
	double minDistNe = Info->field->fieldHeight * 2;
	double minDistNl = Info->field->fieldHeight * 2;
	double minDistR = Info->field->fieldHeight * 2;
	int im = Info->yourNumber;
	int id = Info->robots[im]->playerid;
	int meX = Info->robots[im]->x;
	int meY = Info->robots[im]->y;

	for (int k = 0; k < Info->field->rivals; k++)
	{
		if (id == Info->robots[k]->playerid)
			j = j + 1;//считаем количество собственных роботов на поле
	}

	for (int l = 0; l < (Info->field->rivals-j+1); l++)
	{
		if (Info->robots[l]->alive == false)
			h = h + 1;//считаем количество умерших роботов
	}

	if (!(h == Info->field->rivals - j) && (Info->stepnum<ceil(0.93*Info->field->N))) 
	{
	
	for (int k = 0; k < Info->field->rivals; k++)
	{
		if (!(k == im) && (Info->robots[k]->E == 0))
		{
			int xk = Info->robots[k]->x;
			int yk = Info->robots[k]->y;
			double dm = sqrt((xk - meX)*(xk - meX) + (yk - meY)*(yk - meY));
			double nowmove = (Info->field->Vmax * Info->robots[im]->V * Info->robots[im]->E) / (Info->field->Lmax * Info->field->Emax);
			if (nowmove >= dm)
			{
				q = 1;
				DoAction(Step, ACT_MOVE, xk - meX, yk - meY);
				DoAction(Step, ACT_TECH, 0, (Info->robots[im]->L) * 2 / 3, (Info->robots[im]->L) / 3);
			}
		}
	}
	if ((Info->robots[im]->E <= ((Info->field->Emax) * 0.5)) && ((Info->field->Ne) > 0) && (q == 0))
	{
		for (int k = 0; k < (Info->field->Ne + Info->field->Nl); k++)
		{
			if (Info->objects[k]->type == OBJ_CHARGER)
			{
				int NeX = Info->objects[k]->x;
				int NeY = Info->objects[k]->y;
				double DistNe = sqrt((NeX - meX)*(NeX - meX) + (NeY - meY)*(NeY - meY));
				if (DistNe < minDistNe)
				{
					minDistNe = DistNe;
					NeX1 = NeX;
					NeY1 = NeY;
				}
			}
		}
		if (((Info->robots[im]->L) - (Info->field->Vmax)) >= 0)
		{
			DoAction(Step, ACT_TECH, 0, ((Info->robots[im]->L) - (Info->field->Vmax)), Info->field->Vmax);
		}

		else//выполняем поиск ближайшей кормушки с ТО и едим оттуда
		{
			if (((Info->robots[im]->L) - (Info->field->Vmax)) < 0 && ((Info->field->Nl) > 0))
			{
				p = 1;
				for (int k = 0; k < (Info->field->Ne + Info->field->Nl); k++)
				{
					if (Info->objects[k]->type == OBJ_TECH)
					{
						int NlX = Info->objects[k]->x;
						int NlY = Info->objects[k]->y;
						double DistNl = sqrt((NlX - meX)*(NlX - meX) + (NlY - meY)*(NlY - meY));
						if (DistNl < minDistNl)
						{
							minDistNl = DistNl;
							NlX1 = NlX;
							NlY1 = NlY;
						}
					}
				}
				double maxmove = (Info->field->Vmax * Info->robots[im]->V * Info->robots[im]->E) / (Info->field->Lmax * Info->field->Emax);
				if (minDistNl <= maxmove)
					DoAction(Step, ACT_MOVE, NlX1 - meX, NlY1 - meY);
				else
					DoAction(Step, ACT_MOVE, ceil(abs(NlX1 - meX) / (minDistNl / maxmove)), ceil((abs(NlY1 - meY) / (minDistNl / maxmove))));
			}
		}
		double maxmove = (Info->field->Vmax * Info->robots[im]->V * Info->robots[im]->E) / (Info->field->Lmax * Info->field->Emax);
		if (minDistNe <= maxmove && p == 0)
		{
			DoAction(Step, ACT_MOVE, NeX1 - meX, NeY1 - meY);
		}
		else if (p == 0)
		{
			DoAction(Step, ACT_MOVE, ceil(abs(NeX1 - meX) / (minDistNe / maxmove)), ceil(abs(NeY1 - meY) / (minDistNe / maxmove)));
		}
	}
	//кормушки с ТО
	else if ((Info->robots[im]->L < ((Info->field->Lmax) * 0.5)) && ((Info->field->Nl) > 0) && (p == 0) && (q == 0))
	{
		for (int k = 0; k < (Info->field->Ne + Info->field->Nl); k++)
		{
			if (Info->objects[k]->type == OBJ_TECH)
			{
				int NlX = Info->objects[k]->x;
				int NlY = Info->objects[k]->y;
				double DistNl = sqrt((NlX - meX)*(NlX - meX) + (NlY - meY)*(NlY - meY));
				if (DistNl < minDistNl)
				{
					minDistNl = DistNl;
					NlX1 = NlX;
					NlY1 = NlY;
				}
			}
		}
		if (((Info->robots[im]->L) - (Info->field->Vmax)) >= 0)
			DoAction(Step, ACT_TECH, 0, ((Info->robots[im]->L) - (Info->field->Vmax)), Info->field->Vmax);
		else
			DoAction(Step, ACT_TECH, 0, 0, Info->robots[im]->L);

		double maxmove = (Info->field->Vmax * Info->robots[im]->V * Info->robots[im]->E) / (Info->field->Lmax * Info->field->Emax);

		if (minDistNl <= maxmove)
			DoAction(Step, ACT_MOVE, NlX1 - meX, NlY1 - meY);
		else
			DoAction(Step, ACT_MOVE, ceil(abs(NlX1 - meX) / (minDistNl / maxmove)), ceil(abs(NlY1 - meY) / (minDistNl / maxmove)));
	}
	else if (q == 0)//начинаем работу с атакой
	{
		for (int k = 0; k < Info->field->rivals; k++)
		{
			if ((!(k == im)) && (Info->robots[k]->alive == true) && (!(Info->robots[k]->playerid==id)))
			{
				int xk = Info->robots[k]->x;
				int yk = Info->robots[k]->y;
				float dm = sqrt((xk - meX)*(xk - meX) + (yk - meY)*(yk - meY));
				if (dm < minDistR)
				{
					minDistR = dm;
					k1 = k;
					xk1 = xk;
					yk1 = yk;
				}
			}
		}
		
		if (Info->robots[im]->L >= Info->field->Vmax)
		{
			double maxmove = (Info->field->Vmax * Info->robots[im]->V * Info->robots[im]->E) / (Info->field->Lmax * Info->field->Emax);


			if (minDistR <= maxmove && minDistR > 1)
			{
				a = 1;
				DoAction(Step, ACT_MOVE, xk1 - meX, yk1 - meY - 1);
				if ((Info->robots[k1]->P) <= Info->robots[im]->L && (!(yk1 == meY)))
				{
					if (Info->robots[im]->L - Info->robots[k1]->P <= Info->field->Vmax)
						DoAction(Step, ACT_TECH, (Info->robots[k1]->P), 0, Info->robots[im]->L - Info->robots[k1]->P);
					else
						DoAction(Step, ACT_TECH, Info->robots[im]->L - Info->field->Vmax, 0, Info->field->Vmax);
					DoAction(Step, ACT_ATTACK, xk1 - meX, yk1 - meY);
				}
			}
			else if (minDistR > maxmove)
			{
				DoAction(Step, ACT_MOVE, ceil(abs(xk1 - meX) / (minDistR / maxmove)), ceil(abs(yk1 - meY) / (minDistR / maxmove)));
				DoAction(Step, ACT_TECH, 0, Info->robots[im]->L - Info->field->Vmax, Info->field->Vmax);
			}
			else if (minDistR <= 1)
			{
				if ((Info->robots[k1]->P) <= (Info->robots[im]->L) && (!(yk1 == meY)))
				{
					if (Info->robots[im]->L - Info->robots[k1]->P <= Info->field->Vmax)
						DoAction(Step, ACT_TECH, (Info->robots[k1]->P), 0, Info->robots[im]->L - Info->robots[k1]->P);
					else
						DoAction(Step, ACT_TECH, Info->robots[im]->L - Info->field->Vmax, 0, Info->field->Vmax);
					DoAction(Step, ACT_ATTACK, xk1 - meX, yk1 - meY);
				}
			}
		}
		else//опять идем кушать на станцию ТО
		{
			if (((Info->robots[im]->L) - (Info->field->Vmax)) < 0 && ((Info->field->Nl) > 0))
			{
				p = 1;
				for (int k = 0; k < (Info->field->Ne + Info->field->Nl); k++)
				{
					if (Info->objects[k]->type == OBJ_TECH)
					{
						int NlX = Info->objects[k]->x;
						int NlY = Info->objects[k]->y;
						double DistNl = sqrt((NlX - meX)*(NlX - meX) + (NlY - meY)*(NlY - meY));
						if (DistNl < minDistNl)
						{
							minDistNl = DistNl;
							NlX1 = NlX;
							NlY1 = NlY;
						}
					}
				}

				double maxmove = (Info->field->Vmax * Info->robots[im]->V * Info->robots[im]->E) / (Info->field->Lmax * Info->field->Emax);

				if (minDistNl < maxmove)
				{
					DoAction(Step, ACT_MOVE, NlX1 - meX, NlY1 - meY);
				}
				else
				{
					DoAction(Step, ACT_MOVE, ceil(abs(NlX1 - meX) / (minDistNl / maxmove)), ceil(abs(NlY1 - meY) / (minDistNl / maxmove)));
				}
			}
		}
	}
	}

	else//прячемся в кормушку с энергией
	{
	
	for (int k = 0; k < (Info->field->Ne + Info->field->Nl); k++)
	{
		if (Info->objects[k]->type == OBJ_CHARGER)
		{
			int NeX = Info->objects[k]->x;
			int NeY = Info->objects[k]->y;
			double DistNe = sqrt((NeX - meX)*(NeX - meX) + (NeY - meY)*(NeY - meY));
			if (DistNe < minDistNe)
			{
				minDistNe = DistNe;
				NeX1 = NeX;
				NeY1 = NeY;
			}
		}
	}
	if (((Info->robots[im]->L) - (Info->field->Vmax)) >= 0)
	{
		DoAction(Step, ACT_TECH, 0, ((Info->robots[im]->L) - (Info->field->Vmax)), Info->field->Vmax);
	}

	else//выполняем поиск ближайшей кормушки с ТО и едим оттуда
	{
		if (((Info->robots[im]->L) - (Info->field->Vmax)) < 0 && ((Info->field->Nl) > 0))
		{
			p = 1;
			for (int k = 0; k < (Info->field->Ne + Info->field->Nl); k++)
			{
				if (Info->objects[k]->type == OBJ_TECH)
				{
					int NlX = Info->objects[k]->x;
					int NlY = Info->objects[k]->y;
					double DistNl = sqrt((NlX - meX)*(NlX - meX) + (NlY - meY)*(NlY - meY));
					if (DistNl < minDistNl)
					{
						minDistNl = DistNl;
						NlX1 = NlX;
						NlY1 = NlY;
					}
				}
			}
			double maxmove = (Info->field->Vmax * Info->robots[im]->V * Info->robots[im]->E) / (Info->field->Lmax * Info->field->Emax);
			if (minDistNl <= maxmove)
				DoAction(Step, ACT_MOVE, NlX1 - meX, NlY1 - meY);
			else
				DoAction(Step, ACT_MOVE, ceil(abs(NlX1 - meX) / (minDistNl / maxmove)), ceil((abs(NlY1 - meY) / (minDistNl / maxmove))));
		}
	}
	double maxmove = (Info->field->Vmax * Info->robots[im]->V * Info->robots[im]->E) / (Info->field->Lmax * Info->field->Emax);
	if (minDistNe <= maxmove && p == 0)
	{
		DoAction(Step, ACT_MOVE, NeX1 - meX, NeY1 - meY);
	}
	else if (p == 0)
	{
		DoAction(Step, ACT_MOVE, ceil(abs(NeX1 - meX) / (minDistNe / maxmove)), ceil(abs(NeY1 - meY) / (minDistNe / maxmove)));
	}
	}
	return;
}











