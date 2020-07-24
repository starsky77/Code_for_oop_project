#ifndef _tEXT_H
#define _tEXT_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <stdio.h>
#include <string>
#include<iostream>
#include<fstream>

#include"Entity.h";


using namespace std;


enum LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 4
};



class LButton
{
public:
	//Initializes internal variables
	LButton(int x, int y, const char* file);

	//Sets top left position
	void setPosition(int x, int y);

	//Handles mouse event
	void handleEvent(SDL_Event* e);

	//Shows button sprite
	void render();

	void ChangeShowState();

	void setReactFun(void (*func_react)(LButton* Obj));


private:
	//Top left position
	SDL_Point mPosition;

	//Currently used global sprite
	LButtonSprite mCurrentSprite;

	SDL_Rect gSpriteClips[BUTTON_SPRITE_TOTAL];

	LTexture gButtonSpriteSheetTexture;

	bool isShow;
	bool isPressed;

	void (*React_fun)(LButton* Obj);
};



class Frame
{
public:
	void Render(int camX, int camY, int size = 1);
	void ChangeShowState();
	Frame(const char* file);
	~Frame();

private:
	SDL_Rect rect;
	SDL_Surface* imag = NULL;
	SDL_Texture* tex = NULL;
	bool isShow;

};

class Text
{
public:
	void Render(int x, int y, int size);
	void ChangeShowState();
	Text(const char* s);
	~Text();
private:
	//The actual hardware texture
	SDL_Rect rect;
	SDL_Texture* mTexture;
	//Image dimensions
	int mWidth;
	int mHeight;
	bool isShow;

};


class timer {
public:
	void Render(int x, int y, int size);
	void Setisshow();
	timer();
	~timer() {}
private:
	double second, minute, hour;
	bool isshow;
};


#endif 