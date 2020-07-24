#include"Entity.h";
#include"Utils.h"
#include"Text.h"

const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;


//Tile constants
//40 * 30 Tiles 
const int TILE_WIDTH = 32;
const int TILE_HEIGHT = 32;
const int TOTAL_TILES = 1200;
const int TOTAL_TILE_SPRITES_W = 16;
const int TOTAL_TILE_SPRITES_H = 12;

//The different tile sprites
/*
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
*/

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

LTexture gBGTexture;
LTexture gTileTexture;
Player* testObj;
SDL_Rect gTileClips[TOTAL_TILE_SPRITES_W * TOTAL_TILE_SPRITES_H];


Frame* biaoge = NULL;
Text* text_1 = NULL;


void TestReactFun(Entity* Obj)
{
	biaoge->ChangeShowState();
	text_1->ChangeShowState();
}

void testReacFun_2(LButton* Obj)
{
	biaoge->ChangeShowState();
	text_1->ChangeShowState();
}

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


bool checkNearBy(SDL_Rect a, SDL_Rect b)
{
	int nearByDistance = 10;
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
	if (bottomA <= topB - nearByDistance)
	{
		return false;
	}

	if (topA >= bottomB + nearByDistance)
	{
		return false;
	}

	if (rightA <= leftB - nearByDistance)
	{
		return false;
	}

	if (leftA >= rightB + nearByDistance)
	{
		return false;
	}

	//If none of the sides from A are outside B
	return true;
}

bool checkCollisionTiles(int ID)
{
	return false;
}


bool setTiles(Tile* tiles[])
{
	//Success flag
	bool tilesLoaded = true;

	//The tile offsets
	int x = 0, y = 0;

	//Open the map
	std::ifstream map("img/map/testmap.map");

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
			if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES_W * TOTAL_TILE_SPRITES_H))
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
			for (int i = 0; i < TOTAL_TILE_SPRITES_H; i++)
			{

				for (int j = 0; j < TOTAL_TILE_SPRITES_W; j++)
				{
					gTileClips[i * TOTAL_TILE_SPRITES_H + j].x = i * TILE_WIDTH;
					gTileClips[i * TOTAL_TILE_SPRITES_H + j].y = j * TILE_HEIGHT;
					gTileClips[i * TOTAL_TILE_SPRITES_H + j].w = TILE_WIDTH;
					gTileClips[i * TOTAL_TILE_SPRITES_H + j].h = TILE_HEIGHT;
				}
			}
		}
	}

	//Close the file
	map.close();

	//If the map was loaded fine
	return tilesLoaded;
}

bool touchesWall(SDL_Rect box, Tile* tiles[], vector<Entity*> Objs)
{
	//Go through the tiles
	for (int i = 0; i < TOTAL_TILES; ++i)
	{
		//If the tile is a wall type tile
		if (checkCollisionTiles(tiles[i]->getType()))
		{
			//If the collision box touches the wall tile
			if (checkCollision(box, tiles[i]->getBox()))
			{
				return true;
			}
		}
	}

	for (int i = 0; i < Objs.size(); i++)
	{
		if (checkCollision(box, Objs[i]->getBox()))
		{
			return true;
		}
	}

	//If no wall tiles were touched
	return false;
}

SDL_Rect Entity::getBox()
{
	return mBox;
}

void Entity::ChangeShowState()
{
	isShow = isShow ? false : true;
}

void Entity::ChangeReactState()
{
	isReact = isReact ? false : true;
}

void Entity::setReactFun(void (*func_react)(Entity* Obj))
{
	React_fun = func_react;
	isReact = true;
}

void Entity::handleEvent(SDL_Event& e)
{
	if (!isReact)
		return;
	if (!checkNearBy(this->getBox(), testObj->getBox()))
		return;
	//If a key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_j:
			React_fun(this);
			break;

		}
	}
	//If a key was released
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		}
	}
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
	isShow = true;
	isReact = false;
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
	isShow = true;
	isReact = false;
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
	if (checkCollision(camera, mBox) && isShow)
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
	isShow = true;
	isReact = false;
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

void MoveObj::SetPos(int x, int y)
{
	mBox.x = x;
	mBox.y = y;
}



void MoveObj::Move(Tile* tiles[], vector<Entity*> Objs)
{
	//Move the dot left or right
	mBox.x += mVelX;

	//If the dot went too far to the left or right or touched a wall
	if ((mBox.x < 0) || (mBox.x + mBox.w > LEVEL_WIDTH) || touchesWall(mBox, tiles, Objs))
	{
		//move back
		mBox.x -= mVelX;
	}

	//Move the dot up or down
	mBox.y += mVelY;

	//If the dot went too far up or down or touched a wall
	if ((mBox.y < 0) || (mBox.y + mBox.h > LEVEL_HEIGHT) || touchesWall(mBox, tiles, Objs))
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

	if (checkCollision(camera, mBox) && isShow)
	{
		//Show the tile
		gSpriteSheetTexture.render(mBox.x - camera.x, mBox.y - camera.y, &gSpriteClips[curDirection][frame / MoveSpeedFrame]);
	}
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

Player::Player(const char* file, int w, int h) :MoveObj(file, w, h)
{
	isReact = true;
}

Player::~Player()
{
	gSpriteSheetTexture.free();
}

void Player::handleEvent(SDL_Event& e)
{
	if (!isReact)
		return;

	//If a key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_UP:
			mVelY -= DOT_VEL;
			curDirection = KEY_PRESS_SURFACE_UP;
			isMoving = true;
			break;
		case SDLK_DOWN:
			mVelY += DOT_VEL;
			curDirection = KEY_PRESS_SURFACE_DOWN;
			isMoving = true;
			break;
		case SDLK_LEFT:
			mVelX -= DOT_VEL;
			curDirection = KEY_PRESS_SURFACE_LEFT;
			isMoving = true;
			break;
		case SDLK_RIGHT:
			mVelX += DOT_VEL;
			curDirection = KEY_PRESS_SURFACE_RIGHT;
			isMoving = true;
			break;
		}
	}
	//If a key was released
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_UP:
			mVelY += DOT_VEL;
			isMoving = false;
			break;
		case SDLK_DOWN:
			mVelY -= DOT_VEL;
			isMoving = false;
			break;
		case SDLK_LEFT:
			mVelX += DOT_VEL;
			isMoving = false;
			break;
		case SDLK_RIGHT:
			mVelX -= DOT_VEL;
			isMoving = false;
			break;
		}
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
		testObj = new Player("img/role/man_1.png", 32, 48);
		MoveObj NPC_1("img/role/man_2.png", 32, 48);
		MoveObj NPC_2("img/role/man_3.png", 32, 48);
		MoveObj NPC_3("img/role/woman_1.png", 32, 48);
		StaticObj t1("img/test/hello_world.bmp");
		//The level tiles
		Tile* tileSet[TOTAL_TILES];
		timer time1;
		LButton testBtn(68, 27, "img/text/button_1.png");

		biaoge = new Frame("img/text/frame1.bmp");
		text_1 = new Text("The DDL of the project is in this weekend!!!!");
		biaoge->ChangeShowState();
		text_1->ChangeShowState();
		time1.Setisshow();

		gBGTexture.loadFromFile("img/test/testBG.png");
		gTileTexture.loadFromFile("img/map/map.png");
		setTiles(tileSet);


		NPC_1.SetPos(100, 0);
		NPC_1.setReactFun(TestReactFun);
		NPC_2.SetPos(0, 120);
		NPC_3.SetPos(200, 0);
		testBtn.setPosition(200, 200);
		testBtn.setReactFun(testReacFun_2);
		testBtn.ChangeShowState();



		vector<Entity*> Objs;
		Objs.push_back(&NPC_1);
		Objs.push_back(&NPC_2);
		Objs.push_back(&NPC_3);


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


		Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
		Mix_Music* sound = Mix_LoadMUS("music/music.wav");
		Mix_PlayMusic(sound, -1);

		while (!quit)
		{
			while (SDL_PollEvent(&e) != 0)
			{
				//User requests quit
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
				testObj->handleEvent(e);
				for (int i = 0; i < Objs.size(); i++)
				{
					//react for NPC
					Objs[i]->handleEvent(e);
				}
				testBtn.handleEvent(&e);
			}
			testObj->Move(tileSet, Objs);
			testObj->setCamera(camera);

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
			//t1.Render(camera);
			testObj->Render(camera);

			NPC_1.Render(camera);
			NPC_2.Render(camera);
			NPC_3.Render(camera);
			time1.Render(550, 0, 1);

			testBtn.render();


			biaoge->Render(200, 350, 2);
			text_1->Render(200, 350, 2);

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