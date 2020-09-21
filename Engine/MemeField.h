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
		bool IsHidden() const;
		void ToggleFlag();
		void SetNeighbors(int n);
	private:
		State state = State::Hidden;
		bool hasMeme = false;
		int neighborsMemes = 0;
	};
public:
	MemeField(int nMines); 
	void Draw(Graphics& gfx) const;
	void AtTileClick(const Vei2& screenPos);
	void AtFlagClick(const Vei2& screenPos);
	void SetNeighbors(int n, const Vei2& gridPos);
private:
	static constexpr int width = 20;
	static constexpr int height = 20;
	Tile& TileAt(const Vei2& gridPos);
	const Tile& TileAt(const Vei2& gridPos) const;
	Tile field[width * height];
	Vei2 ScreenToGridPosition(const Vei2& screenPos) const;
	bool IsPosInRect(const Vei2& screenPos) const;
	Vei2 topLeftBoard = {0, 0};
};