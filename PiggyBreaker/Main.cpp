#include "Game.h"
#include <sstream>
#include <vector>
#include "Ball.h"
#include "Cannon.h"
#include "BallController.h"
#include "CollisionSystem.h"

//Various textures and items
SDL_Texture* backgroundTexture;
SDL_Texture* menuTexture;
SDL_Texture* winTexture;
SDL_Texture* loseTexture;
SDL_Texture* highScoreTexture;
SDL_Texture* currentHighScoreTexture;
SDL_Texture* buttonNewGameTexture;
SDL_Texture* buttonResumeTexture;
SDL_Texture* buttonQuitTexture;
SDL_Texture* buttonSpeakerTexture;
SDL_Color color;
SDL_Event e;
string fontName = "Chilanka-Regular.ttf";

//Game Members
Game game;
Vector mouse;
BallController ballController;
Cannon cannon = Cannon(game.ren);
int FPS = 60;

//Game Sounds and Music
Mix_Music* gameMusic = NULL;
Mix_Chunk* bubbleSound = NULL;
Mix_Chunk* winSound = NULL;
Mix_Chunk* loseSound = NULL;

//Buttons Rectangles
SDL_Rect rectNewGame = { 0,0,0,0 };
SDL_Rect rectResume = { 0,0,0,0 };
SDL_Rect rectPlay = { 0,0,0,0 };
SDL_Rect rectQuit = { 0,0,0,0 };
SDL_Rect rectSoundControl = { 0,0,0,0 };

//HighScores Handling and Rendering
int highScore;
int currentHighScore;

//Game Bools
bool inMainMenu = true;
bool isRunning = false;
bool isPaused = false;
bool noSound = false;
bool playSound = false;
bool hasWon = false;
bool hasLost = false;

//Loops
void menu_loop(SDL_Renderer*);
void game_loop(SDL_Renderer*);
void paint_loop(SDL_Renderer*);
void event_loop();

//*NOTE*
//To get access to the console, make sure you run the program in Debug instead of release, when doing so
//a command prompt will pop up along with the SDL window.
int main(int argc, char** argv) {

	srand((int)time(NULL));

	//To load an image you first declare the texture with:
	//SDL_Texture *texture_name, as seen above the int main function.
	//Then you call the load_Texture function with the relative image
	//path, and a reference to the renderer.
	buttonSpeakerTexture = game.load_Texture(game.get_Directory() + "images/speaker.png", game.ren);
	menuTexture = game.load_Texture(game.get_Directory() + "images/menu_texture.png", game.ren);
	winTexture = game.load_Texture(game.get_Directory() + "images/YouWin_background_texture.png", game.ren);
	loseTexture = game.load_Texture(game.get_Directory() + "images/YouLose_background_texture.png", game.ren);
	backgroundTexture = game.load_Texture(game.get_Directory() + "images/game_background_texture.png", game.ren);
	buttonNewGameTexture = game.load_Texture(game.get_Directory() + "images/new_game_button.png", game.ren);
	buttonResumeTexture = game.load_Texture(game.get_Directory() + "images/button_resume.png", game.ren);
	buttonQuitTexture = game.load_Texture(game.get_Directory() + "images/quit_button.png", game.ren);

	//This loop loads every ball textures in a vector<SDL_Textures> in the BallController
	for (int i = 0; i < BallColor::NB_COLORS; i++)
	{
		SDL_Texture* ballTexture = game.load_Texture(game.get_Directory() + "images/" + BallController::getTextureName(i), game.ren);
		BallController::setVectorBalls(ballTexture);
	}

	// Load and Set Cannon texture
	cannon.setTexture(game.load_Texture(game.get_Directory() + "images/cannon.png", game.ren));
	cannon.initBalls();
	
	gameMusic = Mix_LoadMUS("../Debug/sounds/music.mp3");
	bubbleSound = Mix_LoadWAV("../Debug/sounds/bubble.wav");
	winSound = Mix_LoadWAV("../Debug/sounds/win.wav");
	loseSound = Mix_LoadWAV("../Debug/sounds/lose.wav");

	Mix_VolumeMusic(4);
	Mix_VolumeChunk(winSound, 7);
	Mix_VolumeChunk(loseSound, 7);
	Mix_PlayMusic(gameMusic, 1);

	//We set a color here for use in creating font textures such as in the mouse move
	//event where we create a text image of the current mouse X and Y.
	color = { 255, 255, 255, 125 };

	//After all of the basic setup, the game_loop is called with a
	//reference to the renderer. This is where all of the magic happens
	//and as so is located directly below.
	menu_loop(game.ren);

	return 0;
}

void menu_loop(SDL_Renderer* ren) {
	rectNewGame = { 420,485,200,75 };
	rectQuit = { 420,575,200,75 };
	rectSoundControl = { 950,690,50,50 };

	while (inMainMenu) {
		event_loop();

		SDL_RenderClear(ren);

		SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
		SDL_SetTextureBlendMode(menuTexture, SDL_BLENDMODE_NONE);
		SDL_SetTextureAlphaMod(menuTexture, 255);

		if (hasWon) {
			game.render_Texture(winTexture, ren, 0, 0);
		}
		else if (hasLost) {
			game.render_Texture(loseTexture, ren, 0, 0);
		}
		else {
			game.render_Texture(menuTexture, ren, 0, 0);
		}
		
		game.render_Texture(buttonNewGameTexture, ren, 420, 485);
		game.render_Texture(buttonQuitTexture, ren, 420, 575);
		game.render_Texture(buttonSpeakerTexture, ren, 950, 690);

		SDL_RenderPresent(ren);

		SDL_Delay(1000 / FPS);
	}
}

//This is the main game_loop of the program. Once this loop is called it
//will run until running is set to false. This loop first calls the event_loop
//to process any events in the queue. Then it clears the screen and calls the
//paint_loop. Inside the paint loop function is where you would render textures.
//It then presents the renders to the screen and sleeps for 1000 / FPS ms. This 
//prevents the game_loop from running 1000s of time per second and putting an
//unessisary load on the CPU and GPU
void game_loop(SDL_Renderer* ren) {
	rectResume = { 420,395,200,75 };
	rectPlay = { 810,720,70,25 };

	int shape = rand() % 5 + 1;

	BallController::generateBalls(ren, shape);

	while (isRunning) {
		hasWon = Game::hasWon();
		hasLost = BallController::hasLost();

		SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
		SDL_SetTextureBlendMode(backgroundTexture, SDL_BLENDMODE_NONE);
		SDL_SetTextureAlphaMod(backgroundTexture, 255);

		event_loop();		

		//The area below the event_loop being called, and above the SDL_RenderClear function being called
		//is where you would typically put any code that needs to be run every frame of the game.
		//Things such as updating the player position if a key is being held down, or moving an enemy
		//across the screen.

		SDL_RenderClear(ren);

		if (!isPaused) {			
			paint_loop(ren);

			if (!noSound) {
				if (playSound) {
					Mix_PlayChannel(-1, bubbleSound, 0);
				}
			}			
		
			if (hasWon) {
				if (!noSound) {					
					Mix_PlayChannel(-1, winSound, 0);
				}				
				inMainMenu = true;
				isRunning = false;
			}

			if (hasLost) {
				if (!noSound) {
					Mix_PlayChannel(-1, loseSound, 0);
				}				
				inMainMenu = true;
				isRunning = false;
			}

			if (hasWon || hasLost) {
				if (currentHighScore > highScore) {
					Game::writeHighScore();
				}
			}

			SDL_Delay(1000 / FPS);
		}
		else {
			SDL_SetTextureBlendMode(backgroundTexture, SDL_BLENDMODE_BLEND);
			SDL_SetTextureAlphaMod(backgroundTexture, 125);
			game.render_Texture(backgroundTexture, ren, 0, 0);

			game.render_Texture(buttonResumeTexture, ren, 420, 395);
			game.render_Texture(buttonNewGameTexture, ren, 420, 485);
			game.render_Texture(buttonQuitTexture, ren, 420, 575);
			game.render_Texture(buttonSpeakerTexture, ren, 950, 690);
		}
		SDL_RenderPresent(ren);
	}
}

//The paint_loop is resonsible for painting to the renderer every frame. This is
//where you would paint your player or enemies textures, or the map tiles using
//the render_Texture function to set the location and optionally the size of the image.
void paint_loop(SDL_Renderer* ren) {

	std::stringstream currentHighScoreStream;
	std::stringstream highScoreStream;

	currentHighScore = Game::getHighScore();

	//Render the background texture
	game.render_Texture(backgroundTexture, ren, 0, 0);

	//Handles the Cannon angle and Draw
	cannon.draw(ren);

	playSound = BallController::update(ren);

	//Draw each ball of the vector<Ball> and apply the proper texture color to each ball
	for (size_t i = 0; i < BallController::getVectorBalls()->size(); i++)
	{
		//Render the ball texture
		Ball* myBall = BallController::getVectorBalls()->at(i);
		game.render_Texture(myBall->getTexture(), ren, (size_t)myBall->getPosX() - 16, (size_t)myBall->getPosY() - 16, 32, 32);
	}

	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	game.render_Texture(buttonSpeakerTexture, ren, 950, 690);

	currentHighScoreStream << "Your current score: " << currentHighScore;
	currentHighScoreTexture = game.render_Text(currentHighScoreStream.str(), game.get_Directory() + fontName, color, 36, ren);
	game.render_Texture(currentHighScoreTexture, ren, 300, 700);
	SDL_DestroyTexture(currentHighScoreTexture);

	highScoreStream << highScore;
	highScoreTexture = game.render_Text(highScoreStream.str(), game.get_Directory() + fontName, color, 30, ren);
	game.render_Texture(highScoreTexture, ren, 830, 670);
	SDL_DestroyTexture(highScoreTexture);
}

//The event loop. This will check the queue of events every time
//it is called from the game loop and will process events with 
//the SDL Event object e.
void event_loop() {
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			inMainMenu = false;
			isPaused = false;
			isRunning = false;

		}
		if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
			if (inMainMenu) {
				inMainMenu = false;
			}
			else if (isRunning) {
				if (!isPaused) {
					isPaused = true;
				}
				else {
					isPaused = false;
				}
			}
		}
		//Shows the basic use of an event to report information to the screen.
		//In this case whenever the mouse moves we take its X and Y location,
		//render it into a text, and then it gets rendered to the screen during
		//the next paint loop!
		if (!isPaused) {
			if (e.type == SDL_MOUSEMOTION) {
				int x, y;
				SDL_GetMouseState(&x, &y);
				cannon.setMousePos(Vector(x, y));
			}
		}		
		if (e.motion.x >= rectSoundControl.x && e.motion.x <= rectSoundControl.x + rectSoundControl.w && e.motion.y >= rectSoundControl.y && e.motion.y <= rectSoundControl.y + rectSoundControl.h) {
			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					if (!noSound) {
						Mix_PauseMusic();
						noSound = true;
					}
					else {
						Mix_ResumeMusic();
						noSound = false;
					}
				}
			}
		}
		if (e.motion.x >= rectNewGame.x && e.motion.x <= rectNewGame.x + rectNewGame.w && e.motion.y >= rectNewGame.y && e.motion.y <= rectNewGame.y + rectNewGame.h) {
			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					if (inMainMenu) {
						inMainMenu = false;
						isRunning = true;
						Game::setHighScore(0);
						highScore = Game::readHighScore();
						BallController::resetCanvas();
						game_loop(game.ren);
					}
					else if (isPaused) {
						isPaused = false;
						Game::setHighScore(0);
						highScore = Game::readHighScore();
						BallController::resetCanvas();
						cannon.initBalls();
						game_loop(game.ren);
					}
				}
			}
		}
		if (!isPaused) {
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				if (e.button.button == SDL_BUTTON_LEFT) {
					cannon.shoot();
				}
			}
			if (e.motion.x >= rectPlay.x && e.motion.x <= rectPlay.x + rectPlay.w && e.motion.y >= rectPlay.y && e.motion.y <= rectPlay.y + rectPlay.h) {
				if (e.type == SDL_MOUSEBUTTONDOWN)
				{
					if (e.button.button == SDL_BUTTON_LEFT)
					{
						Game::setHighScore(0);
						highScore = Game::readHighScore();
						BallController::resetCanvas();
						cannon.initBalls();
						game_loop(game.ren);
					}
				}
			}
		}
		if (isPaused) {
			if (e.motion.x >= rectResume.x && e.motion.x <= rectResume.x + rectResume.w && e.motion.y >= rectResume.y && e.motion.y <= rectResume.y + rectResume.h) {
				if (e.type == SDL_MOUSEBUTTONDOWN)
				{
					if (e.button.button == SDL_BUTTON_LEFT)
					{
						isPaused = false;
					}
				}
			}
		}
		if (inMainMenu || isPaused) {
			if (e.motion.x >= rectQuit.x && e.motion.x <= rectQuit.x + rectQuit.w && e.motion.y >= rectQuit.y && e.motion.y <= rectQuit.y + rectQuit.h) {
				if (e.type == SDL_MOUSEBUTTONDOWN)
				{
					if (e.button.button == SDL_BUTTON_LEFT)
					{
						isRunning = false;
						isPaused = false;
						inMainMenu = false;
					}
				}
			}
		}		
	}
}