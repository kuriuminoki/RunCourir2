#include "GameDrawer.h"
#include "Game.h"
#include "World.h"
#include "WorldDrawer.h"
#include "Define.h"
#include "DxLib.h"
#include <string>
#include <sstream>


using namespace std;


GameDrawer::GameDrawer(const Game* game) {
	m_game = game;

	m_worldDrawer = new WorldDrawer(NULL);

	m_skillHandle = CreateFontToHandle(NULL, SKILL_SIZE, 10);
}

GameDrawer::~GameDrawer() {
	delete m_worldDrawer;
	DeleteFontToHandle(m_skillHandle);
}

void GameDrawer::draw() {
	// ¢ŠE‚ð•`‰æ
	m_worldDrawer->setWorld(m_game->getWorld());
	m_worldDrawer->draw();
}