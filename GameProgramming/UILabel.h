#pragma once

#include "ECS.h"
#include "AssetManager.h"
#include "Texturemanager.h"
#include "Game.hpp"
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class UILabel : public Component
{
public:
	std::string tag;
	UILabel(std::string t, int xpos, int ypos, std::string text, std::string font, SDL_Color& color) : labelText(text), labelFont(font), textColor(color)
	{
		tag = t;
		position.x = xpos;
		position.y = ypos;

		setLabelText(labelText, labelFont);
	}
	~UILabel()
	{}

	void setLabelText(std::string text, std::string font)
	{
		SDL_Surface* surf = TTF_RenderText_Blended(Game::assets->getFont(font), text.c_str(), textColor);
		if (labelTexture) {
			SDL_DestroyTexture(labelTexture);
		}
		labelTexture = SDL_CreateTextureFromSurface(Game::renderer, surf);
		SDL_FreeSurface(surf);

		SDL_QueryTexture(labelTexture, nullptr, nullptr, &position.w, &position.h);
	}
	void draw() override
	{
		SDL_RenderCopy(Game::renderer, labelTexture, nullptr, &position);
	}
private:
	SDL_Rect position;
	std::string labelText;
	std::string labelFont;
	SDL_Color textColor;
	SDL_Texture* labelTexture;
};