#pragma once
#include "../Libraries/lib_Point/Point.h"
#include <SDL_mixer.h>

static const int APP_WIDTH = 1024;
static const int APP_HEIGHT = 768;
static const int CANVAS_WIDTH = 768;
static const int CANVAS_HEIGHT = 568;

static const Vector CANVAS_MIN = Vector(160, 102);
static const Vector CANVAS_MAX = Vector(870, 382);
static const Vector PLAY_AREA_MIN = Vector(CANVAS_MIN.x - 10, CANVAS_MIN.y);
static const Vector PLAY_AREA_MAX = Vector(CANVAS_MAX.x + 32, CANVAS_MAX.y + 230);