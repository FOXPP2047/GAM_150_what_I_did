/*******************************************************************************
filename     GameState.h
author       Minjeong.Kim, Yunhyuk.Choi, Yongmin.Cho
email        lovebkdn@gmail.com (Minjeong.kim)
						 yunhyeokchoi.digipen@gmail.com (yunhyeok.choi)
						 qlxhdl55@gmail.com (yongmin.cho)
project name Meteorcre Defense
course       GAME150
Update       13/6/2015
copyright    All content (c) 2015 DigiPen (USA) Corporation, all rights
reserved.

Brief Description:
This file is prototype of GameState.c. 

*******************************************************************************/
#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "WEGameData.h" // WEGameData


// GameState Load function.
void GameStateLoad(WEGameData* pGameData);
// GameState Initialize function.
void GameStateInit(WEGameData* pGameData);
// GameState Update function.
void GameStateUpdate(WEGameData* pGameData, float dt);
// Draw function.
static void GameStateDraw(void);
// GameState Shutdown function.
void GameStateShutdown(WEGameData* pGameData);
// GameState Unload function.
void GameStateUnload(WEGameData* pGameData);


#endif /*GAMESTATE1_H*/