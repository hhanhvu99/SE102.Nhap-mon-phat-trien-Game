#include "HUD_Object.h"

HUD_Object::HUD_Object(LPSPRITE sprite)
{
	this->sprite = sprite;
	this->type = eType::HUD_OBJECT_IMAGE;

}

HUD_Object::HUD_Object(string text, int limit)
{
	this->limit = limit;
	this->text = text;
	this->type = eType::HUD_OBJECT_TEXT;
	GetToken(text);
}

void HUD_Object::SetText(string text)
{
	this->text = text;
	GetToken(text);
}

void HUD_Object::GetToken(string text)
{
	//Get tokens
	tokens.clear();

	size_t pos = 0;
	string token;
	setup = true;

	if ((pos = text.find(delimiter)) == std::string::npos)
		tokens.push_back(text);
	else
	{
		while ((pos = text.find(delimiter)) != std::string::npos) {
			token = text.substr(0, pos);
			tokens.push_back(token);
			text.erase(0, pos + delimiter.length());
		}
	}
	

	//Setup string
	string temp;
	int tempLength = limit;

	for (string token : tokens)
	{
		if (token.length() > tempLength)
		{
			token.insert(0, "/");
			tempLength = limit;
		}	
		else
			tempLength -= token.length() + 1;
	}
}

void HUD_Object::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void HUD_Object::Render()
{
	if (type == eType::HUD_OBJECT_IMAGE)
	{
		sprite->Draw(x, y);
	}
	else
	{
		int i = 0, j = 0;

		for (string token : tokens)
		{
			for (auto character : token)
			{
				if (character != '/')
				{
					SpriteManager::GetInstance()->Get(HUD_ID + int(character))->Draw(x + i, y + j);
					i += FONT_WIDTH;
				}
				else
				{
					i = 0;
					j += FONT_HEIGHT;		
				}
			}

			i += FONT_WIDTH;
		}	
		
	}
	
}
