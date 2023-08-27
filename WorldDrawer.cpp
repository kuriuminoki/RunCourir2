#include "WorldDrawer.h"
#include "World.h"
#include "Camera.h"
#include "CharacterDrawer.h"
#include "CharacterAction.h"
#include "ObjectDrawer.h"
#include "AnimationDrawer.h"
#include "TextDrawer.h"
#include "Text.h"
#include "Define.h"
#include "DxLib.h"
#include <queue>

using namespace std;


WorldDrawer::WorldDrawer(const World* world) {
	m_world = world;
	m_characterDrawer = new CharacterDrawer(NULL);
	m_objectDrawer = new ObjectDrawer(NULL);
	m_animationDrawer = new AnimationDrawer(NULL);
	m_conversationDrawer = new ConversationDrawer(NULL);
}

WorldDrawer::~WorldDrawer() {
	delete m_characterDrawer;
	delete m_objectDrawer;
	delete m_animationDrawer;
	delete m_conversationDrawer;
}

// �`�悷��
void WorldDrawer::draw() {
	int bright = m_world->getBrightValue();
	SetDrawBright(bright, bright, bright);
	// �w�i
	int groundGraph = m_world->getBackGroundGraph();
	if (groundGraph != -1) {

	}
	else {
		DrawBox(0, 0, GAME_WIDE, GAME_HEIGHT, m_world->getBackGroundColor(), TRUE);
	}

	// �J�������擾
	const Camera* camera = m_world->getCamera();

	// �eObject��`��
	vector<const Object*> objects = m_world->getBackObjects();
	size_t size = objects.size();
	for (unsigned int i = 0; i < size; i++) {
		// Object��Drawer�ɃZ�b�g
		m_objectDrawer->setObject(objects[i]);

		// �J�������g����Object��`��
		m_objectDrawer->drawObject(camera);
	}

	// �eAction��`��
	vector<const CharacterAction*> actions = m_world->getActions();
	size = actions.size();
	for (unsigned int i = 0; i < size; i++) {
		// �L������Drawer�ɃZ�b�g
		m_characterDrawer->setCharacterAction(actions[i]);

		// �J�������g���ăL������`��
		m_characterDrawer->drawCharacter(camera, bright);
	}

	// �eObject��`��
	objects = m_world->getFrontObjects();
	size = objects.size();
	for (unsigned int i = 0; i < size; i++) {
		// Object��Drawer�ɃZ�b�g
		m_objectDrawer->setObject(objects[i]);

		// �J�������g����Object��`��
		m_objectDrawer->drawObject(camera);
	}

	// �eAnimation��`��
	vector<const Animation*> animations = m_world->getConstAnimations();
	size = animations.size();
	for (unsigned int i = 0; i < size; i++) {
		// Animation��Drawer�ɃZ�b�g
		m_animationDrawer->setAnimation(animations[i]);

		// �J�������g���ăL������`��
		m_animationDrawer->drawAnimation(camera);
	}

	SetDrawBright(255, 255, 255);

	// �e�L�X�g�C�x���g
	const Conversation* conversation = m_world->getConversation();
	if (conversation != NULL) {
		m_conversationDrawer->setConversation(conversation);
		m_conversationDrawer->draw();
	}
}