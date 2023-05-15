
#include <stdbool.h>


/* declare display functions */
void display_init(void);
void display_update(uint8_t *data);
void display_pixel_on(int x, int y, uint8_t *data);
void display_reset(uint8_t *data);
uint8_t spi_send_recv(uint8_t data);
void quicksleep(int cyc);

void display_black(uint8_t *data);
void cleartext(void);

extern const uint8_t const font[128*8];
uint8_t display[512];
char textbuffer[4][16];

void display_string(int line, char *s);

void display_update_string(void);

/* I2C EEPROM/highscore functions */
void i2c_init(void);
void wait(void);
void i2c_start(void);
void i2c_restart(void);
void i2c_stop(void);
void i2c_send(uint8_t data);
uint8_t i2c_receive(void);
void i2c_ack(void);
void i2c_nack(void);
void get_highscore_score(void);
void get_highscore_name(void);
void send_highscore(int score, char *s);
void showhighscore(void);
void input_initial(void);
void wipe_highscores(void);

int hss[5];
char hsn[5][4];

typedef struct {
    bool life;
    int x;
    int y;
} unit;

typedef struct {
    bool life;
    int x;
    int y;
} player;

typedef struct {
    int x;     
    int y;    
} weapon;

typedef struct {
    bool life;
    int x; 
    int y;
} upgrade;

unit monsters[500];

upgrade upg[10];

upgrade upgrades;

unit noTarget;

unit target;

int upgradeCap;

player protagonist;

//weapon projectiles[20];

weapon gungnir; 

int projectileCount;

int mobCount;

int currentUpg;

int projectileSpeed;

int ps;

int psf;

int playerSpeed;

int mobSpeed;

int score;

int killCount;

int upgradeCount;

int mobKills;

int offset;

void initializeProtagonist(void);

void initializeMonster(void);

void projectileRemove();

void initializeGungnir(void);

void initializeUpgrade(void);

void respawnEveryMonster(void);

void getUpgrade();

void spawn(unit* u);

void kill(unit* u);

void moveMobs();

void setDirection(int* pc);

void moveProjectiles();

unit getClosestUnit();

void youDied();

void updateNP();

void killMob();

int getbtn234();

int getbtn1();

void gameinit();

void gameWork();

void game_display();

void enable_interrupt();

void delay(int);
