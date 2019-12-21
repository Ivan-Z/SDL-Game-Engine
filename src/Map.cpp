#include <fstream>
#include "Map.h"
#include "./Game.h"
#include "./EntityManager.h"
#include "./components/TileComponent.h"

extern EntityManager manager;
Map::Map(std::string textureId, int scale, int tileSize): textureId(textureId), scale(scale), tileSize(tileSize) {}

void Map::LoadMap(std::string filePath, int mapSizeX, int mapSizeY) {
	std::fstream mapFile;
	mapFile.open(filePath);

	for (int row = 0; row < mapSizeY; row++) {
		for (int column = 0; column < mapSizeX; column++) {
			char ch;
			mapFile.get(ch);
			int sourceRectY = atoi(&ch) * tileSize;
			mapFile.get(ch);
			int sourceRectX = atoi(&ch) * tileSize;
			AddTile(sourceRectX, sourceRectY,column * (scale * tileSize), row * (scale * tileSize));
			mapFile.ignore();
		}
	}

	mapFile.close();
}

void Map::AddTile(int sourceRectX, int sourceRectY, int x, int y) {
	Entity& newTile(manager.AddEntity("Tile", TILEMAP));
	newTile.AddComponent<TileComponent>(sourceRectX, sourceRectY, x, y, tileSize, scale, textureId);
}
