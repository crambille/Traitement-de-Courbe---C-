#include <utility.h>
#include <analysis.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "DEB2.h"
#include "log.h"
#include "utilities.h"

#define TEST



//////////////////////////////////////////////////FONCTION GET DELTA MAX/////////////////////////////////////////////////////
//
//Recherche du point le plus loin de la droite de regretion lineaire
//Calcul de la distance verticale et orthogonale
//


#ifdef TEST

int Getdeltamax(graphe *pgraphe,double *x, double *y, double* d1, double *d2,double *AA,double*BB)

#else

int Getdeltamax(graphe *pgraphe,double *x, double *y, double* d1, double *d2)

#endif
{
	
	//Verifie que les pointeurs d'entres sont non nuls
	if ((x==NULL)||(y==NULL)||(d1==NULL)||(x==NULL)||(pgraphe==NULL))
	{
		return -1;
	}
	
	
	//Reservation de place pour calculer la covariance
	double * m=NULL;
	
	m=(double*)malloc2(pgraphe->nbpts*2*sizeof(double));
	
	
	//Calcule de la nouvelle colonne (xi*yi)
	int i;
	
	for(i=0;i<pgraphe->nbpts;i++)
	{
		m[i]=pgraphe->x[i]*pgraphe->y[i];
	}
	
	
	//Calcul de la somme des Xi*Y1
	double sxy=0;
	double c1;
	
	for(i=0;i<pgraphe->nbpts;i++)
	{
		sxy=m[i]+sxy;
	}
	
	c1=sxy/pgraphe->nbpts;
	
	
	//Calcul de la somme des Xi
	double sx=0;
	
	for(i=0;i<pgraphe->nbpts;i++)
	{
		sx=pgraphe->x[i]+sx;
	}
	
	
	//Calcul de la somme des Yi
	double sy=0;
	
	for(i=0;i<pgraphe->nbpts;i++)
	{
		sy=pgraphe->y[i]+sy;
	}
	
	
	//Calcule de la moyenne des Xi et Yi
	double mx;
	double my;
	double c2;
	
	mx=sx/(pgraphe->nbpts+1);
	
	my=sy/(pgraphe->nbpts+1);
	
	c2=mx*my;
	
	
	//Calcul de la covariance
	double C;
	C=c1-c2;
	
	//Reservation de la colonne des xi²
	double * x2=NULL;
	x2=(double*)malloc2(pgraphe->nbpts*2*sizeof(double));
	
	
	//Calcul de la colonne des x2
	for(i=0;i<pgraphe->nbpts;i++)
	{
		x2[i]=pgraphe->x[i]*pgraphe->x[i];
	}
	
	//Calcul de la somme des xi²
	double sx2=0;
	
	for(i=0;i<pgraphe->nbpts;i++)
	{
		sx2=x2[i]+sx2;
	}
	
	
	//Calcul de la variance 
	double V;
	V=(sx2/pgraphe->nbpts)-(mx*mx);
	
	//Calcul du coefficient A
	double A;
	
	if(V==0)	
		return -1;
		
	A=C/V;
	
	//Calcul de B
	double B;
	
	B=my-A*mx;
	
	#ifdef TEST 
	*AA=A;
	*BB=B;
	#endif
	
	
	//Reservation de memoire pour la droite de regretion lineaire
	double *d=NULL;
	d=(double*)malloc2(pgraphe->nbpts*2*sizeof(double));
	
	
	//Calcul des coordonnes de chaque point de la droite de regretion lineaire
	for(i=0;i<pgraphe->nbpts;i++)
	{
		d[i]=(A*pgraphe->x[i])+B;
	}
	
	
	//Recherche du points le plus eloigne de la droite
	double v;
	double min=0;
	double xmin;
	double	ymin;
	
	for(i=0;i<pgraphe->nbpts;i++)
	{
		if (fabs((pgraphe->y[i]-d[i]))>min)
		{
			min=pgraphe->y[i]-d[i];
			
			xmin=pgraphe->x[i];
			
			ymin=pgraphe->y[i];
			
			v=i;
		}
		
	}
	
	//Calcul de la distance max orthogonal 
	double dc;
	
	dc=(fabs((A*xmin)-ymin+B))/sqrt(1+(A*A));

							
							
	//liberation de la memoire
	free(m);
	free(x2);
	free(d);
	
	//Affectation
	*x = xmin;
	*y = ymin;
	*d1 = min;
	*d2 = dc;
	
return 0;
	
}

///////////////////////////////////////////////FONCTION WINDOWXS///////////////////////////////////////////////////////////////////////
//
//test booleen Fenetre
//
int Windows(graphe *pgraphe,fenetre * new)
{
	//Securité programme 
	if ((new->xfin-new->xdebut==0)||(pgraphe==NULL)||(new->yfin-new->ydebut==0))
		return -1;
	
	//reservation de memoire pour les points compris dans l'interval choisit
	int i=0;
	double ** inte=NULL;
	inte=(double **) malloc2(2*sizeof(double*));
	
	for(i=0;i<2;i++)
	{
		inte[i]=(double*)malloc2((new->xfin-new->xdebut)*sizeof(double));
	}
	
	//enregistrement des points de l'interval
	int h =0;
	
	for(i=0;i<new->nbpts;i++)
	{
		if ( (pgraphe->x[i]>(new->xdebut-10))&&(pgraphe->x[i]<(new->xfin+10))) //si les points sont compris dans l'interval enregistrer
		{
			inte[0][h]=pgraphe->x[i+1];
			
			inte[1][h]=pgraphe->y[i+1];
			
			h++;
		}
	}
	
	//Securité programme
	if (h==0)
	{
		//liberation de la memoire
		for ( i = 0 ; i<2 ; i++)
		{
	      free(inte[i]);
		  
	      inte[i] = NULL ;
		}
		
		free(inte);
		
		inte = NULL;
		
		
		return -1;
	}
	
	//Test windows
	int cpt1=0;
	int cpt2=0;
	double XC;
	double YC;
	
	//Cas impossible
	if (new->entree==new->sortie) 
	{
		//liberation de la memoire
		for ( i = 0 ; i<2 ; i++)
		{
	      free(inte[i]);
		  
	      inte[i] = NULL ;
		  
		}
		
		free(inte);
		
		inte = NULL;
		
		
		return -1;
	}
	
	//Cas 1 :gauche/droite
	if ((new->entree == 1) && (new->sortie == 2))
	{   
		for(i=0;i<h;i++)
		{
			if ((inte[1][i]<new->ydebut)||(inte[1][i]>new->yfin)) 
				cpt1 ++;
		}
		
		
		
		if ((new->yfin > inte[1][0]) && (inte[1][0] > new->ydebut) && (new->yfin > inte[1][h-1]) && (inte[1][h-1] > new->ydebut) && (cpt1 == 0) && (inte[0][0] < inte[0][1]))
		{
			//liberation de la memoire
			for ( i = 0 ; i<2 ; i++)
			{
		      free(inte[i]);
			  
		      inte[i] = NULL ;
			}
			
			free(inte);
			
			inte = NULL;
		
		
			return 0;
		}
		
		
		else 
		{
			//liberation de la memoire
			for ( i = 0 ; i<2 ; i++)
			{
		      free(inte[i]);
			  
		      inte[i] = NULL ;
			}
			
			free(inte);
			
			inte = NULL;
			
			
			return -1;
		}
	}
	
	
	//Cas 2 :droite/gauche
	if ((new->entree==2)&&(new->sortie==1))
	{   
		for(i=0;i<h;i++)
		{
			if ((inte[1][i]<new->ydebut)||(inte[1][i]>new->yfin)) 
				cpt1 ++;
		}
		
		
		if ((new->yfin>inte[1][0]) && (inte[1][0]>new->ydebut) && (new->yfin>inte[1][h-1]) && (inte[1][h-1]>new->ydebut)&&(cpt1==0)&&(inte[0][0]>inte[0][1]))
		{
			//liberation de la memoire
			for ( i = 0 ; i<2 ; i++)
			{
		      free(inte[i]);
			  
		      inte[i] = NULL ;
			}
			
			free(inte);
			inte = NULL;
			
			
			return 0;
		}
		
		else 
		{
			//liberation de la memoire
			for ( i = 0 ; i<2 ; i++)
			{
		      free(inte[i]);
			  
		      inte[i] = NULL ;
			}
			
			free(inte);
			
			inte = NULL;
			
			free(inte);
			
			
			return -1;
		}
	}
	
	
	//Cas 3:gauche/haut
	 if ((new->entree==1)&&(new->sortie==3))
	 {
		for(i=0;i<h;i++)
		{
			if ( (inte[1][i]<new->yfin)&&(inte[1][i+1]>new->yfin))	
				cpt1++;
				
			if ( (inte[1][i]>new->yfin)&&(inte[1][i+1]<new->yfin))	
				cpt1++;
				
			if ( (inte[1][i]>new->ydebut)&&(inte[1][i+1]<new->ydebut))	
				cpt2++;
				
			if ( (inte[1][i]<new->ydebut)&&(inte[1][i+1]>new->ydebut))	
				cpt2++;
		}
		
		
		if ( (cpt1 == 2) && (cpt2 == 1) && (inte[1][h-1] > new->yfin) && (inte[0][0] <= inte[0][1]))
		{
			//liberation de la memoire
			for ( i = 0 ; i<2 ; i++)
			{
		      free(inte[i]);
			  
		      inte[i] = NULL ;
			}
			
			free(inte);
			
			inte = NULL;
			
			free(inte);
			
			
			return 0;
		}
		
		
		
		else
		{
			//liberation de la memoire
			for ( i = 0 ; i<2 ; i++)
			{
		      free(inte[i]);
			  
		      inte[i] = NULL ;
			}
			free(inte);
			
			inte = NULL;
			
			
			free(inte);
			
			
			return -1;
		}
		
	 }
	 
	 
	 //Cas 4:gauche/haut
	 if ((new->entree == 1) && (new->sortie == 3))
	 {
		for(i=0;i<h;i++)
		{
			if ( (inte[1][i]<new->yfin)&&(inte[1][i+1]>new->yfin))	
				cpt1++;
				
			if ( (inte[1][i]>new->yfin)&&(inte[1][i+1]<new->yfin))	
				cpt1++;
				
			if ( (inte[1][i]>new->ydebut)&&(inte[1][i+1]<new->ydebut))	
				cpt2++;
				
			if ( (inte[1][i]<new->ydebut)&&(inte[1][i+1]>new->ydebut))	
				cpt2++;
		}
		
		
		if ( (cpt1 <= 2) && (cpt2 <= 1) && (inte[1][h-1] > new->yfin) && (inte[0][0] <= inte[0][1]) )
		{
			//liberation de la memoire
			for ( i = 0 ; i<2 ; i++)
			{
		      free(inte[i]);

		      inte[i] = NULL ;
			}
			
			
			free(inte);
			
			inte = NULL;
			

			return 0;
		}
		
		
		else
		{
			//liberation de la memoire
			for ( i = 0 ; i<2 ; i++)
			{
		      free(inte[i]);
			  
		      inte[i] = NULL ;
			}
			
			free(inte);
			
			inte = NULL;
			
		
			return -1;
		}
		
	 }
	 
	 
	 //Cas 5:haut/gauche
	 if ((new->entree == 3) && (new->sortie ==1 ))
	 {
		for(i=0;i<h;i++)
		{
			if ( (inte[1][i]<new->yfin)&&(inte[1][i+1]>new->yfin))	
				cpt1++;
				
			if ( (inte[1][i]>new->yfin)&&(inte[1][i+1]<new->yfin))	
				cpt1++;
				
			if ( (inte[1][i]>new->ydebut)&&(inte[1][i+1]<new->ydebut))
				cpt2++;
				
			if ( (inte[1][i]<new->ydebut)&&(inte[1][i+1]>new->ydebut))
				cpt2++;
		}
		
		
		if ( (cpt1 <= 2) && (cpt2 <= 1) && (inte[1][h-1] < new->yfin) && (inte[1][h-1] > new->ydebut) && (inte[0][1] <= inte[0][0]))
		{
			//liberation de la memoire
			for ( i = 0 ; i<2 ; i++)
			{
		      free(inte[i]);
			  
		      inte[i] = NULL ;
			}
			
			free(inte);
			
			inte = NULL;
			
			
			return 0;
		}
		else
		{
			//liberation de la memoire
			for ( i = 0 ; i<2 ; i++)
			{
		      free(inte[i]);
			  
		      inte[i] = NULL ;
			}
			
			free(inte);
			
			inte = NULL;
			
			
			
			return -1;
		}
	 }
	 
	 
	 //Cas 6:bas/gauche
	 if ((new->entree == 1) && (new->sortie == 4))
	 {
		for(i=0;i<h;i++)
		{
			if ( (inte[1][i]<new->yfin)&&(inte[1][i+1]>new->yfin))	
				cpt1++;
				
			if ( (inte[1][i]>new->yfin)&&(inte[1][i+1]<new->yfin))	
				cpt1++;
				
			if ( (inte[1][i]>new->ydebut)&&(inte[1][i+1]<new->ydebut))	
				cpt2++;
				
			if ( (inte[1][i]<new->ydebut)&&(inte[1][i+1]>new->ydebut))	
				cpt2++;
		}
		
		
		if ( (cpt1 <= 1) && (cpt2 <= 2) && (inte[1][h-1] > new->ydebut) && (inte[1][h-1] < new->yfin) && (inte[0][1] <= inte[0][0]))	
		{
			//liberation de la memoire
			for ( i = 0 ; i<2 ; i++)
			{
		      free(inte[i]);
			  
		      inte[i] = NULL ;
			}
			
			free(inte);
			
			inte = NULL;
			
			
			return 0;
		}
		
		
		else
		{
			//liberation de la memoire
			for ( i = 0 ; i<2 ; i++)
			{
		      free(inte[i]);
			  
		      inte[i] = NULL ;
			}
			
			free(inte);
			
			inte = NULL;
			
			
			return -1;
		}
		
	 }
	 
	 
	 //Cas 7:droite/haut
	 if ((new->entree == 2) && (new->sortie == 3))
	 {
		for(i=0;i<h;i++)
		{
			if ( (inte[1][i]<new->yfin)&&(inte[1][i+1]>new->yfin))	
				cpt1++;
				
			if ( (inte[1][i]>new->yfin)&&(inte[1][i+1]<new->yfin))	
				cpt1++;
				
			if ( (inte[1][i]>new->ydebut)&&(inte[1][i+1]<new->ydebut))	
				cpt2++;
				
			if ( (inte[1][i]<new->ydebut)&&(inte[1][i+1]>new->ydebut))	
				cpt2++;
		}
		
		
		if ( (cpt1 <= 2) && (cpt2 <= 1) && (inte[1][h-1] > new->yfin) && (inte[0][1] < inte[0][0]))
		{ 
			//liberation de la memoire
			for ( i = 0 ; i<2 ; i++)
			{
		      free(inte[i]);
			  
		      inte[i] = NULL ;
			}
			
			free(inte);
			
			inte = NULL;

			
			return 0;
		}
		
		
		else
		{
			//liberation de la memoire
			for ( i = 0 ; i<2 ; i++)
			{
		      free(inte[i]);
			  
		      inte[i] = NULL ;
			}
			
			free(inte);
			
			inte = NULL;
			
			
			return -1;
		}
	 }
	 
	 
	 //Cas 8:haut/droite
	 if ((new->entree == 3) && (new->sortie == 2))
	 {
		for(i=0;i<h;i++)
		{
			if ( (inte[1][i]<new->yfin)&&(inte[1][i+1]>new->yfin))	
				cpt1++;
				
			if ( (inte[1][i]>new->yfin)&&(inte[1][i+1]<new->yfin))	
				cpt1++;
				
			if ( (inte[1][i]>new->ydebut)&&(inte[1][i+1]<new->ydebut))	
				cpt2++;
				
			if ( (inte[1][i]<new->ydebut)&&(inte[1][i+1]>new->ydebut))	
				cpt2++;
		}
		
		
		if ( (cpt1 <= 2) && (cpt2 <= 1) && (inte[1][h-1] < new->yfin) && (inte[1][h-1] > new->ydebut) && (inte[0][0] <= inte[0][1]))
		{
			//liberation de la memoire
			for ( i = 0 ; i<2 ; i++)
			{
		      free(inte[i]);
			  
		      inte[i] = NULL ;
			}
			
			free(inte);
			
			inte = NULL;
			
			
			return 0;
		}
		
		
		else
		{
			//liberation de la memoire
			for ( i = 0 ; i<2 ; i++)
			{
		      free(inte[i]);
			  
		      inte[i] = NULL ;
			  
			}
			
			free(inte);
			
			inte = NULL;
			
			
			
			return -1;
		}
	 }
	 
	 
	 //Cas 9:droite/bas
	 if ((new->entree == 2) && (new->sortie == 4))
	 {
		for(i=0;i<h;i++)
		{
			if ( (inte[1][i]<new->yfin)&&(inte[1][i+1]>new->yfin))	
				cpt1++;
				
			if ( (inte[1][i]>new->yfin)&&(inte[1][i+1]<new->yfin))	
				cpt1++;
				
			if ( (inte[1][i]>new->ydebut)&&(inte[1][i+1]<new->ydebut))	
				cpt2++;
				
			if ( (inte[1][i]<new->ydebut)&&(inte[1][i+1]>new->ydebut))	
				cpt2++;
		}
		
		
		if ( (cpt1 <= 1)&&(cpt2 <= 2) && (inte[0][1] <= inte[0][0]))
		{
			//liberation de la memoire
			for ( i = 0 ; i<2 ; i++)
			{
		      free(inte[i]);
			  
		      inte[i] = NULL ;
			}
			
			free(inte);
			
			inte = NULL;
			
			
			return 0;
		}
		
		
		else
		{
			//liberation de la memoire
			for ( i = 0 ; i<2 ; i++)
			{
		      free(inte[i]);
			  
		      inte[i] = NULL ;
			}
			
			
			free(inte);
			
			inte = NULL;
			
			
			
			return -1;
		}
	 }
	 
	 
	 //Cas 10:bas/droite
	 if ((new->entree == 4) && (new->sortie == 2))
	 {
		for(i=0;i<h;i++)
		{
			if ( (inte[1][i]<new->yfin)&&(inte[1][i+1]>new->yfin))	
				cpt1++;
				
			if ( (inte[1][i]>new->yfin)&&(inte[1][i+1]<new->yfin))	
				cpt1++;
				
			if ( (inte[1][i]>new->ydebut)&&(inte[1][i+1]<new->ydebut))
				cpt2++;
				
			if ( (inte[1][i]<new->ydebut)&&(inte[1][i+1]>new->ydebut))
				cpt2++;
		}
		
		
		if ( (cpt1 <= 2) && (cpt2 <= 2) && (inte[0][0] <= inte[0][1]))
		{
			//liberation de la memoire
			for ( i = 0 ; i<2 ; i++)
			{
		      free(inte[i]);
			  
		      inte[i] = NULL ;
			}
			
			free(inte);
			
			inte = NULL;
			
			
			return 0;
		}
		
		
		else
		{
			//liberation de la memoire
			for ( i = 0 ; i<2 ; i++)
			{
		      free(inte[i]);
			  
		      inte[i] = NULL ;
			}
			
			free(inte);
			
			inte = NULL;
			
			
			return -1;
		}
	 }
	 
	 
	 //Cas 11:haut/bas
	 if ((new->entree == 3) && (new->sortie == 4))
	 {
		for(i=0;i<h;i++)
		{
			if ( (inte[1][i]<new->yfin)&&(inte[1][i+1]>new->yfin))	
				cpt1++;
				
			if ( (inte[1][i]>new->yfin)&&(inte[1][i+1]<new->yfin))	
				cpt1++;
				
			if ( (inte[1][i]>new->ydebut)&&(inte[1][i+1]<new->ydebut))	
				cpt2++;
				
			if ( (inte[1][i]<new->ydebut)&&(inte[1][i+1]>new->ydebut))	
				cpt2++;
		}
		
		
		if ( (cpt1 <= 2) && (cpt2 <= 2) && (inte[1][0] > new->yfin) && (inte[1][h-2] < new->ydebut))
		{
			//liberation de la memoire
			for ( i = 0 ; i<2 ; i++)
			{
		      free(inte[i]);
			  
		      inte[i] = NULL ;
			}
			
			free(inte);
		
			inte = NULL;
			
			
			return 0;
		}
		
		else
		{
			//liberation de la memoire
			for ( i = 0 ; i<2 ; i++)
			{
		      free(inte[i]);
			  
		      inte[i] = NULL ;
			}
			
			free(inte);
			
			inte = NULL;
			
			
			return -1;
		}
	 }
	 
	 
	 //Cas 12:bas/haut
	 if ((new->entree == 4) && (new->sortie == 3))
	 {
		for(i=0;i<h;i++)
		{
			if ( (inte[1][i]<new->yfin) && (inte[1][i+1] > new->yfin))	
				cpt1++;
				
			if ( (inte[1][i]>new->yfin)&&(inte[1][i+1]<new->yfin))	
				cpt1++;
				
			if ( (inte[1][i]>new->ydebut)&&(inte[1][i+1]<new->ydebut))	
				cpt2++;
				
			if ( (inte[1][i]<new->ydebut)&&(inte[1][i+1]>new->ydebut))	
				cpt2++;
		}
		
		
		if ( (cpt1 <= 2) && (cpt2 <= 2) && (inte[1][0] < new->ydebut) && (inte[1][h-2] > new->yfin))
		{
			//liberation de la memoire
			for ( i = 0 ; i<2 ; i++)
			{
		      free(inte[i]);
			  
		      inte[i] = NULL ;
			}
			
			free(inte);
			
			inte = NULL;
			
			
			return 0;
		}
		
		
		else
		{
			free(inte);
			
			return -1;
		}
	 }
	 
	 
	else
	{
		//liberation de la memoire
		for ( i = 0 ; i<2 ; i++)
		{
	      free(inte[i]);
		  
	      inte[i] = NULL ;
		}
		
		free(inte);
		
		inte = NULL;
		
		
		return -1;
	}
}




////////////////////////////////////////FONCTION POINTS D'INFLEXION///////////////////////////////////////////////////////////

//
// Fonction de recherche de point d'inflexion
//


#ifdef TEST

	int inflex(graphe * pgraphe ,double * x , double * y,double infx,double supx , int * m , double ** xxx,double ** yyy,double * p1 , double * p2)

#else
	
	int inflex(graphe * pgraphe ,double * x , double * y,double infx,double supx ,double * p1 , double * p2)

#endif

{
	
	if (((infx-supx)==0)||(supx<pgraphe->x[0]))
		return -1;
	
	
	//Comptage des points compris dans l'interval
	int s =0;
	int i;
	
	for(i=0;i<pgraphe->nbpts;i++)
	{
		if ((pgraphe->x[i]>infx)&&(pgraphe->x[i]<supx)) //si les points sont compris dans l'interval enregistrer
		{
			s++;
		}
	}
	
	
	// trouver le points max dans l'interval

	double maxptsy=-1000;
	double maxptsx=0 ;
	int indice2=0;
	
	for(i=0;i<pgraphe->nbpts;i++)
	{
		if ((pgraphe->x[i]>infx)&&(pgraphe->x[i]<supx)) //si les points sont compris dans l'interval enregistrer
		{
			if (pgraphe->y[i]>maxptsy)
			{
				maxptsy=pgraphe->y[i];
		
				maxptsx=pgraphe->x[i];
		
				indice2 =i;
		
			}
		}
	}
	
	//reservation de la memoire pour les points compris dans l'interval
	double * intex=NULL;
	intex=(double *) malloc((indice2+1)*sizeof(double));
	double * intey=NULL;
	intey=(double *) malloc((indice2+1)*sizeof(double));
	
	
	//Enregistrement des points compris dans l'interval
	int h=0;
	
	for(i=0;i<indice2;i++)
	{
		if ((pgraphe->x[i]>infx)&&(pgraphe->x[i]<supx)) //si les points sont compris dans l'interval enregistrer
		{
			intex[h]=pgraphe->x[i];
			
			intey[h]=pgraphe->y[i];
			
			h++;
		}
	}
	
	
	//calcul de la droite imaginaire premier point - le point max
	double a;
	double b; 
	
	double x01 = intex[h-1];
	double y01=  intey[h-1];
	double x02=  intex[0];
	double y02=  intey[0];
	
	a = (y01-y02)/(x01-x02); //calcul de la droite imaginaire premier point - le points max
	
	b = y01 - (a * x01);
	
	
	//Calcul de l'erreur
	
	double erreur[indice2+1];
	double droite[indice2+1];
	
	
	
	for (i=0;i<h;i++)
	{
		droite[i] = ( a *intex[i] ) + b;
		
		erreur[i] = fabs( intey[i]-droite[i]);
	
	}
	
	
	//Recherche de lerreur max
	double erreurmax = 0;
	int indice = 0;
		
	for (i=0;i<h;i++) 
	{
		  if (erreur[i]> erreurmax)
		  {
			  erreurmax = erreur[i];
			  
			  indice = i ;
		  }
		  
	}
	
	
	//Calcul des pentes avant et apres le point d'inflexion
	double pp1;
	double pp2;
	
	pp1 = (intey[indice]-y02)/(intex[indice]-x02);
	
	lprintf(" p1 %lf \t p2 \n%lf",y01,x01);
	lprintf(" p1 %lf \t p2 \n%lf",intey[indice],intex[indice]);
	
	pp2 = (intey[h-1]-intey[indice])/fabs(intex[h-1]-intex[indice]);
	
	lprintf("la pente avant le point d'inflexion : % lf",pp1);
		
	lprintf("la pente apres le point d'inflexion : % lf",pp2);	
	

	//Affectatation
	*x = intex[indice];
	*y = intey[indice];
	
#ifdef TEST
	
	double * f1;
	f1=(double *) malloc(2*sizeof(double));
	double * f2;
	f2=(double *) malloc(2*sizeof(double));
	
	
	f1[0]=x01;
	f1[1]=x02;
	f2[0]=y01;
	f2[1]=y02;
	

	*m=s;
	*xxx = f1;
	*yyy = f2;
	
#endif
	

	*p1=pp1;
	*p2=pp2;
	
	free(intex);
	free(intey);
	
	
	return 0;
}

/////////////////////////////////////////////FONCTION AMPLITUDE MAX///////////////////////////////////////////////////////////
//
//Calcul de l'amplitude maximum de la courbe version 1
//

#ifdef TEST 
	
int Amplitudemax1(graphe*pgraphe,int v,double **T1,double ** T2,double **W1,double ** W2,double *am,int *b1,int * b2,double ** Ymo)

#else

int Amplitudemax1(graphe*pgraphe,int v,double **T1,double ** T2,double **W1,double ** W2,double *am,int *b1,int * b2)

#endif
{
	//Securite programme
	if ((v==0)||(pgraphe->nbpts < v)||(pgraphe==NULL))
		return -1;
	
	
	//Reservation de la place mémoire pour le tableau de points de la moyenne glissante
	double *Ym = NULL;
	double **MAXi;
	double **MINi;
	Ym = (double *) malloc2 (pgraphe->nbpts*2 * sizeof(double )) ; 
	
	
	//Calcul de la moyenne glisante
	average(pgraphe,v,Ym);
	
	#ifdef TEST
	
	*Ymo=Ym;
	
	#endif
	
	
	//Compte le nombre d'intersection entre la moyenne glissante et la courbe reele
	int nbi;
	nbi=intersection(pgraphe,Ym);
	
	if (nbi==0)	
	{
		free(Ym);
		
		return -1;
	}
	
	
	//Reservation de la place mémoire pour le tableau de point d'intersection
	int i;
	double ** tabinter=NULL;
	tabinter=(double **) malloc2(2*sizeof(double*));
	
	for(i=0;i<2;i++)
	{
		tabinter[i]=(double*)malloc2(nbi*sizeof(double));
	}
	
	
	//Enregistrement des points d'intersections dans un tableau
	saveintersection(pgraphe,Ym,tabinter);
	
	
	//Reservation de la place mémoire pour le tableau min 
	MINi=(double **) malloc2(2*sizeof(double*));
	
	for(i=0;i<2;i++)
	{
		MINi[i]=(double*)malloc2(nbi*sizeof(double));
	}
	
	
	//Reservation de la place mémoire pour le tableau min 
	MAXi=(double **) malloc2(2*sizeof(double*));
	
	for(i=0;i<2;i++)
	{
		MAXi[i]=(double*)malloc2(nbi*sizeof(double));
	}
	
	
	//Recherche enregistrement des points maximum et minimum
	int j=0;
	int k=0;
	double v1;
	double v2;
	i=0;
	
	//1er parti de la courbe: calcul entre le premier point au premier point d'intersection
	v1=tabinter[0][0];
	
	if (pgraphe->x[0]<tabinter[0][0])//courbe croissante
	{
	
		MAXi[1][j]=searchmax2(0,v1,pgraphe);
		
		MAXi[0][j]=searchx2(MAXi[1][j],pgraphe);
		
		j++;
		
	}
	
	else //courbe decroissante
	{
		MINi[1][k]=searchmin(0,v1,pgraphe);
		
		MINi[0][k]=searchx2(MINi[1][k],pgraphe);
		
		k++;
	}	
	
	
	//2eme partie de la courbe: calcul entre chaque point d'intersection
	for(i=0;i<nbi-1;i++)
	{
		v1=tabinter[0][i];
		v2=tabinter[0][i+1];
		
		if ((tabinter[1][i]<tabinter[1][i+1]))/*&&(fabs(j-k)<2))*///courbe croissante
		{
			
			MAXi[1][j]=searchmax2(v1,v2,pgraphe);
			
			MAXi[0][j]=searchx2(MAXi[1][j],pgraphe);
			
			j++;
		}
		
		else //courbe decroissante
		{
			MINi[1][k]=searchmin(v1,v2,pgraphe);
			
			MINi[0][k]=searchx2(MINi[1][k],pgraphe);
			
			k++;
		}		
	}
	
	
	//3eme partie de la courbe:calcul entre le dermier point de la courbe et le dernier point d'intersection
	if (tabinter[1][nbi-1]<pgraphe->y[pgraphe->nbpts-1])//courbe croissante
	{
		MAXi[1][j]=pgraphe->y[pgraphe->nbpts-1];
		
		MAXi[0][j]=pgraphe->x[pgraphe->nbpts-1];
		
		j++;
	}
	
	else //courbe decroissante
	{
		MINi[1][k]=tabinter[1][nbi-1];
		
		MINi[0][k]=pgraphe->x[pgraphe->nbpts-1];
		
		k++;
	}
	
	//condition de securite
	if ((j==0)||(k==0))
	{
		printf("impossible !Il n'y a pas de point max et min");
		
		//liberation de la memoire
		for ( i = 0 ; i<2 ; i++)
		{
	      free(tabinter[i]);
		  
	      tabinter[i] = NULL ;
		}
		
		free(tabinter);
		
		tabinter = NULL;
		
		
		for ( i = 0 ; i<2 ; i++)
		{
	      free(MAXi[i]);
		  
	      MAXi[i] = NULL ;
		}
		
		free(MAXi);
		
		MAXi = NULL;
		
		
		
		for ( i = 0 ; i<2 ; i++)
		{
	      free(MINi[i]);
		  
	      MINi[i] = NULL ;
		}
		
		free(MINi);
		
		MINi = NULL;
	
		free(Ym);

		
		
		return -1;
	}
	
	
	
			
	double *Xmin=NULL;
	double *Ymin=NULL;
	double *Xmax=NULL;
	double *Ymax=NULL;
	Xmin=(double *) malloc2 ((k+2) * sizeof(double ));
	Ymin=(double *) malloc2 ((k+2) * sizeof(double ));
	Xmax=(double *) malloc2 ((j+2) * sizeof(double ));
	Ymax=(double *) malloc2 ((j+2) * sizeof(double ));

	int i1=0;
	int i2=0;
	//1er points

	if (pgraphe->y[v-1]>pgraphe->y[v])
	{
		Xmax[0]=pgraphe->x[v];
		
		Ymax[0]=pgraphe->y[v];
		
		i1++;
	}
	
	
	else
	{
		Xmin[0]=pgraphe->x[v];
		
		Ymin[0]=pgraphe->y[v];;
		
		i2++;
	}

	int q1=0;
	int yy=0;

	//2eme points a l'avant dernier  
	for (i =0; i < j; i++)//Pour max  
	{
		
		if(i1!=0)
		{
			
			if ((MAXi[0][i]>Xmax[0])&&(MAXi[0][i]<pgraphe->x[pgraphe->nbpts-1]))
			{
	
				Xmax[yy+i1]=MAXi[0][i];
			
				Ymax[yy+i1]=MAXi[1][i];
			
				q1++;
			
				yy++;
			} 
		}
		
		else 
		{
			if ((MAXi[0][i]>Xmin[0])&&(MAXi[0][i]<pgraphe->x[pgraphe->nbpts-1]))
			{
	
				Xmax[yy+i1]=MAXi[0][i];
			
				Ymax[yy+i1]=MAXi[1][i];
			
				q1++;
			
				yy++;
			} 
		}	

	}

	lprintf("\n");
	int q2=0;
	int rr=0;
	
	for (i = 0; i < k; i++)//Pour min  
	{	
		if(i1!=0)
		{
			if ((MINi[0][i]>Xmax[0])&&(MINi[0][i]<pgraphe->x[pgraphe->nbpts-1]))
			{
	
				Xmin[i2+rr]=MINi[0][i];
			
				Ymin[i2+rr]=MINi[1][i];
			
				q2++;
			
				rr++;

	
			}
		}
		
		else
		{
			if ((MINi[0][i]>Xmin[0])&&(MINi[0][i]<pgraphe->x[pgraphe->nbpts-1]))
			{
	
				Xmin[i2+rr]=MINi[0][i];
			
				Ymin[i2+rr]=MINi[1][i];
			
				q2++;
			
				rr++;

	
			}
		}	
	}
	
	
	if ((rr<=1)&&(yy<=1))
			//condition de securite
	if ((j==0)||(k==0))
	{
		printf("impossible !Il n'y a pas de point max et min");
		
		//liberation de la memoire
		for ( i = 0 ; i<2 ; i++)
		{
	      free(tabinter[i]);
		  
	      tabinter[i] = NULL ;
		}
		
		free(tabinter);
		
		tabinter = NULL;
		
		
		for ( i = 0 ; i<2 ; i++)
		{
	      free(MAXi[i]);
		  
	      MAXi[i] = NULL ;
		}
		
		free(MAXi);
		
		MAXi = NULL;
		
		
		
		for ( i = 0 ; i<2 ; i++)
		{
	      free(MINi[i]);
		  
	      MINi[i] = NULL ;
		}
		
		free(MINi);
		
		MINi = NULL;
		
	
		free(Ym);
		
		free(Xmin);
		
		free(Ymin);
	
		free(Xmax);
	
		free(Ymax);
		
	}
	
	//calcul de l'amplitude maximum
	double w;
	w=amplitude(rr+i2,yy+i1,Ymax,Ymin,pgraphe,tabinter,nbi);

	
	//Affectation
	
	*T1=Xmin;
	
	*T2=Ymin;
	
	*W1=Xmax;
	
	*W2=Ymax;
	
	*am=w; 
	
	*b1=rr;
	
	*b2=yy;
	
	
	//Resultat
	if ((rr>1)&&(yy>=1))
	{
		lprintf("\nil y a %d points maximum \n",yy+i1);
		
		for (i=0;i<yy+i1;i++)
		{
			lprintf("xmax%d:%lf\tymax%d:%lf\n",i,Xmax[i],i,Ymax[i]);	
		}
	
		lprintf("\nil y a %d points minimum \n",rr+i2);
		
		for (i=0;i<rr+i2;i++)
		{
			lprintf("xmin%d:%lf\tymin%d:%lf\n",i,Xmin[i],i,Ymin[i]);	
		}
	}
	
	
	//liberation de la memoire
	for ( i = 0 ; i<2 ; i++)
	{
      free(tabinter[i]);
	  
      tabinter[i] = NULL ;
	}
	free(tabinter);
	
	tabinter = NULL;

	for ( i = 0 ; i<2 ; i++)
	{
      free(MAXi[i]);
	  
      MAXi[i] = NULL ;
	}
	
	free(MAXi);
	
	MAXi = NULL;
	
	
	
	for ( i = 0 ; i<2 ; i++)
	{
      free(MINi[i]);
	  
      MINi[i] = NULL ;
	}
	
	free(MINi);
	
	MINi = NULL;
	
	
	free(Xmin);
	
	free(Ymin);
	
	free(Xmax);
	
	free(Ymax);
	
	free(Ym);
		
	return 0;	
}



//
//Calcul de l'amplitude maximum de la courbe version 2
//

#ifdef TEST
int Amplitudemax2(graphe*pgraphe,int v,double **T1,double ** T2,double **W1,double ** W2,double *am,int *b1,int * b2,double ** Ymo)
#else
int Amplitudemax2(graphe*pgraphe,int v,double **T1,double ** T2,double **W1,double ** W2,double *am,int *b1,int * b2)
#endif
{
	//Securite programme
	if ((v==0)||(pgraphe->nbpts < v)||(pgraphe==NULL))
		return -1;
	
	
	//Reservation de la place mémoire pour le tableau de points de la moyenne glissante
	double *Ym = NULL;
	double **MAXi;
	double **MINi;
	Ym = (double *) malloc2 (pgraphe->nbpts*2 * sizeof(double )) ; 
	
	
	//Calcul de la moyenne glisante
	average(pgraphe,v,Ym);
	
	#ifdef TEST
	*Ymo=Ym; 
	#endif

	//Compte le nombre d'intersection entre la moyenne glissante et la courbe reele
	int nbi;
	
	nbi=intersection(pgraphe,Ym);
	
	if (nbi==0)	
	{
		free(Ym);
		
		return -1;
	}
	
	
	//Reservation de la place mémoire pour le tableau de point d'intersection
	int i;
	double ** tabinter=NULL;
	
	tabinter=(double **) malloc2(2*sizeof(double*));
	
	for(i=0;i<2;i++)
	{
		tabinter[i]=(double*)malloc2(nbi*sizeof(double));
	}
	
	
	//Enregistrement des points d'intersections dans un tableau
	saveintersection(pgraphe,Ym,tabinter);
	
	//Reservation de la place mémoire pour le tableau min 
	MINi=(double **) malloc2(2*sizeof(double*));
	
	for(i=0;i<2;i++)
	{
		MINi[i]=(double*)malloc2(nbi*sizeof(double));   
	}
	
	
	//Reservation de la place mémoire pour le tableau min 
	MAXi=(double **) malloc2(2*sizeof(double*));
	
	for(i=0;i<2;i++)
	{
		MAXi[i]=(double*)malloc2(nbi*sizeof(double));
	}
	
	
	//Recherche enregistrement des points maximum et minimum
	int j=0;
	int k=0;
	double v1;
	double v2;
	i=0;
	
	
	//1er parti de la courbe: calcul entre le premier point au premier point d'intersection
	v1=tabinter[0][0];
	
	if (pgraphe->y[0]<tabinter[1][0])//courbe croissante
	{
		MAXi[1][j]=searchmax2(v1,0,pgraphe);
		
		MAXi[0][j]=searchx8(MAXi[1][j],pgraphe);
		
		j++;
		
	
	}
	else //courbe decroissante
	{
		MINi[1][k]=searchmin2(v1,0,pgraphe);
		
		MINi[0][k]=searchx2(MINi[1][k],pgraphe);
		
		k++;
	}
	
	
	//2eme partie de la courbe: calcul entre chaque point d'intersection
	for(i=0;i<nbi-1;i++)
	{
		v1=tabinter[0][i+1];
		
		v2=tabinter[0][i];
		
		
		if ((tabinter[1][i]<tabinter[1][i+1]))/*&&(fabs(j-k)<2))*///courbe croissante
		{
			MAXi[1][j]=searchmax2(v2,v1,pgraphe);
			
			MAXi[0][j]=searchx2(MAXi[1][j],pgraphe);
			
			j++;
		}
		
		
		else //courbe decroissante
		{
			
			MINi[1][k]=searchmin(v2,v1,pgraphe);
			
			MINi[0][k]=searchx2(MINi[1][k],pgraphe);
			
			k++;
		}	
	}
	
	
	//3eme partie de la courbe:calcul entre le dermier point de la courbe et le dernier point d'intersection
	if (tabinter[1][nbi-1]<pgraphe->y[pgraphe->nbpts-1])//courbe croissante
	{
		MAXi[1][j]=pgraphe->y[pgraphe->nbpts-1];
		
		MAXi[0][j]=pgraphe->x[pgraphe->nbpts-1];
		
		j++;
	}
	
	else //courbe decroissante
	{
		MINi[1][k]=tabinter[1][nbi-1];
		
		MINi[0][k]=pgraphe->x[pgraphe->nbpts-1];
		
		k++;
	}
	
	//condition de securite
	if ((j==0)||(k==0))
	{
		printf("impossible !Il n'y a pas de point max et min");
		//liberation de la memoire
		for ( i = 0 ; i<2 ; i++)
		{
	      free(tabinter[i]);
		  
	      tabinter[i] = NULL ;
		}
		
		free(tabinter);
		
		tabinter = NULL;
		
		
		
		for ( i = 0 ; i<2 ; i++)
		{
	      free(MAXi[i]);
		  
	      MAXi[i] = NULL ;
		}
		
		free(MAXi);
		
		MAXi = NULL;
		
		
		
		for ( i = 0 ; i<2 ; i++)
		{
	      free(MINi[i]);
		  
	      MINi[i] = NULL ;
		}
		
		free(MINi);
		
		MINi = NULL;
		
		
		
		free(Ym);
		
		return -1;
	}
	

	//enregistrement des points			
	double *Xmin=NULL;
	double *Ymin=NULL;
	double *Xmax=NULL;
	double *Ymax=NULL;
	Xmin=(double *) malloc2 ((k+2) * sizeof(double ));
	Ymin=(double *) malloc2 ((k+2) * sizeof(double ));
	Xmax=(double *) malloc2 ((j+2) * sizeof(double ));
	Ymax=(double *) malloc2 ((j+2) * sizeof(double ));

	int i1=0;
	int i2=0;
	//1er points

	if (pgraphe->y[v-1]>pgraphe->y[v])
	{
		Xmax[0]=pgraphe->x[v];
		
		Ymax[0]=pgraphe->y[v];
		
		i1++;
	}
	
	
	else
	{
		Xmin[0]=pgraphe->x[v];
		
		Ymin[0]=pgraphe->y[v];;
		
		i2++;
	}

	int q1=0;
	int yy=0;

	//2eme points a l'avant dernier  
	for (i =i1; i < j; i++)//Pour max  
	{
		
		if(i1!=0)
		{
			
			if ((MAXi[0][i]<Xmax[0])&&(MAXi[0][i]>pgraphe->x[pgraphe->nbpts-1]))
			{
	
				Xmax[yy+i1]=MAXi[0][i];
			
				Ymax[yy+i1]=MAXi[1][i];
			
				q1++;
			
				yy++;
			} 
		}
		
		else 
		{
			if ((MAXi[0][i]<Xmin[0])&&(MAXi[0][i]>pgraphe->x[pgraphe->nbpts-1]))
			{
	
				Xmax[yy+i1]=MAXi[0][i];
			
				Ymax[yy+i1]=MAXi[1][i];
			
				q1++;
			
				yy++;
			} 
		}	

	}

	lprintf("\n");
	int q2=0;
	int rr=0;
	
	for (i = i2; i < k; i++)//Pour min  
	{	
		if(i1!=0)
		{
			if ((MINi[0][i]<Xmax[0])&&(MINi[0][i]>pgraphe->x[pgraphe->nbpts-1]))
			{
	
				Xmin[i2+rr]=MINi[0][i];
			
				Ymin[i2+rr]=MINi[1][i];
			
				q2++;
			
				rr++;

	
			}
		}
		
		else
		{
			if ((MINi[0][i]<Xmin[0])&&(MINi[0][i]>pgraphe->x[pgraphe->nbpts-1]))
			{
	
				Xmin[i2+rr]=MINi[0][i];
			
				Ymin[i2+rr]=MINi[1][i];
			
				q2++;
			
				rr++;

	
			}
		}	
	}

	
	//calcul de l'amplitude maximum
	double w;
	w=amplitude(rr+i2,yy+i1,Ymax,Ymin,pgraphe,tabinter,nbi);

	
	//Affectation
	
	*am=w;
	
	*T1=Xmin;
	
	*T2=Ymin;
	
	*W1=Xmax;
	
	*W2=Ymax;
	
	/**am=w; */
	
	*b1=rr;
	
	*b2=yy;
	
	//Resultat
	if ((rr>1)&&(yy>=1))
	{
		lprintf("\nil y a %d points maximum \n",yy+i1);
		
		for (i=0;i<yy+i1;i++)
		{
			lprintf("xmax%d:%lf\tymax%d:%lf\n",i,Xmax[i],i,Ymax[i]);	
		}
	
		lprintf("\nil y a %d points minimum \n",rr+i2);
		
		for (i=0;i<rr+i2;i++)
		{
			lprintf("xmin%d:%lf\tymin%d:%lf\n",i,Xmin[i],i,Ymin[i]);	
		}
	}
		

	
	//liberation de la memoire
	for ( i = 0 ; i<2 ; i++)
	{
      free(tabinter[i]);
	  
      tabinter[i] = NULL ;
	}
	
	free(tabinter);
	
	tabinter = NULL;
	
	
	for ( i = 0 ; i<2 ; i++)
	{
      free(MAXi[i]);
	  
      MAXi[i] = NULL ;
	}
	
	free(MAXi);
	
	MAXi = NULL;
	
	
	
	for ( i = 0 ; i<2 ; i++)
	{
      free(MINi[i]);
	  
      MINi[i] = NULL ;
	}
	
	free(MINi);
	
	MINi = NULL;
	
	
	
	free(Xmin);
	
	free(Ymin);
	
	free(Xmax);
	
	free(Ymax);
	
	free(Ym);
	
		
	return 0;	
}



//////////////////////////////////////////////////////////ENVELOPE////////////////////////////////////////////////////////////

//
//Fonction complementaire(utilisé mais deja ecrite dans le programme)
//typedef graphe
//
int envelop(graphe * pgraphe,int N,int M, double C1[2][N], double C2[2][M])
{
	if((N==0)||(M==0)||(pgraphe==NULL))	return -1;
	int i;
	int j;
	int cpt=0;
	
	for(i=0;i<N-1;i++)
	{
		//Calcul de chaque equation de droite de la courbe C1
		double a1;
		int b1;
		
		a1=(C1[1][i+1]-C1[1][i])/(C1[0][i+1]-C1[0][i]);
		b1=C1[1][i]-(C1[0][i]*a1);
		
		for(j=0;j<pgraphe->nbpts;j++)
		{
			if((pgraphe->x[j]>C1[0][i])&&(pgraphe->x[j]<C1[0][i+1]))
			{
				double y1;
				
				y1=(pgraphe->x[j]*a1)+b1; //Calcul de chaque point sur notre droite pour la comparer a la courbe reeel
				
				if (y1<pgraphe->y[j])	
					cpt++;
			}
		}
	}
	
	
	for(i=0;i<M-1;i++)
	{
		//Calcul de chaque equation de droite de la courbe C2
		double a2;
		int b2;
		
		a2=(C2[1][i+1]-C2[1][i])/(C2[0][i+1]-C2[0][i]);
		b2=C2[1][i]-(C2[0][i]*a2);
		
		for(j=0;j<pgraphe->nbpts;j++)
		{
			if((pgraphe->x[j]>C2[0][i])&&(pgraphe->x[j]<C2[0][i+1]))
			{
				double y2;
				
				y2=(pgraphe->x[j]*a2)+b2;//Calcul de chaque point sur notre droite pour la comparer a la courbe reeel
				
				if (y2>pgraphe->y[j])	
					cpt++;
			}
		}
	}
	
	
	if(cpt!=0) 
		return -1;
	
	
	return 0;
}


//////////////////////////////////////////////////////////ECHANTILLONAGE//////////////////////////////////////////////////////////// 

int echantillon(double mindebut , double maxdebut , int p , double * minfin , double * maxfin, double * ec)
{
	
	double min=0 ;
	double max=0;
	double result=0;
	
	
	if (( p == 0 )|| (maxdebut - mindebut == 0) || (maxdebut<mindebut))
		
		return -1;
	
	
	else 
	{
		arrond(mindebut,maxdebut, &min,&max );
	
		lprintf("min :%lf \tmax :%lf" ,min,max); 
	
	
		echan(min , max ,p , &result);
	
		lprintf("frequence conseillée : %lf",result);
		
		*minfin = min;
		
		*maxfin = max;
		
		*ec = result;
		
		return 0;
	}
	
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int panelHandle;

int __stdcall WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpszCmdLine, int nCmdShow)
{
	if (InitCVIRTE (hInstance, 0, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "DEB2.uir", data)) < 0)
		return -1;
	//textbox
	InitLog( panelHandle, data_RESULTATS );
	EnableLog(  ) ;
	
	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle);
	CVIDynamicMemoryInfo ("", NULL, NULL, DYNAMIC_MEMORY_SHOW_ALLOCATED_MEMORY);
	return 0;
}

int CVICALLBACK QUITTER (int panel, int control, int event,void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			QuitUserInterface (0);
			break;
	}
	return 0;
}

int CVICALLBACK windows (int panel, int control, int event,void *callbackData, int eventData1, int eventData2)
{
	int COULEUR1,COULEUR2,COULEUR3,COULEUR4,STYLE1,STYLE2,STYLE3,STYLE4,SHAUT,SBAS,SGAUCHE,SDROITE,EHAUT,EBAS,EGAUCHE,EDROITE;
	double X1,X2,Y1,Y2;
	switch (event)
	{
		case EVENT_COMMIT:
		LogClear(0); 
		DeleteGraphPlot (panelHandle, data_GRAPH, -1, VAL_IMMEDIATE_DRAW);
		
		GetCtrlVal (data, data_COULEUR1, &COULEUR1);
		GetCtrlVal (data, data_STYLE1, &STYLE1);
		GetCtrlVal (data, data_COULEUR2, &COULEUR2);
		GetCtrlVal (data, data_STYLE2, &STYLE2);
		GetCtrlVal (data, data_COULEUR3, &COULEUR3);
		GetCtrlVal (data, data_STYLE3, &STYLE3);
		GetCtrlVal (data, data_COULEUR4, &COULEUR4);
		GetCtrlVal (data, data_STYLE4, &STYLE4);
		
		GetCtrlVal (data, data_SHAUT, &SHAUT);
		GetCtrlVal (data, data_SBAS, &SBAS);
		GetCtrlVal (data, data_SGAUCHE, &SGAUCHE);
		GetCtrlVal (data, data_SDROITE, &SDROITE);
		GetCtrlVal (data, data_EHAUT, &EHAUT);
		GetCtrlVal (data, data_EBAS, &EBAS);
		GetCtrlVal (data, data_EGAUCHE, &EGAUCHE);
		GetCtrlVal (data, data_EDROITE, &EDROITE);
		
		GetCtrlVal (data, data_X1, &X1);
		GetCtrlVal (data, data_X2, &X2);
		GetCtrlVal (data, data_Y1, &Y1);
		GetCtrlVal (data, data_Y2, &Y2);
		
		DeleteGraphPlot (panelHandle, data_GRAPH, -1, VAL_DELAYED_DRAW);
		FILE * f;
		graphe *pgraphe;
		
		int nbl;
		f=fopen("1.txt","r");//ouverture du fichier
		
		
		//Verifie que le fichier est bien ouvert
		if (f==NULL)
		{
			printf("erreur lors de l'ouverture du fichier");
			exit(-1);
		}
		
		
		//compte le nombre de ligne du fichier
		nbl=count(f);
		rewind(f);
		
		
		//Reservation de la place pour le graphique
		pgraphe = NewGraphe();
		
		
		// extrait les coordonnes x et y du fichier texte
		extract1(f,nbl,pgraphe);
		PlotXY (panelHandle, data_GRAPH, pgraphe->x, pgraphe->y, nbl-2, VAL_DOUBLE, VAL_DOUBLE, STYLE1, VAL_EMPTY_SQUARE, VAL_SOLID, 1, COULEUR1);
		
		lprintf("windows");
		fenetre *new;
		
		
		//Choix de la taille d'une fenetre
		new=Newfenetre();
		new->nbpts=nbl-1;
		new->xdebut=X1;
		new->xfin=X2;
		new->ydebut=Y1;
		new->yfin=Y2;
	
		//parametre sortie
		if((SHAUT!=0)&&(SBAS==0)&&(SGAUCHE==0)&&(SDROITE==0))
			new->sortie=3;
		
		if((SBAS!=0)&&(SHAUT==0)&&(SGAUCHE==0)&&(SDROITE==0))	
			new->sortie=4;
		
		if((SGAUCHE!=0)&&(SHAUT==0)&&(SBAS==0)&&(SDROITE==0))	
			new->sortie=1;
		
		if((SDROITE!=0)&&(SHAUT==0)&&(SBAS==0)&&(SGAUCHE==0))	
			new->sortie=2;;
			
		//parametre entree
		if((EHAUT!=0)&&(EBAS==0)&&(EGAUCHE==0)&&(EDROITE==0))	
			new->entree=3;
		
		if((EBAS!=0)&&(EHAUT==0)&&(EGAUCHE==0)&&(EDROITE==0))	
			new->entree=4;
		
		if((EGAUCHE!=0)&&(EHAUT==0)&&(EBAS==0)&&(EDROITE==0))	
			new->entree=1;
		
		if((EDROITE!=0)&&(EHAUT==0)&&(EBAS==0)&&(EGAUCHE==0))	
			new->entree=2;
		
		//graphique fenetre
		double  xfV1[2],yfV1[2];
		xfV1[0]=X1;
		xfV1[1]=X1;
		yfV1[0]=Y1;
		yfV1[1]=Y2;
		
		PlotXY (panelHandle, data_GRAPH, xfV1, yfV1, 2, VAL_DOUBLE, VAL_DOUBLE, STYLE2, VAL_EMPTY_SQUARE, VAL_SOLID, 1, COULEUR2);
		
		
		double  xfV2[2],yfV2[2];
		xfV2[0]=X2;
		xfV2[1]=X2;
		yfV2[0]=Y1;
		yfV2[1]=Y2;
		
		PlotXY (panelHandle, data_GRAPH, xfV2, yfV2, 2, VAL_DOUBLE, VAL_DOUBLE, STYLE2, VAL_EMPTY_SQUARE, VAL_SOLID, 1, COULEUR2);
		
		
		double  xfH1[2],yfH1[2];
		xfH1[0]=X1;
		xfH1[1]=X2;
		yfH1[0]=Y1;
		yfH1[1]=Y1;
		
		
		PlotXY (panelHandle, data_GRAPH, xfH1, yfH1, 2, VAL_DOUBLE, VAL_DOUBLE, STYLE2, VAL_EMPTY_SQUARE, VAL_SOLID, 1, COULEUR2);
		
		double  xfH2[2],yfH2[2];
		xfH2[0]=X1;
		xfH2[1]=X2;
		yfH2[0]=Y2;
		
		yfH2[1]=Y2;
		
		PlotXY (panelHandle, data_GRAPH, xfH2, yfH2, 2, VAL_DOUBLE, VAL_DOUBLE, STYLE2, VAL_EMPTY_SQUARE, VAL_SOLID, 1, COULEUR2);
		
		//Test de la fonction 
		if ((Windows(pgraphe,new)==0)&&(nbl!=0))	
			lprintf("Cette courbe passe bien par l'entree et la sortie que vous avez choisit");
		else	
			lprintf("error");
			
	
			
	break;
	}
	
	return 0;
}

int CVICALLBACK infectionpoints (int panel, int control, int event,void *callbackData, int eventData1, int eventData2)
{
	int COULEUR1,COULEUR2,COULEUR3,COULEUR4,STYLE1,STYLE2,STYLE3,STYLE4; 
	switch (event)
	{
		double X1,X2,FREQUENCEE;
		case EVENT_COMMIT:
		LogClear(0); 
		DeleteGraphPlot (panelHandle, data_GRAPH, -1, VAL_IMMEDIATE_DRAW);
		
		GetCtrlVal (data, data_COULEUR1, &COULEUR1);
		GetCtrlVal (data, data_STYLE1, &STYLE1);
		GetCtrlVal (data, data_COULEUR2, &COULEUR2);
		GetCtrlVal (data, data_STYLE2, &STYLE2);
		GetCtrlVal (data, data_COULEUR3, &COULEUR3);
		GetCtrlVal (data, data_STYLE3, &STYLE3);
		GetCtrlVal (data, data_COULEUR4, &COULEUR4);
		GetCtrlVal (data, data_STYLE4, &STYLE4);
		GetCtrlVal (data, data_X1, &X1);
		GetCtrlVal (data, data_X2, &X2);
		GetCtrlVal (data, data_FREQUENCEE, &FREQUENCEE);
	
		
		FILE * f;
		graphe *pgraphe;
		int nbl;
		f=fopen("51.csv","r");//ouverture du fichier
		
		
		//Verifie que le fichier est bien ouvert
		if (f==NULL)
		{
			printf("erreur lors de l'ouverture du fichier");
			exit(-1);
		}
		
		
		//compte le nombre de ligne du fichier
		nbl=count(f);
		rewind(f);
		
		
		//Reservation de la place pour le graphique
		pgraphe = NewGraphe();
		
		
		// extrait les coordonnes x et y du fichier texte
		extract1(f,nbl,pgraphe);
		PlotXY (panelHandle, data_GRAPH, pgraphe->x, pgraphe->y, nbl-1, VAL_DOUBLE, VAL_DOUBLE, STYLE1, VAL_EMPTY_SQUARE, VAL_SOLID, 1, COULEUR1);

		
		//Choisir un interval
		
		int N=0;
		int nb;
		
		double i1;
		double i2;
		double p1;
		double p2 ;
		double XI = 0;
		double YI = 0;
		
		i1=X1;
		i2=X2;
		
		
		#ifdef TEST
		
		double * dr = NULL;
		double * drx = NULL;
		double * dry = NULL;
		int m;
		if((inflex(pgraphe ,&XI , &YI ,i1,i2 ,&m,&drx,&dry,&p1,&p2)==0)&&(nbl!=0))
		
		#else		
		
		if((inflex(pgraphe ,&XI , &YI ,i1,i2 ,&p1,&p2)==0)&&(nbl!=0))
			
		#endif		
		{
			
			lprintf("\nle point d'inflexion a pour coordonnee \nx: %lf \t y:%lf",XI,YI);
			
				double Xp[1];
				double Yp[1];
		
				Xp[0]=XI;
				Yp[0]=YI;
		
				PlotXY (panelHandle, data_GRAPH, Xp, Yp, 1, VAL_DOUBLE, VAL_DOUBLE, STYLE2, VAL_EMPTY_SQUARE, VAL_SOLID, 1, COULEUR2);
			
				#ifdef TEST 
				PlotXY (panelHandle, data_GRAPH, drx, dry, 2, VAL_DOUBLE, VAL_DOUBLE, STYLE4, VAL_EMPTY_SQUARE, VAL_SOLID, 1, COULEUR4);
				#endif
				
		}
		
		else
			lprintf("error !! \n");
		
		
			
	break;
	
	}
	
	return 0;
}

int CVICALLBACK amplitudemax (int panel, int control, int event,void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		int COULEUR1,COULEUR2,COULEUR3,COULEUR4,STYLE1,STYLE2,STYLE3,STYLE4;
		double FREQUENCE;
		
		
		case EVENT_COMMIT:
		LogClear(0); 
			DeleteGraphPlot (panelHandle, data_GRAPH, -1, VAL_IMMEDIATE_DRAW);
		
			GetCtrlVal (data, data_COULEUR1, &COULEUR1);
			GetCtrlVal (data, data_STYLE1, &STYLE1);
			GetCtrlVal (data, data_COULEUR2, &COULEUR2);
			GetCtrlVal (data, data_STYLE2, &STYLE2);
			GetCtrlVal (data, data_COULEUR3, &COULEUR3);
			GetCtrlVal (data, data_STYLE3, &STYLE3);
			GetCtrlVal (data, data_COULEUR4, &COULEUR4);
			GetCtrlVal (data, data_STYLE4, &STYLE4);
			GetCtrlVal (data, data_FREQUENCEE, &FREQUENCE);
		
			FILE * f;
			graphe *pgraphe;
			int nbl;
			f=fopen("Classeur2.csv","r");//ouverture du fichier
		
			
			//Verifie que le fichier est bien ouvert
			if (f==NULL)
			{
				printf("erreur lors de l'ouverture du fichier");
				exit(-1);
			}
		
		
			//compte le nombre de ligne du fichier
			nbl=count(f);
			rewind(f);
		
		
			//Reservation de la place pour le graphique
			pgraphe = NewGraphe();
		
		
			// extrait les coordonnes x et y du fichier texte
			extract1(f,nbl,pgraphe);
			lprintf("amplitudemax!");
			
			double **MAX=NULL;
			double **MIN=NULL;
			
			#ifdef TEST
			double * Ymo=NULL;
			#endif
			double *T1=NULL;
			double *T2=NULL;
			double *W1=NULL;
			double *W2=NULL;
			
			double am=0;
			double pp=0;
			
			int v=0;
			int q;
			int j;
			int k;
		
			//Affichage de la courbe
			v=30; //frequence
			PlotXY (panelHandle, data_GRAPH, pgraphe->x, pgraphe->y, nbl-1, VAL_DOUBLE, VAL_DOUBLE, STYLE1, VAL_EMPTY_SQUARE, VAL_SOLID, 1, COULEUR1); 
			
		
			//Test fonction
			double ni=0 ;
	
		
			if( pgraphe->x[0]<pgraphe->x[pgraphe->nbpts-1])
			{
				
				#ifdef TEST
		
				if ((nbl>v*2)&&(Amplitudemax1(pgraphe,v,&T1,&T2,&W1,&W2,&am,&j,&k,&Ymo)==0)&&(nbl!=0))
				
				#else
				
				if ((nbl>v*2)&&(Amplitudemax1(pgraphe,v,&T1,&T2,&W1,&W2,&am,&j,&k)==0)&&(nbl!=0))
					
				#endif
					
				{  
					int i;
					
					//Affichage resultat de l'amplitude
					lprintf("l'amplitude maximum est %lf\n",am);
					
					
					//affichage graphique des points min et max
					if ((j!=0)&&(j<5))
						PlotXY (panelHandle, data_GRAPH, T1, T2, j+1, VAL_DOUBLE, VAL_DOUBLE, STYLE4, VAL_EMPTY_SQUARE, VAL_SOLID, 1, COULEUR4);
					
					if((k!=0)&&(k<5))
						PlotXY (panelHandle, data_GRAPH, W1, W2, k+1, VAL_DOUBLE, VAL_DOUBLE, STYLE2, VAL_EMPTY_SQUARE, VAL_SOLID, 1, COULEUR2);
			
					if ((j!=0)&&(j>5))
						PlotXY (panelHandle, data_GRAPH, T1, T2, j, VAL_DOUBLE, VAL_DOUBLE, STYLE4, VAL_EMPTY_SQUARE, VAL_SOLID, 1, COULEUR4);
					
					if((k!=0)&&(k>5))
						PlotXY (panelHandle, data_GRAPH, W1, W2, k, VAL_DOUBLE, VAL_DOUBLE, STYLE2, VAL_EMPTY_SQUARE, VAL_SOLID, 1, COULEUR2);
	
				}
			}
		
			
			
			if (pgraphe->x[0]>pgraphe->x[pgraphe->nbpts-1])
			{
			
				#ifdef TEST
				
				if ((nbl>v*2)&&(Amplitudemax2(pgraphe,v,&T1,&T2,&W1,&W2,&am,&j,&k,&Ymo)==0)&&(nbl!=0))
			

				#else 
					
				if ((nbl>v*2)&&(Amplitudemax2(pgraphe,v,&T1,&T2,&W1,&W2,&am,&j,&k)==0)&&(nbl!=0))
					
				#endif
				{  
					int i;
					
					//Affichage resultat de l'amplitude
					lprintf("l'amplitude maximum est %lf\n",am);
					
					
					//affichage graphique  des points min et max
					if ((j!=0)&&(j<5))
						PlotXY (panelHandle, data_GRAPH, T1, T2, j+1, VAL_DOUBLE, VAL_DOUBLE, STYLE4, VAL_EMPTY_SQUARE, VAL_SOLID, 1, COULEUR4);
					
					if((k!=0)&&(k<5))
						PlotXY (panelHandle, data_GRAPH, W1, W2, k+1, VAL_DOUBLE, VAL_DOUBLE, STYLE2, VAL_EMPTY_SQUARE, VAL_SOLID, 1, COULEUR2);
			
					if ((j!=0)&&(j>5))
						PlotXY (panelHandle, data_GRAPH, T1, T2, j, VAL_DOUBLE, VAL_DOUBLE, STYLE4, VAL_EMPTY_SQUARE, VAL_SOLID, 1, COULEUR4);
					
					if((k!=0)&&(k>5))
						PlotXY (panelHandle, data_GRAPH, W1, W2, k, VAL_DOUBLE, VAL_DOUBLE, STYLE2, VAL_EMPTY_SQUARE, VAL_SOLID, 1, COULEUR2);
		
				} 
			}
			
			
			#ifdef TEST	
			
			int i;
			
			for (i=0;i<v;i++)
			{
				Ymo[i]=pgraphe->y[i];
			}
			
			//affichage graphique de la moyenne
			
			PlotXY (panelHandle, data_GRAPH, pgraphe->x, Ymo,pgraphe->nbpts-1, VAL_DOUBLE, VAL_DOUBLE, STYLE3, VAL_EMPTY_SQUARE, VAL_SOLID, 1, COULEUR3); 

			if ((nbl<v*2)&&((Amplitudemax2(pgraphe,v,&T1,&T2,&W1,&W2,&am,&j,&k,&Ymo)!=0)||(Amplitudemax1(pgraphe,v,&T1,&T2,&W1,&W2,&am,&j,&k,&Ymo)!=0)))

				
			#else
				
			if ((nbl<v*2)&&((Amplitudemax2(pgraphe,v,&T1,&T2,&W1,&W2,&am,&j,&k)!=0)||(Amplitudemax1(pgraphe,v,&T1,&T2,&W1,&W2,&am,&j,&k)!=0)))

			#endif
				
			lprintf("Boulette");	   
		
				
	break;
	}
	return 0;
}

int CVICALLBACK envelope (int panel, int control, int event,void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		int COULEUR1,COULEUR2,COULEUR3,COULEUR4,STYLE1,STYLE2,STYLE3,STYLE4; 
		case EVENT_COMMIT:
		LogClear(0); 
		DeleteGraphPlot (panelHandle, data_GRAPH, -1, VAL_IMMEDIATE_DRAW);
		
				
		GetCtrlVal (data, data_COULEUR1, &COULEUR1);
		GetCtrlVal (data, data_STYLE1, &STYLE1);
		GetCtrlVal (data, data_COULEUR2, &COULEUR2);
		GetCtrlVal (data, data_STYLE2, &STYLE2);
		GetCtrlVal (data, data_COULEUR3, &COULEUR3);
		GetCtrlVal (data, data_STYLE3, &STYLE3);
		GetCtrlVal (data, data_COULEUR4, &COULEUR4);
		GetCtrlVal (data, data_STYLE4, &STYLE4);
		
		
		FILE * f;
		graphe *pgraphe;
		int nbl;
		f=fopen("ouput2.txt","r");//ouverture du fichier
		
		
		//Verifie que le fichier est bien ouvert
		if (f==NULL)
		{
			printf("erreur lors de l'ouverture du fichier");
			exit(-1);
		}
		
		
		//compte le nombre de ligne du fichier
		nbl=count(f);
		rewind(f);
		
		
		//Reservation de la place pour le graphique
		pgraphe = NewGraphe();
		
		
		// extrait les coordonnes x et y du fichier texte
		extract1(f,nbl,pgraphe);
		PlotXY (panelHandle, data_GRAPH, pgraphe->x, pgraphe->y, nbl-2, VAL_DOUBLE, VAL_DOUBLE, STYLE1, VAL_EMPTY_SQUARE, VAL_SOLID, 1, COULEUR1);
		
		lprintf("Envelope!\n");
		
		double C1[2][6]={{100,200,300,400,450,500},{100,104,125,150,275,375}};
		double C2[2][7]={{100,200,300,400,500,520,560},{30,40,50,60,75,250,400}};
		double C1x[6];
		double C1y[6];
		double C2x[7];
		double C2y[7];
		int i;
		
		
		for(i=0;i<6;i++)
		{
			C1x[i]=C1[0][i];
			
			C1y[i]=C1[1][i];
		}
		
		
		for(i=0;i<7;i++)
		{
			C2x[i]=C2[0][i];
			
			C2y[i]=C2[1][i];
		}
		
		
		PlotXY (panelHandle, data_GRAPH,C1x,C1y,5, VAL_DOUBLE, VAL_DOUBLE, STYLE2, VAL_EMPTY_SQUARE, VAL_SOLID, 1, COULEUR2);
		PlotXY (panelHandle, data_GRAPH,C2x,C2y,6, VAL_DOUBLE, VAL_DOUBLE, STYLE3, VAL_EMPTY_SQUARE, VAL_SOLID, 1, COULEUR3);
		
		
		//Test fonction
		if((envelop(pgraphe,6,7,C1,C2)==0)&&(nbl!=0)) 	
			lprintf("BRAVO!!");
		
		else 
			lprintf("BOULETTE!!!");


	break;
	}
	return 0;
}

int CVICALLBACK GETDELTAMAX (int panel, int control, int event,void *callbackData, int eventData1, int eventData2)
{
	int COULEUR1,COULEUR2,COULEUR3,COULEUR4,STYLE1,STYLE2,STYLE3,STYLE4;
	switch (event)
	{
		case EVENT_COMMIT:
		LogClear(0); 
		DeleteGraphPlot (panelHandle, data_GRAPH, -1, VAL_IMMEDIATE_DRAW);
		
		GetCtrlVal (data, data_COULEUR1, &COULEUR1);
		GetCtrlVal (data, data_STYLE1, &STYLE1);
		GetCtrlVal (data, data_COULEUR2, &COULEUR2);
		GetCtrlVal (data, data_STYLE2, &STYLE2);
		GetCtrlVal (data, data_COULEUR3, &COULEUR3);
		GetCtrlVal (data, data_STYLE3, &STYLE3);
		GetCtrlVal (data, data_COULEUR4, &COULEUR4);
		GetCtrlVal (data, data_STYLE4, &STYLE4);
		
		FILE * f;
		graphe *pgraphe;
		int nbl;
		f=fopen("ouput.txt","r");//ouverture du fichier
		
		
		//Verifie que le fichier est bien ouvert
		if (f==NULL)
		{
			printf("erreur lors de l'ouverture du fichier");
			exit(-1);
		}
		
		
		//compte le nombre de ligne du fichier
		nbl=count(f);
		rewind(f);
		
		
		//Reservation de la place pour le graphique
		pgraphe = NewGraphe();
		
		
		// extrait les coordonnes x et y du fichier texte
		extract1(f,nbl,pgraphe);
		PlotXY (panelHandle, data_GRAPH, pgraphe->x, pgraphe->y, nbl-2, VAL_DOUBLE, VAL_DOUBLE, STYLE1, VAL_EMPTY_SQUARE, VAL_SOLID, 1, COULEUR1);
		
		lprintf("GetMaxDelta");
		
		
		//Test de la fonction
		double x;
		double y;
		double d1;
		double d2;
		double ptdx;
		double ptdy;
		
		#ifdef TEST
		
		double AA;
		double BB;
		
		#endif
		
		
		#ifdef TEST
		
		if(( Getdeltamax(pgraphe,&x,&y,&d1,&d2,&AA,&BB) == 0)||(nbl!=0))
			
		#else
		
		if(( Getdeltamax(pgraphe,&x,&y,&d1,&d2) == 0)||(nbl!=0)) 
				
		#endif
		{
			lprintf("\nx: %lf__y:%lf__distance verticale:%lf__distance orthogonale:%lf",x,y,d1,d2);
			
			//Affichage du point d'inflexion
			double v01[1];
			double v02[1];
			
			v01[0]=x;
			v02[0]=y;
			
			PlotXY (panelHandle, data_GRAPH,v01, v02,1, VAL_DOUBLE, VAL_DOUBLE, STYLE2, VAL_EMPTY_SQUARE, VAL_SOLID, 1, COULEUR2);
		
			
			#ifdef TEST	
			
			//Affichage distance vertivale
			double ptx[2]={x,x};
			double pty[2]={y,y-d1};
			
			PlotXY (panelHandle, data_GRAPH,ptx, pty,2, VAL_DOUBLE, VAL_DOUBLE, STYLE2, VAL_EMPTY_SQUARE, VAL_SOLID, 1, COULEUR2);
		

			//Reservation de memoire pour la droite de regretion lineaire
			double *d=NULL;
			d=(double*)malloc2(pgraphe->nbpts*2*sizeof(double));
		
		

			//AFFICHAGE droite
			int i;

			for(i=0;i<pgraphe->nbpts;i++)
			{
				d[i]=(AA*pgraphe->x[i])+BB;
			}
			
			PlotXY (panelHandle, data_GRAPH,pgraphe->x,d,pgraphe->nbpts, VAL_DOUBLE, VAL_DOUBLE, STYLE4, VAL_EMPTY_SQUARE, VAL_SOLID, 1, COULEUR4);

			
			//Affichage dictance orthogonale
			double v1[2];
			double v2[2];
			
			double W=((x-pgraphe->x[0])*(1)+(y-d[0])*(AA))/sqrt((AA)*(AA)+(-1)*(-1));
			
			v1[0]=pgraphe->x[0]+((W*(1))/sqrt((AA)*(AA)+(-1)*(-1)));
			
			v2[0]=d[0]+((W*(AA))/sqrt((AA)*(AA)+(-1)*(-1)));
		    
			v1[1]=x;
			
			v2[1]=y;
			
			PlotXY (panelHandle, data_GRAPH,v1,v2,2, VAL_DOUBLE, VAL_DOUBLE, STYLE3, VAL_EMPTY_SQUARE, VAL_SOLID, 1, COULEUR3);
			
			free(d);
			
			#endif 
		
		}
		
		else
			lprintf("error !! \n");
	break;
	}
	return 0;
}

int CVICALLBACK SAMPLE (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	double X1,X2,points;
	
	switch (event)
	{
		case EVENT_COMMIT:
		
		LogClear(0); 
		DeleteGraphPlot (panelHandle, data_GRAPH, -1, VAL_IMMEDIATE_DRAW);
		
		
		GetCtrlVal (data, data_X1, &X1);
		GetCtrlVal (data, data_X2, &X2);
		GetCtrlVal (data, data_points, &points);
		
		double i1;
		double i2;
		int f;
		
		i1=X1;
		
		i2=X2;
		
		f=points;
		
		
		double minfin=0 ;
		double maxfin=0;
		double ec=0;
	
		
		if (echantillon(i1 , i2 , f , &minfin ,&maxfin, &ec)!=0)
			
			lprintf("Error !!");
		
		break;
	}
	return 0;
}
