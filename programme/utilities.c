#include <analysis.h>
#include <ansi_c.h>
#include "utilities.h"
#include "log.h"

//==============================================================================
//
// Title:       utilities.c
// Purpose:     A short description of the implementation.
//
// Created on:  1/26/2015 at 3:54:10 PM by .
// Copyright:   . All Rights Reserved.
//
//==============================================================================
// Global functions


//
/// reservation de la memoire
//
void *malloc2 (size_t memBlockSize)
{
	
	void *p = malloc( memBlockSize);
	
	lprintf("%p [%d]",p,memBlockSize);
	
	return p;
	
}



//
/// Comptage du nombre de ligne d'un fichier
//

int count(FILE * f)
{
	int c;
	int counter = 1;
	
	while((c = fgetc(f))!=EOF)//tant que le fichier n'est pas fini
	{
	    if(c == '\n')//si on saute une ligne
	  	{
			counter++;//on incremente le compteur de ligne du fichier
  		}
	}
return counter;
}


//
/// Extraction d'un fichier dans le graph
//

void extract1(FILE * f,int nbl,graphe *pgraphe)
{
	int a;
	int i = 0;
	char c;
	
	while(i!=nbl)//tant que le fichier n'est pas fini
	{
		fscanf(f,"%lf %c %lf",&pgraphe->x[i],&c,&pgraphe->y[i]);//recupere et stocke x et y dans un tableau 
		i++; 
	}
	
	pgraphe->nbpts = nbl-1;
	fclose(f);
}


//
/// Affichage d'un tableau a deux dimension.
//

void affiche(double ** tab, int a)
{
  int i;
  
  for (i = 0; i < a; i++)//Pour chaque ligne  
  {
	printf("%lf \t%lf\n", tab[0][i],tab[1][i]);
  }
  
}


//
/// Menu
//

int affichageMenu()
{
	 int choixMenu;
     printf("---Menu---\n\n");
     printf("1.GetMaxDelta\n");
     printf("2.windows\n");
     printf("3.inflexionpoints\n");
     printf("4.Amplitudemax\n");
	 printf("5.Envelope\n\n\n");
     printf("\nVotre choix?\n\n");
     scanf("%d", &choixMenu);
	 return choixMenu;
}

//
/// Creation et initialisation d'un graphique
//
graphe* NewGraphe(void)
{
	graphe* pgraphe; 
	
	pgraphe = (graphe*) malloc2( sizeof(graphe) );
	
	if( pgraphe != NULL )
	{
		pgraphe->name = NULL;
		pgraphe->xchanelname = NULL;
		pgraphe->ychanelname = NULL;
		pgraphe->canvastop = 10.0;
		pgraphe->canvasbottom =  -10.0;
		pgraphe->canvasleft = -10.0;
		pgraphe->canvasright =  10.0;
		pgraphe->nbpts = 0;
		pgraphe->lastnbpts = 0;
		
		pgraphe->x = (double*) malloc2 (10000 * sizeof(double));
		pgraphe->y = (double*) malloc2 (10000 * sizeof(double));
	}
	
	return pgraphe;
}


//
/// Creation et initialisation d'une fenetre
//
fenetre* Newfenetre(void)
{
	fenetre* new; 
	
	new = (fenetre*) malloc2( sizeof(fenetre) );
	
	if( new != NULL )
	{
		new->xdebut = NULL;
		new->xfin = NULL;
		new->ydebut = NULL;
		new->yfin = NULL;
		new->nbpts = 0;
		new->entree=0 ;
		new->sortie=0;

	}
	
	return new;
}


//
//
/// Fonction de recherche du maximun en y.
//

 double searchmax(double a,double b,double * Y) 
{
	int i;
	double max=-1000;
	
	for(i=a;i<b;i++)
	{
		if (Y[i]>max)
		{
			max=Y[i];
		}
	}
	
	return max;
}
 
 
 
//
/// Fonction de recherche d'une coordonnee en x avec sa coordonnee en y en parametre.
//

double searchx(double a,int nbl,double * Y,double * X)
{
	int i;
	double xx=0;
	
	for(i=0;i<nbl;i++)
	{
		if (Y[i]==a)
		{
			xx=X[i];
		}			 
	}
	return xx;
} 


//
/// Fonction de recherche d'une coordonnee en y avec sa coordonnee en x en parametre.
//

double searchy(double a,int nbl,double * Y,double * X)
{
	int i;
	double xx=0;
	
	for(i=0;i<nbl;i++)
	{
		if (X[i]==a)
		{
			xx=Y[i];
		}			 
	}
	return xx;
}


//
/// Creer la valeur moyenne glissante d'une courbe.
//

void average( graphe*pgraphe,int v, double * Ym)
{
	int i,a;
	
	//Calcul et stockage de la moyenne glissante
	double g = 0.0;
	
	Mean (pgraphe->y,v,&g); 
	Ym[v-1]=g; 	
	
	for(i=v;i<(pgraphe->nbpts)-1;i++)
	{
		Ym[i]=Ym[i-1]+(pgraphe->y[i]-pgraphe->y[i-v])/v;
	}
}


//
/// Compte le nombre de points d'intersections entre deux courbes
//

int intersection(graphe*pgraphe,double * Ym)
{
	int i;
	int a=0;
	
	for(i=0;i<pgraphe->nbpts;i++)
	{
		if (((pgraphe->y[i]<Ym[i])&&(pgraphe->y[i+1]>Ym[i+1])) ||((pgraphe->y[i]>Ym[i])&&(pgraphe->y[i+1]<Ym[i+1])))
		{
			a++;
		}
	}
	
	return a;
}


//
/// sauvegarde le nombre de points d'intersections entre deux courbes
//

void saveintersection(graphe*pgraphe,double * Ym, double ** tabinter)
{
	int i;
	int j=0;
	
	for(i=0;i<pgraphe->nbpts;i++)
	{
		if (((Ym[i]<pgraphe->y[i])&&(Ym[i+1]>pgraphe->y[i+1])) ||((Ym[i]>pgraphe->y[i])&&(Ym[i+1]<pgraphe->y[i+1])))
		{
			tabinter[0][j]=i;
			
			tabinter[1][j]=pgraphe->y[i];
			
			j++;
		}
	}
}


//
/// Recherche le minimum en y
//

double searchmin(double a,double b,graphe*pgraphe)
{
	int i;
	double min=1000;
	
	for(i=a;i<b;i++)
	{
		if (pgraphe->y[i]<min)
		{
			min=pgraphe->y[i];
		}
	}
	
	return min;
}


//
/// Recherche le minimum en y
//

double searchmin2(double b,double a,graphe*pgraphe)
{
	int i;
	double min=1000;
	
	for(i=a;i<b;i++)
	{
		if (pgraphe->y[i]<min)
		{
			min=pgraphe->y[i];
		}
	}
	
	return min;
}



//
/// Recherche le minimum en y pour une courbe croissante en x
//

double searchminw(double a,double b,double * Yc)
{
	int i;
	double min=1000;
	
	for(i=a;i<b;i++)
	{
		if (Yc[i]<min)
		{
			min=Yc[i];
		}
	}
	
	return min;
}


//
/// Recherche le maximum en y pour une courbe croissante en x
//

 double searchmax2(double a,double b,graphe*pgraphe) 
{
	int i;
	double max=-1000;
	
	for(i=a;i<b;i++)
	{
		if (pgraphe->y[i]>max)
		{
			max=pgraphe->y[i];
		}
	}
	
	return max;
}
 
 
//
/// Recherche le maximum en y pour une courbe decroissante en x
//
 
 double searchmax8(double b,double a,graphe*pgraphe) 
{
	int i;
	double max=-1000;
	
	for(i=a;i<b;i++)
	{
		if (pgraphe->y[i]>max)
		{
			max=pgraphe->y[i];
		}
	}
	
	return max;
}
 
 
//
/// Recherche le maximum en y pour une courbe croissante en x
//
 
 double searchmax2w(double a,double b,double * Yc) 
{
	int i;
	double max=-1000;
	
	for(i=a;i<b;i++)
	{
		if (Yc[i]>max)
		{
			max=Yc[i];
		}
	}
	
	return max;
} 
 
 
 
//
/// Fonction de recherche d'une coordonnee en x avec sa coordonnee en y en parametre.
//

double searchx2(double a,graphe*pgraphe)
{
	int i;
	double xx=0;
	
	for(i=0;i<pgraphe->nbpts;i++)
	{
		if (pgraphe->y[i]==a)
		{
			xx=pgraphe->x[i];
			
			return xx;
		}			 
	}
	
	return xx;
}


//
/// Fonction de recherche d'une coordonnee en x avec sa coordonnee en y en parametre.
//

double searchx8(double a,graphe*pgraphe)
{
	int i;
	double xx=0;
	
	for(i=0;i<pgraphe->nbpts;i++)
	{
		if (pgraphe->y[i]==a)
		{
			xx=pgraphe->x[i];
			
			return xx;
		}			 
	}
	return xx;
}


//
/// Fonction de recherche d'une coordonnee en x avec sa coordonnee en y en parametre.
//

double searchx2w(double a,double *Xc,double *Yc,int h)
{
	int i;
	double xx=0;
	
	for(i=0;i<h;i++)
	{
		if (Yc[i]==a)
		{
			xx=Xc[i];
			
			return xx;
		}			 
	}
	
	return xx;
}


//
/// Calcul de l'amplitude maximum de la courbe croissante en x
//

double amplitude(int j , int k,double * Ymax , double * Ymin, graphe*pgraphe, double **tabinter,int nbi)
{
	int i,u;
	if (j<k) u=j;
	else u=k; 

	double ampl1[u+1]; 
	double ampl2[u+1];
	double ampld;
	
	if (pgraphe->y[0]>tabinter[1][0])
		ampld=pgraphe->y[0]-tabinter[1][0];

	else
		ampld=tabinter[1][0]-pgraphe->y[0];

	
	
	if (Ymax[0]>0)
	{
		for(i=0;i<u;i++)
		{
			ampl1[i]=Ymax[i]-Ymin[i];
			ampl2[i]=Ymax[i+1]-Ymin[i];
		}
	}
	
	
	else
	{
		for(i=0;i<u-1;i++)
		{
			ampl1[i]=Ymax[i]-Ymin[i];
			ampl2[i]=Ymax[i+1]-Ymin[i];
		}
	}
	
	//trouver la plus grand amplitude
	double z;
	double ampmax1=ampl1[0];
	double ampmax2=ampl2[0];
	
	for (i=1;i<u;i++)
	{
		if ((ampl1[i]>ampmax1) && (ampl1[i]>ampld) )
			ampmax1=ampl1[i];
		if ((ampl2[i]>ampmax2) && (ampl1[i]>ampld) )
			ampmax2=ampl2[i];
		else
			ampmax2=ampld;
	}
	
	
	if (ampmax1>ampmax2)
	{
		z=fabs(ampmax1);
	}
	
	else z=fabs(ampmax2);
	
	
	return z;
}


//
/// Calcul de l'amplitude maximum de la courbe decroissante en x
//
double amplitude2(int j , int k,double ** MAX , double ** MIN, double * Yc, double **tabinter,int nbi)
{
	int i;
	int u;
	
	if (j<k) 
		u=j;
	else 
		u=k; 

	double ampl1[u+1]; 
	double ampl2[u+1];
	double ampld;
	
	if (Yc[0]>tabinter[1][0])
		ampld=Yc[0]-tabinter[1][0];

	else
		ampld=tabinter[1][0]-Yc[0];
	
	
	if (MAX[1][0]>0)
	{
		for(i=0;i<u;i++)
		{
			ampl1[i]=MAX[1][i]-MIN[1][i];
			
			ampl2[i]=MAX[1][i+1]-MIN[1][i];
		}
	}
	
	else
	{
		for(i=0;i<u-1;i++)
		{
			ampl1[i]=MAX[1][i]-MIN[1][i];
			
			ampl2[i]=MAX[1][i+1]-MIN[1][i];
		}
	}	
	
	//trouver la plus grand amplitude
	double z;
	double ampmax1=ampl1[0];
	double ampmax2=ampl2[0];
	
	for (i=1;i<u;i++)
	{
		if ((ampl1[i]>ampmax1) && (ampl1[i]>ampld) )
			ampmax1=ampl1[i];
			
		if ((ampl2[i]>ampmax2) && (ampl1[i]>ampld) )
			ampmax2=ampl2[i];
			
		else
			ampmax2=ampld;
	}
	
	if (ampmax1>ampmax2)
	{
		z=fabs(ampmax1);
	}
	
	else z=fabs(ampmax2);
	
	
	return z;
}



//
//Arrondir les bornes d'une courbe
//

void arrond(double min ,double max, double * a , double * b)
{
	double nbre;
	double nbre01;
	double nbre02;
	double nbre2;
	int i=0;
	int j =0;
	int k =0;
	
	int v1;
	int v2;
	
	
	if(( fabs(max)>0) && ( fabs(max)<pow(10,j)))
		j++;
	
	while (fabs(max)>=pow(10,j)) 
		j++;
	
	
	
	if(( fabs(min)>0) && ( fabs(min)<pow(10,i)))
		i++;
	
	while (fabs(min)>=pow(10,i)) 
		i++;
	
	
	
	while (fabs(max - min)>=pow(10,k)) 
		k++;
	
	if (k!=0)
	{
		v1 = max * 1000000;
		v2 = min * 1000000;
		
		nbre = (int)fabs(v2) % ((int)pow(10,k-1)*1000000);
		nbre01 =nbre/1000000;

		nbre2 = (int)fabs(v1) % ((int)pow(10,k-1)*1000000);
		nbre02 =nbre2/1000000;
		
		if (min <0)
		{
		
			if(nbre01 != 0)
				*a = min + nbre01 - (int)pow(10,k-1) ;
			else
				*a = min;
		}
	
		else 
		{
			if (nbre01 != 0) 
				*a = min - nbre01 ;
		
			else
				*a = min  ;
		}
	
		if (max <0)
		{
		
			if(nbre02 != 0)
				if(max-nbre02==0)
					*b=0;
				else
					*b = max + nbre02 +(int)pow(10,i-1);
			else
				*b = max ;
		}
	
		else 
		{
			if (nbre02 != 0) 
				*b = max - nbre02 + (int)pow(10,j-1) ;
		
			else
				*b = max ;
		}

	}
	
	else
	{
		double m;
		i=1;
		j=1;
		while(fabs(max)<pow(0.1,j))
			j++;
		
		while(fabs(min)<pow(0.1,i))
			i++;
		
		while (fabs(max - min)>=pow(10,k)) 
			k++;
		
		
		v1 = max* pow(10,j)*1000000;
		v2 = min* pow(10,j)*1000000;
		
		m = pow(10,k-1);
		
		nbre = (int)fabs(v2) % (int)(m*1000000);
		nbre01 =nbre/1000000*pow(0.1,j);

		nbre2 = (int)fabs(v1) % ((int)m*1000000);
		nbre02 =nbre2/1000000*pow(0.1,i);
		
		if (min <0)
		{
		
			if(nbre01 != 0)
				*a = min + nbre01 - (int)pow(0.1,i) ;
			else
				*a = min;
		}
	
		else 
		{
			if (nbre01 != 0) 
				*a = min - nbre01 ;
		
			else
				*a = min  ;
		}
	
		if (max <0)
		{
		
			if(nbre02 != 0)
				if(max-nbre02==0)
					*b=0;
				else
					*b = max + nbre02 +(int)pow(0.1,j);
			else
				*b = max ;
		}
	
		else 
		{
			if (nbre02 != 0) 
				*b = max - nbre02 + (int)pow(0.1,j) ;
		
			else
				*b = max ;
		}
		
	}
	
		
	
	
		
		
	
	
}

//
// Trouve la meilleur valeur d'echantillonage
//

void echan(double a,double b ,int p, double * e)
{
	
	double diff;
	
	double ech ;

	double m; 
	
	int i = 0 ;
	
	int j = 0;
	
	
	diff= b-a;
	
	ech = diff / p ;
	

	lprintf("ech %lf  ",ech);
	
	
	while (ech>=pow(10,i)) 
	{
		i++;
	}
	
	m = pow(10,i-1);
	
	if (i==0)
	{
		while(fabs(ech)<pow(0.1,j))
			j++;
		
		m= pow(0.1,j);
	}
	
	

	
	if ((ech ==0 ) || (ech == 0.1 *m) || (ech == 0.2 *m ) || (ech ==0.25 *m) || (ech == 0.5 *m) || (ech == 1 *m) || (ech ==2 *m) || (ech == 2.5 *m)|| (ech ==5 *m))
		*e =ech;
		
	else
	{
		if ( ech <= 0.05 *m)
		*e = 0;
	
		if (( ech > 0.05*m) && ( ech <=0.125*m))
			*e = 0.1*m;
	
		if (( ech > 0.125*m) && ( ech <=0.175*m))
			*e = 0.15*m;
	
		if (( ech > 0.175*m) && ( ech <=0.225*m))
			*e = 0.2*m;
		
		if (( ech > 0.225*m) && ( ech <=0.275*m))
			*e = 0.25*m;
	
		if (( ech > 0.275*m) && ( ech <=0.75*m))
			*e = 0.5*m;
	
		if (( ech > 0.75*m) && ( ech <=1.5*m))
			*e = 1*m;
	
		if (( ech > 1.5*m) && ( ech <=1.75*m))
			*e = 1.5*m;
		
		if (( ech > 1.75*m) && ( ech <=2.25*m))
			*e = 2*m;
	
		if (( ech > 2.25*m) && ( ech <=3.75*m))
			*e = 2.5*m;
	
		if (( ech > 3.75*m) && ( ech <=7.5*m))
			*e = 5*m;
		
		if (( ech > 7.5*m) && ( ech <=15*m))
			*e = 10*m;
		
		if (( ech > 17.5*m) && ( ech <=22.5*m))
			*e = 15*m;
	
		if (( ech > 15*m) && ( ech <=22.5*m))
			*e = 20*m;
	
		if (( ech > 22.5*m) && ( ech <=27.5*m))
			*e = 25*m;
		
		if (( ech > 27.5*m) && ( ech <=75*m))
			*e = 50*m;
	
	}

	

}


