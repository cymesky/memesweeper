#include "MemeField.h"
#include <random>
#include <assert.h>
#include "SpriteCodex.h"
#include <cmath>

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

	for (Vei2 gridPos = { 0, 0 }; gridPos.y < height; gridPos.y++)
	{
		for (gridPos.x = 0; gridPos.x < width; gridPos.x++)
		{
			if (!TileAt(gridPos).HasMeme())
			{
				int count = 0;
				int xStart = std::max(0, gridPos.x - 1);
				int yStart = std::max(0, gridPos.y - 1);
				int xEnd = std::min(width - 1, gridPos.x + 1);
				int yEnd = std::min(height - 1, gridPos.y + 1);

 				for (int y = yStart; y <= yEnd; y++)
				{
					for (int x = xStart; x <= xEnd; x++)
					{
						if (TileAt({ x, y }).HasMeme())
						{
							count++;
						}
					}
				}

				TileAt(gridPos).SetNeighbors(count);
			}
		}
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

void MemeField::AtTileClick(const Vei2& screenPos)
{
	if (IsPosInRect(screenPos))
	{
		Vei2 gridPos = ScreenToGridPosition(screenPos);

		Tile& tile = TileAt(gridPos);

		if (!tile.IsRevealed() && tile.IsHidden())
		{
			tile.Reveal();
		}
	}
}

void MemeField::AtFlagClick(const Vei2& screenPos)
{
	if (IsPosInRect(screenPos))
	{
		Vei2 gridPos = ScreenToGridPosition(screenPos);

		Tile& tile = TileAt(gridPos);

		tile.ToggleFlag();
	}
}

void MemeField::SetNeighbors(int n, const Vei2& gridPos)
{
	TileAt(gridPos).SetNeighbors(n);
}

Vei2 MemeField::ScreenToGridPosition(const Vei2& screenPos) const
{
	return screenPos/SpriteCodex::tileSize;
}

bool MemeField::IsPosInRect(const Vei2& screenPos) const
{
	RectI rect = RectI(topLeftBoard, width * SpriteCodex::tileSize, height * SpriteCodex::tileSize);

	return rect.IsInRect(screenPos);
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
	RectI rect = { screenPos, SpriteCodex::tileSize, SpriteCodex::tileSize };

	gfx.DrawRect(rect, SpriteCodex::baseColor);
	
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
			SpriteCodex::DrawTileNumber(neighborsMemes, screenPos, gfx);
		}
		break;
	case State::Flagged:
		SpriteCodex::DrawTileButton(screenPos, gfx);
		SpriteCodex::DrawTileFlag(screenPos, gfx);
		
		break;
	}
}

void MemeField::Tile::Reveal()
{
	assert(state == State::Hidden);
	state = State::Revealed;
}

bool MemeField::Tile::IsRevealed() const
{
	return state == State::Revealed;
}

bool MemeField::Tile::IsHidden() const
{
	return state == State::Hidden;
}

void MemeField::Tile::ToggleFlag()
{
	if (state != State::Revealed)
	{
		if (state == State::Hidden)
		{
			state = State::Flagged;
		}
		else if (state == State::Flagged)
		{
			state = State::Hidden;
		}
	}
}

void MemeField::Tile::SetNeighbors(int n)
{
	neighborsMemes = n;
}
