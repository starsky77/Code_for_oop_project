#ifndef _eNTITY_H
#define _eNTITY_H

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include<iostream>
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
	//virtual void Render(int camX, int camY, int size = 1) = 0;

	//Position accessors
	virtual int GetPosX();
	virtual int GetPosY();
	virtual int GetW();
	virtual int GetH();

protected:
	//The X and Y offsets of the dot
	int mPosX, mPosY;//the location in the windows
	int w, h;//height,width

};


class StaticObj :public Entity
{
public:
	void Render(int camX, int camY);
	void SetPos(int x, int y);
	StaticObj(const char* file);
	~StaticObj();

private:
	LTexture StaticObjTex;
	SDL_Rect rect;
	SDL_Surface* imag = NULL;
	SDL_Texture* tex = NULL;
};


class MoveObj :public Entity
{
public:
	MoveObj(const char* file, int w, int h);
	~MoveObj();

	virtual void Move();
	virtual void Render(int camX, int camY);
	void handleEvent(SDL_Event& e);

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

/*
class Role :MoveObj
{
public:
	Role();
	~Role();

private:

};


class Player :Role
{
public:
	Player();
	~Player();
	void ControlMove();


private:

};
*/

#endif // !1
