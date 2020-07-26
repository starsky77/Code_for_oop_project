#include"Entity.h";
#include"Utils.h"
#include"Text.h"


SDL_Color color = { 0, 0, 0 };


LButton::LButton(int x, int y, const char* file)
{
	mPosition.x = 0;
	mPosition.y = 0;

	mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;


	gButtonSpriteSheetTexture.loadFromFile(file);


	for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
	{
		gSpriteClips[i].x = 0;
		gSpriteClips[i].y = i * y;
		gSpriteClips[i].w = x;
		gSpriteClips[i].h = y;
	}

	isShow = false;
	isPressed = false;
}

void LButton::setPosition(int x, int y)
{
	mPosition.x = x;
	mPosition.y = y;
}

void LButton::ChangeShowState()
{
	isShow = isShow ? false : true;
}

void LButton::setAlpha(Uint8 a)
{
	gButtonSpriteSheetTexture.setAlpha(a);
}


void LButton::handleEvent(SDL_Event* e)
{
	if (!isShow)
		return;
	//If mouse event happened
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if (x < mPosition.x)
		{
			inside = false;
		}
		//Mouse is right of the button
		else if (x > mPosition.x + gSpriteClips[0].w)
		{
			inside = false;
		}
		//Mouse above the button
		else if (y < mPosition.y)
		{
			inside = false;
		}
		//Mouse below the button
		else if (y > mPosition.y + gSpriteClips[0].h)
		{
			inside = false;
		}

		//Mouse is outside button
		if (!inside)
		{
			mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
		}
		//Mouse is inside button
		else
		{
			//Set mouse over sprite
			switch (e->type)
			{
			case SDL_MOUSEMOTION:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
				break;

			case SDL_MOUSEBUTTONDOWN:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
				isPressed = true;
				break;

			case SDL_MOUSEBUTTONUP:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
				if (isPressed)
				{
					isPressed = 0;
					React_fun(this);
				}
				break;
			}
		}
	}
}

void LButton::render()
{
	if (!isShow)
		return;
	//Show current button sprite
	gButtonSpriteSheetTexture.render(mPosition.x, mPosition.y, &gSpriteClips[mCurrentSprite]);
}


void LButton::setReactFun(void (*func_react)(LButton* Obj))
{
	React_fun = func_react;

}

Frame::Frame(const char* file)
{
	frameTex.loadFromFile(file);
	
	rect.x = 0;
	rect.y = 0;
	rect.h = frameTex.getHeight();
	rect.w = frameTex.getWidth();
	isShow = false;
	//SDL_FreeSurface(imag);
	return;
}

void Frame::Render()
{
	if (!isShow)
		return;


	frameTex.render(rect.x, rect.y);
}

void Frame::setPos(int x, int y)
{
	rect.x = x;
	rect.y = y;
}

SDL_Rect Frame::getBox()
{
	return rect;
}

void Frame::Render(int camX, int camY)
{
	if (!isShow)
		return;

	frameTex.render(rect.x, rect.y);

}


void Frame::setAlpha(Uint8 a)
{
	frameTex.setAlpha(a);
}

void Frame::ChangeShowState()
{
	isShow = isShow ? false : true;
}

Frame::~Frame()
{
	SDL_FreeSurface(imag);
	SDL_DestroyTexture(tex);
}


Text::Text(const string s,int size,Uint32 width)
{
	char* st1 = const_cast<char*>(s.c_str());
	TTF_Font* font = TTF_OpenFont("ttf/English_1.ttf", size);
	SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(font, st1,color, width);
	//SDL_Surface* textSurface = TTF_RenderText_Solid(font, s, color);
	isShow = false;
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
			rect.w = mWidth;
			rect.h = mHeight;
		}
	}
}

SDL_Rect Text::getBox()
{
	return rect;
}



Text::Text(const string s,int size)
{
	char* st1 = const_cast<char*>(s.c_str());
	TTF_Font* font = TTF_OpenFont("ttf/English_1.ttf", size);
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, st1, color);
	isShow = false;
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
			rect.w = mWidth;
			rect.h = mHeight;
		}
	}
}

void Text::ChangeShowState()
{
	isShow = isShow ? false : true;
}

bool Text::getIsShow()
{
	return isShow;
}


void Text::Render(int x, int y)
{
	if (!isShow)
		return;
	SDL_Rect tmp;
	tmp.x = x;
	tmp.y = y;
	tmp.h = mHeight;
	tmp.w = mWidth;
	SDL_RenderCopy(gRenderer, mTexture, NULL, &tmp);
}

void Text::Render()
{
	if (!isShow)
		return;

	SDL_RenderCopy(gRenderer, mTexture, NULL, &rect);
}

void Text::setPos(int x, int y)
{
	rect.x = x;
	rect.y = y;
}

Text::~Text()
{

}

timer::timer()
{
	second = minute = hour = 0;
	isshow = 0;
}

void timer::Setisshow()
{
	isshow = 1 - isshow;
}

void timer::Render(int x, int y, int size)
{
	if (!isshow) {
		return;
	}
	second = second + 0.0182;
	if (second >= 60) {
		second = second - 60;
		minute++;
	}
	if (minute >= 60) {
		minute = minute - 60;
		hour++;
	}
	int ss = second;
	int mm = minute;
	int hh = hour;
	char s[5];
	char m[5];
	char h[5];
	sprintf_s(s, "%02d", ss);
	sprintf_s(m, "%02d", mm);
	sprintf_s(h, "%02d", hh);
	Text s1(s,18);
	Text m1(m,18);
	Text h1(h,18);
	s1.Render(x + 50, y);
	m1.Render(x + 25, y);
	h1.Render(x, y);
}