#include "Game.h"
#include "World.h"
#include "Define.h"
#include "Sound.h"
#include "Story.h"
#include "Control.h"
#include "Character.h"
#include "CharacterAction.h"
#include "CharacterController.h"
#include "Brain.h"
#include "ControllerRecorder.h"
#include "DxLib.h"

/*
* キャラのデータ
*/
CharacterData::CharacterData(const char* name) {
	m_name = name;
	m_hp = -1;
}


/*
* ゲームのセーブデータ
*/
// 初期状態のデータを作成
GameData::GameData() {
	m_saveFilePath = "data/save/savedata1.dat";

	m_areaNum = 1;
	m_storyNum = 1;
	m_soundVolume = 50;

	// 主要キャラを設定
	m_characterData.push_back("クーリール");

}

// ファイルを指定してデータを復元
GameData::GameData(const char* saveFilePath):
	GameData()
{
	m_saveFilePath = saveFilePath;
	// セーブデータを読み込んで初期状態のデータを上書き

}

void GameData::asignWorld(World* world) {
	size_t size = m_characterData.size();
	for (unsigned int i = 0; i < size; i++) {
		world->asignedCharacterData(m_characterData[i].name(), &m_characterData[i]);
	}
}

void GameData::asignedWorld(World* world) {
	size_t size = m_characterData.size();
	for (unsigned int i = 0; i < size; i++) {
		world->asignCharacterData(m_characterData[i].name(), &m_characterData[i]);
	}
}


/*
* ゲーム本体
*/
Game::Game() {
	// データ
	m_gameData = new GameData();

	// サウンドプレイヤー
	m_soundPlayer = new SoundPlayer();
	m_soundPlayer->setVolume(m_gameData->getSoundVolume());

	// 世界
	m_world = new World(-1, m_gameData->getAreaNum(), m_soundPlayer);

	// データを世界に反映
	m_gameData->asignWorld(m_world);

	// ストーリー
	m_story = new Story(m_gameData->getStoryNum(), m_world, m_soundPlayer);
}

Game::~Game() {
	delete m_gameData;
	delete m_soundPlayer;
	delete m_world;
}

bool Game::play() {

	// これ以上ストーリーを進ませない（テスト用）
	if (m_gameData->getStoryNum() == 1) {
		m_world->battle();
		m_soundPlayer->play();
		return false;
	}
	
	// ストーリー進行
	if (m_story->play()) {
		// 次のストーリーへ
		m_gameData->setStoryNum(m_gameData->getStoryNum() + 1);
		delete m_story;
		m_story = new Story(m_gameData->getStoryNum(), m_world, m_soundPlayer);
		m_world->addObject(m_story->getObjectLoader());
	}

	// 音
	m_soundPlayer->play();

	// エリア移動
	//if (m_world->getBrightValue() == 0) {
	//	int nextAreaNum = m_world->getAreaNum();
	//	m_gameData->asignedWorld(m_world);
	//	delete m_world;
	//	InitGraph();
	//	m_world = new World(m_gameData->getAreaNum(), nextAreaNum, m_soundPlayer);
	//	m_gameData->asignWorld(m_world);
	//	m_gameData->setAreaNum(nextAreaNum);
	//	m_world->addObject(m_story->getObjectLoader());
	//	m_story->setWorld(m_world);
	//	return true;
	//}
	return false;
}