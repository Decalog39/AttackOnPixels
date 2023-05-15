#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include <stdio.h>
#include <stdbool.h>
#include "header.h"

int timeoutcount2;
int timeoutcount;
int difficulty = 30;
int worldClock = 0;

// Interrupt Service Routine
void user_isr( void )
{ 
	/*if (timeoutcount == 100) { 	 //Code so that every 10 seconds player will launch weapon.
		setDirection(&projectileCount);
		projectiles[projectileCount].life = true;		//Sets projectile to life, and this projectile will be moved in the gameWork function. 

		if(projectileCount == (sizeof(projectiles)/sizeof(projectiles[0])-1)){
			projectileCount = 0;
		} else {
			projectileCount++;
		}
		timeoutcount = 0;
	}
	else {
		timeoutcount++;
	} */

	 //Another code so that every 5 seconds mob will spawn at the edge of the screen. 
	
	
	if(timeoutcount2 == difficulty){
		if(!monsters[mobCount].life){
			monsters[mobCount].life = true; 
		}

		if((mobCount % 6) == 0 ) {
			monsters[mobCount].x = 0;
			monsters[mobCount].y = 0;	
		} else if ((mobCount % 6) == 1 ){
			monsters[mobCount].x = 0;
			monsters[mobCount].y = 31;	

		} else if ((mobCount % 6) == 2){
			monsters[mobCount].x = 127;
			monsters[mobCount].y = 0;	
		} else if ((mobCount % 6) == 3){
			monsters[mobCount].x = 127;
			monsters[mobCount].y = 31;	
		} else if ((mobCount % 6) == 4){
			monsters[mobCount].x = 64;
			monsters[mobCount].y = 31;	
		} else if ((mobCount % 6) == 5){
			monsters[mobCount].x = 64;
			monsters[mobCount].y = 0;	
		} 	

		  
		 
		if(mobCount == (sizeof(monsters)/sizeof(monsters[0])-1)){
			mobCount = 0;
		} else {
			mobCount++;
		}

		if(difficulty > 0){	 
			difficulty--; 
		}

		timeoutcount2 = 0;
	} else {
		timeoutcount2++;
	}

	
	IFS(0) = IFS(0) & 0xFFFFFEFF; //Set flag to 0.

}

int indicate;
//function that will be called once to initialize. 
void gameinit(void){
    initializeProtagonist();
	initializeMonster();
	initializeGungnir();
	initializeUpgrade();
	noTarget.life = false;
	noTarget.x = 2222;
	noTarget.y = 0;
    timeoutcount = 0;
	timeoutcount2 = 0;
	difficulty = 30;
	ps = 0;
	psf = 1;
	upgradeCount = 0;
	currentUpg = 0;
	offset = 0;
	score = 0;
	mobKills = 0;
	upgradeCap = 0;
	killCount = 0;
	TRISD = TRISD | 0x0E0;   //Enable button 4 3 and 2 (Set it as input)
	TRISF = TRISF | 0x002;   //Enable button 1 (Set it as input)
    indicate = 0;
	IPC(2) = 0x01F; //Set priority = 7, sub priority = 3;
	IEC(0) = IEC(0) | 0x0100; //Set time interrupt on.
	T2CON = 0x0;  //Set everything to 0.
	T2CON = T2CON | 0x00000070; //Turn scaling to 1:256, Turn bits(6:4)=111;
	T2CON = T2CON | 0x00008000; //Turn Bit15 = 1, TURN ON THE TIMERRRR!
	TMR2 = 0; //Setting timer value to 0. 
	PR2 = 31250; //Because scale is 1:256, it tickes 312500 in 1 sec. Since you want to tick 10 times, 80m/256/10.
	IFS(0) = IFS(0) & 0xFFFFFEFF; //Set flag to 0.
	projectileCount = 0;
	mobCount = 0;
	playerSpeed = 2;
	projectileSpeed = 1;
	mobSpeed = 1;
	display_init();
	display_reset(display);
	cleartext();
	enable_interrupt();
    return;
}

//function that will be repetitively called from main function. 
void gameWork(void){

if ((getbtn234() & 0x04) > 0) { //Button 4 
	if((protagonist.x-1) > 2){ 
	  protagonist.x -= playerSpeed;
		}

  }

  if ((getbtn234() & 0x02) > 0) { //Button 3 
 	 if((protagonist.y-1) > 2){
	  protagonist.y -= playerSpeed;
	  }
  }

  if ((getbtn234() & 0x01) > 0) { //Button 2 
		if((protagonist.y+1) < 29){
	  protagonist.y += playerSpeed; }

  }

  if((getbtn1() & 0x01) > 0){   //Button 1
		if((protagonist.x+1) < 125){
		protagonist.x += playerSpeed; }

  }
	target = getClosestUnit();
	while(ps < psf){
		moveProjectiles();
		ps++;
	}
	ps = 0;
    moveMobs();
	getUpgrade();
	projectileRemove();

	//updateNP();
}

void game_display(){
	display_reset(display);

	int i;
	for(i = 0; i < sizeof(monsters)/sizeof(monsters[0]); i++){    //Update display bits for each mobs
		if(monsters[i].life){
			display_pixel_on(monsters[i].x, monsters[i].y, display);
			display_pixel_on(monsters[i].x+2, monsters[i].y, display);
			display_pixel_on(monsters[i].x-2, monsters[i].y, display);
			display_pixel_on(monsters[i].x, monsters[i].y+1, display);
			display_pixel_on(monsters[i].x, monsters[i].y-1, display);
			display_pixel_on(monsters[i].x+1, monsters[i].y-1, display);
			display_pixel_on(monsters[i].x+1, monsters[i].y+1, display);
			display_pixel_on(monsters[i].x-1, monsters[i].y+1, display);
			display_pixel_on(monsters[i].x-1, monsters[i].y-1, display);
			display_pixel_on(monsters[i].x+2, monsters[i].y-1, display);
			display_pixel_on(monsters[i].x+2, monsters[i].y+1, display);
			display_pixel_on(monsters[i].x+2, monsters[i].y+2, display);
			display_pixel_on(monsters[i].x-2, monsters[i].y-1, display);
			display_pixel_on(monsters[i].x-2, monsters[i].y+1, display);
			display_pixel_on(monsters[i].x-2, monsters[i].y+2, display);
		}
	}

	/*int j; 
	for(j = 0; j < sizeof(projectiles)/sizeof(projectiles[0]); j++){	//Update display bits for weapon
		if(projectiles[j].life){
			display_pixel_on(projectiles[i].x, projectiles[i].y, display);
		}
	} */
	display_pixel_on(gungnir.x, gungnir.y, display);		//Update display for the gungnir
	display_pixel_on(gungnir.x+1, gungnir.y, display);
	display_pixel_on(gungnir.x+2, gungnir.y, display);
	display_pixel_on(gungnir.x-1, gungnir.y, display);
	display_pixel_on(gungnir.x-2, gungnir.y, display);
	display_pixel_on(gungnir.x, gungnir.y+1, display);
	display_pixel_on(gungnir.x, gungnir.y+2, display);
	display_pixel_on(gungnir.x, gungnir.y-1, display);
	display_pixel_on(gungnir.x, gungnir.y-2, display);


	display_pixel_on(protagonist.x-1, protagonist.y-1, display);	//upadate display for player
	display_pixel_on(protagonist.x-1, protagonist.y, display);
	display_pixel_on(protagonist.x-1, protagonist.y+1, display);
	display_pixel_on(protagonist.x, protagonist.y-1, display);
	display_pixel_on(protagonist.x, protagonist.y+1, display);
	display_pixel_on(protagonist.x+1, protagonist.y-1, display);
	display_pixel_on(protagonist.x+1, protagonist.y, display);
	display_pixel_on(protagonist.x+1, protagonist.y+1, display);

	int j;
	for(j = 0; j < sizeof(upg)/sizeof(upg[0]); j++) {
		if(upg[j].life){
			display_pixel_on(upg[j].x, upg[j].y, display);
			display_pixel_on(upg[j].x, upg[j].y+1, display);
			display_pixel_on(upg[j].x, upg[j].y+2, display);
			display_pixel_on(upg[j].x, upg[j].y-1, display);
			display_pixel_on(upg[j].x, upg[j].y-2, display);
			display_pixel_on(upg[j].x+1, upg[j].y, display);
			display_pixel_on(upg[j].x+1, upg[j].y+2, display);
			display_pixel_on(upg[j].x+1, upg[j].y-2, display);
			display_pixel_on(upg[j].x+2, upg[j].y, display);
			display_pixel_on(upg[j].x+2, upg[j].y+1, display);
			display_pixel_on(upg[j].x+2, upg[j].y-1, display);
			display_pixel_on(upg[j].x-1, upg[j].y, display);
			display_pixel_on(upg[j].x-1, upg[j].y+2, display);
			display_pixel_on(upg[j].x-1, upg[j].y-2, display);
			display_pixel_on(upg[j].x-2, upg[j].y, display);
			display_pixel_on(upg[j].x-2, upg[j].y+1, display);
			display_pixel_on(upg[j].x-2, upg[j].y-1, display);

		}

	}
	

	display_update(display);
}
