/*******************************************************************************
filename     GameState2.h
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
This file is prototype of GameState2.c.

*******************************************************************************/
#ifndef GAMESTATE2_H
#define GAMESTATE2_H

#include "WEGameData.h" // WEGameData


// GameState2 Load function.
void GameState2Load(WEGameData* pGameData);
// GameState2 Initialize function.
void GameState2Init(WEGameData* pGameData);
// GameState2 Update function.
void GameState2Update(WEGameData* pGameData, float dt);
// Draw function.
static void GameState2Draw(void);
// GameState2 Shutdown function.
void GameState2Shutdown(WEGameData* pGameData);
// GameState Unload function.
void GameState2Unload(WEGameData* pGameData);


#endif /*GAMESTATE2_H*/