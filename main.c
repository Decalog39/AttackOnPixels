#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "header.h"

#define EEPROM_ADRESS 0x50

int main() {
        /*
	  This will set the peripheral bus clock to the same frequency
	  as the sysclock. That means 80 MHz, when the microcontroller
	  is running at 80 MHz. Changed 2017, as recommended by Axel.
	*/

	SYSKEY = 0xAA996655;  /* Unlock OSCCON, step 1 */
	SYSKEY = 0x556699AA;  /* Unlock OSCCON, step 2 */
	while(OSCCON & (1 << 21)); /* Wait until PBDIV ready */
	OSCCONCLR = 0x180000; /* clear PBDIV bit <0,1> */
	while(OSCCON & (1 << 21));  /* Wait until PBDIV ready */
	SYSKEY = 0x0;  /* Lock OSCCON */
	
	/* Set up output pins */
	AD1PCFG = 0xFFFF;
	ODCE = 0x0;
	TRISECLR = 0xFF;
	PORTE = 0x0;
	
	/* Output pins for display signals */
	PORTF = 0xFFFF;
	PORTG = (1 << 9);
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200;
	
	/* Set up input pins */
	TRISDSET = (1 << 8);
	TRISFSET = (1 << 1);
	
	/* Set up SPI as master */
	SPI2CON = 0;
	SPI2BRG = 4;
	/* SPI2STAT bit SPIROV = 0; */
	SPI2STATCLR = 0x40;
	/* SPI2CON bit CKP = 1; */
        SPI2CONSET = 0x40;
	/* SPI2CON bit MSTEN = 1; */
	SPI2CONSET = 0x20;
	/* SPI2CON bit ON = 1; */
	SPI2CONSET = 0x8000;

	i2c_init();


	int gameState = 1;
	int buttonState;
	int asd = 1;
	int aasd = 1;
	char scdis[16];
	int i;
	while(gameState == 1){
		gameinit();
		aasd = 1;
		while(aasd == 1){
			gameWork();
			game_display();

			if(protagonist.life == false){
				asd = 1;
				score = mobKills;
		   		display_black(display);
				cleartext();
				display_string(0, "      GAME");
				display_string(1, "      OVER");
				scdis[0] = 'S';
				scdis[1] = 'c';
				scdis[2] = 'o';
				scdis[3] = 'r';
				scdis[4] = 'e';
				scdis[5] = ':';
				scdis[6] = score / 100 + '0';
        		scdis[7] = (score % 100) / 10 +'0';
        		scdis[8] = score % 10 + '0';
				for(i = 9; i < 16; i++){
					scdis[i] = ' ';
				}
				display_string(3, scdis);
				display_update(display);
				display_update_string();
				delay(1000);
				while(asd == 1) {
					buttonState = getbtn234();
					if ((getbtn234() & 0x04) > 0) { //Button 4	//Restart
						asd = 0;		
						aasd = 0;				 
					}

					if ((getbtn234() & 0x02) > 0) { //Button 3  //Go to leaderboard,
						asd = 0;
						aasd = 0;
						showhighscore();

	  				}

					if ((getbtn234() & 0x01) > 0) { //Button 2  // Add record onto leaderboard
						asd = 0;
						aasd = 0;
						input_initial();
		
					}

					if((getbtn1() & 0x01) > 0){   //Button 1  //Turnoff
						gameState = 0;
						asd = 0;
						aasd = 0;
						display_black(display);
						display_update(display);
 					}
 				}
			}
			delay(100);
		}			
	}
}
	


