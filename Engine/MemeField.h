#pragma once
#include "Vei2.h"
#include "Graphics.h"

class MemeField
{
private:
	class Tile
	{
	public:
		enum class State
		{
			Hidden,
			Revealed,
			Flagged
		};
		bool HasMeme() const;
		void SpawnMeme();
		void Draw(const Vei2& screenPos, Graphics& gfx) const;
		void Reveal();
		bool IsRevealed() const;
	private:
		State state = State::Hidden;
		bool hasMeme = false;
	};
public:
	MemeField(int nMines); 
	void Draw(Graphics& gfx) const;
	void AtTileClick(const Vei2& screenPos);
private:
	static constexpr int width = 20;
	static constexpr int height = 20;
	Tile& TileAt(const Vei2& gridPos);
	const Tile& TileAt(const Vei2& gridPos) const;
	Tile field[width * height];
	Vei2 ScreenToGridPosition(const Vei2& screenPos) const;
};