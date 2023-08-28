#include "World.h"
#include "Object.h"
#include "Character.h"
#include "CharacterAction.h"
#include "CharacterController.h"
#include "Camera.h"
#include "Animation.h"
#include "Sound.h"
#include "CsvReader.h"
#include "Control.h"
#include "Define.h"
#include "Text.h"
#include "Brain.h"
#include "ControllerRecorder.h"
#include "ObjectLoader.h"
#include "Game.h"
#include "DxLib.h"
#include <algorithm>


using namespace std;


// vector�ɓ������S�I�u�W�F�N�g���폜����
void deleteAllObject(vector<Object*>& objects) {
	for (int i = (int)objects.size() - 1; i >= 0; i--) {
		delete objects[i];
		objects.pop_back();
	}
}

// vector�ɓ�����deleteFlag��tarue�̃I�u�W�F�N�g���폜����
void deleteObject(vector<Object*>& objects) {
	for (unsigned int i = 0; i < objects.size(); i++) {
		// deleteFlag��true�Ȃ�폜����
		if (objects[i]->getDeleteFlag()) {
			delete objects[i];
			// �������폜�����������
			objects[i] = objects.back();
			objects.pop_back();
			i--;
		}
	}
}

// vector�ɓ������S�I�u�W�F�N�g�𓮂���
void actionObject(vector<Object*>& objects) {
	// �ǂ⏰�I�u�W�F�N�g�̏��� (�����蔻��Ɠ���)
	for (unsigned int i = 0; i < objects.size(); i++) {
		// �I�u�W�F�N�g�̓���
		objects[i]->action();
		// deleteFlag��true�Ȃ�폜����
		if (objects[i]->getDeleteFlag()) {
			delete objects[i];
			// �������폜�����������
			objects[i] = objects.back();
			objects.pop_back();
			i--;
		}
	}
}

// �L�����N�^�[�ƃI�u�W�F�N�g�̊ђʎ�����
void penetrationCharacterAndObject(CharacterController* controller, vector<Object*> objects) {
	// �ǂ⏰�I�u�W�F�N�g�̏��� (�����蔻��Ɠ���)
	for (unsigned int i = 0; i < objects.size(); i++) {
		// �����蔻��������ōs��
		objects[i]->penetration(controller);
	}
}


/*
* �I�u�W�F�N�g�̃��[�h�Ȃ�
*/
World::World(int fromAreaNum, int toAreaNum, SoundPlayer* soundPlayer) {
	m_duplicationFlag = false;

	m_brightValue = 255;

	// �T�E���h�v���C���[
	m_soundPlayer_p = soundPlayer;

	// ��b�C�x���g
	m_conversation_p = NULL;

	// �X�L��������
	m_skillFlag = false;

	// ��l���̃X�^�[�g�n�_
	m_areaNum = toAreaNum;

	// �G���A�����[�h
	const AreaReader data(fromAreaNum, toAreaNum, m_soundPlayer_p);
	m_camera = data.getCamera();
	m_focusId = data.getFocusId();
	m_playerId = data.getPlayerId();
	m_soundPlayer_p->setBGM(data.getBgm(), data.getBgmVolume());
	m_characters = data.getCharacters();
	m_characterControllers = data.getCharacterControllers();
	m_stageObjects = data.getObjects();
	m_doorObjects = data.getDoorObjects();
	data.getBackGround(m_backGroundGraph, m_backGroundColor);
}

World::~World() {
	// �J�������폜����
	delete m_camera;

	// �S�I�u�W�F�N�g���폜����B
	deleteAllObject(m_stageObjects);
	deleteAllObject(m_attackObjects);
	deleteAllObject(m_doorObjects);

	// �U���G�t�F�N�g�폜
	for (unsigned i = 0; i < m_animations.size(); i++) {
		delete m_animations[i];
	}

	// �S�R���g���[�����폜����B
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		delete m_characterControllers[i];
	}

	// �S�L�����N�^�[���폜����B
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		delete m_characters[i];
	}

	// �w�i
	if (!m_duplicationFlag) {
		DeleteGraph(m_backGroundGraph);
	}
}

World::World(const World* original) {
	m_duplicationFlag = true;
	m_brightValue = 255;
	m_conversation_p = NULL;
	m_skillFlag = false;
	m_areaNum = original->getAreaNum();

	// �G���A���R�s�[
	m_camera = new Camera(original->getCamera());
	m_focusId = original->getFocusId();
	m_playerId = original->getPlayerId();
	m_soundPlayer_p = original->getSoundPlayer();
	// �L�������R�s�[
	for (unsigned int i = 0; i < original->getCharacters().size(); i++) {
		Character* copy;
		copy = original->getCharacters()[i]->createCopy();
		m_characters.push_back(copy);
	}
	// �R���g���[�����R�s�[
	for (unsigned int i = 0; i < original->getCharacterControllers().size(); i++) {
		CharacterController* copy;
		// Brain��Action�R�s�[�p��Character�ƃJ������n��
		copy = original->getCharacterControllers()[i]->createCopy(m_characters, m_camera);
		m_characterControllers.push_back(copy);
	}
	for (unsigned int i = 0; i < original->getStageObjects().size(); i++) {
		Object* copy;
		copy = original->getStageObjects()[i]->createCopy();
		m_stageObjects.push_back(copy);
	}
	for (unsigned int i = 0; i < original->getDoorObjects().size(); i++) {
		Object* copy;
		copy = original->getDoorObjects()[i]->createCopy();
		m_doorObjects.push_back(copy);
	}
	for (unsigned int i = 0; i < original->getAttackObjects().size(); i++) {
		Object* copy;
		copy = original->getAttackObjects()[i]->createCopy();
		m_attackObjects.push_back(copy);
	}
	for (unsigned int i = 0; i < original->getAnimations().size(); i++) {
		Animation* copy;
		copy = original->getAnimations()[i]->createCopy();
		m_animations.push_back(copy);
	}
	m_backGroundGraph = original->getBackGroundGraph();
	m_backGroundColor = original->getBackGroundColor();
}

// �X�g�[���[��C�x���g�ɂ��ǉ��I�u�W�F�N�g
void World::addObject(ObjectLoader* objectLoader) {
	pair<vector<Object*>, vector<Object*> > p = objectLoader->getObjects(m_areaNum);
	// �ǂ⏰
	m_stageObjects.insert(m_stageObjects.end(), p.first.begin(), p.first.end());
	// �h�A
	m_doorObjects.insert(m_doorObjects.end(), p.second.begin(), p.second.end());
}

std::vector<CharacterController*> World::getCharacterControllers() const {
	return m_characterControllers;
}
std::vector<Character*> World::getCharacters() const {
	return m_characters;
}
std::vector<Object*> World::getStageObjects() const {
	return m_stageObjects;
}
std::vector<Object*> World::getDoorObjects() const {
	return m_doorObjects;
}
std::vector<Object*> World::getAttackObjects() const {
	return m_attackObjects;
}
std::vector<Animation*> World::getAnimations() const {
	return m_animations;
}

void World::pushCharacter(Character* character, CharacterController* controller) {
	m_characters.push_back(character);
	m_characterControllers.push_back(controller);
}

void World::popCharacter(int id) {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == "�n�[�g") {
			continue;
		}
		if (m_characterControllers[i]->getAction()->getCharacter()->getId() == id) {
			delete m_characterControllers[i];
			m_characterControllers[i] = m_characterControllers.back();
			m_characterControllers.pop_back();
			i--;
		}
	}
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characters[i]->getName() == "�n�[�g") {
			continue;
		}
		if (m_characters[i]->getId() == id) {
			for (unsigned int j = 0; j < m_characterControllers.size(); j++) {
				if (m_characterControllers[j]->getBrain()->getTargetId() == id) {
					m_characterControllers[j]->setTarget(NULL);
				}
			}
			//m_characters[i]->setHp(0);
			delete m_characters[i];
			m_characters[i] = m_characters.back();
			m_characters.pop_back();
			i--;
		}
	}
}


// ���R�[�_���쐬���g�p���J�n
void World::createRecorder() {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == "�n�[�g") { continue; }
		m_characterControllers[i]->setStickRecorder(new ControllerRecorder(0));
		m_characterControllers[i]->setJumpRecorder(new ControllerRecorder(0));
		m_characterControllers[i]->setSquatRecorder(new ControllerRecorder(0));
		m_characterControllers[i]->setSlashRecorder(new ControllerRecorder(0));
		m_characterControllers[i]->setBulletRecorder(new ControllerRecorder(0));
		m_characterControllers[i]->setDamageRecorder(new ControllerRecorder(0));
	}
}

// ���R�[�_�̎��Ԃ��ŏ��ɖ߂�
void World::initRecorder() {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == "�n�[�g") { continue; }
		m_characterControllers[i]->initRecorder();
	}
}

// ���R�[�_�̎g�p����߂č폜����
void World::eraseRecorder() {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == "�n�[�g") { continue; }
		m_characterControllers[i]->eraseRecorder();
	}
}


// �L�����̏�Ԃ�ύX����
void World::asignedCharacterData(const char* name, CharacterData* data) {
	size_t size = m_characters.size();
	for (unsigned i = 0; i < size; i++) {
		if (name == m_characters[i]->getName()) {
			m_characters[i]->setHp(data->hp());
			m_characters[i]->setMaxStamina(data->maxStamina());
			m_characters[i]->setStamina(data->maxStamina());
			m_characters[i]->setMaxSpeed(data->maxSpeed());
		}
	}
}

// �L�����̏�Ԃ�������
void World::asignCharacterData(const char* name, CharacterData* data) {
	size_t size = m_characters.size();
	for (unsigned i = 0; i < size; i++) {
		if (name == m_characters[i]->getName()) {
			data->setHp(m_characters[i]->getHp());
			data->setMaxStamina(m_characters[i]->getMaxStamina());
			data->setMaxSpeed(m_characters[i]->getMaxSpeed());
			return;
		}
	}
}

// CharacterAction��vector��Ԃ�
vector<const CharacterAction*> World::getActions() const {
	vector<const CharacterAction*> actions;
	size_t size = m_characterControllers.size();
	for (unsigned int i = 0; i < size; i++) {
		actions.push_back(m_characterControllers[i]->getAction());
	}
	return actions;
}

// Object��vector��Ԃ�
vector<const Object*> World::getFrontObjects() const {

	vector<const Object*> allObjects;
	allObjects.insert(allObjects.end(), m_stageObjects.begin(), m_stageObjects.end());
	allObjects.insert(allObjects.end(), m_attackObjects.begin(), m_attackObjects.end());

	return allObjects;
}

// �L���������ɕ`�悷��Object��vector��Ԃ�
vector<const Object*> World::getBackObjects() const {

	vector<const Object*> allObjects;
	allObjects.insert(allObjects.end(), m_doorObjects.begin(), m_doorObjects.end());

	return allObjects;
}

// Animation��vector��Ԃ�
vector<const Animation*> World::getConstAnimations() const {

	vector<const Animation*> allAnimations;
	allAnimations.insert(allAnimations.end(), m_animations.begin(), m_animations.end());

	return allAnimations;
}

// ��킹��
void World::battle() {
	// ��ʈÓ]��
	if (m_brightValue != 255) {
		m_brightValue = max(0, m_brightValue - 10);
		return;
	}

	// HP0�̃L�����R���g���[���폜
	cleanCharacterController();

	// deleteFlag��true�̃I�u�W�F�N�g���폜����B
	deleteObject(m_stageObjects);
	deleteObject(m_attackObjects);

	// �L�����̍X�V�i�U���Ώۂ̕ύX�j
	// ��ŃL�������폜��������X�V��������K�v
	updateCharacter();

	// �L�����N�^�[�̓���
	controlCharacter();

	// �I�u�W�F�N�g�̓���
	controlObject();

	// �J�����̍X�V
	updateCamera();

	// �T�E���h�v���C���[�̃p���ݒ�p
	m_soundPlayer_p->setCameraX(m_camera->getX());

	// �A�j���[�V�����̍X�V
	updateAnimation();

}

// �g�o�O�̃L�����R���g���[���폜
void World::cleanCharacterController() {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getHp() == 0) {
			delete m_characterControllers[i];
			m_characterControllers[i] = m_characterControllers.back();
			m_characterControllers.pop_back();
			i--;
		}
	}
}

// �L�����̍X�V�i�U���Ώۂ̕ύX�j
void World::updateCharacter() {
	size_t size = m_characterControllers.size();
	for (unsigned int i = 0; i < size; i++) {
		// Brain�̗v���ōU���ΏەύX
		if (m_characterControllers[i]->getBrain()->needSearchTarget()) { 
			Character* target = m_characters[GetRand((int)m_characters.size() - 1)];
			m_characterControllers[i]->searchTargetCandidate(target);
		}
	}
}

// �J�����̍X�V
void World::updateCamera() {
	size_t size = m_characters.size();
	int x = 0, y = 0;
	// �L�����ƃJ�����̋����̍ő�
	int max_dx = 0, max_dy = 0;
	// ��ʓ��ɓ���悤�Ƃ��鋗���̍ő�@�����藣�ꂽ�L�����͖���
	const int MAX_DISABLE = 2000;
	for (unsigned int i = 0; i < size; i++) {
		// ���t�H�[�J�X���Ă���L�����̍��W�ɍ��킹��
		if (m_focusId == m_characters[i]->getId()) {
			x = m_characters[i]->getCenterX();
			y = m_characters[i]->getCenterY();
			m_camera->setGPoint(x, y);
		}
		else if (m_characters[i]->getHp() > 0) {
			int dx = abs(m_camera->getX() - m_characters[i]->getX()) + m_characters[i]->getWide();
			if (dx < MAX_DISABLE) {
				max_dx = max(max_dx, dx);
				max_dy = max(max_dy, abs(m_camera->getY() - m_characters[i]->getY()) + m_characters[i]->getHeight());
			}
		}
	}
	// �J�����͂�����蓮��
	m_camera->move();

	// �J�����̊g��E�k��
	// �傫���ύX����K�v������ꍇ�قǁA�傫���g�嗦��ύX����B
	double nowEx = m_camera->getEx();
	int leftShift = controlLeftShift();
	if (leftShift > 0) {
		m_camera->setEx(max(nowEx - 0.01, 0.1));
	}
	else {
		int nowWide = (int)(GAME_WIDE / 2 / nowEx);
		int nowHeight = (int)(GAME_HEIGHT / 2 / nowEx);
		max_dx = (int)(max_dx * nowEx);
		max_dy = (int)(max_dy + nowEx);
		if (nowEx > 0.5 && (max_dx > nowWide || max_dy > nowHeight)) {
			double d = double(max(max_dx - nowWide, max_dy - nowHeight));
			m_camera->setEx(nowEx - min(0.05, d / 200000));
		}
		else if (nowEx < 1.5 && (max_dx < nowWide && max_dy < nowHeight)) {
			double d = double(max(nowWide - max_dx, nowHeight - max_dy));
			m_camera->setEx(nowEx + min(0.05, d / 200000));
		}
	}
}

// �A�j���[�V�����̍X�V
void World::updateAnimation() {
	for (unsigned int i = 0; i < m_animations.size(); i++) {
		m_animations[i]->count();
		if (m_animations[i]->getFinishFlag()) {
			delete m_animations[i];
			m_animations[i] = m_animations.back();
			m_animations.pop_back();
			i--;
		}
	}
}

// �L�����N�^�[�ƃI�u�W�F�N�g�̓����蔻��
void World::atariCharacterAndObject(CharacterController* controller, vector<Object*>& objects) {
	// �ǂ⏰�I�u�W�F�N�g�̏��� (�����蔻��Ɠ���)
	for (unsigned int i = 0; i < objects.size(); i++) {
		// �����蔻��������ōs��
		if (objects[i]->atari(controller)) {
			// ���������ꍇ �G�t�F�N�g�쐬
			int x = controller->getAction()->getCharacter()->getCenterX();
			int y = controller->getAction()->getCharacter()->getCenterY();
			m_animations.push_back(objects[i]->createAnimation(x, y, 3));
			int soundHandle = objects[i]->getSoundHandle();
			int panPal = adjustPanSound(x, m_camera->getX());
			m_soundPlayer_p->pushSoundQueue(soundHandle, panPal);
		}
		// deleteFlag��true�Ȃ�폜����
		if (objects[i]->getDeleteFlag()) {
			delete objects[i];
			// �������폜�����������
			objects[i] = objects.back();
			objects.pop_back();
			i--;
		}
	}
}

// �L�����N�^�[�Ɣ��I�u�W�F�N�g�̓����蔻��
void World::atariCharacterAndDoor(CharacterController* controller, vector<Object*>& objects) {

	// �X�L���������͔��͓���Ȃ�
	if (m_skillFlag) { return; }

	// �ǂ⏰�I�u�W�F�N�g�̏��� (�����蔻��Ɠ���)
	for (unsigned int i = 0; i < objects.size(); i++) {
		// �����蔻��������ōs��
		if (objects[i]->atari(controller) && controller->getActionKey()) {
			// ���������ꍇ �G���A�ړ�������
			m_areaNum = objects[i]->getAreaNum();
			// ��ʂ��Ó]
			m_brightValue--;
		}
	}

}

// �L�����N�^�[�̓���
void World::controlCharacter() {
	size_t size = m_characterControllers.size();
	for (unsigned int i = 0; i < size; i++) {
		CharacterController* controller = m_characterControllers[i];
		// �s���O�̏���
		controller->init();

		// �I�u�W�F�N�g�Ƃ̓����蔻��
		atariCharacterAndObject(controller, m_stageObjects);
		atariCharacterAndObject(controller, m_attackObjects);
		if (controller->getAction()->getCharacter()->getId() == m_playerId) {
			atariCharacterAndDoor(controller, m_doorObjects);
		}

		// ����
		if (!m_duplicationFlag || m_characterControllers[i]->getAction()->getCharacter()->getId() != m_playerId) {
			controller->control();
		}

		// �ˌ��U��
		Object* bulletAttack = controller->bulletAttack();
		if (bulletAttack != NULL) { m_attackObjects.push_back(bulletAttack); }

		// �a���U��
		Object* slashAttack = controller->slashAttack();
		if (slashAttack != NULL) { m_attackObjects.push_back(slashAttack); }

		// ���f
		if (!m_duplicationFlag || m_characterControllers[i]->getAction()->getCharacter()->getId() != m_playerId) {
			controller->action();
		}

		// �I�u�W�F�N�g�Ƃ̊ђʔ���
		penetrationCharacterAndObject(controller, m_stageObjects);
	}
}

// �I�u�W�F�N�g�̓���
void World::controlObject() {
	// �ǂ⏰�̓���
	actionObject(m_stageObjects);

	// �U�������蔻��̓���
	actionObject(m_attackObjects);

	// �ǂ⏰<->�U���̓����蔻��
	atariStageAndAttack();

	// �U��<->�U���̓����蔻��
	atariAttackAndAttack();
}

// �ǂ⏰<->�U���̓����蔻��
void World::atariStageAndAttack() {
	for (unsigned int i = 0; i < m_attackObjects.size(); i++) {
		for (unsigned int j = 0; j < m_stageObjects.size(); j++) {
			// �U�����Ǐ��ɓ������Ă��邩����
			if (m_stageObjects[j]->atariObject(m_attackObjects[i])) {
				// ���������ꍇ �G�t�F�N�g�쐬
				int x = m_attackObjects[i]->getCenterX();
				int y = m_attackObjects[i]->getCenterY();
				m_animations.push_back(m_attackObjects[i]->createAnimation(x, y, 3));
				int soundHandle = m_attackObjects[i]->getSoundHandle();
				int panPal = adjustPanSound(x, m_camera->getX());
				m_soundPlayer_p->pushSoundQueue(soundHandle, panPal);
			}
			// �Ǐ���deleteFlag��true�Ȃ�폜����
			if (m_stageObjects[j]->getDeleteFlag()) {
				delete m_stageObjects[j];
				m_stageObjects[j] = m_stageObjects.back();
				m_stageObjects.pop_back();
				j--;
			}
		}
		// �U����deleteFlag��true�Ȃ�폜����
		if (m_attackObjects[i]->getDeleteFlag()) {
			delete m_attackObjects[i];
			m_attackObjects[i] = m_attackObjects.back();
			m_attackObjects.pop_back();
			i--;
		}
	}
}

// �U��<->�U���̓����蔻��
void World::atariAttackAndAttack() {
	if (m_attackObjects.size() == 0) { return; }
	for (unsigned int i = 0; i < m_attackObjects.size() - 1; i++) {
		for (unsigned int j = i + 1; j < m_attackObjects.size(); j++) {
			// �U�����Ǐ��ɓ������Ă��邩����
			if (m_attackObjects[j]->atariObject(m_attackObjects[i])) {
				// ���������ꍇ �G�t�F�N�g�쐬
				int x = m_attackObjects[i]->getCenterX();
				int y = m_attackObjects[i]->getCenterY();
				m_animations.push_back(m_attackObjects[j]->createAnimation(x, y, 3));
				int soundHandle = m_attackObjects[i]->getSoundHandle();
				int panPal = adjustPanSound(x, m_camera->getX());
				m_soundPlayer_p->pushSoundQueue(soundHandle, panPal);
			}
		}
	}
}

// ��b������
void World::talk() {
	if (m_conversation_p != NULL) {
		// ��b�I��
		if (m_conversation_p->play()) {
			m_conversation_p = NULL;
		}
	}
}


Character* World::getCharacterWithName(string characterName) const {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characters[i]->getName() == characterName) {
			return m_characters[i];
		}
	}
	return NULL;
}

Character* World::getCharacterWithId(int id) const {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characters[i]->getId() == id) {
			return m_characters[i];
		}
	}
	return NULL;
}

void World::setBrainWithId(int id, Brain* brain) {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getId() == id) {
			m_characterControllers[i]->setBrain(brain);
		}
	}
}

CharacterController* World::getControllerWithName(string characterName) const {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == characterName) {
			return m_characterControllers[i];
		}
	}
	return NULL;
}