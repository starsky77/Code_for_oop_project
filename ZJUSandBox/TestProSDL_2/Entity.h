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
	virtual void Render(int camX, int camY) = 0;

	//Position accessors
	int GetPosX();
	int GetPosY();


protected:
	//The X and Y offsets of the dot
	int mPosX, mPosY;

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
	virtual void Render(int camX, int camY);
	StaticObj();
	~StaticObj();

private:

};

class Role :MoveObj
{
public:
	Role();
	~Role();

private:

};


class Player:Role
{
public:
	Player();
	~Player();
	void ControlMove();


private:

};


#endif // !1
