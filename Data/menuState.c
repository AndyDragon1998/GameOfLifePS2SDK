/*****************************************************************************/
/*  Автор         		: Дракончик				                             */
/*  Приставка     		: Sony PlayStation 2					             */
/*  ОС			  		: OSDSYS											 */
/*  Язык	      		: GNU C                                              */
/*                                                                           */
/*  Содержимое файла	: Стадия главного меню		                         */
/*  Атрибут файла		: SOURCE                                             */
/*  Имя файла     		: menuState.c	                                     */
/*                                                                           */
/*****************************************************************************/


#include "include/gameState.h"
#include "include/menuState.h"
#include "include/overState.h"
#include "include/introState.h"
#include "include/musicManager.h"
#include "include/textToGraphics.h"
#include "include/pad.h"


/****** User External Files ********/
#include "extern/menuState.ext"

/****** User Table Files ***********/


/****** User Define Files ***********/
#include "define/menuState.def"

GSTEXTURE TitleImage; 

GSTEXTURE Title; 
GSTEXTURE Sphere1;
GSTEXTURE Sphere2;
GSTEXTURE Sphere3;
GSTEXTURE Sphere4;
BGM MenuMusicy;

int gameType; // 3 - 3 x 3 | 5 - 5 x 5 | 7 - 7 x 7

int selectedOption; // 0 - New Game 1 - Load Game 2- ShutDown game

int idleTimer;

// Animation of the Title screen sequence
float A, a;
float B, b;
float delta;
float t;

float X1position[9];
float Y1position[9];
float X2position[9];
float Y2position[9];
float X3position[9];
float Y3position[9];
float X4position[9];
float Y4position[9];


void MenuStart(GSGLOBAL* gsGlobal)
{
	idleTimer = 0; // Once the Idle timer reaches 6000 then restart the screen to prevent screen burn on CRT
	initMusicFormat();
	
	char temp[4096];
	strcpy(temp, relativePath);
	strcat(temp, PickMusic(1));
	MenuMusicy.fileName = temp;
	
	LoadMusic(&MenuMusicy);

	char tempy[4096];
	strcpy(tempy, relativePath);
	strcat(tempy, TITLEIMAGEPATH);
	gsKit_texture_png(gsGlobal, &TitleImage,tempy);
	
	char temp1[4096];
	strcpy(temp1, relativePath);
	strcat(temp1, TITLETEXT1);
	gsKit_texture_png(gsGlobal, &Title,temp1);
	
	char temp2[4096];
	strcpy(temp2, relativePath);
	strcat(temp2, TITLESPHERE1);
	gsKit_texture_png(gsGlobal, &Sphere1,temp2);
	
	char temp3[4096];
	strcpy(temp3, relativePath);
	strcat(temp3, TITLESPHERE2);
	gsKit_texture_png(gsGlobal, &Sphere2,temp3);
	
	char temp4[4096];
	strcpy(temp4, relativePath);
	strcat(temp4, TITLESPHERE3);
	gsKit_texture_png(gsGlobal, &Sphere3,temp4);
	
	char temp5[4096];
	strcpy(temp5, relativePath);
	strcat(temp5, TITLESPHERE4);
	gsKit_texture_png(gsGlobal, &Sphere4,temp5);

	
	SetUpFont(gsGlobal);
	
	selectedOption = 0;
	
	A = 100;
	B = 100;
	a = 5;
	b = 4;
	delta = 3.14/2;
	t = 0.0f;
	for(int i  = 8; i > 0; i--)
	{
		X1position[i] = 0;
		Y1position[i] = 0;
		X2position[i] = 0;
		Y2position[i] = 0;
		X3position[i] = 0;
		Y3position[i] = 0;
		X4position[i] = 0;
		Y4position[i] = 0;
	}
}

void MenuUpdate(GSGLOBAL* gsGlobal)
{
	// This part here plays the music
	PlayMusic(&MenuMusicy);
	idleTimer++; // Once the Idle timer reaches 6000 then restart the screen to prevent screen burn on CRT
	//printf("%d IdleTimer\n", idleTimer);
	// This is to prevent the screen burn. We force the game to reset
	if(idleTimer > 19000)
	{
		idleTimer = 0;
		StateMachineChange(&GameMachineState, &IntroState, gsGlobal);
	}
	
	// Change Menu
	if(PlaystationGamePad.UP_KEY_TAP && selectedOption != 0)
    {
    	selectedOption--;
    }
    else if(PlaystationGamePad.DOWN_KEY_TAP && selectedOption != 3)
    {
    	selectedOption++;
    }

	if(PlaystationGamePad.START_KEY_TAP)
    	{	
		switch(selectedOption)
		{
			// Start New Game
			case 0:
				gameType = 3; // 3 - 3 x 3 | 5 - 5 x 5 | 7 - 7 x 7
				StateMachineChange(&GameMachineState, &GameState, gsGlobal);
			break;
			// Start Co Op Game
			case 1: 
	   			gameType = 5; // 3 - 3 x 3 | 5 - 5 x 5 | 7 - 7 x 7
				StateMachineChange(&GameMachineState, &GameState, gsGlobal);
			break;
			// Settings
			case 2:
	   			gameType = 7; // 3 - 3 x 3 | 5 - 5 x 5 | 7 - 7 x 7
				StateMachineChange(&GameMachineState, &GameState, gsGlobal);
			break;
			// Shut Down
			case 3:
				GameRunning = 0;
				UnloadMusic(&MenuMusicy);
			break;
			
			default:
			
		}
	}
	
	t+=0.015f;
	
	for(int i  = 8; i > 0; i--)
	{
		X1position[i] = X1position[i-1];
		Y1position[i] = Y1position[i-1];
		X2position[i] = X2position[i-1];
		Y2position[i] = Y2position[i-1];
		X3position[i] = X3position[i-1];
		Y3position[i] = Y3position[i-1];
		X4position[i] = X4position[i-1];
		Y4position[i] = Y4position[i-1];
	}
	
	
	// Lossajous Curve
	X1position[0] = A * sin(a*t + delta);
    Y1position[0] = B * sin(b*t);
    X2position[0] = A * sin(a*(t+0.4f) + delta);
    Y2position[0] = B * sin(b*(t+0.4f));
    X3position[0] = A * sin(a*(t+0.8f) + delta);
    Y3position[0] = B * sin(b*(t+0.8f));
    X4position[0] = A * sin(a*(t+1.2f) + delta);
    Y4position[0] = B * sin(b*(t+1.2f));
	
    

}

void MenuDraw(GSGLOBAL* gsGlobal, u64 colour)
{

	/*int i = (4096 - gsGlobal->CurrentPointer/1024);
	printf("VRAM: %d\n", i);*/
	
	gsKit_prim_sprite_texture(gsGlobal, &TitleImage,0,  // X1
						0,  // Y2
						0.0f,  // U1
						0.0f,  // V1
						TitleImage.Width * 2, // X2
						TitleImage.Height * 2, // Y2
						TitleImage.Width * 2, // U2
						TitleImage.Height * 2, // V2
						2,
						colour);

    for(int i = 0; i < 9; i++)
	{
		gsKit_prim_sprite_texture(gsGlobal, &Sphere1,X1position[i] + 250,  // X1
						Y1position[i] + 150,  // Y2
						0.0f,  // U1
						0.0f,  // V1
						(Sphere1.Width + X1position[i]) + 250, // X2
						(Sphere1.Height + Y1position[i]) + 150, // Y2
						Sphere1.Width, // U2
						Sphere1.Height, // V2
						2,
						colour);
						
		gsKit_prim_sprite_texture(gsGlobal, &Sphere2,X2position[i] + 250,  // X1
						Y2position[i] + 150,  // Y2
						0.0f,  // U1
						0.0f,  // V1
						(Sphere2.Width + X2position[i]) + 250, // X2
						(Sphere2.Height + Y2position[i]) + 150, // Y2
						Sphere2.Width, // U2
						Sphere2.Height, // V2
						2,
						colour);
		gsKit_prim_sprite_texture(gsGlobal, &Sphere3,X3position[i] + 250,  // X1
						Y3position[i] + 150,  // Y2
						0.0f,  // U1
						0.0f,  // V1
						(Sphere3.Width + X3position[i]) + 250, // X2
						(Sphere3.Height + Y3position[i]) + 150, // Y2
						Sphere3.Width, // U2
						Sphere3.Height, // V2
						2,
						colour);
						
		gsKit_prim_sprite_texture(gsGlobal, &Sphere4,X4position[i] + 250,  // X1
						Y4position[i] + 150,  // Y2
						0.0f,  // U1
						0.0f,  // V1
						(Sphere4.Width + X4position[i]) + 250, // X2
						(Sphere4.Height + Y4position[i]) + 150, // Y2
						Sphere4.Width, // U2
						Sphere4.Height, // V2
						2,
						colour);
	}
						
	gsKit_prim_sprite_texture(gsGlobal, &Title,80,  // X1
						20,  // Y2
						0.0f,  // U1
						0.0f,  // V1
						Title.Width, // X2
						Title.Height, // Y2
						Title.Width, // U2
						Title.Height, // V2
						2,
						colour);
						
						
						



	switch(selectedOption)
	{
		// Start New Game
		case 0:
			DrawText(120, 360, "3 { 3",2, gsGlobal, &colour,1);
			DrawText(120, 385, "5 { 5",2, gsGlobal, &colour,0);
			DrawText(120, 410, "7 { 7",2, gsGlobal, &colour,0);
			DrawText(120, 435, "PFDTHIBNM BUHE",2, gsGlobal, &colour,0);
		break;
		case 1: 
	  		DrawText(120, 360, "3 { 3",2, gsGlobal, &colour,0);
			DrawText(120, 385, "5 { 5",2, gsGlobal, &colour,1);
			DrawText(120, 410, "7 { 7",2, gsGlobal, &colour,0);
			DrawText(120, 435, "PFDTHIBNM BUHE",2, gsGlobal, &colour,0);
		break;
		case 2:
	   		DrawText(120, 360, "3 { 3",2, gsGlobal, &colour,0);
			DrawText(120, 385, "5 { 5",2, gsGlobal, &colour,0);
			DrawText(120, 410, "7 { 7",2, gsGlobal, &colour,1);
			DrawText(120, 435, "PFDTHIBNM BUHE",2, gsGlobal, &colour,0);
		break;
		case 3:
			DrawText(120, 360, "3 { 3",2, gsGlobal, &colour,0);
			DrawText(120, 385, "5 { 5",2, gsGlobal, &colour,0);
			DrawText(120, 410, "7 { 7",2, gsGlobal, &colour,0);
			DrawText(120, 435, "PFDTHIBNM BUHE",2, gsGlobal, &colour,1);
		break;
		default:
	}
	
	//gsKit_clear(gsGlobal, GS_SETREG_RGBAQ(0x00,0x00,0x00,0x00,0x00));
}

void MenuEnd(GSGLOBAL* gsGlobal)
{
	printf("This should Run when MenuState is Gone.\n");
	// Clear VRAM after the Menu is done~
	gsKit_vram_clear(gsGlobal);
	UnloadMusic(&MenuMusicy);
}

StateManager MenuState =
{
	MenuStart,
	MenuUpdate,
	MenuDraw,
	MenuEnd
};
