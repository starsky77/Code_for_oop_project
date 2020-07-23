#ifndef _eNTITY_H
#define _eNTITY_H

#include <SDL.h>
#include <SDL_image.h>
#include<SDL_mixer.h>
#include <vector> 
#include <stdio.h>
#include <string>
#include<iostream>
#include<fstream>
#include"Utils.h"

using namespace std;


extern const int LEVEL_WIDTH;
extern const int LEVEL_HEIGHT;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int SCREEN_FPS;
extern const int SCREEN_TICK_PER_FRAME;

extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
extern LTexture gBGTexture;



enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_TOTAL
};


class Entity
{
public:
	SDL_Rect getBox();
	void ChangeShowState();
	void ChangeReactState();
	virtual void handleEvent(SDL_Event& e);
	virtual void Render(SDL_Rect& camera) = 0;
	void setReactFun(void (*func_react)(Entity* Obj));

protected:
	//Collision box of the Entity
	SDL_Rect mBox;
	bool isShow;
	bool isReact;
	void (*React_fun)(Entity* Obj);

};

class Tile :public Entity
{
public:
	//Initializes position and type
	Tile(int x, int y, int tileType);

	//Shows the tile
	virtual  void Render(SDL_Rect& camera);

	//Get the tile type
	int getType();


private:
	//The tile type
	int mType;
};




class StaticObj :public Entity
{
public:
	virtual void Render(SDL_Rect& camera);
	void SetPos(int x, int y);
	StaticObj(const char* file);
	StaticObj(int x, int y, int tileType);
	~StaticObj();
private:
	LTexture StaticObjTex;

};


class MoveObj :public Entity
{
public:
	MoveObj(const char* file, int w, int h);
	~MoveObj();

	virtual void Move(Tile* tiles[], vector<Entity*> Objs);
	virtual void Render(SDL_Rect& camera);
	//virtual void handleEvent(SDL_Event& e);
	void setCamera(SDL_Rect& camera);
	void SetPos(int x, int y);

	int frame;

	virtual int GetVelX();
	virtual int GetVelY();
	virtual int GetMaxFrames();



protected:
	//The total number of pictures in a set of actions
	static const int  WALKING_ANIMATION_FRAMES = 4;
	//Number of pixels moved per time
	static const int DOT_VEL = 4;
	//The number of frames interval between each switching action
	static const int MoveSpeedFrame = 12;

	//The velocity of the Obj
	int mVelX, mVelY;

	//Store image files
	SDL_Rect  gSpriteClips[KEY_PRESS_SURFACE_TOTAL][WALKING_ANIMATION_FRAMES];
	LTexture gSpriteSheetTexture;


	//Indicates the current direction
	KeyPressSurfaces curDirection;
	bool isMoving;
};


class Player :public MoveObj
{

public:
	Player(const char* file, int w, int h);
	~Player();

	virtual void handleEvent(SDL_Event& e);

private:

};




#endif // !1