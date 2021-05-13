#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "BallController.h"
#include "../Libraries/lib_Point/Point.h"
using namespace std;

class Game {
	static int m_highScore;
	static std::string m_highScoreFilename;
	static std::string m_tempHighScoreLine;
	static std::ifstream m_readHighScoreStream;
	static std::ofstream m_writeHighScoreStream;
public:
	SDL_Window* win;
	SDL_Renderer* ren;
	Game();
	void load_SDL_Libraries();
	SDL_Window* load_SDL_Window();
	SDL_Renderer* load_SDL_Renderer(SDL_Window*);
	SDL_Texture* load_Texture(const std::string& file, SDL_Renderer* ren);
	void render_Texture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y, int w, int h);
	void render_Texture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y);
	SDL_Texture* render_Text(const std::string& message, const std::string& fontFile, SDL_Color color, int fontSize, SDL_Renderer* renderer);
	std::string get_Directory();
	void cleanup_SDL();	
	static int getHighScore();
	static void setHighScore(int highScore);
	static int readHighScore();
	static void writeHighScore();
	static bool hasWon();
};
