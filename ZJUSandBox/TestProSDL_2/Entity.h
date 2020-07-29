#ifndef _eNTITY_H
#define _eNTITY_H

#include <SDL.h>
#include <SDL_image.h>
#include<SDL_mixer.h>

#include <vector> 
#include <stdio.h>
#include <string>
#include <string.h>
#include <fstream>
#include <sstream>
#include <iostream>

#include"Utils.h"
#include"Text.h"

using namespace std;

extern const string SPLITCHAR;

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

//Role attribute value range
enum CharacterAttribures
{
	Attribures_0,
    Attribures_1,
	Attribures_2,
	Attribures_3,
	Attribures_4,
	Attribures_5,
	Attribures_6,
	Attribures_7,
	Attribures_8,
	Attribures_9,
};

//The base class for all entities in the game
class Entity
{
public:
	Entity(int mapID);
	~Entity();

	virtual void handleEvent(SDL_Event& e);
	virtual void Render(SDL_Rect& camera) = 0;
	virtual void Render() {};
	void setReactFun(void (*func_react)(Entity* Obj));
	void setTextBox(const char* file_box, const char* file_text);
	void setmapID(int mapID);
	void textShow();
	void ChangeTextShowState();
    void ChangeShowState();
	void ChangeReactState();
	void ChangeCollisionState();
	void updataTextBox(int newID);

	SDL_Rect getBox();
	int getcurTextID();
	bool getTextIsShow();
	bool getIsShow();
	bool getIsReact();
	bool getisCollision();
	

protected:
	//Collision box of the Entity
	SDL_Rect mBox;

	bool isShow;
	bool isReact;
	bool isPressed;
	bool isCollision;

	Frame* textBox;
	vector <string> textContent;
	Text* text;
	//ID of the currently displayed text
	int curTextID;
	//The map ID to which the marked object belongs to
	int mapID;

	//Callback function
	void (*React_fun)(Entity* Obj);

};


//Textures used to compose the map
class Tile :public Entity
{
public:
	//Initializes position and type
	Tile(int x, int y, int tileType, int mapID);
	//Shows the tile
	virtual  void Render(SDL_Rect& camera);
	//Get the tile type
	int getType();

private:
	//The tile type
	int mType;
};

//Static objects in the game,can not be moved, only has one display method
class StaticObj :public Entity
{
public:
	StaticObj(const char* file, int mapID, double size = 1);
	~StaticObj();
	//Render according to the coordinate position in the map (not moving with the camera)
	virtual void Render(SDL_Rect& camera);
	//Render based on absolute position (always move with the camera)
	virtual void Render();
	void SetPos(int x, int y);
	//Used to change the transparency of the object, the value range is 0-255
	void setAlpha(Uint8 a);

protected:
	LTexture StaticObjTex;
};




class MoveObj :public Entity
{
public:
	MoveObj(const char* file, int w, int h, int mapID);
	~MoveObj();

	virtual void Move(Tile* tiles[], vector<Entity*> Objs);
	virtual void Render(SDL_Rect& camera);
	void setCamera(SDL_Rect& camera);
	void SetPos(int x, int y);
	void SetDir(KeyPressSurfaces nextDirection);

	virtual int GetVelX();
	virtual int GetVelY();
	virtual int GetMaxFrames();

	//Indicates which frame the object is currently in the display state
	int frame;


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
	Player(const char* file, int w, int h,int mapID);
	~Player();

	virtual void handleEvent(SDL_Event& e);
	void showAttribures();
	void setHealeth(int offset);
	void setMood(int offset);
	void setGPA(float offset);

private:
	CharacterAttribures health;
	CharacterAttribures mood;
	float GPA;

	//Icon showing Attribures
	vector<StaticObj*> health_icon;
	vector<StaticObj*> mood_icon;
	Text* GPA_icon;
};

class LButton
{
public:
	//Initializes internal variables
	LButton(int x, int y, const char* file);

	~LButton();

	//Sets top left position
	void setPosition(int x, int y);

	//Handles mouse event
	void handleEvent(SDL_Event* e, Entity* Obj);

	//Shows button sprite
	void render();

	void ChangeShowState();

	void ChangeReactState();

	void setReactFun(void (*func_react)(LButton* Btn, Entity* Obj));

	void setAlpha(Uint8 a);


private:
	//Top left position
	SDL_Point mPosition;

	//Currently used global sprite
	LButtonSprite mCurrentSprite;

	SDL_Rect gSpriteClips[BUTTON_SPRITE_TOTAL];

	LTexture gButtonSpriteSheetTexture;

	bool isShow;
	bool isPressed;
	bool isReact;

	void (*React_fun)(LButton* Btn, Entity* Obj);
};


#endif // !1