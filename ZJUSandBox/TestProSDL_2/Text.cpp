#include"Entity.h";
#include"Utils.h"
#include"Text.h"



SDL_Color color = { 0, 0, 0 };

Frame::Frame(const char* file) 
{
	imag = SDL_LoadBMP(file);
	if (imag == NULL)
	{
		cout << "Frame:load imag error\n";
		return;
	}
	rect.x = 0;
	rect.y = 0;
	rect.h = imag->h;
	rect.w = imag->w;
	isShow = true;
	tex = SDL_CreateTextureFromSurface(gRenderer, imag);
	//SDL_FreeSurface(imag);
	return;
}

void Frame::Render(int camX, int camY, int size) 
{
	if (!isShow)
		return;

	rect.x = camX;
	rect.y = camY;
	rect.h = imag->h / size;
	rect.w = imag->w / size;
	SDL_RenderCopy(gRenderer, tex, NULL, &rect);

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


Text::Text(const char* s)
{
	TTF_Font* font = TTF_OpenFont("img/lazy.ttf", 18);
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, s, color);
	isShow = true;
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
		}
	}
}

void Text::ChangeShowState()
{
	isShow = isShow ? false : true;
}

void Text::Render(int x, int y, int size) 
{
	if (!isShow)
		return;
	rect.x = x;
	rect.y = y;
	rect.h = mHeight;
	rect.w = mWidth;
	SDL_RenderCopy(gRenderer, mTexture, NULL, &rect);
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
	Text s1(s);
	Text m1(m);
	Text h1(h);
	s1.Render(x + 50, y, size);
	m1.Render(x + 25, y, size);
	h1.Render(x, y, size);
}