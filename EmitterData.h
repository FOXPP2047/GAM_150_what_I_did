/*******************************************************************************
filename     EmitterData.h
author       Minjeong.Kim, Yongmin.Cho
email        lovebkdn@gmail.com (Minjeong.kim)
						 qlxhdl55@gmail.com (yongmin.cho)
project name Meteorcre Defense
course       GAME150
Update       13/6/2015
copyright    All content (c) 2015 DigiPen (USA) Corporation, all rights
reserved.

Brief Description: the file has a function prototype for set position and 
color.

-SetPositionAndColor

*******************************************************************************/
#ifndef EMITTERDATA_H
#define EMITTERDATA_H

#include "WarpEngine.h"
typedef enum  WhatEmitters EmitterType;
typedef struct MJEmitter Emitter;

void SetPositionAndColor(EmitterType id, Emitter* pCur);
#endif /* EMITTERDATA_H */