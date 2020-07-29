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

//Possible button states corresponding to buttons
enum LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 4
};


class Frame
{
public:
	void Render(int camX, int camY);
	void Render();
	void ChangeShowState();
	void setAlpha(Uint8 a);
	void setPos(int x, int y);
	SDL_Rect getBox();
	Frame(const char* file);
	Frame() { }
	~Frame();

private:
	SDL_Rect rect;
	LTexture frameTex;
	bool isShow;

};

class Text
{
public:
	void Render(int x, int y);
	void Render();
	void ChangeShowState();
	Text(const string s, int size, Uint32 width);
	Text(const string s, int size);
	SDL_Rect getBox();
	void setPos(int x, int y);
	bool getIsShow();
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

//Show the clock in the game
class timer {
public:
	void Render(int x, int y, int size);
	void Setisshow();
	void SetTime(double news, double newm, double newh);
	double getHour();
	double getMin();
	double getSec();
	timer();
	~timer() {}
private:
	double second, minute, hour;
	bool isshow;
};


#endif 