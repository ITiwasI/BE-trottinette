

#include "main.h"
#include "Clock_F103.h"
#include "TimeManagement.h"

#include "LCD_Balance.h"
#include "StringFct.h"
#include "ToolBox_NRJ_uC_Interface.h"


/******************************************************************************
* File Name          : Principal.c 
* Author             : T Rocacher
* Version            : V0
* Date release			 : 21/01/2025
* Description        : Code de départ pour contrôleur de puissance Etudiant Hall Sensor						
* Processeur				 : STM32F103, 
* Outil Developpement: KEIL 5.40.0.3
* Documentation			 : _
********************************************************************************/



/* Fonctionnement de départ 
- L'ADC est configuré en polling (lancement/attente/lecture résultat)
- les valeurs 12bits peuvent être lues par les fonctions décrites dans ToolBox_NRJ_uC_Interface.h

- La PWM est assurée par TIM3, configuré en mode triangle à 20kHz
- les sorties sont complémentaires
- la résolution est de 1000pts 

- une interruption configurée pour claquer à Te est mise en place,
récupérée par le callback  IT_Principale


*/


/* Define*/
#define ft 200
#define Fe (9*ft) 					/* [Hz] */
#define Te (1.0/Fe)					/* [sec] */
#define R 1
#define L 2e-3
#define Si 120e-3
#define Ubatt 24
#define Kh (2*Ubatt)
#define C14 12e-9
#define R7 2.7e3
#define C2 12e-9
#define C5 22e-9
#define R5 10e3
#define R6 10e3
#define pi 3.14
#define Ki (R*2*pi*ft/(Kh*Si))
#define K ((L/R)*Ki)

/*IT Functions*/
void IT_Principale(void);

/* LCD functions complémentaires*/
void LCD_ClearTopLine(void);
void LCD_ClearDownLine(void);




/*****************************************************************
**								MAIN Fonction 															  **
******************************************************************/


char MessageLCD[50];
int ucons=0;
float cons=0;
int ui=0;
float i=0;
float erreur_act=0;
float erreur_mem=0;
float sigma_act=0;
float sigma_mem=0;
float alpha=0;

int main(void)

{
	/* Configuration Clock HSI si HSE ne fonctionne pas (pb quartz)*/
	Clock_F103_ConfMainClk64MHz(Clock_HSE_Quartz);
	/* intialisation Time pour pouvoir utiliser la fonction Time_Delay_x_ms*/
	Time_Init(); 
		
	/*======================= CONF IO  ============================*/
	Conf_Generale_IO_Carte();
  
	
	
	/*======================= CONF LCD ============================*/	
	/*LCD*/
	lcd_Init();
	Time_Delay_x_ms(300); /* indispensable pour que l'init du LCD se passe bien*/
	lcd_Clear();
	
	LCD_ClearTopLine();
	lcd_Print("Bienvenue ! ");
	LCD_ClearDownLine();

	/*============== Conf PWM  ==================*/
	Triangle(20.0);   /*  PWM en ref triangulaire 20kHz*/
	Active_Voie_PWM(1);
	Active_Voie_PWM(2);
	Inv_Voie(2);
	R_Cyc_1(500);
	R_Cyc_2(500);
	
	/*======================= CONF ADC ============================*/	
	Conf_ADC();
 
		

	/*======================= CONF TIM2 en interruption à Te     ===*/	 
	Conf_IT_Principale_TIM(TIM2,IT_Principale,Te*1000000.0); 
	
	/* ====================================================
	               Main loop		
		=====================================================*/ 
	while(1)
	{	
		
	}
	
	
}


/*****************************************************************
**								Fonction Interruptions 											  **
******************************************************************/

void IT_Principale(void)
{
	//ucons= Read_Cons();
		//ui= Read_I();
		ui=0;
		
		//cons=(3.3/4096)*ucons;
		cons=0.1;
		i=(3.3/4096)*ui;
		erreur_mem=erreur_act;
		erreur_act= cons - i;
		sigma_mem=sigma_act;
		sigma_act= sigma_mem+(erreur_act*(Ki*Te+2*K)+erreur_mem*(Ki*Te-2*K))/2;
	if (sigma_act>0.495){
		sigma_act=0.495;
	}
	if (sigma_act<-0.495) {
		sigma_act=-0.495;
	}
}



/*****************************************************************
**						Fonction 	autres..															  **
******************************************************************/



/* FUNCTIONS LCD SPECFICS*/
void LCD_ClearTopLine(void)
{
	lcd_Set_cursor(0,0);
	lcd_Print("                ");
	lcd_Set_cursor(0,0);
}

void LCD_ClearDownLine(void)
{
	lcd_Set_cursor(0,1);
	lcd_Print("                ");
	lcd_Set_cursor(0,1);
}
