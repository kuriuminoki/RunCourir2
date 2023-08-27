#ifndef GAME_DRAWER_H_INCLUDED
#define GAME_DRAWER_H_INCLUDED

class Game;
class WorldDrawer;

class GameDrawer {
private:
	const Game* m_game;

	WorldDrawer* m_worldDrawer;

	// �X�L���̏��̃t�H���g
	const int SKILL_SIZE = 100;
	int m_skillHandle;

public:
	GameDrawer(const Game* game);

	~GameDrawer();

	void draw();
};


#endif