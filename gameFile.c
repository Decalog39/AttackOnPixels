#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include <stdio.h>
#include <stdbool.h>
#include "header.h"
#include <math.h>

void spawn();

void initializeProtagonist(){
    protagonist.life = true;
    protagonist.x = 64;
    protagonist.y = 16;
}

void initializeMonster(){
    int i;
    for(i = 0; i < sizeof(monsters)/sizeof(monsters[0]); i++){
        monsters[i].life = false;
        monsters[i].x = 0;
        monsters[i].y = 0;
    }
}

void initializeGungnir(){
    gungnir.x = protagonist.x;
    gungnir.y = protagonist.y;
}

void initializeUpgrade(){
    int i;
    for(i = 0; i < sizeof(upg)/sizeof(upg[0]); i++){
        upg[i].life = false;
        upg[i].x = 23;
        upg[i].y = 23;
    }
    
}

void respawnEveryMonster(){
int i;
for(i = 0; i < sizeof(monsters)/sizeof(monsters[0]); i++){
    spawn(&monsters[i]);
  }
}

void spawn(unit *u){
    u->life = true;
}

void kill(unit *u){
        u->life = false;
}

void youDied(unit *u){         //Function where it checkes if player has collided with mobs. If yes, fire extinguishes.
    if((u->x == protagonist.x && u->y == protagonist.y && u->life == true) || 
        (u->x == protagonist.x+1 && u->y == protagonist.y && u->life == true) ||
        (u->x == protagonist.x+1 && u->y == protagonist.y+1 && u->life == true) ||
        (u->x == protagonist.x+1 && u->y == protagonist.y-1 && u->life == true) ||
        (u->x == protagonist.x && u->y == protagonist.y+1 && u->life == true) ||
        (u->x == protagonist.x && u->y == protagonist.y-1 && u->life == true) ||
        (u->x == protagonist.x-1 && u->y == protagonist.y && u->life == true) ||
        (u->x == protagonist.x-1 && u->y == protagonist.y+1 && u->life == true) ||
        (u->x == protagonist.x-1 && u->y == protagonist.y-1 && u->life == true)){  
        protagonist.life = false;
    }
}

void killMob(unit *u, weapon *w){   //Function checks if projectile hit mobs
    if(w->x == u->x && w->y == u->y && u->life == true){
        u->life = false;
        mobKills++;
        killCount++;
        //score += 20;
        if(upgradeCap != 5){
            if(killCount == 5 + offset){
             upg[currentUpg].x = u->x;
                upg[currentUpg].y = u->y;
                upg[currentUpg].life = true;
            
             offset += 10;
               killCount = 0;
              if(currentUpg == sizeof(upg)/sizeof(upg[0])-1){
                 currentUpg = 0;
               } else {
                  currentUpg++;
              }
            }
        }
    }
}

void getUpgrade(){
    int i;
    for(i = 0; i < sizeof(upg)/sizeof(upg[0]); i++){
        if((protagonist.x == upg[i].x && protagonist.y == upg[i].y && upg[i].life) ||
            (protagonist.x == upg[i].x && protagonist.y && upg[i].y+1 && upg[i].life) ||
            (protagonist.x == upg[i].x && protagonist.y && upg[i].y+2 && upg[i].life) ||
            (protagonist.x == upg[i].x && protagonist.y && upg[i].y-1 && upg[i].life) ||
            (protagonist.x == upg[i].x && protagonist.y && upg[i].y-2 && upg[i].life) ||
            (protagonist.x == upg[i].x+2 && protagonist.y && upg[i].y && upg[i].life) ||
            (protagonist.x == upg[i].x+2 && protagonist.y && upg[i].y+1 && upg[i].life) ||
            (protagonist.x == upg[i].x+2 && protagonist.y && upg[i].y-1 && upg[i].life) ||
            (protagonist.x == upg[i].x+1 && protagonist.y && upg[i].y+2 && upg[i].life) ||
            (protagonist.x == upg[i].x+1 && protagonist.y && upg[i].y-2 && upg[i].life) ||
            (protagonist.x == upg[i].x-1 && protagonist.y && upg[i].y+2 && upg[i].life) ||
            (protagonist.x == upg[i].x-1 && protagonist.y && upg[i].y-2 && upg[i].life) ||
            (protagonist.x == upg[i].x-2 && protagonist.y && upg[i].y && upg[i].life) ||
            (protagonist.x == upg[i].x-2 && protagonist.y && upg[i].y+1 && upg[i].life) ||
            (protagonist.x == upg[i].x-2 && protagonist.y && upg[i].y-1 && upg[i].life)){
            psf++;
            upgradeCap++;
            upg[i].life = false;
        }
    }
}

void moveMobs(){
    int xDiff; 
    int yDiff;
    int i;
    for(i = 0; i < sizeof(monsters)/sizeof(monsters[0]); i++){
        if(monsters[i].life == true){
          xDiff = monsters[i].x - protagonist.x;
          yDiff = monsters[i].y - protagonist.y;

          if(xDiff < 0){                                           //If player is located in x-axis more positive side,
          monsters[i].x = monsters[i].x + mobSpeed;            //Moves x coordinate of mob by mobSpeed value towards positive side.
          } else if(xDiff > 0) {                                   //If player is located in x-axis more negative side,
          monsters[i].x = monsters[i].x - mobSpeed;            //moves x coordinate of mob by mobSpeed value towards negative side. 
          }

          if(yDiff < 0){                                          //If player is located in y-axis more positive side,
           monsters[i].y = monsters[i].y + mobSpeed;            //Moves y coordinate of mob by mobSpeed value towards postivie side.
           } else if(yDiff > 0) {                                   //If player is located in y-axis more negative side,   
            monsters[i].y = monsters[i].y - mobSpeed;            //moves y coordinate of mob by mobSpeed value towards negative side.
           }
        }


        youDied(&monsters[i]);
        killMob(&monsters[i], &gungnir);
    }

}

void moveProjectiles(){
    int xDiff; 
    int yDiff;
    if(target.x != 2222){
          xDiff = gungnir.x - target.x;
          yDiff = gungnir.y - target.y;

          if(xDiff < 0){                                           //If player is located in x-axis more positive side,
          gungnir.x = gungnir.x + projectileSpeed;            //Moves x coordinate of gungnir by projectileSpeed value towards positive side.
          } else if(xDiff > 0) {                                   //If player is located in x-axis more negative side,
          gungnir.x = gungnir.x - projectileSpeed;            //moves x coordinate of gungnir by projectileSpeed value towards negative side. 
          }

          if(yDiff < 0){                                          //If player is located in y-axis more positive side,
           gungnir.y = gungnir.y + projectileSpeed;            //Moves y coordinate of gungnir by projectileSpeed value towards postivie side.
           } else if(yDiff > 0) {                                   //If player is located in y-axis more negative side,   
            gungnir.y = gungnir.y - projectileSpeed;            //moves y coordinate of gungnir by projectileSpeed value towards negative side.
           }
    }

        int i;
         for(i = 0; i < sizeof(monsters)/sizeof(monsters[0]); i++){
         killMob(&monsters[i], &gungnir);
         }
    /*for(i = 0; i < sizeof(projectiles)/sizeof(projectiles[0]); i++){
        if(projectile[i].life == true){
          target = getClosestUnit();
          xDiff = projectiles[i].x - target.x
          yDiff = projectiles[i].y - target.y;

          if(xDiff < 0){                                           //If player is located in x-axis more positive side,
          projectiles[i].x = projectiles[i].x + projectileSpeed;            //Moves x coordinate of mob by mobSpeed value towards positive side.
          } else if(xDiff > 0) {                                   //If player is located in x-axis more negative side,
          projectiles[i].x = projectiles[i].x - projectileSpeed;            //moves x coordinate of mob by mobSpeed value towards negative side. 
          }

          if(yDiff < 0){                                          //If player is located in y-axis more positive side,
           projectiles[i].y = projectiles[i].y + projectileSpeed;            //Moves y coordinate of mob by mobSpeed value towards postivie side.
           } else if(yDiff > 0) {                                   //If player is located in y-axis more negative side,   
            projectiles[i].y = projectiles[i].y - projectileSpeed;            //moves y coordinate of mob by mobSpeed value towards negative side.
           }
        }
    */
}

unit getClosestUnit(){
    int xDiff2;
    int yDiff2;
    int distance;
    int minDist = 0;
    int i;
    int j = -123; 
    for(i = 0; i < sizeof(monsters)/sizeof(monsters[0]); i++){
        if(monsters[i].life){
            xDiff2 = (monsters[i].x - protagonist.x) * (monsters[i].x - protagonist.x);
            yDiff2 = (monsters[i].y - protagonist.y) * (monsters[i].y - protagonist.y);
            distance = xDiff2 + yDiff2;

            if(distance < minDist || j == -123){
            minDist = distance;
            j = i;
            } 
      }
    }
    if(j != -123){
    return monsters[j]; }
    else {
        return noTarget;}
} 

/*void setDirection(int *pc){          //Function so that direction of projectile face towards closest enemy. 
    unit enemy = getClosestUnit();
    projectiles[*pc].xd = enemy.x - protagonist.x;
    projectiles[*pc].yd = enemy.y - protagonist.y;
    projectiles[*pc].Direction = atan(projectiles[*pc].yd/projectiles[*pc].xd);
}

void moveProjectiles(){
    int i;
    for (i = 0; i < sizeof(projectiles)/sizeof(projectiles[0]); i++){
        if(projectiles[i].life){
            if(projectiles[i].xd < 0){
                    projectiles[i].x -= cos(projectiles[i].Direction);
                     projectiles[i].y -= sin(projectiles[i].Direction);
             } else {
                    projectiles[i].x += cos(projectiles[i].Direction);
                    projectiles[i].y += sin(projectiles[i].Direction);
                 }
        }
    }
} */

void projectileRemove(){
    int i;
   
        if(gungnir.x == 0 || gungnir.y == 0 ||
         gungnir.x == 127 || gungnir.y == 31){
         gungnir.x = protagonist.x;
         gungnir.y = protagonist.y;
     }
    

}

/*void updateNP(){
   int i;
    for(i = 0; i < sizeof(projectiles)/sizeof(projectiles); i++){
        if(!projectiles[i].life){
            projectiles[i].x = protagonist.x;
            projectiles[i].y = protagonist.y;
        }
    }

} */

int getbtn234(){
    int buttons234 = (PORTD >> 5) & 0x07;
	return buttons234; 
}

int getbtn1(){
    int button1 = ((PORTF >> 1) & 0x01);
    return button1;
}


