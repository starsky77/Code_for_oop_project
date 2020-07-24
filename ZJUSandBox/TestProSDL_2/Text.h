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



class Frame {
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