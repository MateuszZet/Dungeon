#include <stdio.h>
#include <gb/gb.h>
#include "back.c"
#include "obj.c"
#include "tit.c"	
#include "l1.c"
#include "over.c"
#include "win.c"

void checkInput();
void title_screen();
void level1();
void lvl1_col();
void win_screen();

int lvl1_colmap[323] = { //collision map
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};


UINT8 player[2];
UINT8 enemy[2], enemy2[2], enemy3[0];
UINT8 i, win_flag, lose_flag, flag3, key_flag, f=0;
UINT8 key1[1], key2[2], key3[2];
UINT8 door0[2], door1[2], door2[2];

unsigned int gridx, gridy, x, y, scrx, scry = 0;


void main() {


	
	begin:
	lose_flag = 0;
	NR52_REG = 0x8F;	// Turn on the sound
	NR51_REG = 0x11;	// Enable the sound channels
	NR50_REG = 0x77;	// Increase the volume to its max
	
	title_screen();
	
	
	level1();
	win_screen();
	
	if(lose_flag==1){
		DISPLAY_ON;
		lose_flag=0;
		set_bkg_data(0, 54, background);
		set_bkg_tiles(0,0,20,18, game_over);
		SHOW_BKG;
		waitpad(J_A);
		goto begin;
	}
	
	waitpad(J_A);
	goto begin;
	
		
		
}

void title_screen() {
	
	DISPLAY_ON;			// Turn on the display
	
	
	HIDE_SPRITES;
	SHOW_BKG;
	HIDE_WIN;
	
	
	set_bkg_data(0, 54, background);
	set_bkg_tiles(0,0,20,18, title);
	waitpad(J_START);
		NR10_REG = 0x34U;
		NR11_REG = 0x70U;
		NR12_REG = 0xF0U;
		NR13_REG = 0x0AU;
		NR14_REG = 0xC6U;
	HIDE_BKG;
	DISPLAY_OFF;
	 
	
}

void win_screen() {
	
	DISPLAY_ON;			// Turn on the display
	
	
	HIDE_SPRITES;
	SHOW_BKG;
	HIDE_WIN;
	
	
	set_bkg_data(0, 54, background);
	set_bkg_tiles(0,0,20,18, winner); 
	
}

void level1(){
	
	
	DISPLAY_ON;			// Turn on the display
	
	
	SHOW_SPRITES;
	SHOW_BKG;
	HIDE_WIN;
	set_bkg_data(0, 54, background);
	set_bkg_tiles(0,0,20,18, lvl1);
	set_sprite_data(0, 9, sprites);
	set_sprite_tile(0,1);
	set_sprite_tile(1,2);
	set_sprite_tile(2,2);
	set_sprite_tile(3,3);
	set_sprite_tile(4,4);
	
	player[0] = 15;
	player[1] = 15;
	//player[0] = 158;
	//player[1] = 136;
	
	enemy[0] = 144;
	enemy[1] = 136;
	
	enemy2[0] = 100;
	enemy2[1] = 50;
	
	key1[0] = 50;
	key1[1] = 100;
	
	door0[0] = 100;
	door0[1] = 100;
	
	while(win_flag!=1 && lose_flag!=1){
	
		
		checkInput();
		lvl1_col();
		wait_vbl_done();	
		
		
	}
	
	win_flag=0;
	HIDE_SPRITES;
	HIDE_BKG;
	DISPLAY_OFF;
}


UINT8 collisionCheck(UINT8 x1, UINT8 y1, UINT8 w1, UINT8 h1, UINT8 x2, UINT8 y2, UINT8 w2, UINT8 h2) {

	if ((x1 < (x2+w2)) && ((x1+w1) > x2) && (y1 < (h2+y2)) && ((y1+h1) > y2)) {
		
		return 1;
	
	} else {
		
		return 0;
	
	}

}

//long gridpos(int addx, int addy, int colmap[]){   //returns the current grid value relative to the player + (addx, addy)
//    gridx = (player[0]/8) + addx;
//    gridy = (player[1]/8) + addy;
//    return colmap[((20*gridy) + gridx)];
//}

int gridpos(int colmap[]){   //returns the current grid value
    gridx = (player[0]/8);
    gridy = (player[1]/8);
    return colmap[((19*gridy) + gridx)];
}

void checkInput() {

		scrx = player[0]+ 8;   //screen coordinates are offset from sprite's coordinates
        scry = player[1]+16;
		
        if(joypad() & J_SELECT){
            SHOW_WIN;
			HIDE_SPRITES;
        }
		
		if(joypad() & J_START){
            HIDE_WIN;
			SHOW_SPRITES;
        }
		
        if(joypad() & J_UP && gridpos(lvl1_colmap) == 0){
            player[1]--;
        }
		if(joypad() & J_UP && gridpos(lvl1_colmap) == 1){
            player[1]++;
        }
		
        if(joypad() & J_DOWN && gridpos(lvl1_colmap) == 0){
		player[1]++;
        }
		if(joypad() & J_DOWN && gridpos(lvl1_colmap) == 1){
		player[1]--;
        }
		
        if(joypad() & J_RIGHT && gridpos(lvl1_colmap) == 0){
		player[0]++;}
		if(joypad() & J_RIGHT && gridpos(lvl1_colmap) == 1){
		player[0]--;}
        
		
        if(joypad() & J_LEFT && gridpos(lvl1_colmap) == 0){
            player[0]--;
        }
		if(joypad() & J_LEFT && gridpos(lvl1_colmap) == 1){
            player[0]++;
        }



}

void lvl1_col(){
	
	//old player collision with walls
	//if (player[0] < 16){player[0]=16;}
	//if (player[0] > 152){player[0]=152;}
	//if (player[1] < 24){player[1]=24;}
	//if (player[1] > 144){player[1]=144;}
	
	//player movement
	move_sprite(0,scrx,scry);
	//enemy movement
	if(enemy2[0]<24){enemy2[0]=100;}
	
	//enemy movement
	move_sprite(2, enemy2[0]--, enemy2[1]);
	
	//set enemy flag
	if(enemy[0]==48){f=1;}
	if(enemy[0]==144){f=0;}
	//patrol move pattern -faster to right
	if(f==0){
		move_sprite(1, enemy[0]--, enemy[1]);
	}
	if(f==1){
	move_sprite(1, enemy[0]++, enemy[1]);
}
	//key and door movement
	move_sprite(3, key1[0], key1[1]);
	move_sprite(4, door0[0], door0[1]);
	

	//colision with enemy
	if(collisionCheck(scrx, scry, 8, 8, enemy[0], enemy[1], 8, 8) == 1) {
		set_sprite_tile(0,0);
		//win_flag=0; lose_flag=0; flag3=0;
		
		lose_flag=1;
		key_flag=0;
		
		NR10_REG = 0x1E;
		NR11_REG = 0x10;
		NR12_REG = 0xF3;
		NR13_REG = 0x00;
		NR14_REG = 0x87;
	} 
	
	//colision with enemy
	if(collisionCheck(scrx, scry, 8, 8, enemy2[0], enemy2[1], 8, 8) == 1) {
		set_sprite_tile(0,0);
		
		HIDE_SPRITES;
		lose_flag=1;
		key_flag=0;
		
		NR10_REG = 0x1E;
		NR11_REG = 0x10;
		NR12_REG = 0xF3;
		NR13_REG = 0x00;
		NR14_REG = 0x87;
	} 
	
	//colision with key
	if(collisionCheck(scrx, scry, 8, 8, key1[0], key1[1], 8, 8) == 1) {
		set_sprite_tile(3,0);
		set_sprite_tile(4,5);
		key_flag=1;
		
		NR10_REG = 0x4U;
		NR11_REG = 0xFEU;
		NR12_REG = 0xA1U;
		NR13_REG = 0x8FU;
		NR14_REG = 0x86U;
		key1[0]=200;
		key1[1]=200;
	} 
	
	
	
	//colision with door
	if((collisionCheck(scrx, scry, 8, 8, door0[0], door0[1], 8, 8) == 1) && key_flag == 1) {
		win_flag=1;
		key_flag=0;
		NR10_REG = 0x34U;
		NR11_REG = 0x80U;
		NR12_REG = 0xF0U;
		NR13_REG = 0x0AU;
		NR14_REG = 0xC6U;
		
	} 
}