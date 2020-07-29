#include"Entity.h";
#include"Utils.h"
#include"Text.h"

//When reading a txt file, the text will be divided into multiple segments for storage according to the string
const string SPLITCHAR = "\n";
const int TEXTSIZE = 18;

const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

//Num of map in the game
const int MapNum = 2;

//Tile constants
//40 * 30 Tiles 
const int TILE_WIDTH = 32;
const int TILE_HEIGHT = 32;
const int TOTAL_TILES = 1200;
const int TOTAL_TILE_SPRITES_W = 16;
const int TOTAL_TILE_SPRITES_H = 64;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

LTexture gBGTexture;
LTexture gTileTexture;
SDL_Rect gTileClips[TOTAL_TILE_SPRITES_W * TOTAL_TILE_SPRITES_H];
Player* PlayerRole;
StaticObj* BlackBox;

LButton* Button_accept_stu;
LButton* Button_refuse_stu;

timer time1;

int curMapID = 0;

CharacterAttribures& operator++(CharacterAttribures& op)
{
	if (op + 1 > Attribures_9)
		op = op;
	else
	{
		op = CharacterAttribures(op + 1);
	}
	return op;
}

CharacterAttribures& operator--(CharacterAttribures& op)
{
	if (op - 1 < Attribures_0)
		op = op;
	else
	{
		op = CharacterAttribures(op - 1);
	}
	return op;
}

//An animation where the screen will gradually become black and then brighter
void TurnBlack()
{
	BlackBox = new StaticObj("img/Object/black.png", 0);
	BlackBox->setAlpha(0);
	BlackBox->ChangeShowState();
	BlackBox->SetPos(0, 0);

	BlackBox->Render();
	SDL_RenderPresent(gRenderer);

	
	LTimer* timer = new LTimer();
	for(int i = 0; i <= 255; i++)
	{
		timer->start();
		BlackBox->setAlpha(i);
		BlackBox->Render();
		SDL_RenderPresent(gRenderer);
		if (int frameTicks= timer->getTicks() <= 4)
		{
			SDL_Delay(4 - frameTicks);
		}
		timer->stop();
	}
	for (int i = 255; i >= 0; i-=5)
	{
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);
		timer->start();
		BlackBox->setAlpha(i);
		BlackBox->Render();
		SDL_RenderPresent(gRenderer);
		if (int frameTicks = timer->getTicks() <= 4)
		{
			SDL_Delay(4 - frameTicks);
		}
		timer->stop();
	}
	
}


//The following are the various callback functions set
void map0to1(Entity* Obj)
{
	curMapID = 1;
	PlayerRole->SetPos(0, 600);
	TurnBlack();
}

void map1to0(Entity* Obj)
{
	curMapID = 0;
	PlayerRole->SetPos(0, 20);
	TurnBlack();
}

void ReactTeacher(Entity* Obj)
{
	int TextID_tmp = Obj->getcurTextID();
	if (Obj->getcurTextID() == 3)
	{
		PlayerRole->ChangeReactState();
		Obj->updataTextBox(0);
		Obj->ChangeTextShowState();
		PlayerRole->setGPA(0.3);
		PlayerRole->setMood(-1);
	}
	else if (Obj->getcurTextID() == 2)
	{
		TurnBlack();
		time1.SetTime(time1.getSec(), time1.getMin(), time1.getHour() + 5);
		Obj->updataTextBox(TextID_tmp + 1);
	}
	else if (Obj->getcurTextID() == 0)
	{
		if (Obj->getTextIsShow())
		{
			Obj->updataTextBox(TextID_tmp + 1);
		}
		else
		{
			PlayerRole->ChangeReactState();
			Obj->ChangeTextShowState();
		}
	
	}
	else
	{
		Obj->updataTextBox(TextID_tmp + 1);
	}
}

void ReactStudent_1(Entity* Obj)
{
	int TextID_tmp = Obj->getcurTextID();
	if (Obj->getcurTextID() == 1 || Obj->getcurTextID() == 2)
	{
		PlayerRole->ChangeReactState();
		Obj->updataTextBox(0);
		Obj->ChangeTextShowState();
	}
	else if (Obj->getcurTextID() == 0)
	{
		if (Obj->getTextIsShow())
		{
			Obj->updataTextBox(TextID_tmp + 1);
		}
		else
		{
			PlayerRole->ChangeReactState();
			Obj->ChangeTextShowState();
			Button_accept_stu->ChangeShowState();
			Button_refuse_stu->ChangeShowState();
			Button_accept_stu->ChangeReactState();
			Button_refuse_stu->ChangeReactState();
			Obj->ChangeReactState();
		}

	}
}

void Refuse_stu_react(LButton* Btn, Entity* Obj)
{
	Obj->updataTextBox(2);
	Obj->ChangeReactState();
	Btn->ChangeReactState();
	Btn->ChangeShowState();
	Button_accept_stu->ChangeReactState();
	Button_accept_stu->ChangeShowState();
}

void Accept_stu_react(LButton* Btn, Entity* Obj)
{
	Obj->updataTextBox(1);
	PlayerRole->setGPA(-0.1);
	PlayerRole->setMood(+1);
	Obj->ChangeReactState();
	Btn->ChangeReactState();
	Btn->ChangeShowState();
	Button_refuse_stu->ChangeShowState();
	Button_refuse_stu->ChangeReactState();
	
	TurnBlack();
	time1.SetTime(time1.getSec(), time1.getMin(), time1.getHour() + 3);
}


//Rectangle collision detection
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

//Detect whether two objects are adjacent
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

//Determine whether it is a texture with collision volume
bool checkCollisionTiles(int ID)
{
	if (ID == 16)
		return true;
	return false;
}

//Initialize the map (texture)
bool setTiles(Tile* tiles[],const char * str, int mapID)
{
	//Success flag
	bool tilesLoaded = true;

	//The tile offsets
	int x = 0, y = 0;

	//Open the map
	std::ifstream map(str);

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
				tiles[i] = new Tile(x, y, tileType,mapID);
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
			for (int j = 0; j < TOTAL_TILE_SPRITES_H; j++)
			{

				for (int i = 0; i < TOTAL_TILE_SPRITES_W; i++)
				{
					gTileClips[j * TOTAL_TILE_SPRITES_W + i].x = i * TILE_WIDTH;
					gTileClips[j * TOTAL_TILE_SPRITES_W + i].y = j * TILE_HEIGHT;
					gTileClips[j * TOTAL_TILE_SPRITES_W + i].w = TILE_WIDTH;
					gTileClips[j * TOTAL_TILE_SPRITES_W + i].h = TILE_HEIGHT;
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
		//When the object is not displayed or does not participate in collision detection, 
		//collision detection will not be performed
		if (!Objs[i]->getIsShow())
			continue;
		if (!Objs[i]->getisCollision())
			continue;
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

Entity::Entity(int mapID)
{
	textBox = NULL;
	text = NULL;
	curTextID = 0;
	isCollision = true;
	this->mapID = mapID;
}

Entity::~Entity()
{
	free(textBox);
	free(text);
	textContent.clear();
}

//Initialize the text box and dialog content of an object, 
//and will automatically wrap according to the size of the text box
void Entity::setTextBox(const char* file_box, const char* file_text)
{
	//init textBox
	textBox = new Frame(file_box);
	textBox->setAlpha(200);

	//textBox is in the center of screen
	int text_X = SCREEN_WIDTH / 2 - textBox->getBox().w / 2;
	int text_Y = SCREEN_HEIGHT / 2 - textBox->getBox().h / 2;
	textBox->setPos(text_X, text_Y);

	//read from file and convert into string
	string str;
	ifstream ifile(file_text);
	ostringstream buf;
	char ch;
	while (buf && ifile.get(ch))
	{
		buf.put(ch);
	}
	str = buf.str();

	//split into several strings
	if ("" == str)
	{
		text = new Text("", TEXTSIZE, textBox->getBox().w - 10);
		textContent.push_back("");
		return;
	}
	char* strs = new char[str.length() + 1]; 
	strcpy(strs, str.c_str());
	char* d = new char[SPLITCHAR.length() + 1];
	strcpy(d, SPLITCHAR.c_str());
	char* p = strtok(strs, d);
	while (p) {
		string s = p; 
		textContent.push_back(s);
		p = strtok(NULL, d);
	}
	delete strs;
	delete d;

	text = new Text(textContent[0], TEXTSIZE, textBox->getBox().w - 10);
	text->setPos(text_X + 5, text_Y + 10);
}

void Entity::ChangeTextShowState()
{
	textBox->ChangeShowState();
	text->ChangeShowState();
}

bool Entity::getIsShow()
{
	return isShow;
}

bool Entity::getIsReact()
{
	return isReact;
}

bool Entity::getisCollision()
{
	return isCollision;
}

//Update the content of the current conversation
void Entity::updataTextBox(int newID)
{
	if (newID >= textContent.size())
	{
		newID = textContent.size() - 1;
	}
	else if (newID < 0)
	{
		newID = 0;
	}
	curTextID = newID;

	bool tmp = text->getIsShow();
	int lastPos_X = text->getBox().x;
	int lastPos_Y = text->getBox().y;
	free(text);
	text = new Text(textContent[curTextID], TEXTSIZE, textBox->getBox().w - 10);
	text->setPos(lastPos_X, lastPos_Y);
	if (tmp)
	{
		text->ChangeShowState();
	}
}

int Entity::getcurTextID()
{
	return curTextID;
}

bool Entity::getTextIsShow()
{
	return text->getIsShow();
}

void Entity::textShow()
{
	textBox->Render();
	text->Render();
}

void Entity::setmapID(int ID)
{
	mapID = ID;
}

void Entity::ChangeShowState()
{
	isShow = isShow ? false : true;
}

void Entity::ChangeReactState()
{
	isReact = isReact ? false : true;
}

void Entity::ChangeCollisionState()
{
	isCollision = isCollision ? false : true;
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
	if (!checkNearBy(this->getBox(), PlayerRole->getBox()))
		return;
	//If a key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_j:
			isPressed = true;
			break;

		}
	}
	//If a key was released
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_j:
			if (isPressed)
			{
				isPressed = false;
				React_fun(this);
			}
			break;
		}
	}
}

Tile::Tile(int x, int y, int tileType,int mapID):Entity(mapID)
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
	if (checkNearBy(camera, mBox))
	{
		//Show the tile
		gTileTexture.render(mBox.x - camera.x, mBox.y - camera.y, &gTileClips[mType]);
	}
}

int Tile::getType()
{
	return mType;
}



StaticObj::StaticObj(const char* file, int mapID,double size) :Entity(mapID)
{

	if (!StaticObjTex.loadFromFile(file,size))
	{
		printf("Failed to load the texture!\n");
	}
	mBox.x = 0;
	mBox.y = 0;
	mBox.w = StaticObjTex.getHeight();
	mBox.h = StaticObjTex.getWidth();
	isShow = false;
	isReact = false;
}


void StaticObj::setAlpha(Uint8 a)
{
	StaticObjTex.setAlpha(a);
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

void StaticObj::Render()
{
	if (!isShow)
		return;
	StaticObjTex.render(mBox.x, mBox.y);
}

StaticObj::~StaticObj()
{
	StaticObjTex.free();
}



//for the test piecture,w should be 32,h should be 48
MoveObj::MoveObj(const char* file, int w, int h,int mapID) :Entity(mapID)
{
	mVelX = 0;
	mVelY = 0;
	mBox.x = 0;
	mBox.y = 0;
	mBox.w = w;
	mBox.h = h;
	frame = 0;
	mBox.w = w;
	mBox.h = h;   
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
	isShow = false;
	isReact = false;
	isCollision = true;
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

void MoveObj::SetDir(KeyPressSurfaces nextDirection)
{
	curDirection = nextDirection;
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


Player::Player(const char* file, int w, int h,int mapID) :MoveObj(file, w, h,mapID)
{
	isReact = true;
	isShow = true;
	health = Attribures_9;
	mood = Attribures_9;
	GPA = 4.2;

	StaticObj *icon = NULL;
	string health_file = "img/UI/health_";
	string mood_file = "img/UI/mood_";
	string PNG = ".png";
	for (int i = Attribures_0; i <= Attribures_9; i++)
	{
		string postfix = to_string(i * 2);
		string fullName = health_file + postfix + PNG;
		char* str = (char*)fullName.data();
		icon = new StaticObj(str, 0.5);
		icon->SetPos(0, -30);
		icon->ChangeShowState();
		icon->setAlpha(200);
		health_icon.push_back(icon);

		fullName= mood_file + postfix + PNG;
		str = (char*)fullName.data();
		icon = new StaticObj(str, 0.5);
		icon->SetPos(0, 20);
		icon->ChangeShowState();
		icon->setAlpha(200);
		mood_icon.push_back(icon);
	}

	string GPA_full = "GPA:" + to_string(GPA);
	GPA_icon = new Text((char*)GPA_full.data(), TEXTSIZE);
	GPA_icon->setPos(0, 110);
	GPA_icon->ChangeShowState();
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

void Player::showAttribures()
{
	health_icon[health]->Render();
	mood_icon[mood]->Render();

	

	char GPA_tmp[8]="GPA:x.x";
	sprintf_s(GPA_tmp, sizeof(GPA_tmp),"GPA:%.1f", GPA);

	Text* tmp = GPA_icon;
	
	GPA_icon = new Text(GPA_tmp, TEXTSIZE);
	GPA_icon->setPos(0, 110);
	GPA_icon->ChangeShowState();
	GPA_icon->Render();

	delete(tmp);
	
}


void Player::setHealeth(int offset)
{
	if (offset >= 0)
	{
		for (int i = 0; i < offset; i++)
		{
			++health;
		}
	}
	else
	{
		for (int i = 0; i > offset; i--)
		{
			--health;
		}
	}
}

void Player::setMood(int offset)
{
	if (offset >= 0)
	{
		for (int i = 0; i < offset; i++)
		{
			++mood;
		}
	}
	else
	{
		for (int i = 0; i > offset; i--)
		{
			--mood;
		}
	}
}
void Player::setGPA(float offset)
{
	GPA += offset;
	if (GPA >= 5.0)
		GPA = 5.0;
}


int main(int argc, char* args[])
{

	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Initialization of basic variables
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

		//Initialization of other variables
		PlayerRole = new Player("img/role/man_1.png", 32, 48,0);
		StaticObj mapChangeIcon_0("img/Object/up.png", 0);
		StaticObj mapChangeIcon_1("img/Object/left.png", 1);
		MoveObj Teacher("img/role/man_3.png", 32, 48, 0);
		MoveObj Student_1("img/role/man_2.png", 32, 48, 0);
		Button_accept_stu = new LButton(68,27,"img/text/button_accept.png");
		Button_refuse_stu = new LButton(68,27, "img/text/button_refuse.png");

		Tile* tiledBG[MapNum][TOTAL_TILES];
		Tile* tiledMI[MapNum][TOTAL_TILES];
		Tile* tiledFR[MapNum][TOTAL_TILES];

		

		gBGTexture.loadFromFile("img/test/testBG.png");
		gTileTexture.loadFromFile("img/map/Tiled.png");
		

		//Some adjustments after variable initialization
		setTiles(tiledBG[0], "img/map/ClassRoom/ButtomMap.map", 0);
		setTiles(tiledMI[0], "img/map/ClassRoom/MiddleMap.map", 0);
		setTiles(tiledFR[0], "img/map/ClassRoom/TopMap.map", 0);
		setTiles(tiledBG[1], "img/map/Bridge/ButtomMap.map", 1);
		setTiles(tiledMI[1], "img/map/Bridge/MiddleMap.map", 1);
		setTiles(tiledFR[1], "img/map/Bridge/TopMap.map", 1);

		mapChangeIcon_0.setReactFun(map0to1);
		mapChangeIcon_0.SetPos(10,10);
		mapChangeIcon_0.ChangeShowState();
		mapChangeIcon_0.ChangeCollisionState();
		mapChangeIcon_1.setReactFun(map1to0);
		mapChangeIcon_1.SetPos(0, 600);
		mapChangeIcon_1.ChangeCollisionState();
		mapChangeIcon_1.ChangeShowState();
		Teacher.SetPos(1200, 700);
		Teacher.ChangeShowState();
		Teacher.SetDir(KEY_PRESS_SURFACE_LEFT);
		Teacher.setTextBox("img/text/frame3.png", "img/textContent/TeacherText.txt");
		Teacher.setReactFun(ReactTeacher);
		Student_1.SetPos(1000,180);
		Student_1.ChangeShowState();
		Student_1.setTextBox("img/text/frame3.png", "img/textContent/Student_1Text.txt");
		Student_1.setReactFun(ReactStudent_1);
		time1.Setisshow();
		time1.SetTime(0, 0, 9);
		Button_accept_stu->setPosition(200, 280);
		Button_accept_stu->setReactFun(Accept_stu_react);
		Button_refuse_stu->setPosition(400, 280);
		Button_refuse_stu->setReactFun(Refuse_stu_react);
		//time1


		PlayerRole->SetPos(0,300);
		


		//Objects with collision volume
		vector<vector<Entity*>> Objs;
		vector<Entity*> Objs_1;
		vector<Entity*> Objs_2;
		Objs_1.push_back(&mapChangeIcon_0);
		Objs_1.push_back(&Teacher);
		Objs_1.push_back(&Student_1);
		Objs_2.push_back(&mapChangeIcon_1);
		Objs.push_back(Objs_1);
		Objs.push_back(Objs_2);


		//start the timer
		fpsTimer.start();

		//open the muisc
		Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
		Mix_Music* sound = Mix_LoadMUS("music/music.wav");
		Mix_PlayMusic(sound, -1);

		//main loop
		while (!quit)
		{
			while (SDL_PollEvent(&e) != 0)
			{
				
				//User requests quit
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
				//All the "hadnleEvent" func should place below
				int curMapID_tmp = curMapID;
				PlayerRole->handleEvent(e);
				for (int i = 0; i < Objs[curMapID_tmp].size(); i++)
				{
					//react for NPC
					Objs[curMapID_tmp][i]->handleEvent(e);
				}
				Button_accept_stu->handleEvent(&e, &Student_1);
				Button_refuse_stu->handleEvent(&e, &Student_1);
				
			}
			//react of input

			

			if (time1.getHour() >= 21 || time1.getHour() <= 8)
			{
				if (Teacher.getIsShow() && PlayerRole->getIsReact())
				{
					Teacher.ChangeReactState();
					Teacher.ChangeShowState();
				}
			}
			else
			{
				if (!Teacher.getIsShow())
				{
					Teacher.ChangeReactState();
					Teacher.ChangeShowState();
				}
			}

			PlayerRole->Move(tiledBG[curMapID], Objs[curMapID]);
			PlayerRole->setCamera(camera);

			//Calculate and correct fps
			float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
			if (avgFPS > 2000000)
			{
				avgFPS = 0;
			}

			//Clear screen
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(gRenderer);

			//render 

			for (int i = 0; i < TOTAL_TILES; ++i)
			{
				tiledBG[curMapID][i]->Render(camera);
			}
			for (int i = 0; i < TOTAL_TILES; ++i)
			{
				tiledMI[curMapID][i]->Render(camera);
			}
			PlayerRole->Render(camera);
			for (int i = 0; i < TOTAL_TILES; ++i)
			{
				tiledFR[curMapID][i]->Render(camera);
			}


			for (int i = 0; i < Objs[curMapID].size(); i++)
			{
				//react for NPC
				Objs[curMapID][i]->Render(camera);
			}
			
			PlayerRole->showAttribures();
			Teacher.textShow();
			Student_1.textShow();
			time1.Render(550, 10, 18);
			Button_accept_stu->render();
			Button_refuse_stu->render();
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