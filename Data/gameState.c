/*****************************************************************************/
/*  Автор         		: Дракончик				                             */
/*  Приставка     		: Sony PlayStation 2					             */
/*  ОС			  		: OSDSYS											 */
/*  Язык	      		: GNU C                                              */
/*                                                                           */
/*  Содержимое файла	: Стадия игровой детельности                         */
/*  Атрибут файла		: SOURCE                                             */
/*  Имя файла     		: gameState.c	                                     */
/*                                                                           */
/*****************************************************************************/

#include "include/gameState.h"
#include "include/menuState.h"
#include "include/overState.h"
#include "include/introState.h"
#include "include/pad.h"
#include "include/musicManager.h"


extern StateMachine GameMachineState;

/****** User External Files ********/
#include "extern/gameState.ext"

/****** User Table Files ***********/


/****** User Define Files ***********/
#include "define/gameState.def"



// Array for the playing field that's used to determinate what game grid is gonna be used
// gameGrid[X][Y][on/off]



int chosenKriterij = 0;

// Критерий Вальда (MaxMin)
int KriterijValda = 0;
int KriterijValdaX = 0;
int KriterijValdaY = 0;

// Критерий оптимизма (MaxMax)
int KriterijSevidza = 0;
int KriterijSevidzaX = 0;
int KriterijSevidzaY = 0;

// Критерий песимизма (MinMin)
int KriterijOptimistic = 0;
int KriterijOptimisticX = 0;
int KriterijOptimisticY = 0;

// Критерий Сэвиджа
int KriterijPesimistic = 0;
int KriterijPesimisticX = 0;
int KriterijPesimisticY = 0;

// Критерий Гурвица
float KriterijGurvica = 0.0f;
int KriterijGurvicaInt = 0;
int KriterijGurvicaX = 0;
int KriterijGurvicaY = 0;

float LamdaGurvica = 0.5f;

int gameGrid[16][16];
int gridBool[16][16];

int score = 500;

GSTEXTURE BG; 
GSTEXTURE Girl;
GSTEXTURE Cat;
GSTEXTURE Sputnik;
GSTEXTURE Box;
GSTEXTURE BoxSel;

BGM GameMusicy;


void GameStart(GSGLOBAL* gsGlobal)
{
	char tempz[4096];
	strcpy(tempz, relativePath);
	strcat(tempz, PickMusic(3));
	GameMusicy.fileName = tempz;

	char temp1[4096];
	strcpy(temp1, relativePath);
	strcat(temp1, TITLEIMAGEPATH);
	gsKit_texture_png(gsGlobal, &BG,temp1);
	
	char temp2[4096];
	strcpy(temp2, relativePath);
	strcat(temp2, GIRL);
	gsKit_texture_png(gsGlobal, &Girl,temp2);
	
	char temp3[4096];
	strcpy(temp3, relativePath);
	strcat(temp3, CAT);
	gsKit_texture_png(gsGlobal, &Cat,temp3);
	
	char temp4[4096];
	strcpy(temp4, relativePath);
	strcat(temp4, SPUTNIK);
	gsKit_texture_png(gsGlobal, &Sputnik,temp4);
	
	char temp5[4096];
	strcpy(temp5, relativePath);
	strcat(temp5, BOX);
	gsKit_texture_png(gsGlobal, &Box,temp5);
	
	char temp6[4096];
	strcpy(temp6, relativePath);
	strcat(temp6, BOXSEL);
	gsKit_texture_png(gsGlobal, &BoxSel,temp6);
	
	srand(time(NULL));
	
	SetUpFont(gsGlobal);
	
	LoadMusic(&GameMusicy);
	GeneratePlayField();
}

void GameUpdate(GSGLOBAL* gsGlobal)
{
	PlayMusic(&GameMusicy);
	
	KriterijValda = MaxMin(&KriterijValdaX, &KriterijValdaY);
	KriterijSevidza = MinMax(&KriterijSevidzaX, &KriterijSevidzaY);
	KriterijOptimistic = MaxMax(&KriterijOptimisticX, &KriterijOptimisticY);
	KriterijPesimistic = MinMin(&KriterijPesimisticX, &KriterijPesimisticY);
	KriterijGurvica = Gurvic(&KriterijGurvicaX, &KriterijGurvicaY, &KriterijGurvicaInt);
	
	// Change Menu
	if(PlaystationGamePad.UP_KEY_TAP && chosenKriterij != 0)
    {
    	chosenKriterij--;
    }
    else if(PlaystationGamePad.DOWN_KEY_TAP && chosenKriterij != 4)
    {
    	chosenKriterij++;
    	
    }
	
	if(PlaystationGamePad.LEFT_KEY_TAP && chosenKriterij == 4 && LamdaGurvica != 0.0f)
	{
		LamdaGurvica -= 0.01f;
		printf("%f",LamdaGurvica);
	}	
	else if(PlaystationGamePad.RIGHT_KEY_TAP && chosenKriterij == 4 && LamdaGurvica != 1.0f)
	{
		LamdaGurvica += 0.01f;
		printf("%f",LamdaGurvica);
	}
	
	if(PlaystationGamePad.BUTTON_X_KEY_TAP && score > 0)
	{
		GamePlaying();
	}
	
	if(PlaystationGamePad.BUTTON_O_KEY_TAP)
	{

	}
}

void GameDraw(GSGLOBAL* gsGlobal, u64 colour)
{
	gsKit_prim_sprite_texture(gsGlobal, &BG,0,  // X1
						0,  // Y2
						0.0f,  // U1
						0.0f,  // V1
						BG.Width * 2, // X2
						BG.Height * 2, // Y2
						BG.Width * 2, // U2
						BG.Height * 2, // V2
						2,
						colour);
						
		char numby[10];
	for(int i = 0; i < gameType; i++)
	{
		for(int j = 0; j < gameType; j++)
		{
			
			if(i == KriterijValdaX && j == KriterijValdaY && chosenKriterij == 0)
			{
				gsKit_prim_sprite_texture(gsGlobal, &BoxSel,196 + i * Box.Width,  // X1
						j * Box.Height,  // Y2
						0.0f,  // U1
						0.0f,  // V1
						196.0f + i * Box.Width + Box.Width, // X2
						j * Box.Height + Box.Height, // Y2
						Box.Width, // U2
						Box.Height, // V2
						2,
						colour);
			}
			else if(i == KriterijSevidzaX && j == KriterijSevidzaY && chosenKriterij == 1)
			{
				gsKit_prim_sprite_texture(gsGlobal, &BoxSel,196 + i * Box.Width,  // X1
						j * Box.Height,  // Y2
						0.0f,  // U1
						0.0f,  // V1
						196.0f + i * Box.Width + Box.Width, // X2
						j * Box.Height + Box.Height, // Y2
						Box.Width, // U2
						Box.Height, // V2
						2,
						colour);
			}
			else if(i == KriterijOptimisticX && j == KriterijOptimisticY && chosenKriterij == 2)
			{
				gsKit_prim_sprite_texture(gsGlobal, &BoxSel,196 + i * Box.Width,  // X1
						j * Box.Height,  // Y2
						0.0f,  // U1
						0.0f,  // V1
						196.0f + i * Box.Width + Box.Width, // X2
						j * Box.Height + Box.Height, // Y2
						Box.Width, // U2
						Box.Height, // V2
						2,
						colour);
			}
			else if(i == KriterijPesimisticX && j == KriterijPesimisticY && chosenKriterij == 3)
			{
				gsKit_prim_sprite_texture(gsGlobal, &BoxSel,196 + i * Box.Width,  // X1
							j * Box.Height,  // Y2
							0.0f,  // U1
							0.0f,  // V1
							196.0f + i * Box.Width + Box.Width, // X2
							j * Box.Height + Box.Height, // Y2
							Box.Width, // U2
							Box.Height, // V2
							2,
							colour);
			}
			else if(i == KriterijGurvicaX && j == KriterijGurvicaY && chosenKriterij == 4)
			{
				gsKit_prim_sprite_texture(gsGlobal, &BoxSel,196 + i * Box.Width,  // X1
							j * Box.Height,  // Y2
							0.0f,  // U1
							0.0f,  // V1
							196.0f + i * Box.Width + Box.Width, // X2
							j * Box.Height + Box.Height, // Y2
							Box.Width, // U2
							Box.Height, // V2
							2,
							colour);
			}
			else
			{
			
			
				gsKit_prim_sprite_texture(gsGlobal, &Box,196 + i * Box.Width,  // X1
							j * Box.Height,  // Y2
							0.0f,  // U1
							0.0f,  // V1
							196.0f + i * Box.Width + Box.Width, // X2
							j * Box.Height + Box.Height, // Y2
							Box.Width, // U2
							Box.Height, // V2
							2,
							colour);
			}
			
						
			
			sprintf(numby, "%d", gameGrid[i][j]);
			if(gridBool[i][j] != 0)
			{
				DrawText(196 + i * Box.Width + 16, j * Box.Height + 16, numby,1, gsGlobal, &colour);
			}
		}
	}



	
	gsKit_prim_sprite_texture(gsGlobal, &Girl,10.0f,  // X1
						250.0f,  // Y2
						0.0f,  // U1
						0.0f,  // V1
						10.0f + Girl.Width, // X2
						250.0f + Girl.Height, // Y2
						Girl.Width, // U2
						Girl.Height, // V2
						2,
						colour);

	gsKit_prim_sprite_texture(gsGlobal, &Cat,450.0f,  // X1
						330.0f,  // Y2
						0.0f,  // U1
						0.0f,  // V1
						450.0f + Cat.Width, // X2
						330.0f + Cat.Height, // Y2
						Cat.Width, // U2
						Cat.Height, // V2
						2,
						colour);

	gsKit_prim_sprite_texture(gsGlobal, &Sputnik,500.0f,  // X1
						140.0f,  // Y2
						0.0f,  // U1
						0.0f,  // V1
						500.0f + Sputnik.Width, // X2
						140.0f + Sputnik.Height, // Y2
						Sputnik.Width, // U2
						Sputnik.Height, // V2
						2,
						colour);
		char str[10];			
	switch(chosenKriterij)
	{
		// Start New Game
		case 0:
			DrawText(40, 60, "DFKMLF",1, gsGlobal, &colour,0);	

  			sprintf(str, "%d", KriterijValda);
  			DrawText(140, 60, str,1, gsGlobal, &colour);

		break;
		case 1: 
	  		DrawText(40, 60, "C'DBL:F",1, gsGlobal, &colour,0);	

  			sprintf(str, "%d", KriterijSevidza);
  			DrawText(140, 60, str,1, gsGlobal, &colour);

		break;
		case 2:
	   		DrawText(40, 60, "JGNBVBPV",1, gsGlobal, &colour,0);	

  			sprintf(str, "%d", KriterijOptimistic);
  			DrawText(140, 60, str,1, gsGlobal, &colour);

		break;
		case 3:
			DrawText(40, 60, "GTCBVBPV",1, gsGlobal, &colour,0);	

  			sprintf(str, "%d", KriterijPesimistic);
  			DrawText(140, 60, str,1, gsGlobal, &colour);

		break;
		case 4://Критерий Гурвица
			DrawText(40, 60, "UEHDBWF",1, gsGlobal, &colour,0);	

  			sprintf(str, "%f", KriterijGurvica);
  			DrawText(40, 75, str,1, gsGlobal, &colour);
  			sprintf(str, "%d", KriterijGurvicaInt);
  			DrawText(40, 90, str,1, gsGlobal, &colour);
  			
  			DrawText(40, 110, "KFVLF",1, gsGlobal, &colour,0);	
  			sprintf(str, "%f", LamdaGurvica);
  			DrawText(40, 130, str,1, gsGlobal, &colour);

		break;
		default:
	}
	
	
		DrawText(40, 210, "CX~N",1, gsGlobal, &colour,0);	
  		sprintf(str, "%d", score);
  		DrawText(40, 230, str,1, gsGlobal, &colour);
  		
  		if(score < 0)
  		{
  			DrawText(40, 210, "BUHF JRJYXTYJ",3, gsGlobal, &colour,0);	
  		}

}

void GameEnd(GSGLOBAL* gsGlobal)
{

}

void GeneratePlayField()
{
	for(int i = 0; i < gameType; i++)
	{
		for(int j = 0; j < gameType; j++)
		{
			gridBool[i][j] = 1;
			gameGrid[i][j] = rand() % 100;
			
			printf("%d ",gameGrid[j][i]);
			
			if(gameGrid[i][j] == 0)
			{
				gameGrid[i][j] = 1;
			}
		}
		printf("\n");
	}
}


int MaxMin(int* X, int* Y)
{

	int Valda = 0;
	int Mins[16];
	int MinsX[16];
	int MinsY[16];
	
	for(int i = 0; i < gameType; i++)
	{
		Mins[i] = 100;
		for(int j = 0; j < gameType; j++)
		{
			if((Mins[i] > gameGrid[j][i]) && gridBool[j][i] != 0)
			{
				Mins[i] = gameGrid[j][i];
				printf("%d  %d\n", i, Mins[i]);
				MinsX[i] = j;
				MinsY[i] = i; 
			}
		}
	}
	
	for(int z = 0; z < gameType; z++)
	{
		if(Valda < Mins[z] && Mins[z] != 100)
		{
		      Valda = Mins[z];
				*X = MinsX[z];
				*Y = MinsY[z]; 
		}
	}
	return Valda;
}

int MaxMax(int* X, int* Y)
{
	int Optimistic = 0;
	
	for(int i = 0; i < gameType; i++)
	{
		for(int j = 0; j < gameType; j++)
		{
			if((Optimistic < gameGrid[i][j]) && gridBool[i][j] != 0)
			{
				Optimistic = gameGrid[i][j];
				*X = i;
				*Y = j; 
			}
		}
	}
	
	return Optimistic;
}

int MinMin(int* X, int* Y)
{
	int Pesimistic = 100;
		
	for(int i = 0; i < gameType; i++)
	{
		for(int j = 0; j < gameType; j++)
		{
			if((Pesimistic > gameGrid[i][j]) && gridBool[i][j] != 0)
			{
				Pesimistic = gameGrid[i][j];
				*X = i;
				*Y = j; 
			}
		}
	}
	return Pesimistic;
}

int MinMax(int* X, int* Y)
{
	int Sevidza = 100;
	int Maxes[16];
	int MaxesX[16];
	int MaxesY[16];
	
	for(int i = 0; i < gameType; i++)
	{
		Maxes[i] = 0;
		for(int j = 0; j < gameType; j++)
		{
			if((Maxes[i] < gameGrid[j][i]) && gridBool[j][i] != 0)
			{
				Maxes[i] = gameGrid[j][i];
				MaxesX[i] = j;
				MaxesY[i] = i; 
			}
		}
	}
	
	for(int z = 0; z < gameType; z++)
	{
		if(Sevidza > Maxes[z] && Maxes[z] != 0)
		{
		    Sevidza = Maxes[z];
				*X = MaxesX[z];
				*Y = MaxesY[z]; 
		}
	}
	
	return Sevidza;
}


float Gurvic(int* X, int* Y, int* value)
{
	float gurMax[16];
	int gurMaxX[16];
	int gurMaxY[16];
	float gurMin[16];
	int gurMinX[16];
	int gurMinY[16];
	
	float gurFinal = 0.0f;
	int offset = 0;
	int hasMatch = 0;
	
	for(int i = 0; i < gameType; i++)
	{
		gurMax[i] = 0;
		gurMin[i] = 100;
		for(int j = 0; j < gameType; j++)
		{
			if(((gurMax[i] < gameGrid[j][i]) && gridBool[j][i] != 0) && gameGrid[j][i] !=100)
			{
				gurMax[i] = gameGrid[j][i];
				gurMaxX[i] = i;
				gurMaxY[i] = j; 
			}
			
			if(((gurMin[i] > gameGrid[j][i]) && gridBool[j][i] != 0) && gameGrid[j][i] != 0)
			{
				gurMin[i] = gameGrid[j][i];
				gurMinX[i] = i;
				gurMinY[i] = j; 
			}
		}
	}
	
	for(int i = 0; i < gameType; i++)
	{
		float calculation = (LamdaGurvica * gurMax[i]) + ((1.0f-LamdaGurvica)* gurMin[i]);
		if(gurFinal < calculation)
		{
			Repeat:
			gurFinal = calculation;
			for(int j = 0; j < gameType; j++)
			{
				if((gameGrid[j][i] + offset) == (int)gurFinal)
				{
					hasMatch = 1;
					*X = j;
					*Y = i;
					*value = gameGrid[j][i];
					offset = 0;
				}
			}
			
			if(hasMatch == 0)
			{
				offset +=1;
				goto Repeat;
			}
		}
		hasMatch = 0;
	}
	
	return gurFinal;

}

void GamePlaying()
{
	int Hit = 0;
	for(int i = 0; i < 4; i++)
	{

		int X = rand() % gameType;
		int Y = rand() % gameType;
		
		if(gridBool[X][Y] != 0)
		{
			switch(chosenKriterij)
			{
				case 0:
					if(X == KriterijValdaX && Y == KriterijValdaY)
					{
						Hit = 1;
					}
					break;
				case 1:
					if(X == KriterijSevidzaX && Y == KriterijSevidzaY)
					{
						Hit = 1;
					}
					break;
				case 2:
					if(X == KriterijOptimisticX && Y == KriterijOptimisticY)
					{
						Hit = 1;
					}
					break;
				case 3:
					if(X == KriterijPesimisticX && Y == KriterijPesimisticY)
					{
						Hit = 1;
					}

					break;
				case 4:
					if(X == KriterijGurvicaX && Y == KriterijGurvicaY)
					{
						Hit = 1;
					}	
					break;
				default:
			}
			
			gridBool[X][Y] = 0;
		}
		else
		{
			i--;
		}

	}

			switch(chosenKriterij)
			{
				case 0:
					if(Hit == 1)
					{
						score += gameGrid[KriterijValdaX][KriterijValdaY];
					}
					else
					{
						score -= gameGrid[KriterijValdaX][KriterijValdaY];
					}
					break;
				case 1:
					if(Hit == 1)
					{
						score += gameGrid[KriterijSevidzaX][KriterijSevidzaY];
					}
					else
					{
						score -= gameGrid[KriterijSevidzaX][KriterijSevidzaY];
					}
					break;
				case 2:
					if(Hit == 1)
					{
						score += gameGrid[KriterijOptimisticX][KriterijOptimisticY];
					}	
					else
					{
						score -= gameGrid[KriterijOptimisticX][KriterijOptimisticY];
					}
					break;
				case 3:
					if(Hit == 1)
					{
						score += gameGrid[KriterijPesimisticX][KriterijPesimisticY];
					}
					else
					{
						score -= gameGrid[KriterijPesimisticX][KriterijPesimisticY];
					}
					break;
				case 4:
					if(Hit == 1)
					{
						score += gameGrid[KriterijGurvicaX][KriterijGurvicaY];
					}	
					else
					{
						score -= gameGrid[KriterijGurvicaX][KriterijGurvicaY];
					}	
					break;
				default:
			}
	

}



StateManager GameState =
{
	GameStart,
	GameUpdate,
	GameDraw,
	GameEnd
};
