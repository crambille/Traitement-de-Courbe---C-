/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2016. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  data                             1
#define  data_GRAPH                       2       /* control type: graph, callback function: (none) */
#define  data_COULEUR2                    3       /* control type: color, callback function: (none) */
#define  data_COULEUR4                    4       /* control type: color, callback function: (none) */
#define  data_COULEUR3                    5       /* control type: color, callback function: (none) */
#define  data_COULEUR1                    6       /* control type: color, callback function: (none) */
#define  data_SORTIE                      7       /* control type: command, callback function: (none) */
#define  data_ENTREE                      8       /* control type: command, callback function: (none) */
#define  data_ZOOM                        9       /* control type: command, callback function: (none) */
#define  data_QUITTER                     10      /* control type: command, callback function: QUITTER */
#define  data_DECORATION_3                11      /* control type: deco, callback function: (none) */
#define  data_DECORATION                  12      /* control type: deco, callback function: (none) */
#define  data_DECORATION_2                13      /* control type: deco, callback function: (none) */
#define  data_STYLE2                      14      /* control type: ring, callback function: (none) */
#define  data_STYLE4                      15      /* control type: ring, callback function: (none) */
#define  data_STYLE3                      16      /* control type: ring, callback function: (none) */
#define  data_STYLE1                      17      /* control type: ring, callback function: (none) */
#define  data_sample                      18      /* control type: command, callback function: SAMPLE */
#define  data_getdeltamax                 19      /* control type: command, callback function: GETDELTAMAX */
#define  data_WINDOWS                     20      /* control type: command, callback function: windows */
#define  data_INFECTIONPOINTS             21      /* control type: command, callback function: infectionpoints */
#define  data_AMPLITUDEMAX                22      /* control type: command, callback function: amplitudemax */
#define  data_ENVELOPE                    23      /* control type: command, callback function: envelope */
#define  data_RESULTATS                   24      /* control type: textBox, callback function: (none) */
#define  data_SBAS                        25      /* control type: radioButton, callback function: (none) */
#define  data_SGAUCHE                     26      /* control type: radioButton, callback function: (none) */
#define  data_SDROITE                     27      /* control type: radioButton, callback function: (none) */
#define  data_SHAUT                       28      /* control type: radioButton, callback function: (none) */
#define  data_EBAS                        29      /* control type: radioButton, callback function: (none) */
#define  data_EGAUCHE                     30      /* control type: radioButton, callback function: (none) */
#define  data_EDROITE                     31      /* control type: radioButton, callback function: (none) */
#define  data_EHAUT                       32      /* control type: radioButton, callback function: (none) */
#define  data_Y2                          33      /* control type: numeric, callback function: (none) */
#define  data_DECORATION_5                34      /* control type: deco, callback function: (none) */
#define  data_Y1                          35      /* control type: numeric, callback function: (none) */
#define  data_X2                          36      /* control type: numeric, callback function: (none) */
#define  data_points                      37      /* control type: numeric, callback function: (none) */
#define  data_FREQUENCEE                  38      /* control type: numeric, callback function: (none) */
#define  data_X1                          39      /* control type: numeric, callback function: (none) */
#define  data_DECORATION_4                40      /* control type: deco, callback function: (none) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK amplitudemax(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK envelope(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK GETDELTAMAX(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK infectionpoints(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QUITTER(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SAMPLE(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK windows(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
