#include "Tile.h"
#include "Renderer.h"

Tile::Tile(SDL_Rect s, SDL_Rect d, bool o, bool h) :SpriteTileObject(s, d), m_obstacle(o), m_hazard(h) {}

Tile* Tile::Clone()
{
	return new Tile(m_src, m_dst, m_obstacle, m_hazard);
}

void Tile::update() {}
void Tile::draw() {}
void Tile::clean() {}

bool Tile::isObstacle()
{
	return m_obstacle;
}

void Tile::setXY(float x, float y)
{
	m_dst.x = x;
	m_dst.y = y;
}

TiledLevel::TiledLevel(const unsigned short r, const unsigned short c, const int w, const int h,
	const char* tileData, const char* levelData, const std::string tileKey) : m_rows(r), m_cols(c), m_tileKey(tileKey)
{
	std::ifstream inFile(tileData); // create the 16 prototype tiles
	if (inFile.is_open())
	{
		std::string key;
		int x, y;
		bool obs, haz;
		while (!inFile.eof())
		{
			inFile >> key >> x >> y >> obs >> haz;
			m_tiles.emplace(key, new Tile({ x * w, y * h, w, h }, { 0, 0, w, h }, obs, haz));
		}
	}
	inFile.close();

	inFile.open(levelData); //create the whole tiled level now from the text file
	if (inFile.is_open())
	{
		std::string key;
		m_level.resize(m_rows); //sets size of the first dimension
		for (unsigned short row = 0; row < m_rows; row++)
		{
			m_level[row].resize(m_cols);
			for (unsigned short col = 0; col < m_cols; col++)
			{
				inFile >> key;
				m_level[row][col] = m_tiles[key]->Clone(); //common prototype method
				m_level[row][col]->setXY((float)col * w, (float)row * h);
				if (m_level[row][col]->isObstacle())
				{
					m_obstacles.push_back(m_level[row][col]);
				}
			}
		}
	}
	inFile.close();
}

TiledLevel::~TiledLevel()
{
	//clear clones
	for (unsigned short row = 0; row < m_rows; row++)
	{

		for (unsigned short col = 0; col < m_cols; col++)
		{
			delete m_level[row][col];
			m_level[row][col] = nullptr;
		}

	}
	m_level.clear();
	m_obstacles.clear();

	//clear prototypes
	for (std::map<std::string, Tile*>::iterator i = m_tiles.begin(); i != m_tiles.end(); i++)
	{
		delete i->second;
		i->second = nullptr;
	}
	m_tiles.clear();
}

void TiledLevel::update()
{
}

void TiledLevel::draw()
{
	for (unsigned short row = 0; row < m_rows; row++)
	{

		for (unsigned short col = 0; col < m_cols; col++)
		{
			m_level[row][col]->getDst()->x = col * m_level[row][col]->getSrc()->h;
			m_level[row][col]->getDst()->y = row * m_level[row][col]->getSrc()->w;
			SDL_RenderCopy(Renderer::Instance().getRenderer(), TextureManager::Instance().getTexture(m_tileKey), m_level[row][col]->getSrc(), m_level[row][col]->getDst());
		}
	}
}

void TiledLevel::clean()
{
}