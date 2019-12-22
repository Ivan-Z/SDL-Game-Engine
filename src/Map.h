#ifndef MAP_H
#define MAP_H

#include <string>

class Map {
	public:	
		Map(std::string textureId, int scale, int tileSize);
		~Map();
		void LoadMap(std::string filePath, int mapSizeX, int mapSizeY);
		void AddTile(int sourceX, int sourceY, int x, int y);
		int GetWidth();
		int GetHeight();
	private:
		std::string textureId;
		int width;
		int height;
		int scale;
		int tileSize;
};

#endif
