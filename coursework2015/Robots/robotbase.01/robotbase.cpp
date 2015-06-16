// robotbase.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include "robotbase.h"

using namespace std;
int xx1,yy1,xxarh1,yyarh1,xx11,yy11,xxarh11,yyarh11;
int type;//1=Ne; -1=Nl
int fl=-1,fl2=-1,fl3=-1,fl4=-1,fl5=-1;//если не дошли до нужных станций первый раз
int f=0,f2=0,f3=0,f4=0,f5=0;
int h=-1,h2=-1,h3=-1,h4=-1,h5=-1;  //hh=0 если нет партнера вообще 
int xL,xE,yL,yE;
extern "C" 
	void DoStep(stepinfo *Info, step *Step)
{//100000000000000000000000000000
	double Rrob[400];
	double Rrobarh[400];
	double RNeNl[400];
	int Ne[100];
	int Nl[100];
	double ourvic[400];
	double myvic[400];
	int nstep;

	int i,j,z,idarh,id,Nrob,yarh,xarh,W,H,min,imin,min2,imin2,NeNl,x,y,x1,y1,xarh1,yarh1,L,Larh,A,Aarh,E,Earh,Lmax,Emax,buf,SSIN,SCOS;
	double RasNl,RasNe,RasNlarh,RasNearh,s,Smax,SAmax,FA,Smaxarh,SAmaxarh,FAarh; 
	int idarh2,idarh3,idarh4,idarh5,hh;

	////////////////////////////////////////
	z=0; imin=0;idarh=0; int Ncmd=0; int Ncmdarh=0;
	hh=0;//hh=0 если нет партнера вообще 

	Lmax=Info->field->Lmax;
	Emax=Info->field->Emax;
	NeNl=(Info->field->Ne)+(Info->field->Nl);
	W=Info->field->fieldWidth;
	H=Info->field->fieldHeight;
	Nrob=Info->field->rivals;
	id=Info->yourNumber;

	int Plid,Plidarh;

	if ((Info->robots[id]->name=="robotbase.01")||(Info->robots[id]->name=="robotbase.01.1"))
		for (i=0;i<Nrob;i++)		
			if ((Info->robots[i]->name=="robotbase.10")||(Info->robots[i]->name=="robotbase.10.1"))
			{idarh=i;hh=1;}

	if ((Info->robots[id]->name=="robotbase.01.2"))
		for (i=0;i<Nrob;i++)
			if (Info->robots[i]->name=="robotbase.10.2")
				{idarh=i;hh=1;}

	if ((Info->robots[id]->name=="robotbase.01.3"))
		for (i=0;i<Nrob;i++)
			if (Info->robots[i]->name=="robotbase.10.3")
				{idarh=i;hh=1;}

	if ((Info->robots[id]->name=="robotbase.01.4"))
		for (i=0;i<Nrob;i++)
			if (Info->robots[i]->name=="robotbase.10.4")
				{idarh=i;hh=1;}

	if ((Info->robots[id]->name=="robotbase.01.5"))
		for (i=0;i<Nrob;i++)
			if (Info->robots[i]->name=="robotbase.10.5")
				{idarh=i;hh=1;}

	Plid=Info->robots[id]->playerid;
	for (i=0;i<Nrob;i++)
	{	ourvic[i]=-999999;
	myvic[i]=-999999;
	if ((Info->robots[i]->playerid==Info->robots[id]->playerid)&&(Info->robots[i]->alive))
		Ncmd++;
	if ((Info->robots[i]->playerid==Info->robots[idarh]->playerid)&&(Info->robots[i]->alive))
		Ncmdarh++;
	}

	L=Info->robots[id]->L;
	E=Info->robots[id]->E;
	A=Info->robots[id]->A;

	Larh=Info->robots[idarh]->L;
	Earh=Info->robots[idarh]->E;
	Aarh=Info->robots[idarh]->A;

	yarh = Info->robots[idarh]->y;
	y=Info->robots[id]->y;
	xarh= Info->robots[idarh]->x; //координаты арх
	x= Info->robots[id]->x;

	for (i=0;i<Nrob;i++)
		if ((Info->robots[i]->alive)&&(i!=idarh)&&(i!=id)) 
			Rrob[i]=(sqrt((((Info->robots[i]->x)-x)*((Info->robots[i]->x)-x)+((Info->robots[i]->y)-y)*((Info->robots[i]->y)-y))));
		else Rrob[i]=W*H;

		for (i=0;i<Nrob;i++)
			if ((Info->robots[i]->alive)&&(i!=idarh)&&(i!=id)) 
				Rrobarh[i]=(sqrt((((Info->robots[i]->x)-xarh)*((Info->robots[i]->x)-xarh)+((Info->robots[i]->y)-yarh)*((Info->robots[i]->y)-yarh))));
			else Rrobarh[i]=W*H;

			Smaxarh=((Info->field->Vmax)*(Info->robots[idarh]->V)/(Info->field->Lmax)*(Info->robots[idarh]->E)/(Info->field->Emax));// мах перемещение арх
			Smax=((Info->field->Vmax)*(Info->robots[id]->V)/(Info->field->Lmax)*(Info->robots[id]->E)/(Info->field->Emax));// мах перемещение
			SAmaxarh=((Info->field->Rmax)*(Info->robots[idarh]->V)/(Info->field->Lmax)*(Info->robots[idarh]->E)/(Info->field->Emax));// max расстояние атаки арх
			SAmax=((Info->field->Rmax)*(Info->robots[id]->V)/(Info->field->Lmax)*(Info->robots[id]->E)/(Info->field->Emax));// max расстояние атаки
			FAarh=((Info->robots[idarh]->A)*(Info->robots[idarh]->E)/(Info->field->Emax));// – max сила атаки арх
			FA=((Info->robots[id]->A)*(Info->robots[id]->E)/(Info->field->Emax));// – max сила атаки

			////////////////////////////////////////////
			////////////////////////////////////////////

		if (hh==1)
			{if (Info->stepnum==1)//на первом шаге находим минимальное расстояние между станциями Е и L
			{
				if ((Info->robots[id]->name=="robotbase.01")|| (Info->robots[id]->name=="robotbase.01.1"))
					fl=0;
				if ((Info->robots[id]->name=="robotbase.01.2"))
					fl2=0;
				if ((Info->robots[id]->name=="robotbase.01.3"))
					fl3=0;
				if ((Info->robots[id]->name=="robotbase.01.4"))
					fl4=0;
				if ((Info->robots[id]->name=="robotbase.01.5"))
					fl5=0;

				DoAction(Step, ACT_TECH, 0, 0, 0, 0.5*L, 0.5*L);
				for (i=0;i<NeNl;i++)
					if (Info->objects[i]->type==OBJ_CHARGER)
					{
						for (j=0;j<NeNl;j++)
						{
							if (Info->objects[j]->type==OBJ_TECH)
							{
								RNeNl[z]=(sqrt(((Info->objects[i]->x)-(Info->objects[j]->x))*((Info->objects[i]->x)-(Info->objects[j]->x))+((Info->objects[i]->y)-(Info->objects[j]->y))*((Info->objects[i]->y)-(Info->objects[j]->y))));
								Ne[z]=i;
								Nl[z]=j;
								z=z+1;
							}
						}

					}
					min=1000;
					for (i=0;i<z-1;i++)
						if (RNeNl[i]<min)
						{
							min=RNeNl[i];
							imin=i;
						}

						RasNl=(sqrt((((Info->objects[Nl[imin]]->x)-(x))*((Info->objects[Nl[imin]]->x)-(x))+((Info->objects[Nl[imin]]->y)-(y))*((Info->objects[Nl[imin]]->y)-(y)))));
						RasNe=(sqrt((((Info->objects[Ne[imin]]->x)-(x))*((Info->objects[Ne[imin]]->x)-(x))+((Info->objects[Ne[imin]]->y)-(y))*((Info->objects[Ne[imin]]->y)-(y)))));
						RasNlarh=(sqrt((((Info->objects[Nl[imin]]->x)-(xarh))*((Info->objects[Nl[imin]]->x)-(xarh))+((Info->objects[Nl[imin]]->y)-(yarh))*((Info->objects[Nl[imin]]->y)-(yarh)))));
						RasNearh=(sqrt((((Info->objects[Ne[imin]]->x)-(xarh))*((Info->objects[Ne[imin]]->x)-(xarh))+((Info->objects[Ne[imin]]->y)-(yarh))*((Info->objects[Ne[imin]]->y)-(yarh)))));
						xL=Info->objects[Nl[imin]]->x;
						xE=Info->objects[Ne[imin]]->x;
						yL=Info->objects[Nl[imin]]->y;
						yE=Info->objects[Ne[imin]]->y;
						/////////выбираем кто к какой станции пойдет////////////
						if ((RasNl>=RasNlarh)&&(RasNe<=RasNearh))
						{
							x1=Info->objects[Ne[imin]]->x; y1=Info->objects[Ne[imin]]->y;
							xarh1=Info->objects[Nl[imin]]->x; yarh1=Info->objects[Nl[imin]]->y;
							type=1;
						}
						else if ((RasNl<RasNlarh)&&(RasNe>RasNearh))
						{
							x1=Info->objects[Nl[imin]]->x; y1=Info->objects[Nl[imin]]->y;
							xarh1=Info->objects[Ne[imin]]->x; yarh1=Info->objects[Ne[imin]]->y;
							type=-1;
						}
						else if ((RasNl>=RasNlarh)&&(RasNe>=RasNearh))
						{
							if (RasNl>=RasNe)
							{
								x1=Info->objects[Ne[imin]]->x; y1=Info->objects[Ne[imin]]->y;
								xarh1=Info->objects[Nl[imin]]->x; yarh1=Info->objects[Nl[imin]]->y;
								type=1;
							}
							else
							{
								x1=Info->objects[Nl[imin]]->x; y1=Info->objects[Nl[imin]]->y;
								xarh1=Info->objects[Ne[imin]]->x; yarh1=Info->objects[Ne[imin]]->y;
								type=-1;
							}
						}
						else
						{
							if (RasNlarh>=RasNearh)
							{
								x1=Info->objects[Nl[imin]]->x; y1=Info->objects[Nl[imin]]->y;
								xarh1=Info->objects[Ne[imin]]->x; yarh1=Info->objects[Ne[imin]]->y;
								type=-1;
							}
							else
							{
								x1=Info->objects[Ne[imin]]->x; y1=Info->objects[Ne[imin]]->y;
								xarh1=Info->objects[Nl[imin]]->x; yarh1=Info->objects[Nl[imin]]->y;
								type=1;
							}
						}
						xx1=x1;yy1=y1;xxarh1=xarh1;yyarh1=yarh1;//запомнили координаты выбранной каждым станции
			}
			else 
			//	if (hh==1)
				{if ((fl==0)||(fl2==0)||(fl3==0)||(fl4==0)||(fl5==0))//если идем до станций в первый раз
				{
					if ((x!=xx1)||(y!=yy1))//пока не дошли
					{
						s=(sqrt((x-xx1)*(x-xx1)+(y-yy1)*(y-yy1)));

						if (s<=Smax)//если можем дойти за 1 шаг
						{
							DoAction(Step, ACT_MOVE, (xx1-x), (yy1-y), 0, 0, 0);//делаем его
						}
						else
						{//медленно но верно движемся к цели в нужном направлении
							if (xx1==x) 
							{
								SCOS=0; 
								SSIN=Smax;
							}
							else
							{
								SCOS=Smax*cos(atan(abs(yy1-y)/abs(xx1-x)));
								SSIN=Smax*sin(atan(abs(yy1-y)/abs(xx1-x)));
							}
							if (xx1<x) SCOS=-SCOS;
							if (yy1<y) SSIN=-SSIN;
							DoAction(Step, ACT_MOVE, SCOS, SSIN, 0, 0, 0);
						}
					}
					else 
					{
						if (((Info->robots[id]->name=="robotbase.01")||(Info->robots[id]->name=="robotbase.01.1")))
							fl=1; 
						if (((Info->robots[id]->name=="robotbase.01.2")))
							fl2=1;
						if (((Info->robots[id]->name=="robotbase.01.3")))
							fl3=1;
						if (((Info->robots[id]->name=="robotbase.01.4")))
							fl4=1;
						if (((Info->robots[id]->name=="robotbase.01.5")))
							fl5=1;
						DoAction(Step, ACT_TECH, 0, 0, (0.36-0.03*Ncmd)*L, 0.3*L, (1-0.3-(0.36-0.03*Ncmd))*L);
					}
				}	
				////////////напарник жив////////////////////////	
				else if (Info->robots[idarh]->alive)
				{
					//////////когда нашли нужные станции///////////
					DoAction(Step, ACT_TECH, 0, 0, (0.36-0.03*Ncmd)*L, 0.3*L, (1-0.3-(0.36-0.03*Ncmd))*L);
							if ((x==xL)&&(y==yL)) 
							if ((xarh==xE)&&(yarh==yE)) 
						{type=-1;
						xx1=xL;
						xxarh1=xE;
						yy1=yL;
						yyarh1=yE;} 
							else
							{
								type=0;
							}

						if ((x==xE)&&(y==yE)) 
							if ((xarh==xL)&&(yarh==yL))
						{type=1;
						xx1=xE;
						xxarh1=xL;
						yy1=yE;
						yyarh1=yL;}
							else
							{
								type=0;
							}
					
						if ( ((type==1)&&((L<0.8*Lmax)||(Earh<0.9*Emax))) || ((type==-1)&&((E<0.9*Emax)||(Larh<0.8*Lmax)))  )	
						{
							if (xxarh1==x) {SCOS=0; SSIN=Smax;}
							else
							{
								SCOS=Smax*cos(atan(abs(yyarh1-y)/abs(xxarh1-x)));
								SSIN=Smax*sin(atan(abs(yyarh1-y)/abs(xxarh1-x)));
							}
							if (((x!=xxarh1)||(y!=yyarh1)))//пока не дошли
							{
								if (xxarh1<x) SCOS=-SCOS;
								if (yyarh1<y) SSIN=-SSIN;
								s=(sqrt((x-xxarh1)*(x-xxarh1)+(y-yyarh1)*(y-yyarh1)));

								if (s<Smax)//если можем дойти за 1 шаг
								{
									DoAction(Step, ACT_MOVE, (xxarh1-x), (yyarh1-y), 0, 0, 0);	//делаем его
								}
								else
								{//медленно но верно движемся к цели в нужном направлении
									for (i=0;i<NeNl;i++)//избегая попадания на левую станцию
										if (((x+SCOS)==Info->objects[i]->x)&&(y+SSIN)==Info->objects[i]->y)
										{
											SCOS--; SSIN--;
											if (((x+SCOS)==Info->objects[i]->x)&&(y+SSIN)==Info->objects[i]->y)
												DoAction(Step, ACT_MOVE, SCOS, SSIN, 0, 0, 0);
										}
										else
											DoAction(Step, ACT_MOVE, SCOS, SSIN, 0, 0, 0);
								}
							}
						}

					////АТАКА////////////////
					Plidarh=Info->robots[idarh]->playerid;

					for (i=0;i<Nrob;i++)
					{
						if ((Rrob[i]<=SAmax)&&(Rrobarh[i]<=SAmaxarh)&&(Info->robots[i]->alive)&&(Info->robots[i]->name!="robotbase.10")&&(Info->robots[i]->name!="robotbase.10.1")&&(Info->robots[i]->name!="robotbase.10.2")&&(Info->robots[i]->name!="robotbase.10.3")&&(Info->robots[i]->name!="robotbase.10.4")&&(Info->robots[i]->name!="robotbase.10.5")&&(Info->robots[i]->name!="robotbase.01")&&(Info->robots[i]->name!="robotbase.01.1")&&(Info->robots[i]->name!="robotbase.01.2")&&(Info->robots[i]->name!="robotbase.01.3")&&(Info->robots[i]->name!="robotbase.01.4")&&(Info->robots[i]->name!="robotbase.01.5"))

							ourvic[i]= ((Info->field->rndmin)*(FA*Ncmd)+(Info->field->rndmin)*(FAarh*Ncmdarh))-((1-(Info->field->rndmin))*(Info->robots[i]->P)*(Info->robots[i]->E))/Emax;

						if ((Rrob[i]<=SAmax)&&(Rrobarh[i]>SAmaxarh)&&(Info->robots[i]->alive)&&(Info->robots[i]->name!="robotbase.10")&&(Info->robots[i]->name!="robotbase.10.1")&&(Info->robots[i]->name!="robotbase.10.2")&&(Info->robots[i]->name!="robotbase.10.3")&&(Info->robots[i]->name!="robotbase.10.4")&&(Info->robots[i]->name!="robotbase.10.5")&&(Info->robots[i]->name!="robotbase.01")&&(Info->robots[i]->name!="robotbase.01.1")&&(Info->robots[i]->name!="robotbase.01.2")&&(Info->robots[i]->name!="robotbase.01.3")&&(Info->robots[i]->name!="robotbase.01.4")&&(Info->robots[i]->name!="robotbase.01.5"))
							myvic[i]= (Info->field->rndmin)*FA*Ncmd-((1-(Info->field->rndmin))*(Info->robots[i]->P)*(Info->robots[i]->E))/Emax;
					}

					int maxourv=-999999;
					int nmaxourv=0;
					int maxmyv=-999999;
					int nmaxmyv=0;
					for (i=0;i<Nrob;i++)
					{
						if (ourvic[i]>maxourv) 
						{
							maxourv=ourvic[i];
							nmaxourv=i;
						}
						if (myvic[i]>maxmyv) 
						{
							maxmyv=myvic[i];
							nmaxmyv=i;
						}
					}
					if (
						((Info->robots[nmaxourv]->x==xL)&&(Info->robots[nmaxourv]->y==yL))||((Info->robots[nmaxourv]->x==xE)&&(Info->robots[nmaxourv]->y==yE))
						||((Info->robots[nmaxmyv]->x==xL)&&(Info->robots[nmaxmyv]->y==yL))||((Info->robots[nmaxmyv]->x==xE)&&(Info->robots[nmaxmyv]->y==yL))
						)
					{ourvic[nmaxourv]=-999999;
					myvic[nmaxmyv]=-999999;
					maxourv=-999999;
					maxmyv=-999999;}

					if (maxourv>0)
					{
						DoAction(Step, ACT_ATTACK, (Info->robots[nmaxourv]->x)-x, (Info->robots[nmaxourv]->y)-y, 0,0,0);
					}
					else if (maxmyv>0)
					{
						DoAction(Step, ACT_ATTACK, (Info->robots[nmaxmyv]->x)-x, (Info->robots[nmaxmyv]->y)-y, 0,0,0); 
					}
					////////КОНЕЦ АТАКИ/////////////
				}

				else //если сдох напарник :'(
				{
				
						DoAction(Step, ACT_TECH, 0, 0, (0.36-0.03*Ncmd)*L, 0.3*L, (1-0.3-(0.36-0.03*Ncmd))*L);
						
					if ((x==xL)&&(y==yL)) 		
						{type=-1;
						xx1=xL;
						xxarh1=xE;
						yy1=yL;
						yyarh1=yE;} 

						if ((x==xE)&&(y==yE)) 
						{type=1;
						xx1=xE;
						xxarh1=xL;
						yy1=yE;
						yyarh1=yL;}
					
						if ( ((type==1)&&((L<0.8*Lmax)&&(E>0.3*Emax))) || ((type==-1)&&((E<0.9*Emax)&&(L>0.3*Lmax)))  )	
						{
							if (xxarh1==x) {SCOS=0; SSIN=Smax;}
							else
							{
								SCOS=Smax*cos(atan(abs(yyarh1-y)/abs(xxarh1-x)));
								SSIN=Smax*sin(atan(abs(yyarh1-y)/abs(xxarh1-x)));
							}
							if (((x!=xxarh1)||(y!=yyarh1)))//пока не дошли
							{
								if (xxarh1<x) SCOS=-SCOS;
								if (yyarh1<y) SSIN=-SSIN;
								s=(sqrt((x-xxarh1)*(x-xxarh1)+(y-yyarh1)*(y-yyarh1)));

								if (s<Smax)//если можем дойти за 1 шаг
								{
									DoAction(Step, ACT_MOVE, (xxarh1-x), (yyarh1-y), 0, 0, 0);	//делаем его
								}
								else
								{//медленно но верно движемся к цели в нужном направлении

									for (i=0;i<NeNl;i++)//избегая попадания на левую станцию
										if (((x+SCOS)==Info->objects[i]->x)&&(y+SSIN)==Info->objects[i]->y)
										{
											SCOS--; SSIN--;
											if (((x+SCOS)==Info->objects[i]->x)&&(y+SSIN)==Info->objects[i]->y)
												DoAction(Step, ACT_MOVE, SCOS, SSIN, 0, 0, 0);
										}
										else
											DoAction(Step, ACT_MOVE, SCOS, SSIN, 0, 0, 0);
								}
							}
						}
					
					////АТАКА////////////////
					for (i=0;i<Nrob;i++)
					{
						if ((Rrob[i]<=SAmax)&&(Rrobarh[i]>SAmaxarh)&&(Info->robots[i]->alive)&&(Info->robots[i]->name!="robotbase.10")&&(Info->robots[i]->name!="robotbase.10.1")&&(Info->robots[i]->name!="robotbase.10.2")&&(Info->robots[i]->name!="robotbase.10.3")&&(Info->robots[i]->name!="robotbase.10.4")&&(Info->robots[i]->name!="robotbase.10.5")&&(Info->robots[i]->name!="robotbase.01")&&(Info->robots[i]->name!="robotbase.01.1")&&(Info->robots[i]->name!="robotbase.01.2")&&(Info->robots[i]->name!="robotbase.01.3")&&(Info->robots[i]->name!="robotbase.01.4")&&(Info->robots[i]->name!="robotbase.01.5"))
							myvic[i]= (Info->field->rndmin)*FA*Ncmd-((1-(Info->field->rndmin))*(Info->robots[i]->P)*(Info->robots[i]->E))/Emax;
					}
					int maxmyv=-999999;
					int nmaxmyv=0;
					for (i=0;i<Nrob;i++)
					{
						if (myvic[i]>maxmyv) 
						{
							maxmyv=myvic[i];
							nmaxmyv=i;
						}
					}
					if (
						((Info->robots[nmaxmyv]->x==xx1)&&(Info->robots[nmaxmyv]->y==yy1))||((Info->robots[nmaxmyv]->x==xxarh1)&&(Info->robots[nmaxmyv]->y==yyarh1))
						)
					{
						myvic[nmaxmyv]=-999999;
						maxmyv=-999999;}

					if (maxmyv>0)
					{
						DoAction(Step, ACT_ATTACK, (Info->robots[nmaxmyv]->x)-x, (Info->robots[nmaxmyv]->y)-y, 0,0,0); 
					}
					////////КОНЕЦ АТАКИ/////////////
				}
				}
				///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				}
				else//hh=0 !
				{
				if (Info->stepnum==1)//на первом шаге находим минимальное расстояние между станциями Е и L
			{
				DoAction(Step, ACT_TECH, 0, 0, 0, 0.5*L, 0.5*L);
				for (i=0;i<NeNl;i++)
					if (Info->objects[i]->type==OBJ_CHARGER)
					{
						for (j=0;j<NeNl;j++)
						{
							if (Info->objects[j]->type==OBJ_TECH)
							{
								RNeNl[z]=(sqrt(((Info->objects[i]->x)-(Info->objects[j]->x))*((Info->objects[i]->x)-(Info->objects[j]->x))+((Info->objects[i]->y)-(Info->objects[j]->y))*((Info->objects[i]->y)-(Info->objects[j]->y))));
								Ne[z]=i;
								Nl[z]=j;
								z=z+1;
							}
						}

					}
					min=1000;
					for (i=0;i<z-1;i++)
						if (RNeNl[i]<min)
						{
							min=RNeNl[i];
							imin=i;
						}

						RasNl=(sqrt((((Info->objects[Nl[imin]]->x)-(x))*((Info->objects[Nl[imin]]->x)-(x))+((Info->objects[Nl[imin]]->y)-(y))*((Info->objects[Nl[imin]]->y)-(y)))));
						RasNe=(sqrt((((Info->objects[Ne[imin]]->x)-(x))*((Info->objects[Ne[imin]]->x)-(x))+((Info->objects[Ne[imin]]->y)-(y))*((Info->objects[Ne[imin]]->y)-(y)))));
						
						xL=Info->objects[Nl[imin]]->x;
						xE=Info->objects[Ne[imin]]->x;
						yL=Info->objects[Nl[imin]]->y;
						yE=Info->objects[Ne[imin]]->y;
						/////////выбираем кто к какой станции пойдет////////////
						if (RasNl>=RasNe)
						{
							x1=Info->objects[Ne[imin]]->x; y1=Info->objects[Ne[imin]]->y;
							xarh1=Info->objects[Nl[imin]]->x; yarh1=Info->objects[Nl[imin]]->y;
							type=1;
						}
						else
						{
							x1=Info->objects[Nl[imin]]->x; y1=Info->objects[Nl[imin]]->y;
							xarh1=Info->objects[Ne[imin]]->x; yarh1=Info->objects[Ne[imin]]->y;
							type=-1;
						}
						
						xx11=x1;yy11=y1;xxarh11=xarh1;yyarh11=yarh1;//запомнили координаты выбранной каждым станции
			}

				else {
					
					
					
					
					
					if ((h==0)||(h2==0)||(h3==0)||(h4==0)||(h5==0))//если идем до станций в первый раз
				{
					if ((x!=xx11)||(y!=yy11))//пока не дошли
					{
						s=(sqrt((x-xx11)*(x-xx11)+(y-yy11)*(y-yy11)));

						if (s<=Smax)//если можем дойти за 1 шаг
						{
							DoAction(Step, ACT_MOVE, (xx11-x), (yy11-y), 0, 0, 0);//делаем его
						}
						else
						{//медленно но верно движемся к цели в нужном направлении
							if (xx11==x) 
							{
								SCOS=0; 
								SSIN=Smax;
							}
							else
							{
								SCOS=Smax*cos(atan(abs(yy11-y)/abs(xx11-x)));
								SSIN=Smax*sin(atan(abs(yy11-y)/abs(xx11-x)));
							}
							if (xx11<x) SCOS=-SCOS;
							if (yy11<y) SSIN=-SSIN;
							DoAction(Step, ACT_MOVE, SCOS, SSIN, 0, 0, 0);
						}
					}
					else 
					{
						if (((Info->robots[id]->name=="robotbase.01")||(Info->robots[id]->name=="robotbase.01.1")))
							h=1; 
						if (((Info->robots[id]->name=="robotbase.01.2")))
							h2=1;
						if (((Info->robots[id]->name=="robotbase.01.3")))
							h3=1;
						if (((Info->robots[id]->name=="robotbase.01.4")))
							h4=1;
						if (((Info->robots[id]->name=="robotbase.01.5")))
							h5=1; 
						DoAction(Step, ACT_TECH, 0, 0, (0.36-0.03*Ncmd)*L, 0.3*L, (1-0.3-(0.36-0.03*Ncmd))*L);
					}
				}
				else
				{
					
					//////////когда нашли нужные станции///////////
		
				DoAction(Step, ACT_TECH, 0, 0, (0.36-0.03*Ncmd)*L, 0.3*L, (1-0.3-(0.36-0.03*Ncmd))*L);
						
					if ((x==xL)&&(y==yL)) 		
						{type=-1;
						xx11=xL;
						xxarh11=xE;
						yy11=yL;
						yyarh11=yE;} 

						if ((x==xE)&&(y==yE)) 
						{type=1;
						xx11=xE;
						xxarh11=xL;
						yy11=yE;
						yyarh11=yL;}
					
						if ( ((type==1)&&((L<0.8*Lmax)&&(E>0.3*Emax))) || ((type==-1)&&((E<0.9*Emax)&&(L>0.3*Lmax)))  )	
						{
							if (xxarh11==x) {SCOS=0; SSIN=Smax;}
							else
							{
								SCOS=Smax*cos(atan(abs(yyarh11-y)/abs(xxarh11-x)));
								SSIN=Smax*sin(atan(abs(yyarh11-y)/abs(xxarh11-x)));
							}
							if (((x!=xxarh11)||(y!=yyarh11)))//пока не дошли
							{
								if (xxarh11<x) SCOS=-SCOS;
								if (yyarh11<y) SSIN=-SSIN;
								s=(sqrt((x-xxarh11)*(x-xxarh11)+(y-yyarh11)*(y-yyarh11)));

								if (s<Smax)//если можем дойти за 1 шаг
								{
									DoAction(Step, ACT_MOVE, (xxarh11-x), (yyarh11-y), 0, 0, 0);	//делаем его
								}
								else
								{//медленно но верно движемся к цели в нужном направлении

									for (i=0;i<NeNl;i++)//избегая попадания на левую станцию
										if (((x+SCOS)==Info->objects[i]->x)&&(y+SSIN)==Info->objects[i]->y)
										{
											SCOS--; SSIN--;
											if (((x+SCOS)==Info->objects[i]->x)&&(y+SSIN)==Info->objects[i]->y)
												DoAction(Step, ACT_MOVE, SCOS, SSIN, 0, 0, 0);
										}
										else
											DoAction(Step, ACT_MOVE, SCOS, SSIN, 0, 0, 0);
								}
							}
						}
					////АТАКА////////////////
					for (i=0;i<Nrob;i++)
					{
						if ((Rrob[i]<=SAmax)&&(Rrobarh[i]>SAmaxarh)&&(Info->robots[i]->alive)&&(Info->robots[i]->name!="robotbase.10")&&(Info->robots[i]->name!="robotbase.10.1")&&(Info->robots[i]->name!="robotbase.10.2")&&(Info->robots[i]->name!="robotbase.10.3")&&(Info->robots[i]->name!="robotbase.10.4")&&(Info->robots[i]->name!="robotbase.10.5")&&(Info->robots[i]->name!="robotbase.01")&&(Info->robots[i]->name!="robotbase.01.1")&&(Info->robots[i]->name!="robotbase.01.2")&&(Info->robots[i]->name!="robotbase.01.3")&&(Info->robots[i]->name!="robotbase.01.4")&&(Info->robots[i]->name!="robotbase.01.5"))
							myvic[i]= (Info->field->rndmin)*FA*Ncmd-((1-(Info->field->rndmin))*(Info->robots[i]->P)*(Info->robots[i]->E))/Emax;
					}
					int maxmyv=-999999;
					int nmaxmyv=0;
					for (i=0;i<Nrob;i++)
					{
						if (myvic[i]>maxmyv) 
						{
							maxmyv=myvic[i];
							nmaxmyv=i;
						}
					}
					if (
						((Info->robots[nmaxmyv]->x==xx11)&&(Info->robots[nmaxmyv]->y==yy11))||((Info->robots[nmaxmyv]->x==xxarh11)&&(Info->robots[nmaxmyv]->y==yyarh11))
						)
					{
						myvic[nmaxmyv]=-999999;
						maxmyv=-999999;}

					if (maxmyv>0)
					{
						DoAction(Step, ACT_ATTACK, (Info->robots[nmaxmyv]->x)-x, (Info->robots[nmaxmyv]->y)-y, 0,0,0); 
					}
					////////КОНЕЦ АТАКИ/////////////
				}
				}
				}
				
				return;
}