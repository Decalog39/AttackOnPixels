#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "header.h"

#define EEPROM_ADRESS 0x50

void i2c_init(void){
    I2C1CON = 0x0;
    I2C1BRG = 0x062;   //80MHz/(400*2) kHz - 2 (peripheral clock of 80 MHz) = 98 = 0x062
    I2C1STAT = 0x0;
    I2C1CONSET = 1 << 13;   //set stop in idle bit
    I2C1CONSET = 1 << 15;   //set I2C enable bit
}

void wait(void){
    while(I2C1STAT & (1 << 14));    //loop while transmit is in progress
    while(I2C1CON & 0x01F);         //loop while control bits are not cleared by the modules
}

void i2c_start(void){
    wait();
    I2C1CONSET = 1; //set start condition enable bit
    wait();
}

void i2c_restart(void){
    wait();
    I2C1CONSET = 1 << 1;    //set restart condition enable bit
    wait();
}

void i2c_stop(void){
    wait();
    I2C1CONSET = 1 << 2;    //set stop condition enable bit
    wait();
    delay(10);
}

void i2c_send(uint8_t data){
    wait();
    I2C1TRN = data;
    wait();
    while(I2C1STAT & (1 << 15));    //loop until acknowledge is received
}

uint8_t i2c_receive(void){
    wait();
    I2C1CONSET = 1 << 3;    //set receive enable bit
    wait();
    return I2C1RCV;
}

void i2c_ack(void){
    wait();
    I2C1CONSET = 1 << 4;    //set acknowledge seqeunce enable bit
    I2C1CONCLR = 1 << 5;    //clear acknowledge data bit
    wait();
}

void i2c_nack(void){
    wait();
    I2C1CONSET = 1 << 4;
    I2C1CONSET = 1 << 5;    //set acknowledge data bit
    wait();
}

void get_highscore_score(void){
    int i;
    for (i = 0; i < 5; i++){
        i2c_start();
        i2c_send(EEPROM_ADRESS << 1);
        i2c_send(0x0);
        i2c_send(i);
        i2c_start();
        i2c_send(EEPROM_ADRESS << 1 | 1);
        hss[i] = i2c_receive();
        i2c_nack();
        i2c_stop();
    }
}

void get_highscore_name(void){
    int i, j;
    for (i = 0; i < 5; i++){
        for(j = 0; j < 4; j++){
            i2c_start();
            i2c_send(EEPROM_ADRESS << 1);
            i2c_send(0x0 | (j+1));
            i2c_send(i);
            i2c_start();
            i2c_send(EEPROM_ADRESS << 1 | 1);
            hsn[i][j] = i2c_receive();
            i2c_nack();
            i2c_stop();
        }
    }
}

void send_highscore(int score, char *s){
    get_highscore_score();
    get_highscore_name();
    int hssnew[5];
    char hsnnew[5][4];
    int i, j, k;
    int offset = 0;
    for (i = 0; i < 5; i++){
        hssnew[i] = hss[i - offset];
        for(j = 0; j < 4; j++){
            hsnnew[i][j] = hsn[i - offset][j];
        }
        if(score > hss[i] && offset == 0){
            hssnew[i] = score;
            for(k = 0; k < 4; k++){
                hsnnew[i][k] = s[k];
            }
            offset = 1;
        }
    }

    int l, m;
    for (l = 0; l < 5; l++){
        i2c_start();
        i2c_send(EEPROM_ADRESS << 1);
        i2c_send(0x0);
        i2c_send(0x0 | l);
        i2c_send(0x0 | hssnew[l]);
        i2c_stop();
        for(m = 0; m < 4; m++){
            i2c_start();
            i2c_send(EEPROM_ADRESS << 1);
            i2c_send(0x0 | (m+1));
            i2c_send(0x0 | l);
            i2c_send(0x0 | hsnnew[l][m]);
            i2c_stop();
        }
    }
}

void showhighscore(void){
    int a = 1;
    int asd = 1;
    int buttonState;
    get_highscore_score();
    get_highscore_name();
    int offset = 0;  //for scrolling in highscore leaderboard
    char line2[16];
    char line3[16];
    char line4[16];
    int i, j, k;
    int l;
    
    while(a == 1){
        asd = 1;
        cleartext();
        display_black(display);
        display_string(0, "HIGHSCORE:");
        line2[0] = 49 + offset;
        line2[1] = '.';
        line2[2] = ' ';
        for (i = 0; i < 4; i++){
            line2[3+i] = hsn[0 + offset][i];
        }
        line2[7] = ' ';
        line2[8] = hss[0 + offset] / 100 + '0';
        line2[9] = (hss[0 + offset] % 100) / 10 +'0';
        line2[10] = hss[0 + offset] % 10 + '0';
        line3[0] = 50 + offset;
        line3[1] = '.';
        line3[2] = ' ';
        for (j = 0; j < 4; j++){
            line3[3+j] = hsn[1 + offset][j];
        }
        line3[7] = ' ';
        line3[8] = hss[1 + offset] / 100 + '0';
        line3[9] = (hss[1 + offset] % 100) / 10 +'0';
        line3[10] = hss[1 + offset] % 10 + '0';
        line4[0] = 51 + offset;
        line4[1] = '.';
        line4[2] = ' ';
        for (k = 0; k < 4; k++){
            line4[3+k] = hsn[2 + offset][k];
        }
        line4[7] = ' ';
        line4[8] = hss[2 + offset] / 100 + '0';
        line4[9] = (hss[2 + offset] % 100) / 10 +'0';
        line4[10] = hss[2 + offset] % 10 + '0';
        for (l = 11; l < 16; l++){
            line2[l] = ' ';
            line3[l] = ' ';
            line4[l] = ' ';
        }
        display_string(1, line2);
        display_string(2, line3);
        display_string(3, line4);
        display_update(display);
        display_update_string();
        while(asd == 1) {
			buttonState = getbtn234();
			if ((getbtn234() & 0x02) > 0 && offset < 2) { //Button 3 
				asd = 0;
                offset++;
	  		}

			if ((getbtn234() & 0x01) > 0 && offset > 0) { //Button 2 
			    asd = 0;
                offset--;
			}
 		}
        delay(500);
    }    
}

void input_initial(void){
    int abcd = 1;
    int asd = 1;
    int buttonState;
    int offset = 0;  //for scrolling
    char line2[16];
    char line3[16];
    char line4[16];
    char init[4];
    int i, j, k, l;
    int in = 0;
    
    while(abcd == 1){
        asd = 1;
        cleartext();
        display_black(display);
        display_string(0, "INITIALS(MAX 4):");
        line2[0] = 'S';
        line2[1] = 'E';
        line2[2] = 'L';
        line2[3] = 'E';
        line2[4] = 'C';
        line2[5] = 'T';
        line2[6] = ':';
        line2[7] = ' '; 
        line2[8] = 'A' + offset;
        if(offset == 26){
            line2[8] = ' ';
        }
        for (i = 0; i < 8; i++){
            line3[i] = ' ';
            line4[i] = ' ';
        }
        line3[8] = 'B' + offset;
        line4[8] = 'C' + offset;
        for(j = 9; j < 16; j++){
            line2[j] = ' ';
            line3[j] = ' ';
            line4[j] = ' ';
        }
        if (offset == 24){
            for(k = 0; k < 16; k++){
                line4[k] = ' ';
            }
        }
        if (offset == 25 | offset == 26){
            for(l = 0;l < 16; l++){
                line3[l] = ' ';
                line4[l] = ' ';
            }
        }
        display_string(1, line2);
        display_string(2, line3);
        display_string(3, line4);
        display_update(display);
        display_update_string();
        while(asd == 1) {
			buttonState = getbtn234();
            if ((getbtn234() & 0x04) > 0) { //Button 4 (go back)
                if(!(in == 0)){
                    asd = 0;		
				    in -= 1;
                    offset = 0;
                    delay(1000);
                }								 
			}
			if ((getbtn234() & 0x02) > 0 && offset < 26) { //Button 3 
				asd = 0;
                offset++;
	  		}

			if ((getbtn234() & 0x01) > 0 && offset > 0) { //Button 2 
			    asd = 0;
                offset--;
			}
            if((getbtn1() & 0x01) > 0){   //Button 1 (select button)
                init[in] = 'A' + offset;
                if (offset == 26){
                    init[in] = ' ';
                }
                offset = 0;
                in += 1;
                asd = 0;
                if(in == 4){
                    abcd = 0;
                    cleartext;
                    display_update_string;
                    delay(1000);
                }
                delay(500);
            }
 		}
        delay(500);
    }
    send_highscore(score, init);
    delay(500);
    showhighscore();    
}

void wipe_highscores(void){
    int l, m;
	for (l = 0; l < 5; l++){
        i2c_start();
        i2c_send(EEPROM_ADRESS << 1);
        i2c_send(0x0);
        i2c_send(0x0 | l);
        i2c_send(0x0 | 0);
        i2c_stop();
        for(m = 0; m < 4; m++){
            i2c_start();
            i2c_send(EEPROM_ADRESS << 1);
            i2c_send(0x0 | (m+1));
            i2c_send(0x0 | l);
            i2c_send(0x0 | ' ');
            i2c_stop();
        }
    }
}