//==============================================================================
//
// Title:       utilities.h
// Purpose:     A short description of the interface.
//
// Created on:  1/26/2015 at 3:54:10 PM by .
// Copyright:   . All Rights Reserved.
//
//==============================================================================

#ifndef __utilities_H__
#define __utilities_H__

#ifdef __cplusplus
    extern "C" {
#endif

//==============================================================================
// Include files

#include "cvidef.h"

//==============================================================================
// Constants

//==============================================================================
// Types

typedef struct     {
    char*   name;
	char* 	xchanelname;
	char*   ychanelname;
	int     pen;
	int		color;
	double	canvastop;
	double	canvasbottom;
	double	canvasleft;
	double	canvasright;
	int		panelHandle;
	int 	controlID;
	int     canvasheight;
	int     canvaswidth;
	int     nbpts;
	int     lastnbpts;
	double* x;
	double* y;
}  graphe ;


graphe* 	NewGraphe(void);
graphe*		FreeGraphe( graphe* pgraphe );


typedef struct{
	double xdebut;
	double xfin;
	double ydebut;
	double yfin;
	int nbpts;
	int entree;
	int sortie;
} fenetre;

fenetre* new;


//==============================================================================
// External variables

//==============================================================================
// Global functions
		
void *malloc2 (size_t memBlockSize) ;

int count(FILE * f);

void extract1(FILE * f,int nbl,graphe *pgraphe);

void affiche(double ** tab, int a)  ;

graphe* NewGraphe(void) ;

fenetre* Newfenetre(void);

double searchmax(double a,double b,double * Y);
 
double searchx(double a,int nbl,double * Y,double * X) ;

double searchy(double a,int nbl,double * Y,double * X);

void average( graphe*pgraphe,int v, double * Ym) ;

int intersection(graphe*pgraphe,double * Ym) ;

void saveintersection(graphe*pgraphe,double * Ym, double ** tabinter);

double searchmin(double a,double b,graphe*pgraphe) ;

double searchmin2(double b,double a,graphe*pgraphe);

double searchminw(double a,double b,double * Yc);

double searchmax2(double a,double b,graphe*pgraphe);
 
double searchmax8(double b,double a,graphe*pgraphe) ;

double searchmax2w(double a,double b,double * Yc);

double searchx2(double a,graphe*pgraphe);

double searchx8(double a,graphe*pgraphe) ;

double searchx2w(double a,double *Xc,double *Yc,int h) ;

double amplitude(int j , int k,double * Ymax , double * Ymin, graphe*pgraphe, double **tabinter,int nbi);

double amplitude2(int j , int k,double ** MAX , double ** MIN, double * Yc, double **tabinter,int nbi);

void arrond(double min ,double max, double * a , double * b);

void echan(double a,double b ,int p, double * e)  ;
 
 
 
 



#ifdef __cplusplus
    }
#endif

#endif  /* ndef __utilities_H__ */
