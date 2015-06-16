// robotbase.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include "robotbase.h"

using namespace std;

double dlina(int x1, int y1, int x2, int y2, int H, int W)
{
	double s1,s2,s3,s4;
	s1 = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
	s2 = sqrt(pow(abs(x1 - x2)-W,2) + (y1 - y2)*(y1 - y2));
	s3 = sqrt(pow(abs(y1 - y2) - H, 2) + (x1 - x2)*(x1 - x2));
	s4 = sqrt(pow(abs(y1 - y2) - H, 2) + pow(abs(x1 - x2) - W, 2));
	if (s1 > s2) s1 = s2;
	if (s3 > s4) s3 = s4;
	if (s1 > s3) s1 = s3;
	return s1;
}

int shag(int x, int Pi)
{
	int x1,x2;
	x1 = x + Pi;
	x2 = x - Pi;
	if (abs(x1) > abs(x2)) x1 = x2;
	if (abs(x) > abs(x1)) x = x1;
	return x;

}

struct point
{
	int x, y;
};

//роботы, которые могут убить
struct gangster
{
	int mass_number;//номер в структуре роботов
	double max_A;//максимальная атака
	double s;//расстояние до моего робота
	double max_E;// максимальная энергия, которую у меня могут отнять
};

//роботы, на которых я могу напасть
struct defender
{
	int mass_number;//номер в структуре роботов
	double min_P, max_P;//минимальная и максимальная защита
	bool loser;// если при любом раскладе убью
	bool rast;//в пределе расстояния атаки
	double s;
};

//100% убийство
struct loser
{
	int mass_number;//номер в структуре роботов
	double max_P;//минимальная и максимальная защита
	bool rast;//в пределе расстояния атаки
	double s;
};

point max_pyt(int x1, int y1, int x2, int y2, double p_s_lim, int H, int W)
{
	point t;

	if (dlina(x1, y1, x2, y2, H, W) <= p_s_lim)
	{
		t.x = x2;
		t.y = y2;
		return t;
	}

	int znak_x=0, znak_y=0;

	if (abs(x1 - x2) > W / 2)
	{
		if (x1 <= W / 2) x1 = x1 + W;
		else x2 = x2 + W;
	}

	if (abs(y1 - y2) > H / 2)
	{
		if (y1 <= H / 2) y1 = y1 + H;
		else y2 = y2 + H;
	}

	double dolya_x, dolya_y;

	if (abs(x1 - x2)) znak_x = (x2 - x1) / abs(x2 - x1);
	if (abs(y1 - y2)) znak_y = (y2 - y1) / abs(y2 - y1);


	t.x = x1;
	t.y = y1;

	if (!(znak_x))
	{
		t.y = t.y + znak_y* int(p_s_lim);
	}

	if (!(znak_y))
	{
		t.x = t.x + znak_x* int(p_s_lim);
	}
	
	if ((znak_x) && (znak_y))
	{
		double s;

		dolya_x = abs(x2 - x1*1.0)/(abs(x2 - x1) + abs(y2 - y1));
		dolya_y = abs(y2 - y1*1.0)/(abs(x2 - x1) + abs(y2 - y1));

		s = p_s_lim / (sqrt(dolya_x*dolya_x + dolya_y*dolya_y));

		dolya_x = dolya_x *s;
		dolya_y = dolya_y *s;

		t.x = t.x + znak_x * int(dolya_x);
		t.y = t.y + znak_y * int(dolya_y);

		while (dlina(x1,y1,t.x,t.y,H,W)>=p_s_lim)
		{
			if (t.x != x1)
			{
				t.x = t.x - znak_x;
			}
			else
			{
				t.y = t.y - znak_y;
			}
		}


	}

	if (t.x >= W) t.x = t.x - W;

	if (t.y >= H) t.y = t.y - H;

	return t;

};



extern "C" 
void DoStep(stepinfo *Info, step *Step)
{
	//AfxMessageBox("DLL");
	srand(time(0));

	vector <gangster> gangsters;
	vector <defender> defenders;
	vector <loser> losers;
	vector <int> friends04;

	double RNDmin = Info->field->rndmin;
	double RNDmax = Info->field->rndmax;

	bool vverx = false;

	int x_sr=0, y_sr=0;//место сбора моего собственного альянса

	//оптимальное место расположения на карте:
	int mesto_dlina = 10000000;
	int mesto_x, mesto_y;


	int n = Info->field->rivals;//Кол-во роботов на поле

	for (int i = 0; i < n; i++)
	{
		CString str = Info->robots[i]->name;
		if ((str[10] == '0') && (str[11] == '4') || (str[10] == '0') && (str[11] == '2') || (str[10] == '1') && (str[11] == '7'))
		{
			friends04.push_back(i);
			x_sr = x_sr + Info->robots[i]->x;
			y_sr = y_sr + Info->robots[i]->y;
		}
			
	}

	x_sr = x_sr / friends04.size();
	y_sr = y_sr / friends04.size();

	int myNumber = Info->yourNumber;
	int myX = Info->robots[myNumber]->x;
	int myY = Info->robots[myNumber]->y;
	int L = Info->robots[myNumber]->L;
	int A = Info->robots[myNumber]->A;
	int P = Info->robots[myNumber]->P;
	int V = Info->robots[myNumber]->V;
	int E = Info->robots[myNumber]->E;

	//параметры поля:
	int Lmax = Info->field->Lmax;
	int Emax = Info->field->Emax;
	int Vmax = Info->field->Vmax;
	int Rmax = Info->field->Rmax;
	int W = Info->field->fieldWidth;
	int H = Info->field->fieldHeight;



	double p_s_lim = Vmax*V*E / (Lmax*Emax); // передел перемещения
	double a_s_lim = Rmax*V*E / (Lmax*Emax); // предел расстояния атаки
	double min_A = RNDmin * A * E / Emax; // минимальная возможная атака моего робота
	double max_A = RNDmax * A * E / Emax; // максимальная возможная атака моего робота
	double min_P = (1 - RNDmax) * P * E / Emax; // минимальная возможная оборона моего робота	
	double max_P = (1 - RNDmin) * P * E / Emax; // максимальная возможная оборона моего робота	
	
	//Поиск ближайшей кормушки:
	int Ne = Info->field->Ne;
	double se = dlina(Info->objects[0]->x, Info->objects[0]->y, myX, myY, H, W);
	double xe = Info->objects[0]->x;
	double ye = Info->objects[0]->y;
	int ne = 0;
	for (int i = 1; i < Ne; i++)
	{
		if (dlina(Info->objects[i]->x, Info->objects[i]->y, myX, myY, H, W) < se)
		{
			se = dlina(Info->objects[i]->x, Info->objects[i]->y, myX, myY, H, W);
			ne = i;
			xe = Info->objects[i]->x;
			ye = Info->objects[i]->y;
		}
	}


	//Поиск ближайшей мастерской:
	int Nl = Info->field->Nl;
	double sl = dlina(Info->objects[0]->x, Info->objects[0]->y, myX, myY, H, W);
	double xl = Info->objects[Ne]->x;
	double yl = Info->objects[Ne]->y;
	int nl = Ne;
	for (int i = Ne+1; i < Ne + Nl; i++)
	{
		if (dlina(Info->objects[i]->x, Info->objects[i]->y, myX, myY, H, W) < sl)
		{
			sl = dlina(Info->objects[i]->x, Info->objects[i]->y, myX, myY, H, W);
			nl = i;
			xl = Info->objects[i]->x;
			yl = Info->objects[i]->y;
		}
	}



	//поиск оптимального места на поле
	for (int i1 = 0; i1 < Ne; i1++)
	{
		for (int i2 = Ne; i2 < Ne + Nl; i2++)
		{
			double dl = dlina(Info->objects[i1]->x, Info->objects[i1]->y, Info->objects[i2]->x, Info->objects[i2]->y, H, W);
			if (dl < mesto_dlina)
			{
				mesto_dlina = dl;
				
				if (abs(Info->objects[i1]->x - Info->objects[i2]->x)>H / 2)
				{
					mesto_x = (Info->objects[i1]->x + Info->objects[i2]->x + H) / 2;
					if (mesto_x >= H) mesto_x = mesto_x - H;
				}
				else
				{
					mesto_x = (Info->objects[i1]->x + Info->objects[i2]->x) / 2;
				}

				if (abs(Info->objects[i1]->y - Info->objects[i2]->y)>W / 2)
				{
					mesto_y = (Info->objects[i1]->y + Info->objects[i2]->y + W) / 2;
					if (mesto_y >= W) mesto_y = mesto_y - W;
				}
				else
				{
					mesto_y = (Info->objects[i1]->y + Info->objects[i2]->y) / 2;
				}


			}
		}
	}


	//поиск представляющих опасность роботов:
	for (int i = 0; i < n; i++)
	{
		if (i == myNumber) continue;

		bool proverka = false;

		for (int i1 = 0; i1 < friends04.size(); i1++)
		{
			if (i == friends04[i1])
			{
				proverka = true;
				break;
			}
		}

		if (proverka) continue;

		double max_drA = RNDmax * (Info->robots[i]->A) * (Info->robots[i]->E) / (Info->field->Emax);

		double dr_dlina = dlina(myX, myY, Info->robots[i]->x, Info->robots[i]->y, H, W);
		double dr_a_s_lim = Rmax*(Info->robots[i]->V)*(Info->robots[i]->E) / (Lmax*Emax);

		if ((max_drA >= max_P) && (dr_a_s_lim >= dr_dlina))
		{
			gangster gn;
			gn.mass_number = i;
			gn.max_A = dr_a_s_lim;
			gn.s = dr_dlina;
			gn.max_E = abs(dr_a_s_lim - min_P)* Emax / Lmax;
			gangsters.push_back(gn);
		}
	}


	//поиск роботов, на которых можно напасть
	for (int i = 0; i < n; i++)
	{
		if (!(Info->robots[i]->alive)) 
			continue;

		if (i == myNumber) continue;

		bool proverka = false;

		for (int i1 = 0; i1 < friends04.size(); i1++)
		{
			if (i == friends04[i1])
			{
				proverka = true;
				break;
			}
		}

		if (proverka) continue;

		for (int i1 = 0; i1 < friends04.size(); i1++)
		{
			if (i1 == i) continue;

			if ((Info->robots[friends04[i1]]->x == Info->robots[i]->x) && (Info->robots[friends04[i1]]->y == Info->robots[i]->y))
			{
				vverx = true;
				proverka = true;
				break;
			}
		}

		if (proverka) continue;

		double min_drP = (1 - RNDmax) * (Info->robots[i]->P) * (Info->robots[i]->E) / Emax;
		double max_drP = (1 - RNDmin) * (Info->robots[i]->P) * (Info->robots[i]->E) / Emax;
		double dr_dlina = dlina(myX, myY, Info->robots[i]->x, Info->robots[i]->y, H, W);

		if ((max_A > max_drP) && (a_s_lim >= dr_dlina))
		{
			defender dfn;
			dfn.mass_number = i;
			dfn.min_P = min_drP;
			dfn.max_P = max_drP;

			if (max_drP <= min_A)
			{
				dfn.loser = true;

				//100% смертники
				if (abs(min_A - max_drP)*Emax / Lmax > Info->robots[i]->E)
				{
					loser los;
					los.mass_number = i;
					los.max_P = max_drP;
					losers.push_back(los);
				}
			}
			else dfn.loser = false;

			defenders.push_back(dfn);
		}

	}


	int kol;

	//сортировка убийц
	kol = gangsters.size() - 1;
	for (int i1 = 0; i1 < kol; i1++)
	{
		for (int i2 = 0; i2 < kol; i2++)
		{
			gangster gn;
			if (gangsters[i2].max_A < gangsters[i2 + 1].max_A)
			{
				gn = gangsters[i2];
				gangsters[i2] = gangsters[i2 + 1];
				gangsters[i2 + 1] = gn;
			}
		}
	}

	//сортировка защищающихся
	kol = defenders.size() - 1;
	for (int i1 = 0; i1 < kol; i1++)
	for (int i2 = 0; i2 < kol; i2++)
	{
		defender def;
		if (defenders[i2].max_P>defenders[i2 + 1].max_P)
		{
			def = defenders[i2];
			defenders[i2] = defenders[i2 + 1];
			defenders[i2 + 1] = def;
		}
	}

	//сортировка предполагаемых мертвецов:
	kol = losers.size() - 1;
	for (int i1 = 0; i1 < kol; i1++)
	for (int i2 = 0; i2 < kol; i2++)
	{
		loser los;
		if (losers[i2].max_P>losers[i2 + 1].max_P)
		{
			los = losers[i2];
			losers[i2] = losers[i2 + 1];
			losers[i2 + 1] = los;
		}
	}

	if ((gangsters.size()) && (V != 0))
	{

		DoAction(Step, ACT_TECH, 0.3*L, 0.3*L, 0.4*L);

		DoAction(Step, ACT_MOVE, int(p_s_lim), 0);

		return;
	}

	

	

	//если требуется подзарядка
	if (E < 0.9*Emax)
	{
		if (se <= p_s_lim)
		{
			if (V == 0)
			{
				DoAction(Step, ACT_TECH, 0.2*L, 0.4*L, 0.4*L);
				return;
			}

			DoAction(Step, ACT_MOVE, shag(xe - myX, W), shag(ye - myY, H));
			if (P != L)
				DoAction(Step, ACT_TECH, 0, 0.6*L, 0.4*L);
			return;
		}
		else
		{
			if (V == 0)
			{
				DoAction(Step, ACT_TECH, 0.2*L, 0.4*L, 0.4*L);
				return;
			}

			if (L < 0.70*Lmax)
			{
				if (sl < p_s_lim)
				{
					if (sl)
						DoAction(Step, ACT_MOVE, shag(xl - myX, W), shag(yl - myY, H));
					if ((P != 0.6*L) || (V != 0.4*L))
						DoAction(Step, ACT_TECH, 0, 0.6*L, 0.4*L);
					return;
				}
			}

			point t = max_pyt(myX, myY, xe, ye, p_s_lim, H, W);

			DoAction(Step, ACT_MOVE, shag(t.x - myX, W), shag(t.y - myY, H));

			if (L >= 0.4*Lmax)
			{
				if ((P != 0.6*L) || (V != 0.4*L))
					DoAction(Step, ACT_TECH, 0, 0.6*L, 0.4*L);
			}
			else
			{
				DoAction(Step, ACT_TECH, 0, 0, L);
			}


			return;
		}


	}

	//если требуется на пункт ТО
	if ((L < 0.90*Lmax) && (V != 0))
	{
		if (sl < p_s_lim)
		{
			if (sl)
				DoAction(Step, ACT_MOVE, shag(xl - myX, W), shag(yl - myY, H));

			if (P != L)
				DoAction(Step, ACT_TECH, 0.6*L, 0.4*L, 0);

			return;
		}

		else
		{
			point t = max_pyt(myX, myY, xl, yl, p_s_lim, H, W);

			DoAction(Step, ACT_MOVE, shag(t.x - myX, W), shag(t.y - myY, H));

			if (L >= 0.4*Lmax)
			{
				if ((P != 0.6*L) || (V != 0.4*L))
					DoAction(Step, ACT_TECH, 0, 0.6*L, 0.4*L);
			}
			else
			{
				DoAction(Step, ACT_TECH, 0, 0, L);
			}

			return;
		}

	}



	//после 990 хода бежать на кормушку:
	if (Info->stepnum > 990)
	{
		if (se <= p_s_lim)
		{
			if (se != 0)
				DoAction(Step, ACT_MOVE, shag(xe - myX, W), shag(ye - myY, H));

			if (L >= 0.4*Lmax)
			{
				if ((P != 0.6*L) || (V != 0.4*L))
					DoAction(Step, ACT_TECH, 0, 0.6*L, 0.4*L);
			}
			else
			{
				DoAction(Step, ACT_TECH, 0, 0, L);
			}

			return;
		}
		else
		{
			point t = max_pyt(myX, myY, xe, ye, p_s_lim, H, W);

			DoAction(Step, ACT_MOVE, shag(t.x - myX, W), shag(t.y - myY, H));

			if ((P != 0.6*L) || (V != 0.4*L))
				DoAction(Step, ACT_TECH, 0, 0.6*L, 0.4*L);

			return;
		}
	}

	if ((vverx) && (V != 0))
	{

		DoAction(Step, ACT_TECH, 0.3*L, 0.3*L, 0.4*L);

		DoAction(Step, ACT_MOVE, int(p_s_lim), 0);

		return;
	}


	/*//просчет альянса
	int alyans_kol = 0; //считаю, если меня вдруг окружили
	double alyans_E = 0; //посчитать предельный урон, который они могут мне нанести
	kol = gangsters.size();
	for (int i = 0; i < kol; i++)
	{
		if (gangsters[i].s <= 2)
		{
			alyans_kol++;
			alyans_E = alyans_E + gangsters[i].max_E;
		}
	}


	if ((E>0.965*Emax) && (se == 0))
	{
		if (L < 0.70 * Lmax)
		{
			if (V == 0)
			{
				DoAction(Step, ACT_TECH, 0, 0.6*L, 0.4*L);
			}
			else
			{
				if (sl <= p_s_lim)
				{
					DoAction(Step, ACT_MOVE, shag(xl - myX, W), shag(yl - myY, H));

					if ((P != 0.6*L) || (V != 0.4*L))
						DoAction(Step, ACT_TECH, 0, 0.6*L, 0.4*L);

					return;
				}

				else
				{
					point t = max_pyt(myX, myY, xl, yl, p_s_lim, H, W);

					DoAction(Step, ACT_MOVE, shag(t.x - myX, W), shag(t.y - myY, H));

					if ((P != 0.6*L) || (V != 0.4*L))
					DoAction(Step, ACT_TECH, 0, 0.6*L, 0.4*L);

					return;
				}
			}
		}
		else
		{
			if (alyans_kol > 0)
			{
				DoAction(Step, ACT_TECH, 0, L, 0);
				return;
			}


			if (alyans_kol == 0)
			{
				if (V == 0)
				{
					DoAction(Step, ACT_TECH, 0.45*L, 0.35*L, 0.2*L);
					return;
				}
			}
		}
	}
	*/
	if (losers.size() != 0)
	{
		int idid = losers[0].mass_number;
		int drx = Info->robots[idid]->x;
		int dry = Info->robots[idid]->y;

		DoAction(Step, ACT_ATTACK, shag(drx - myX, W), shag(dry - myY, H));
		DoAction(Step, ACT_MOVE, shag(drx - myX, W), shag(dry - myY, H));
		return;
	}

	if (defenders.size() != 0)
	{
		if (defenders[0].loser)
		{
			int idid = defenders[0].mass_number;
			int drx = Info->robots[idid]->x;
			int dry = Info->robots[idid]->y;

			DoAction(Step, ACT_TECH, 0.55*L, 0.15*L, 0.3*L);
			DoAction(Step, ACT_ATTACK, shag(drx - myX, W), shag(dry - myY, H));
			DoAction(Step, ACT_MOVE, shag(drx - myX, W), shag(dry - myY, H));
			return;
		}

		int idid = defenders[0].mass_number;
		int drx = Info->robots[idid]->x;
		int dry = Info->robots[idid]->y;

		DoAction(Step, ACT_TECH, 0.55*L, 0.15*L, 0.3*L);
		DoAction(Step, ACT_ATTACK, shag(drx - myX, W), shag(dry - myY, H));
		DoAction(Step, ACT_MOVE, shag(drx - myX, W), shag(dry - myY, H));
		return;
	}

	defenders.clear();
	losers.clear();

	
	//поиск роботов, на которых можно напасть не на расстоянии атаки
	for (int i = 0; i < n; i++)
	{
		if (!(Info->robots[i]->alive)) continue;

		if (i == myNumber) continue;

		bool proverka = false;

		for (int i1 = 0; i1 < friends04.size(); i1++)
		{
			if (i == friends04[i1])
			{
				proverka = true;
				break;
			}
		}

		if (proverka) continue;

		for (int i1 = 0; i1 < friends04.size(); i1++)
		{
			if ((Info->robots[friends04[i1]]->x == Info->robots[i]->x) && (Info->robots[friends04[i1]]->y == Info->robots[i]->y))
			{
				proverka = true;
				break;
			}
		}

		if (proverka) continue;

		double min_drP = (1 - RNDmax) * (Info->robots[i]->P) * (Info->robots[i]->E) / Emax;
		double max_drP = (1 - RNDmin) * (Info->robots[i]->P) * (Info->robots[i]->E) / Emax;
		double dr_dlina = dlina(myX, myY, Info->robots[i]->x, Info->robots[i]->y, H, W);

		if ((max_A > max_drP))
		{
			defender dfn;
			dfn.mass_number = i;
			dfn.min_P = min_drP;
			dfn.max_P = max_drP;
			dfn.s = dr_dlina;

			if (max_drP <= min_A)
			{
				dfn.loser = true;

				//100% смертники
				if (abs(min_A - max_drP)*Emax / Lmax > Info->robots[i]->E)
				{
					loser los;
					los.mass_number = i;
					los.max_P = max_drP;
					los.s = dr_dlina;
					losers.push_back(los);
				}
			}
			else dfn.loser = false;

			defenders.push_back(dfn);
		}

	}



	//сортировка защищающихся
	kol = defenders.size() - 1;
	for (int i1 = 0; i1 < kol; i1++)
	for (int i2 = 0; i2 < kol; i2++)
	{
		defender def;
		if (defenders[i2].s>defenders[i2 + 1].s)
		{
			def = defenders[i2];
			defenders[i2] = defenders[i2 + 1];
			defenders[i2 + 1] = def;
		}
	}

	//сортировка предполагаемых мертвецов:
	kol = losers.size() - 1;
	for (int i1 = 0; i1 < kol; i1++)
	for (int i2 = 0; i2 < kol; i2++)
	{
		loser los;
		if (losers[i2].s>losers[i2 + 1].s)
		{
			los = losers[i2];
			losers[i2] = losers[i2 + 1];
			losers[i2 + 1] = los;
		}
	}



	if (defenders.size() != 0)
	{
		if (defenders[0].loser)
		{
			int idid = defenders[0].mass_number;
			int drx = Info->robots[idid]->x;
			int dry = Info->robots[idid]->y;

			point t = max_pyt(myX, myY, drx, dry, p_s_lim, H, W);

			DoAction(Step, ACT_TECH, 0.45*L, 0.2*L, 0.35*L);
			DoAction(Step, ACT_MOVE, shag(t.x - myX, W), shag(t.y - myY, H));
			return;
		}

		int idid = defenders[0].mass_number;
		int drx = Info->robots[idid]->x;
		int dry = Info->robots[idid]->y;

		point t = max_pyt(myX, myY, drx, dry, p_s_lim, H, W);

		DoAction(Step, ACT_TECH, 0.45*L, 0.2*L, 0.35*L);
		DoAction(Step, ACT_MOVE, shag(t.x - myX, W), shag(t.y - myY, H));
		return;
	}

	if ((E <= 0.95*Emax) && (se <= p_s_lim) && (se!=0))
	{
		DoAction(Step, ACT_MOVE, shag(xe - myX, W), shag(ye - myY, H));
		DoAction(Step, ACT_TECH, 0.45*L, 0.35*L, 0.2*L);
		return;
	}

	if ((L <= 0.9*Lmax) && (sl <= p_s_lim) && (sl!=0))
	{
		DoAction(Step, ACT_MOVE, shag(xl - myX, W), shag(yl - myY, H));
		DoAction(Step, ACT_TECH, 0.45*L, 0.35*L, 0.2*L);
		return;
	}






	if ((dlina(myX, myY, mesto_x, mesto_y, H, W) >= 50) && (se != 0) && (sl != 0))
	{
		point t = max_pyt(myX, myY, mesto_x, mesto_y, p_s_lim, H, W);

		DoAction(Step, ACT_TECH, 0.5*L, 0.2*L, 0.3*L);

		DoAction(Step, ACT_MOVE, shag(t.x - myX, W), shag(t.y - myY, H));
		return;
	}
	
	if ((P != 0.6*L) || (V != 0.4*L))
	{
		DoAction(Step, ACT_TECH, 0, 0.6*L, 0.4*L);
	}

	DoAction(Step, ACT_MOVE, int(p_s_lim), 0);

	return;
}


