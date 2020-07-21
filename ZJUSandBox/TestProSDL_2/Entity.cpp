#include"Entity.h";
#include"Utils.h"



const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;


SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
LTexture gBGTexture;



int Entity::GetPosX()
{
	return mPosX;
}
int Entity::GetPosY()
{
	return mPosY;
}
int Entity::GetW()
{
	return w;
}
int Entity::GetH()
{
	return h;
}

StaticObj::StaticObj(const char* file)
{

	if (!StaticObjTex.loadFromFile(file))
	{
		printf("Failed to load the texture!\n");
	}
	rect.x = 0;
	rect.y = 0;
	rect.h = StaticObjTex.getHeight();
	rect.w = StaticObjTex.getWidth();
	return;
}

void StaticObj::SetPos(int x, int y)
{
	mPosX = x;
	mPosY = y;
}

void StaticObj::Render(int camX, int camY)
{
	rect.x = camX;
	rect.y = camY;

	StaticObjTex.render(mPosX - camX, mPosY - camY);
}

StaticObj::~StaticObj() {
	SDL_FreeSurface(imag);
	SDL_DestroyTexture(tex);
}


MoveObj::MoveObj(const char* file, int w, int h)
{
	mVelX = 0;
	mVelY = 0;
	mPosX = 0;
	mPosY = 0;
	frame = 0;
	this->w = w;
	this->h = h;   //for the test piecture,w should be 32,h should be 48
	//Load sprite sheet texture
	if (!gSpriteSheetTexture.loadFromFile(file))
	{
		printf("Failed to load the texture!\n");
	}
	else
	{
		for (int j = KEY_PRESS_SURFACE_DOWN; j < KEY_PRESS_SURFACE_TOTAL; j++)
		{
			for (int i = 0; i < WALKING_ANIMATION_FRAMES; i++)
			{
				gSpriteClips[j][i].x = w * i;
				gSpriteClips[j][i].y = h * j;
				gSpriteClips[j][i].w = w;
				gSpriteClips[j][i].h = h;

			}
		}

	}
	isMoving = false;
	curDirection = KEY_PRESS_SURFACE_DOWN;
}

MoveObj::~MoveObj()
{
	gSpriteSheetTexture.free();
}

int MoveObj::GetVelX()
{
	return mVelX;
}
int MoveObj::GetVelY()
{
	return mVelY;
}


void MoveObj::handleEvent(SDL_Event& e)
{
	//If a key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		isMoving = true;
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_UP:
			mVelY -= DOT_VEL;
			curDirection = KEY_PRESS_SURFACE_UP;
			break;
		case SDLK_DOWN:
			mVelY += DOT_VEL;
			curDirection = KEY_PRESS_SURFACE_DOWN;
			break;
		case SDLK_LEFT:
			mVelX -= DOT_VEL;
			curDirection = KEY_PRESS_SURFACE_LEFT;
			break;
		case SDLK_RIGHT:
			mVelX += DOT_VEL;
			curDirection = KEY_PRESS_SURFACE_RIGHT;
			break;
		}
	}
	//If a key was released
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		isMoving = false;
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: mVelY += DOT_VEL; break;
		case SDLK_DOWN: mVelY -= DOT_VEL; break;
		case SDLK_LEFT: mVelX += DOT_VEL; break;
		case SDLK_RIGHT: mVelX -= DOT_VEL; break;
		}
	}
}

void MoveObj::Move()
{
	//Move the dot left or right
	mPosX += mVelX;

	//If the dot went too far to the left or right
	if ((mPosX < 0) || (mPosX + w > LEVEL_WIDTH))
	{
		//Move back
		mPosX -= mVelX;
	}

	//Move the dot up or down
	mPosY += mVelY;

	//If the dot went too far up or down
	if ((mPosY < 0) || (mPosY + h > LEVEL_HEIGHT))
	{
		//Move back
		mPosY -= mVelY;
	}
}

void MoveObj::Render(int camX, int camY)
{
	if (isMoving)
	{
		frame++;
		if (frame / MoveSpeedFrame >= WALKING_ANIMATION_FRAMES)
		{
			frame = 0;
		}
	}
	else
	{
		frame = 0;
	}

	gSpriteSheetTexture.render(mPosX - camX, mPosY - camY, &gSpriteClips[curDirection][frame / MoveSpeedFrame]);
	//SDL_RenderPresent(gRenderer);

}

int MoveObj::GetMaxFrames()
{
	return WALKING_ANIMATION_FRAMES;
}





int main(int argc, char* args[]) {


	/*
	SDL_Init(SDL_INIT_EVERYTHING);
	gWindow = SDL_CreateWindow("test1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Window_WIDTH, Window_HEIGHT, SDL_WINDOW_SHOWN);
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	StaticObj t1("img/hello.bmp");
	t1.Render(100, 100, 1);
	SDL_Delay(1000);
	SDL_DestroyWindow(gWindow);
	SDL_DestroyRenderer(gRenderer);
	SDL_Quit();
	system("pause");
	return 0;
	*/
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		MoveObj testObj("img/man_1.png", 32, 48);
		StaticObj t1("img/hello_world.bmp");
		gBGTexture.loadFromFile("img/testBG.png");
		bool quit = false;

		//Event handler
		SDL_Event e;

		SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

		//The frames per second timer
		LTimer fpsTimer;

		//The frames per second cap timer
		LTimer capTimer;

		//Start counting frames per second
		int countedFrames = 0;
		fpsTimer.start();

		while (!quit)
		{
			//Handle events on queue
			while (SDL_PollEvent(&e) != 0)
			{
				//User requests quit
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
				testObj.handleEvent(e);
			}
			testObj.Move();

			camera.x = (testObj.GetPosX() + testObj.GetW() / 2) - SCREEN_WIDTH / 2;
			camera.y = (testObj.GetPosY() + testObj.GetH() / 2) - SCREEN_HEIGHT / 2;

			//Calculate and correct fps
			float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
			if (avgFPS > 2000000)
			{
				avgFPS = 0;
			}

			//Keep the camera in bounds
			if (camera.x < 0)
			{
				camera.x = 0;
			}
			if (camera.y < 0)
			{
				camera.y = 0;
			}
			if (camera.x > LEVEL_WIDTH - camera.w)
			{
				camera.x = LEVEL_WIDTH - camera.w;
			}
			if (camera.y > LEVEL_HEIGHT - camera.h)
			{
				camera.y = LEVEL_HEIGHT - camera.h;
			}

			//Clear screen
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(gRenderer);

			gBGTexture.render(0, 0, &camera);
			t1.Render(camera.x, camera.y);
			testObj.Render(camera.x, camera.y);
			SDL_RenderPresent(gRenderer);

			++countedFrames;

			//If frame finished early
			int frameTicks = capTimer.getTicks();
			if (frameTicks < SCREEN_TICK_PER_FRAME)
			{
				//Wait remaining time
				SDL_Delay(SCREEN_TICK_PER_FRAME - frameTicks);
			}

		}

	}
	close();
	return 0;
}