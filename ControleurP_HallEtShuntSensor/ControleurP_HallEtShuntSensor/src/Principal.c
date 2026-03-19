

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
#define Fe 1000.0 					/* [Hz] */
#define Te (1.0/Fe)					/* [sec] */


/*IT Functions*/
void IT_Principale(void);

/* LCD functions complémentaires*/
void LCD_ClearTopLine(void);
void LCD_ClearDownLine(void);




/*****************************************************************
**								MAIN Fonction 															  **
******************************************************************/


char MessageLCD[50];

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
