#ifndef _eNTITY_H
#define _eNTITY_H

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include<iostream>

using namespace std;


class Entity
{
public:
	virtual void Render(int camX, int camY, int size = 1) = 0;

	//Position accessors
	int GetPosX();
	int GetPosY();


protected:
	//The X and Y offsets of the dot
	int mPosX, mPosY;
	//if need to draw
	int x, y;//the location in the windows
	int w, h;//height,width

};


class MoveObj :Entity
{
public:
	virtual void Move();
	virtual void Render(int camX, int camY);
	MoveObj();
	~MoveObj();
	int GetVelX();
	int GetVelY();



protected:
	//The velocity of the dot
	int mVelX, mVelY;

};

class StaticObj :Entity
{
public:
	void Render(int camX, int camY, int size = 1);
	StaticObj(const char* file);
	~StaticObj();

private:
	SDL_Rect rect;
	SDL_Surface* imag = NULL;
	SDL_Texture* tex = NULL;
};

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


#endif // !1
