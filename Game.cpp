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
* �L�����̃f�[�^
*/
CharacterData::CharacterData(const char* name) {
	m_name = name;
	m_hp = -1;
}


/*
* �Q�[���̃Z�[�u�f�[�^
*/
// ������Ԃ̃f�[�^���쐬
GameData::GameData() {
	m_saveFilePath = "data/save/savedata1.dat";

	m_areaNum = 1;
	m_storyNum = 1;
	m_soundVolume = 50;

	// ��v�L������ݒ�
	m_characterData.push_back("�N�[���[��");

}

// �t�@�C�����w�肵�ăf�[�^�𕜌�
GameData::GameData(const char* saveFilePath):
	GameData()
{
	m_saveFilePath = saveFilePath;
	// �Z�[�u�f�[�^��ǂݍ���ŏ�����Ԃ̃f�[�^���㏑��

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
* �Q�[���{��
*/
Game::Game() {
	// �f�[�^
	m_gameData = new GameData();

	// �T�E���h�v���C���[
	m_soundPlayer = new SoundPlayer();
	m_soundPlayer->setVolume(m_gameData->getSoundVolume());

	// ���E
	m_world = new World(-1, m_gameData->getAreaNum(), m_soundPlayer);

	// �f�[�^�𐢊E�ɔ��f
	m_gameData->asignWorld(m_world);

	// �X�g�[���[
	m_story = new Story(m_gameData->getStoryNum(), m_world, m_soundPlayer);
}

Game::~Game() {
	delete m_gameData;
	delete m_soundPlayer;
	delete m_world;
}

bool Game::play() {

	// ����ȏ�X�g�[���[��i�܂��Ȃ��i�e�X�g�p�j
	if (m_gameData->getStoryNum() == 1) {
		m_world->battle();
		m_soundPlayer->play();
		return false;
	}
	
	// �X�g�[���[�i�s
	if (m_story->play()) {
		// ���̃X�g�[���[��
		m_gameData->setStoryNum(m_gameData->getStoryNum() + 1);
		delete m_story;
		m_story = new Story(m_gameData->getStoryNum(), m_world, m_soundPlayer);
		m_world->addObject(m_story->getObjectLoader());
	}

	// ��
	m_soundPlayer->play();

	// �G���A�ړ�
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