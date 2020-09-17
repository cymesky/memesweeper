#include "MemeField.h"
#include <random>
#include <assert.h>
#include "SpriteCodex.h"

MemeField::MemeField(int nMines)
{
	std::random_device rd;
	std::mt19937 rnd(rd());

	for (int i = 0; i < nMines; i++)
	{
		std::uniform_int_distribution <int> xDist(0, width);
		std::uniform_int_distribution <int> yDist(0, height);

		Vei2 gridPos;

		do
		{
			gridPos = { xDist(rnd), yDist(rnd) };
		} while (TileAt(gridPos).HasMeme());
		
		TileAt(gridPos).SpawnMeme();
	}
}

void MemeField::Draw(Graphics& gfx) const
{
	for (Vei2 gridPos = { 0, 0 }; gridPos.y < height; gridPos.y++)
	{
		for (gridPos.x = 0; gridPos.x < width; gridPos.x++)
		{
			Vei2 screenPos = gridPos * SpriteCodex::tileSize;
			TileAt(gridPos).Draw(screenPos, gfx);
		}
	}
}

MemeField::Tile& MemeField::TileAt(const Vei2& gridPos)
{
	return field[gridPos.y * width + gridPos.x];
}

const MemeField::Tile& MemeField::TileAt(const Vei2& gridPos) const
{
	return field[gridPos.y * width + gridPos.x];
}



bool MemeField::Tile::HasMeme() const
{
	return hasMeme;
}

void MemeField::Tile::SpawnMeme()
{
	assert(!hasMeme);
	hasMeme = true;
}

void MemeField::Tile::Draw(const Vei2& screenPos, Graphics& gfx) const
{
	switch (state)
	{
	case State::Hidden:
		SpriteCodex::DrawTileButton(screenPos, gfx);
		break;
	case State::Revealed:
		if (hasMeme)
		{
			SpriteCodex::DrawTileBomb(screenPos, gfx);
		}
		else
		{
			SpriteCodex::DrawTile0(screenPos, gfx);
		}
		break;
	case State::Flagged:
		SpriteCodex::DrawTileFlag(screenPos, gfx);
		break;
	}
}
