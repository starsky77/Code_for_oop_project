#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include<iostream>

#include"Entity.h";

const int Window_WIDTH = 640;
const int Window_HEIGHT = 480;
SDL_Window* window = NULL;
SDL_Renderer* render = NULL;
int Entity::GetPosX() {
	return x;
}
int Entity::GetPosY() {
	return y;
}
StaticObj::StaticObj(const char* file) {
	imag = SDL_LoadBMP(file);
	if (imag == NULL)
	{
		cout << "StaticObj:load imag error\n";
		return;
	}
	rect.x = 0;
	rect.y = 0;
	rect.h = imag->h;
	rect.w = imag->w;
	tex = SDL_CreateTextureFromSurface(render, imag);
	//SDL_FreeSurface(imag);
	return;
}
void StaticObj::Render(int camX, int camY, int size) {
	rect.x = camX;
	rect.y = camY;
	rect.h = rect.h / size;
	rect.w = rect.w / size;
	bool quit = false;
	SDL_Event event;
	while (!quit)
	{
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
				cout << "quit\n";
			}
		}
		SDL_RenderCopy(render, tex, NULL, &rect);
		SDL_RenderPresent(render);
		SDL_Delay(1000);
	}
}
StaticObj::~StaticObj() {
	SDL_FreeSurface(imag);
	SDL_DestroyTexture(tex);
}






int main(int argc, char* args[]) {
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("test1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Window_WIDTH, Window_HEIGHT, SDL_WINDOW_SHOWN);
	render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	StaticObj t1("img/hello.bmp");
	t1.Render(100, 100, 1);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(render);
	SDL_Quit();
	system("pause");
	return 0;

}