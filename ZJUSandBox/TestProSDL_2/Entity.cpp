#include"Entity.h";
#include"Utils.h"

const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;


//Tile constants
const int TILE_WIDTH = 80;
const int TILE_HEIGHT = 80;
const int TOTAL_TILES = 192;
const int TOTAL_TILE_SPRITES = 12;

//The different tile sprites
const int TILE_RED = 0;
const int TILE_GREEN = 1;
const int TILE_BLUE = 2;
const int TILE_CENTER = 3;
const int TILE_TOP = 4;
const int TILE_TOPRIGHT = 5;
const int TILE_RIGHT = 6;
const int TILE_BOTTOMRIGHT = 7;
const int TILE_BOTTOM = 8;
const int TILE_BOTTOMLEFT = 9;
const int TILE_LEFT = 10;
const int TILE_TOPLEFT = 11;


SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
LTexture gBGTexture;

LTexture gTileTexture;
SDL_Rect gTileClips[TOTAL_TILE_SPRITES];

bool checkCollision(SDL_Rect a, SDL_Rect b)
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	//Calculate the sides of rect B
	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	//If any of the sides from A are outside of B
	if (bottomA <= topB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}

	//If none of the sides from A are outside B
	return true;
}

bool setTiles(Tile* tiles[])
{
	//Success flag
	bool tilesLoaded = true;

	//The tile offsets
	int x = 0, y = 0;

	//Open the map
	std::ifstream map("img/lazy.map");

	//If the map couldn't be loaded
	if (map.fail())
	{
		printf("Unable to load map file!\n");
		tilesLoaded = false;
	}
	else
	{
		//Initialize the tiles
		for (int i = 0; i < TOTAL_TILES; ++i)
		{
			//Determines what kind of tile will be made
			int tileType = -1;

			//Read tile from map file
			map >> tileType;

			//If the was a problem in reading the map
			if (map.fail())
			{
				//Stop loading map
				printf("Error loading map: Unexpected end of file!\n");
				tilesLoaded = false;
				break;
			}

			//If the number is a valid tile number
			if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES))
			{
				tiles[i] = new Tile(x, y, tileType);
			}
			//If we don't recognize the tile type
			else
			{
				//Stop loading map
				printf("Error loading map: Invalid tile type at %d!\n", i);
				tilesLoaded = false;
				break;
			}

			//Move to next tile spot
			x += TILE_WIDTH;

			//If we've gone too far
			if (x >= LEVEL_WIDTH)
			{
				//Move back
				x = 0;

				//Move to the next row
				y += TILE_HEIGHT;
			}
		}

		//Clip the sprite sheet
		if (tilesLoaded)
		{
			gTileClips[TILE_RED].x = 0;
			gTileClips[TILE_RED].y = 0;
			gTileClips[TILE_RED].w = TILE_WIDTH;
			gTileClips[TILE_RED].h = TILE_HEIGHT;

			gTileClips[TILE_GREEN].x = 0;
			gTileClips[TILE_GREEN].y = 80;
			gTileClips[TILE_GREEN].w = TILE_WIDTH;
			gTileClips[TILE_GREEN].h = TILE_HEIGHT;

			gTileClips[TILE_BLUE].x = 0;
			gTileClips[TILE_BLUE].y = 160;
			gTileClips[TILE_BLUE].w = TILE_WIDTH;
			gTileClips[TILE_BLUE].h = TILE_HEIGHT;

			gTileClips[TILE_TOPLEFT].x = 80;
			gTileClips[TILE_TOPLEFT].y = 0;
			gTileClips[TILE_TOPLEFT].w = TILE_WIDTH;
			gTileClips[TILE_TOPLEFT].h = TILE_HEIGHT;

			gTileClips[TILE_LEFT].x = 80;
			gTileClips[TILE_LEFT].y = 80;
			gTileClips[TILE_LEFT].w = TILE_WIDTH;
			gTileClips[TILE_LEFT].h = TILE_HEIGHT;

			gTileClips[TILE_BOTTOMLEFT].x = 80;
			gTileClips[TILE_BOTTOMLEFT].y = 160;
			gTileClips[TILE_BOTTOMLEFT].w = TILE_WIDTH;
			gTileClips[TILE_BOTTOMLEFT].h = TILE_HEIGHT;

			gTileClips[TILE_TOP].x = 160;
			gTileClips[TILE_TOP].y = 0;
			gTileClips[TILE_TOP].w = TILE_WIDTH;
			gTileClips[TILE_TOP].h = TILE_HEIGHT;

			gTileClips[TILE_CENTER].x = 160;
			gTileClips[TILE_CENTER].y = 80;
			gTileClips[TILE_CENTER].w = TILE_WIDTH;
			gTileClips[TILE_CENTER].h = TILE_HEIGHT;

			gTileClips[TILE_BOTTOM].x = 160;
			gTileClips[TILE_BOTTOM].y = 160;
			gTileClips[TILE_BOTTOM].w = TILE_WIDTH;
			gTileClips[TILE_BOTTOM].h = TILE_HEIGHT;

			gTileClips[TILE_TOPRIGHT].x = 240;
			gTileClips[TILE_TOPRIGHT].y = 0;
			gTileClips[TILE_TOPRIGHT].w = TILE_WIDTH;
			gTileClips[TILE_TOPRIGHT].h = TILE_HEIGHT;

			gTileClips[TILE_RIGHT].x = 240;
			gTileClips[TILE_RIGHT].y = 80;
			gTileClips[TILE_RIGHT].w = TILE_WIDTH;
			gTileClips[TILE_RIGHT].h = TILE_HEIGHT;

			gTileClips[TILE_BOTTOMRIGHT].x = 240;
			gTileClips[TILE_BOTTOMRIGHT].y = 160;
			gTileClips[TILE_BOTTOMRIGHT].w = TILE_WIDTH;
			gTileClips[TILE_BOTTOMRIGHT].h = TILE_HEIGHT;
		}
	}

	//Close the file
	map.close();

	//If the map was loaded fine
	return tilesLoaded;
}

bool touchesWall(SDL_Rect box, Tile* tiles[])
{
	//Go through the tiles
	for (int i = 0; i < TOTAL_TILES; ++i)
	{
		//If the tile is a wall type tile
		if ((tiles[i]->getType() >= TILE_CENTER) && (tiles[i]->getType() <= TILE_TOPLEFT))
		{
			//If the collision box touches the wall tile
			if (checkCollision(box, tiles[i]->getBox()))
			{
				return true;
			}
		}
	}

	//If no wall tiles were touched
	return false;
}

SDL_Rect Entity::getBox()
{
	return mBox;
}



Tile::Tile(int x, int y, int tileType)
{
	//Get the offsets
	mBox.x = x;
	mBox.y = y;

	//Set the collision box
	mBox.w = TILE_WIDTH;
	mBox.h = TILE_HEIGHT;

	//Get the tile type
	mType = tileType;
}

void Tile::Render(SDL_Rect& camera)
{
	//If the tile is on screen
	if (checkCollision(camera, mBox))
	{
		//Show the tile
		gTileTexture.render(mBox.x - camera.x, mBox.y - camera.y, &gTileClips[mType]);
	}
}

int Tile::getType()
{
	return mType;
}



StaticObj::StaticObj(const char* file)
{

	if (!StaticObjTex.loadFromFile(file))
	{
		printf("Failed to load the texture!\n");
	}
	mBox.x = 0;
	mBox.y = 0;
	mBox.w = StaticObjTex.getHeight();
	mBox.h = StaticObjTex.getWidth();
	return;
}

StaticObj::StaticObj(int x, int y, int tileType)
{
	//Get the offsets
	mBox.x = x;
	mBox.y = y;

	//Set the collision box
	mBox.w = TILE_WIDTH;
	mBox.h = TILE_HEIGHT;

}

void StaticObj::SetPos(int x, int y)
{
	mBox.x = x;
	mBox.y = y;
}

void StaticObj::Render(SDL_Rect& camera)
{
	if (checkCollision(camera, mBox))
	{
		//Show the tile
		StaticObjTex.render(mBox.x - camera.x, mBox.y - camera.y);
	}
}

StaticObj::~StaticObj()
{
	StaticObjTex.free();
}




MoveObj::MoveObj(const char* file, int w, int h)
{
	mVelX = 0;
	mVelY = 0;
	mBox.x = 0;
	mBox.y = 0;
	mBox.w = w;
	mBox.h = h;
	frame = 0;
	mBox.w = w;
	mBox.h = h;   //for the test piecture,w should be 32,h should be 48
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

void MoveObj::Move(Tile* tiles[])
{
	//Move the dot left or right
	mBox.x += mVelX;

	//If the dot went too far to the left or right or touched a wall
	if ((mBox.x < 0) || (mBox.x + mBox.w > LEVEL_WIDTH) || touchesWall(mBox, tiles))
	{
		//move back
		mBox.x -= mVelX;
	}

	//Move the dot up or down
	mBox.y += mVelY;

	//If the dot went too far up or down or touched a wall
	if ((mBox.y < 0) || (mBox.y + mBox.h > LEVEL_HEIGHT) || touchesWall(mBox, tiles))
	{
		//move back
		mBox.y -= mVelY;
	}
}

void MoveObj::Render(SDL_Rect& camera)
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

	gSpriteSheetTexture.render(mBox.x - camera.x, mBox.y - camera.y, &gSpriteClips[curDirection][frame / MoveSpeedFrame]);
	//SDL_RenderPresent(gRenderer);

}

int MoveObj::GetMaxFrames()
{
	return WALKING_ANIMATION_FRAMES;
}

void MoveObj::setCamera(SDL_Rect& camera)
{
	camera.x = (mBox.x + mBox.w / 2) - SCREEN_WIDTH / 2;
	camera.y = (mBox.y + mBox.h / 2) - SCREEN_HEIGHT / 2;

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
}





int main(int argc, char* args[])
{

	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		MoveObj testObj("img/man_1.png", 32, 48);
		StaticObj t1("img/hello_world.bmp");
		gBGTexture.loadFromFile("img/testBG.png");
		gTileTexture.loadFromFile("img/tiles.png");

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

		//The level tiles
		Tile* tileSet[TOTAL_TILES];

		setTiles(tileSet);

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
			testObj.Move(tileSet);
			testObj.setCamera(camera);

			//Calculate and correct fps
			float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
			if (avgFPS > 2000000)
			{
				avgFPS = 0;
			}

			//Clear screen
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(gRenderer);

			for (int i = 0; i < TOTAL_TILES; ++i)
			{
				tileSet[i]->Render(camera);
			}

			//gBGTexture.render(0, 0, &camera);
			t1.Render(camera);
			testObj.Render(camera);
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

