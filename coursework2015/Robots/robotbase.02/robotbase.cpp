// robotbase.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include "robotbase.h"

using namespace std;

extern "C" 
	int flag=0;

struct robo 
{
	CString name;
	bool party;
	double lengthTo;
	double prot;
	int E;
	bool live;
	int i;
	int x,y;
};

struct obj
{
	int X;
	int Y;
	double lengthTo;
};

int coef=0;

void DoStep(stepinfo *Info, step *Step)
{
	//AfxMessageBox("DLL");
	srand(time(0));
	/*Step->action = ACT_MOVE;
	Step->dx = rand() % 3 -1;
	Step->dy = rand() % 3 -1;*/
	/*Step->numberOfActions = 1;
	Step->actions = new action[Step->numberOfActions];
	Step->actions[0].act = ACT_MOVE;
	Step->actions[0].dx = rand() % 3 - 1;
	Step->actions[0].dy = rand() % 3 - 1;*/
	//SetEvent(CommitStep);
	/*int myNumber = Info->yourNumber;
	int myX = Info->robots[myNumber]->x;
	int myY = Info->robots[myNumber]->y;*/

	int myNumber = Info->yourNumber;
	int myX = Info->robots[myNumber]->x;
	int myY = Info->robots[myNumber]->y;
	int rivals=Info->field->rivals;
	robo *robots;
	obj *objects2;
	obj *objects3;
	CString target;
	robots=new robo[rivals-1]; 
	objects2=new obj[(Info->field->Ne)]; 
	objects3=new obj[(Info->field->Nl)]; 
	CString ali[12];
	ali[0]="robotbase.04";
	ali[1]="robotbase.04.1";
	ali[2]="robotbase.04.2";
	ali[3]="robotbase.04.3";
	ali[4]="robotbase.04.4";
	ali[5]="robotbase.04.5";
	ali[6]="robotbase.17";
	ali[7]="robotbase.17.1";
	ali[8]="robotbase.17.2";
	ali[9]="robotbase.17.3";
	ali[10]="robotbase.17.4";
	ali[11]="robotbase.17.5";

	Info->robots;

	for (int i = 0; i < rivals-1; i++)
	{
		robots[i].name=Info->robots[i]->name;
		if ((Info->robots[i]->name=="robotbase.02")||(Info->robots[i]->name=="robotbase.02.1")||(Info->robots[i]->name=="robotbase.02.2")||(Info->robots[i]->name=="robotbase.02.3")||(Info->robots[i]->name=="robotbase.02.4")||(Info->robots[i]->name=="robotbase.02.5"))
		{
			robots[i].lengthTo=-66;
		} else {
			
			/*double lenght = sqrt((Info->robots[i]->x-myX)*(Info->robots[i]->x-myX) + (Info->robots[i]->y-myY)*(Info->robots[i]->y-myY));
			double forlenght1 = sqrt((abs(Info->robots[i]->x-myX)-Info->field->fieldWidth)*(abs(Info->robots[i]->x-myX)-Info->field->fieldWidth) + (Info->robots[i]->y-myY)*(Info->robots[i]->y-myY));
			double forlenght2 = sqrt((abs(Info->robots[i]->y-myY) - Info->field->fieldHeight)*(abs(Info->robots[i]->y-myY) - Info->field->fieldHeight) + (Info->robots[i]->x-myX)*(Info->robots[i]->x-myX));
			double forlenght3 = sqrt((abs(Info->robots[i]->y-myY) - Info->field->fieldHeight)*(abs(Info->robots[i]->y-myY) - Info->field->fieldHeight) + (abs(Info->robots[i]->x-myX) - Info->field->fieldWidth)*(abs(Info->robots[i]->x-myX) - Info->field->fieldWidth));
			if (lenght > forlenght1) lenght = forlenght1;
			if (forlenght2 > forlenght3) forlenght2 = forlenght3;
			if (lenght > forlenght2) lenght = forlenght2;
			robots[i].lengthTo=lenght;*/
			robots[i].lengthTo=sqrt(pow(abs(myX - Info->robots[i]->x), 2) + pow(abs(myY - Info->robots[i]->y), 2));
		}



		if ((Info->robots[i]->name==ali[0])||(Info->robots[i]->name==ali[1])||
			(Info->robots[i]->name==ali[2])||(Info->robots[i]->name==ali[3])||
			(Info->robots[i]->name==ali[4])||(Info->robots[i]->name==ali[5])||
			(Info->robots[i]->name==ali[6])||(Info->robots[i]->name==ali[7])||
			(Info->robots[i]->name==ali[8])||(Info->robots[i]->name==ali[9])||
			(Info->robots[i]->name==ali[10])||(Info->robots[i]->name==ali[11])) 
		{robots[i].party=true;} else {robots[i].party=false;} 
		robots[i].prot=(1-Info->field->rndmax)*(Info->robots[i]->P)*(Info->robots[i]->E)/(Info->field->Emax);
		robots[i].E=(Info->robots[i]->E);
		robots[i].live=(Info->robots[i]->alive);
		robots[i].i=i;
		robots[i].x=(Info->robots[i]->x);
		robots[i].y=(Info->robots[i]->y);

	}
	

	CString tmpname;
	bool tmpparty;
	bool tmplive;
	double tmplengthTo;
	double tmpprot;
	int tmpE,tmpi,tmpx,tmpy;
	for(int i = 0; i < rivals - 2; ++i) 
    {            
        for(int j = 0; j < rivals - 2; ++j)
        {     
            if (robots[j + 1].lengthTo < robots[j].lengthTo) 
            {
				tmpname=robots[j + 1].name;
				tmpparty=robots[j + 1].party;
                tmplengthTo = robots[j + 1].lengthTo; 
				tmpprot=robots[j + 1].prot;
				tmpE=robots[j + 1].E;
				tmplive=robots[j + 1].live;
				tmpi=robots[j + 1].i;
				tmpx=robots[j + 1].x;
				tmpy=robots[j + 1].y;

                robots[j + 1].name = robots[j].name;
                robots[j + 1].party = robots[j].party;
                robots[j + 1].lengthTo = robots[j].lengthTo;
                robots[j + 1].prot = robots[j].prot;
                robots[j + 1].E = robots[j].E;
                robots[j + 1].live = robots[j].live;
                robots[j + 1].i = robots[j].i;
                robots[j + 1].x = robots[j].x;
                robots[j + 1].y = robots[j].y;


                robots[j].name = tmpname;
                robots[j].party = tmpparty;
                robots[j].lengthTo = tmplengthTo;
                robots[j].prot = tmpprot;
                robots[j].E = tmpE;
                robots[j].live = tmplive;
                robots[j].i = tmpi;
                robots[j].x = tmpx;
                robots[j].y = tmpy;
            }
        }
    }
	double minprot=666666;
	double targetLen;
	int targetX, targetY, targetI;
	int ip=4;
		for(int i = 0; i < ip; i++) 
    {  
		if (robots[i].party) ip++;
		if (!robots[i].live) ip++;
		if (robots[i].lengthTo==-66) ip++;
		if ((robots[i].E<100)&&(!robots[i].party)&&(robots[i].lengthTo!=-66)&&(robots[i].live)) {
			minprot=robots[i].prot;
			target=robots[i].name;
			targetLen=robots[i].lengthTo;
			targetX=robots[i].x;
			targetY=robots[i].y;
			targetI=robots[i].i;
		} 
		else if ((robots[i].prot<minprot)&&(!robots[i].party)&&(robots[i].lengthTo!=-66)&&(robots[i].live)) {
			minprot=robots[i].prot;
			target=robots[i].name;
			targetLen=robots[i].lengthTo;
			targetX=robots[i].x;
			targetY=robots[i].y;
			targetI=robots[i].i;

		}
		if (target=="") ip++;
		if (ip>Info->field->rivals) ip=rivals;

	}

		double len2=1000000;
		double len3=1000000;
		int x2,y2,x3,y3;

		int ti=0;
		int tj=0;
		bool dead;
		for (int i = 0; i < ((Info->field->Ne)+(Info->field->Nl)); i++)
		{
			dead=1;
			for (int j = 0; j < Info->field->rivals; j++)
			{
				if ((Info->robots[j]->x==Info->objects[i]->x)&&(Info->robots[j]->y==Info->objects[i]->y)&&!(Info->robots[j]->alive)) dead=0;
			}
			if ((Info->objects[i]->type==-2)&&(dead!=0)) {
				//objects2[ti].X=Info->objects[i]->x;
				//objects2[ti].Y=Info->objects[i]->y;
				/*double lenghtob2 = sqrt((Info->objects[i]->x-myX)*(Info->objects[i]->x-myX) + (Info->objects[i]->y-myY)*(Info->objects[i]->y-myY));
				double forlenght1ob2 = sqrt((abs(Info->objects[i]->x-myX)-Info->field->fieldWidth)*(abs(Info->objects[i]->x-myX)-Info->field->fieldWidth) + (Info->objects[i]->y-myY)*(Info->objects[i]->y-myY));
				double forlenght2ob2 = sqrt((abs(Info->objects[i]->y-myY) - Info->field->fieldHeight)*(abs(Info->objects[i]->y-myY) - Info->field->fieldHeight) + (Info->objects[i]->x-myX)*(Info->objects[i]->x-myX));
				double forlenght3ob2 = sqrt((abs(Info->objects[i]->y-myY) - Info->field->fieldHeight)*(abs(Info->objects[i]->y-myY) - Info->field->fieldHeight) + (abs(Info->objects[i]->x-myX) - Info->field->fieldWidth)*(abs(Info->objects[i]->x-myX) - Info->field->fieldWidth));
				if (lenghtob2 > forlenght1ob2) lenghtob2 = forlenght1ob2;
				if (forlenght2ob2 > forlenght3ob2) forlenght2ob2 = forlenght3ob2;
				if (lenghtob2 > forlenght2ob2) lenghtob2 = forlenght2ob2;*/
				double lenghtob2=sqrt(pow(abs(myX - Info->objects[i]->x), 2) + pow(abs(myY - Info->objects[i]->y), 2));

				if (lenghtob2<len2) {
					len2=lenghtob2;
					x2=Info->objects[i]->x;
					y2=Info->objects[i]->y;
				}
				//objects2[ti].lengthTo=lenghtob2;
				//ti++;			
			}
			if ((Info->objects[i]->type==-3)&&(dead!=0)) {
				//objects3[tj].X=Info->objects[i]->x;
				//objects3[tj].Y=Info->objects[i]->y;
				/*double lenghtob3 = sqrt((Info->objects[i]->x-myX)*(Info->objects[i]->x-myX) + (Info->objects[i]->y-myY)*(Info->objects[i]->y-myY));
				double forlenght1ob3 = sqrt((abs(Info->objects[i]->x-myX)-Info->field->fieldWidth)*(abs(Info->objects[i]->x-myX)-Info->field->fieldWidth) + (Info->objects[i]->y-myY)*(Info->objects[i]->y-myY));
				double forlenght2ob3 = sqrt((abs(Info->objects[i]->y-myY) - Info->field->fieldHeight)*(abs(Info->objects[i]->y-myY) - Info->field->fieldHeight) + (Info->objects[i]->x-myX)*(Info->objects[i]->x-myX));
				double forlenght3ob3 = sqrt((abs(Info->objects[i]->y-myY) - Info->field->fieldHeight)*(abs(Info->objects[i]->y-myY) - Info->field->fieldHeight) + (abs(Info->objects[i]->x-myX) - Info->field->fieldWidth)*(abs(Info->objects[i]->x-myX) - Info->field->fieldWidth));
				if (lenghtob3 > forlenght1ob3) lenghtob3 = forlenght1ob3;
				if (forlenght2ob3 > forlenght3ob3) forlenght2ob3 = forlenght3ob3;
				if (lenghtob3 > forlenght2ob3) lenghtob3 = forlenght2ob3;*/
				double lenghtob3=sqrt(pow(abs(myX - Info->objects[i]->x), 2) + pow(abs(myY - Info->objects[i]->y), 2));

				if (lenghtob3<len3) {
					len3=lenghtob3;
					x3=Info->objects[i]->x;
					y3=Info->objects[i]->y;
				}
				//objects3[tj].lengthTo=lenghtob3;
				//tj++;			
			}

		}

for (int i = 0; i < rivals-1; i++)
{
		if(Info->robots[i]->name==target) targetI=i;
}


	
if(target==""){
		DoAction(Step, ACT_TECH, 0, (0.65)*(Info->robots[myNumber]->L), (0.35)*(Info->robots[myNumber]->L));

		if (len2<=((Info->robots[myNumber]->V)*(Info->field->Vmax)*(Info->robots[myNumber]->E)/((Info->field->Lmax)*(Info->field->Emax))))
		{

			DoAction(Step, ACT_MOVE,x2-myX, y2-myY);

		} else {
				int shagxe = abs(myX - x2)*((Info->robots[myNumber]->V)*(Info->field->Vmax)*(Info->robots[myNumber]->E)/((Info->field->Lmax)*(Info->field->Emax))) / len2;
				int shagye = abs(myY - y2)*((Info->robots[myNumber]->V)*(Info->field->Vmax)*(Info->robots[myNumber]->E)/((Info->field->Lmax)*(Info->field->Emax))) / len2;
			double qazasd=((Info->robots[myNumber]->V)*(Info->field->Vmax)*(Info->robots[myNumber]->E)/((Info->field->Lmax)*(Info->field->Emax)));
			
				if (myX - x2 < 0){
					shagxe = shagxe;
				} else{
					shagxe = -shagxe;
				}
				if (myY - y2 < 0) { 
					shagye = shagye;
				} else {
					shagye = -shagye;
				}
				
				DoAction(Step, ACT_MOVE,shagxe,shagye);
		
		}
		

		
	}
else
if((Info->stepnum)<0.990*(Info->field->N))
{
			
	//основной код ниже
	if (((Info->robots[myNumber]->E)>0.80*(Info->field->Emax))&&((Info->robots[myNumber]->L)>0.80*(Info->field->Lmax))){//тут пинаю
		
		if (targetLen<=((Info->robots[myNumber]->V)*(Info->field->Vmax)*(Info->robots[myNumber]->E)/((Info->field->Lmax)*(Info->field->Emax))))
		{
			DoAction(Step, ACT_MOVE,targetX-myX-1, targetY-myY-1);
			if (((Info->field->rndmax)*(Info->robots[myNumber]->A)*(Info->robots[myNumber]->E)/(Info->field->Emax))>((1-Info->field->rndmax)*(Info->robots[targetI]->P)*(Info->robots[targetI]->E)/(Info->field->Emax))){
				if((Info->field->Rmax)*(Info->robots[myNumber]->V)*(Info->robots[myNumber]->E)/((Info->field->Emax)*(Info->field->Lmax)))
				{
					DoAction(Step, ACT_ATTACK,targetX-myX, targetY-myY);
					coef=0;
				}
			} else {
				DoAction(Step, ACT_TECH, (coef+0.45)*(Info->robots[myNumber]->L), (0.20-2*coef)*(Info->robots[myNumber]->L), (coef+0.35)*(Info->robots[myNumber]->L));
				coef=coef+0.05;
			}
			//тут закончил пинать
		} else {
				DoAction(Step, ACT_TECH, (0.45)*(Info->robots[myNumber]->L), (0.20)*(Info->robots[myNumber]->L), (0.35)*(Info->robots[myNumber]->L));
			double qasd=((Info->robots[myNumber]->V)*(Info->field->Vmax)*(Info->robots[myNumber]->E)/((Info->field->Lmax)*(Info->field->Emax)));

				int shagx = abs(myX - targetX)*((Info->robots[myNumber]->V)*(Info->field->Vmax)*(Info->robots[myNumber]->E)/((Info->field->Lmax)*(Info->field->Emax))) / targetLen;
				int shagy = abs(myY - targetY)*((Info->robots[myNumber]->V)*(Info->field->Vmax)*(Info->robots[myNumber]->E)/((Info->field->Lmax)*(Info->field->Emax))) / targetLen;

				if (myX - targetX < 0) {
					shagx = shagx;
				} else {
					shagx = -shagx;
				}
				if (myY - targetY < 0) {
					shagy = shagy;
				} else {
					shagy = -shagy;
				}

				DoAction(Step, ACT_MOVE,shagx,shagy);
		}
		
	
		
	} else if ((Info->robots[myNumber]->L)<0.80*(Info->field->Lmax)) {
		DoAction(Step, ACT_TECH, 0, (0.65)*(Info->robots[myNumber]->L), (0.35)*(Info->robots[myNumber]->L));

		if (len3<=((Info->robots[myNumber]->V)*(Info->field->Vmax)*(Info->robots[myNumber]->E)/((Info->field->Lmax)*(Info->field->Emax))))
		{

			DoAction(Step, ACT_MOVE,x3-myX, y3-myY);

		} else {
			double asd=((Info->robots[myNumber]->V)*(Info->field->Vmax)*(Info->robots[myNumber]->E)/((Info->field->Lmax)*(Info->field->Emax)));
				int shagxl = abs(myX - x3)*((Info->robots[myNumber]->V)*(Info->field->Vmax)*(Info->robots[myNumber]->E)/((Info->field->Lmax)*(Info->field->Emax))) / len3;
				int shagyl = abs(myY - y3)*((Info->robots[myNumber]->V)*(Info->field->Vmax)*(Info->robots[myNumber]->E)/((Info->field->Lmax)*(Info->field->Emax))) / len3;

				if (myX - x3 < 0) {
					shagxl = shagxl;
				} else {
					shagxl = -shagxl;
				}
				if (myY - y3 < 0) {
					shagyl = shagyl;
				} else {
					shagyl = -shagyl;
				}

				DoAction(Step, ACT_MOVE,shagxl,shagyl);
		}
		
		
	}  else if ((Info->robots[myNumber]->E)<0.80*(Info->field->Emax)) {
		DoAction(Step, ACT_TECH, 0, (0.65)*(Info->robots[myNumber]->L), (0.35)*(Info->robots[myNumber]->L));

		if (len2<=((Info->robots[myNumber]->V)*(Info->field->Vmax)*(Info->robots[myNumber]->E)/((Info->field->Lmax)*(Info->field->Emax))))
		{

			DoAction(Step, ACT_MOVE,x2-myX, y2-myY);

		} else {
				int shagxe = abs(myX - x2)*((Info->robots[myNumber]->V)*(Info->field->Vmax)*(Info->robots[myNumber]->E)/((Info->field->Lmax)*(Info->field->Emax))) / len2;
				int shagye = abs(myY - y2)*((Info->robots[myNumber]->V)*(Info->field->Vmax)*(Info->robots[myNumber]->E)/((Info->field->Lmax)*(Info->field->Emax))) / len2;
			double qazasd=((Info->robots[myNumber]->V)*(Info->field->Vmax)*(Info->robots[myNumber]->E)/((Info->field->Lmax)*(Info->field->Emax)));
			
				if (myX - x2 < 0){
					shagxe = shagxe;
				} else{
					shagxe = -shagxe;
				}
				if (myY - y2 < 0) { 
					shagye = shagye;
				} else {
					shagye = -shagye;
				}
				DoAction(Step, ACT_MOVE,shagxe,shagye);
		
		}
		

		
	}
} else {
		DoAction(Step, ACT_TECH, 0, (0.65)*(Info->robots[myNumber]->L), (0.35)*(Info->robots[myNumber]->L));

		if (len2<=((Info->robots[myNumber]->V)*(Info->field->Vmax)*(Info->robots[myNumber]->E)/((Info->field->Lmax)*(Info->field->Emax))))
		{

			DoAction(Step, ACT_MOVE,x2-myX, y2-myY);

		} else {
				int shagxe = abs(myX - x2)*((Info->robots[myNumber]->V)*(Info->field->Vmax)*(Info->robots[myNumber]->E)/((Info->field->Lmax)*(Info->field->Emax))) / len2;
				int shagye = abs(myY - y2)*((Info->robots[myNumber]->V)*(Info->field->Vmax)*(Info->robots[myNumber]->E)/((Info->field->Lmax)*(Info->field->Emax))) / len2;
			double qazasd=((Info->robots[myNumber]->V)*(Info->field->Vmax)*(Info->robots[myNumber]->E)/((Info->field->Lmax)*(Info->field->Emax)));
			
				if (myX - x2 < 0){
					shagxe = shagxe;
				} else{
					shagxe = -shagxe;
				}
				if (myY - y2 < 0) { 
					shagye = shagye;
				} else {
					shagye = -shagye;
				}
				
				DoAction(Step, ACT_MOVE,shagxe,shagye);
		
		}
		

		
	}
	delete []robots; 
	delete []objects2; 
	delete []objects3; 
	return;
}


