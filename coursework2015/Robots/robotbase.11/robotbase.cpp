// robotbase.cpp: ���������� ���������������� ������� ��� ���������� DLL.
//

#include "stdafx.h"
#include "robotbase.h"
#include <fstream>
#include <iostream>

using namespace std;

extern "C" 
void DoStep(stepinfo *Info, step *Step)
{
	srand(time(0));
	int myNumber = Info->yourNumber;//��� �����
	int myE = Info->robots[myNumber]->E;//��� �������� �������
	int myL = Info->robots[myNumber]->L;//��� ���� �������
	int myX = Info->robots[myNumber]->x;//��� ����������
	int myY = Info->robots[myNumber]->y;
	int myId = Info->robots[myNumber]->playerid;
	int n = Info->field->rivals;//���-�� ������� �� ����
	int *A = new int[n];//������ ����
	int *P = new int[n];//������ ������
	int *V = new int[n];//������ ��������
	int *E = new int[n];//������ �������
	int *x = new int[n];//������ ���������
	int *y = new int[n];//������
	double radattack = (Info->field->Rmax)*(Info->robots[myNumber]->V) / (Info->field->Lmax)*(Info->robots[myNumber]->E) / (Info->field->Emax);//������ �����
	int move = (int)((Info->field->Vmax)*(Info->robots[myNumber]->V) / (Info->field->Lmax)*(Info->robots[myNumber]->E) / (Info->field->Emax));//��������� ����
	int Ne = Info->field->Ne;//������ ������ ��
	int Nl = Info->field->Nl;


	///////////////////////////////////////////////////////////
	////1.������� � �������
	///////////////////////////////////////////////////////////
	if ((Info->stepnum) != 1)//���� ��� ����� ��������, ��������� ������
	{
		for (int i = 0; i < n; i++)//��������� c �����
		{
			char ch[69];
			sprintf(ch, "../Robots/robotbase.11/Robot%d", i);
			string fileName = ch;
			fileName += ".txt";
			ifstream file(fileName);
			file >> A[i] >> P[i] >> V[i] >> E[i] >> x[i] >> y[i];//��� ���������� � ������
			file.close();
		}
	}


	for (int i = 0; i < n; i++)//���������� � ����
	{

		char ch[69];
		sprintf(ch, "../Robots/robotbase.11/Robot%d", i);
		string fileName = ch;
		fileName += ".txt";
		ofstream file(fileName);
		file << Info->robots[i]->A << endl << Info->robots[i]->P << endl << Info->robots[i]->V << endl << Info->robots[i]->E << endl << Info->robots[i]->x << endl << Info->robots[i]->y;
		file.close();
	}


	/////////////////////////////////////////////////////////////
	////2.������� ���������� ������
	/////////////////////////////////////////////////////////////
	double realx = Info->field->fieldWidth;
	int realy = 0;

	ifstream file("../Robots/robotbase.11/test.txt");
	file >> realy;//���������� ������
	file.close();

	if (Info->robots[realy]->E < 0 || Info->stepnum == 1)
	{
		for (int i = 0; i<Info->field->rivals; i++)
		{
			int id = Info->robots[i]->playerid;
			if (id!=myId)
			{
				double x = sqrt(pow(Info->robots[i]->x - myX, 2) + pow(Info->robots[i]->y - myY, 2));
				if (x < realx)
				{
					realx = x;
					realy = i;
					ofstream file("../Robots/robotbase.11/test.txt");
					file << realy << endl;//��� ���������� � ������
					file.close();
				}
			}

		}
	}



	/////////////////////////////////////////////////////////////
	////3.��������� ��������(�������)
	////////////////////////////////////////////////////////////

	double realxe = Info->field->fieldWidth;
	int realye = 0;
	for (int i = 0; i < Ne + Nl; i++)
	{
		if (Info->objects[i]->type == OBJ_CHARGER)
		{
			double xe = pow(pow((Info->objects[i]->x - myX), 2) + pow((Info->objects[i]->y - myY), 2), 1.0 / 2);
			if (realxe > xe)
			{
				realxe = xe;
				realye = i;
			}
		}
	}
	/////////////////////////////////////////////////////////////
	////4.��������� ��������(���)
	////////////////////////////////////////////////////////////
	double realxl = Info->field->fieldWidth;
	int realyl = 0;
	for (int i = 0; i < Ne + Nl; i++)
	{
		if (Info->objects[i]->type == OBJ_TECH)
		{
			double xl = pow(pow((Info->objects[i]->x - myX), 2) + pow((Info->objects[i]->y - myY), 2), 1.0 / 2);
			if (realxl > xl)
			{
				realxl = xl;
				realyl = i;
			}
		}
	}

	/////////////////////////////////////////////////////////////
	////5.���-�� ����� �������
	////////////////////////////////////////////////////////////
	int nalive = 0;
	for (int i = 0; i < n; i++)
	{
		if (Info->robots[i]->alive == true)
		{
			nalive++;
		}
	}

	//////////////////////////////////////////////////////////////////
	if ((realxe > 0) && (realxl > 0))
	{
		if (myE <= 0.6*Info->field->Emax && myL <= 0.6*Info->field->Lmax)//���� ������� � ��� ������ ������������, �� ����� � ��������
		{//������� ����� �� ��������
			//��������� ���������� ������������ ����
			int dx = (Info->objects[realye]->x) - myX;
			int dy = (Info->objects[realye]->y) - myY;
			double  h = realxe / move;
			if (h < 1)
			{
				int dx = (Info->objects[realye]->x) - myX;
				int dy = (Info->objects[realye]->y) - myY;
				DoAction(Step, ACT_MOVE, dx, dy);

			}
			else//��������� ������������� ����
			{
				int newx = ((Info->objects[realye]->x) - myX) / h;
				int newy = ((Info->objects[realye]->y) - myY) / h;
				DoAction(Step, ACT_MOVE, newx, newy);
			}
		}

		///////////////////////////////////////////////////////////////////

		if (myE >= 0.6*Info->field->Emax && myL <= 0.6*Info->field->Lmax)//���� ��� ������ ������������, �� ����� � ��������
		{
			int dx = (Info->objects[realyl]->x) - myX;
			int dy = (Info->objects[realyl]->y) - myY;
			double  h = realxl / move;
			if (h < 1)
			{
				int dx = (Info->objects[realyl]->x) - myX;
				int dy = (Info->objects[realyl]->y) - myY;
				DoAction(Step, ACT_MOVE, dx, dy);
			}
			else//��������� ������������� ����
			{
				int newx = ((Info->objects[realyl]->x) - myX) / h;
				int newy = ((Info->objects[realyl]->y) - myY) / h;
				DoAction(Step, ACT_MOVE, newx, newy);
			}
		}

		///////////////////////////////////////////////////////////////

		if (myE <= 0.6*Info->field->Emax && myL >= 0.6*Info->field->Lmax)//���� ������� ������ ������������, �� ����� � ��������
		{//��������� ���������� ������������ ����
			double  h = realxe / move;
			if (h < 1)
			{
				int dx = (Info->objects[realye]->x) - myX;
				int dy = (Info->objects[realye]->y) - myY;
				DoAction(Step, ACT_MOVE, dx, dy);

			}
			else//��������� ������������� ����
			{
				int newx = ((Info->objects[realye]->x) - myX) / h;
				int newy = ((Info->objects[realye]->y) - myY) / h;
				DoAction(Step, ACT_MOVE, newx, newy);
			}
		}

		////////////////////////////////////////////////////////////////

		if (myE > 0.6*Info->field->Emax && myL > 0.6*Info->field->Lmax)
		{
			if (nalive >= 3)//////////////////////ATTACK
			{
				int newA = (int)(0.35*myL);
				int newP = (int)(0.15* myL);
				int newV = (int)(0.5*myL);
				DoAction(Step, ACT_TECH, newA, newP, newV);
				int myP = Info->robots[myNumber]->P;
				int myA = Info->robots[myNumber]->A;
				int enP = Info->robots[myNumber]->P;
				int enA = Info->robots[myNumber]->A;
				if (enP < (0.7*Info->field->Lmax) && enA <= myA + 0.05*(Info->field->Lmax))
				{
					double  h = realx / move;
					if (h < 1)
					{
						int dx = (Info->robots[realy]->x) - myX;
						int dy = (Info->robots[realy]->y) - myY;
						DoAction(Step, ACT_MOVE, dx - 1, dy);
						if ((Info->robots[realy]->x) != myX || (Info->robots[realy]->y) != myY)
						{
							DoAction(Step, ACT_ATTACK, dx, dy);
						}

					}
					else//��������� ������������� ����
					{
						int newx = ((Info->robots[realy]->x) - myX) / h;
						int newy = ((Info->robots[realy]->y) - myY) / h;
						DoAction(Step, ACT_MOVE, newx, newy);
					}
				}
				if (enP > (0.7*Info->field->Lmax))
				{
					if (myE < 700)
					{
						int dx = (Info->objects[realye]->x) - myX;
						int dy = (Info->objects[realye]->y) - myY;
						double  h = realxe / move;
						if (h < 1)
						{
							int dx = (Info->objects[realye]->x) - myX;
							int dy = (Info->objects[realye]->y) - myY;
							DoAction(Step, ACT_MOVE, dx, dy);

						}
						else//��������� ������������� ����
						{
							int newx = ((Info->objects[realye]->x) - myX) / h;
							int newy = ((Info->objects[realye]->y) - myY) / h;
							DoAction(Step, ACT_MOVE, newx, newy);
						}
					}
					else
					{
						int newA = (int)(0.6*myL);
						int newP = (int)(0.1* myL);
						int newV = (int)(0.3*myL);
						DoAction(Step, ACT_TECH, newA, newP, newV);
						double  h = realx / move;
						if (h < 1)
						{
							int dx = (Info->robots[realy]->x) - myX;
							int dy = (Info->robots[realy]->y) - myY;
							DoAction(Step, ACT_MOVE, dx - 1, dy);
							if ((Info->robots[realy]->x) != myX || (Info->robots[realy]->y) != myY)
							{
								DoAction(Step, ACT_ATTACK, dx, dy);
							}

						}
						else//��������� ������������� ����
						{
							int newx = ((Info->robots[realy]->x) - myX) / h;
							int newy = ((Info->robots[realy]->y) - myY) / h;
							DoAction(Step, ACT_MOVE, newx, newy);
						}
					}

				}
				if (enP < (0.7*Info->field->Lmax) && enA > myA + 0.05*(Info->field->Lmax))
				{
					int newA = (int)(0.3*myL);
					int newP = (int)(0.2* myL);
					int newV = (int)(0.5*myL);
					DoAction(Step, ACT_TECH, newA, newP, newV);
					double  h = realx / move;
					if (h < 1)
					{
						int dx = (Info->robots[realy]->x) - myX;
						int dy = (Info->robots[realy]->y) - myY;
						DoAction(Step, ACT_MOVE, dx - 1, dy);
						if ((Info->robots[realy]->x) != myX || (Info->robots[realy]->y) != myY)
						{
							DoAction(Step, ACT_ATTACK, dx, dy);
						}

					}
					else//��������� ������������� ����
					{
						int newx = ((Info->robots[realy]->x) - myX) / h;
						int newy = ((Info->robots[realy]->y) - myY) / h;
						DoAction(Step, ACT_MOVE, newx, newy);
					}
				}
			}
			if (nalive < 3 || Info->stepnum == (Info->field->N - 0.2*Info->field->N))
			{
				double  h = realxe / move;
				if (h < 1)
				{
					int dx = (Info->objects[realye]->x) - myX;
					int dy = (Info->objects[realye]->y) - myY;
					DoAction(Step, ACT_MOVE, dx, dy);
					int newA = (int)(0 * myL);
					int newP = (int)(myL);
					int newV = (int)(0 * myL);
					DoAction(Step, ACT_TECH, newA, newP, newV);

				}
				else//��������� ������������� ����
				{
					int newx = ((Info->objects[realye]->x) - myX) / h;
					int newy = ((Info->objects[realye]->y) - myY) / h;
					DoAction(Step, ACT_MOVE, newx, newy);
					int newA = (int)(0 * myL);
					int newP = (int)(myL);
					int newV = (int)(0 * myL);
					DoAction(Step, ACT_TECH, newA, newP, newV);
				}

			}
		}
	}
	/////////////////////////////////////////////////////////////////////////
	if (realxe == 0 && ((Info->robots[myNumber]->E) != (Info->field->Emax)))
	{
		return;
	}
	if (realxe == 0 && ((Info->robots[myNumber]->E) == (Info->field->Emax)))
	{
		DoAction(Step, ACT_MOVE, -1, -1);
	}
	if (realxl == 0 && ((Info->robots[myNumber]->L) != (Info->field->Lmax)))
	{
		return;
	}
	if (realxl == 0 && ((Info->robots[myNumber]->L) == (Info->field->Lmax)))
	{
		DoAction(Step, ACT_MOVE, -1, -1);
	}
}



