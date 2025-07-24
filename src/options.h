#pragma once

#include "raylib.h"
#include "options.h"
#include "play.h"
#include "images.h"
#include <iostream>

void UpdateOptions();

void DrawOptions();

extern bool BackPressed;
extern bool InfiniteGold;
extern bool AllowPausing;