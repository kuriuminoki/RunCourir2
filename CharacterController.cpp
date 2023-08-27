#include "CharacterController.h"
#include "CharacterAction.h"
#include "Character.h"
#include "Camera.h"
#include "Control.h"
#include "Brain.h"
#include "ControllerRecorder.h"
#include "Define.h"
#include "DxLib.h"
#include <algorithm>


/*
* �R���g���[��
*/
CharacterController::CharacterController(Brain* brain, CharacterAction* characterAction) {

	// ������
	m_brain = brain;
	m_characterAction = characterAction;

	// Brain��Action�𒍓�
	m_brain->setCharacterAction(m_characterAction);

	// ���R�[�_�̓f�t�H���g�Ŏg��Ȃ�
	m_stickRecorder = NULL;
	m_jumpRecorder = NULL;
	m_squatRecorder = NULL;
	m_slashRecorder = NULL;
	m_bulletRecorder = NULL;
	m_damageRecorder = NULL;

	m_duplicationFlag = false;
}

CharacterController::CharacterController() :
	CharacterController(NULL, NULL)
{

}

// Action�C���X�^���X�͂����ł̂݃f���[�g����
CharacterController::~CharacterController() {
	if (m_characterAction != NULL) {
		delete m_characterAction;
	}
	if (m_brain != NULL) {
		delete m_brain;
	}
	if (m_stickRecorder != NULL && !m_duplicationFlag) {
		delete m_stickRecorder;
		delete m_jumpRecorder;
		delete m_squatRecorder;
		delete m_slashRecorder;
		delete m_bulletRecorder;
	}
	if (m_damageRecorder != NULL && !m_duplicationFlag) {
		delete m_damageRecorder;
	}
}

// ���R�[�_��������
void CharacterController::initRecorder() {
	if (m_stickRecorder != NULL) {
		m_stickRecorder->init();
		m_jumpRecorder->init();
		m_squatRecorder->init();
		m_slashRecorder->init();
		m_bulletRecorder->init();
	}
	if (m_damageRecorder != NULL) {
		m_damageRecorder->init();
	}
}

// ���R�[�h����߂�
void CharacterController::eraseRecorder() { 
	if (m_stickRecorder != NULL) { 
		delete m_stickRecorder;
		delete m_jumpRecorder;
		delete m_squatRecorder;
		delete m_slashRecorder;
		delete m_bulletRecorder;
		m_stickRecorder = NULL;
		m_jumpRecorder = NULL;
		m_squatRecorder = NULL;
		m_slashRecorder = NULL;
		m_bulletRecorder = NULL;
	}
	if (m_damageRecorder != NULL) {
		delete m_damageRecorder;
		m_damageRecorder = NULL;
	}
}

// �b������������ɓ������肷��{�^����true��
bool CharacterController::getActionKey() const { return m_brain->actionOrder(); }

void CharacterController::setAction(CharacterAction* action) {
	delete m_characterAction;
	m_characterAction = action;
	m_brain->setCharacterAction(m_characterAction);
}
void CharacterController::setBrain(Brain* brain) {
	delete m_brain;
	m_brain = brain;
	m_brain->setCharacterAction(m_characterAction);
}

// ���R�[�_�̃Z�b�^
void CharacterController::setStickRecorder(ControllerRecorder* recorder) {
	m_stickRecorder = recorder;
}
void CharacterController::setJumpRecorder(ControllerRecorder* recorder) {
	m_jumpRecorder = recorder;
}
void CharacterController::setSquatRecorder(ControllerRecorder* recorder) {
	m_squatRecorder = recorder;
}
void CharacterController::setSlashRecorder(ControllerRecorder* recorder) {
	m_slashRecorder = recorder;
}
void CharacterController::setBulletRecorder(ControllerRecorder* recorder) {
	m_bulletRecorder = recorder;
}
void CharacterController::setDamageRecorder(ControllerRecorder* recorder) {
	m_damageRecorder = recorder;
}

void CharacterController::setTarget(Character* character) { 
	m_brain->setTarget(character);
}

// �A�N�V�����̃Z�b�^
void CharacterController::setCharacterGrand(bool grand) {
	m_characterAction->setGrand(grand);
}
void CharacterController::setCharacterGrandRightSlope(bool grand) {
	m_characterAction->setGrandRightSlope(grand);
}
void CharacterController::setCharacterGrandLeftSlope(bool grand) {
	m_characterAction->setGrandLeftSlope(grand);
}
void CharacterController::setActionRightLock(bool lock) {
	m_characterAction->setRightLock(lock);
}
void CharacterController::setActionLeftLock(bool lock) {
	m_characterAction->setLeftLock(lock);
}
void CharacterController::setActionUpLock(bool lock) {
	m_characterAction->setUpLock(lock);
}
void CharacterController::setActionDownLock(bool lock) {
	m_characterAction->setDownLock(lock);
}
void CharacterController::setActionBoost() {
	m_characterAction->setBoost();
}

// �L�����N�^�[�̃Z�b�^
void CharacterController::setCharacterX(int x) {
	m_characterAction->setCharacterX(x);
}
void CharacterController::setCharacterY(int y) {
	m_characterAction->setCharacterY(y);
}

// �s���O�̏���
void CharacterController::init() {
	m_characterAction->init();
}

// �U���Ώۂ�ύX
void CharacterController::searchTargetCandidate(Character* character) {
	m_brain->searchTarget(character);
}

// �ǐՑΏۂ�ύX
void CharacterController::searchFollowCandidate(Character* character) {
	m_brain->searchFollow(character);
}

// �s���̌��ʔ��f
void CharacterController::action() {
	// �������Z
	m_characterAction->action();
}


/*
* �L�[�{�[�h�ɂ��L�����R���g���[�� �}�E�X���g���̂�Camera���K�v
*/
NormalController::NormalController(Brain* brain, CharacterAction* characterAction):
	CharacterController(brain, characterAction)
{

}

CharacterController* NormalController::createCopy(std::vector<Character*> characters, const Camera* camera) {
	CharacterAction* action = m_characterAction->createCopy(characters);
	Brain* brain = m_brain->createCopy(characters, camera);
	brain->setCharacterAction(action);
	CharacterController* res = new NormalController(brain, action);
	// �����̓��R�[�_���f���[�g���Ȃ�����Flag��true�ɂ���
	res->setDuplicationFlag(true);
	res->setStickRecorder(m_stickRecorder);
	res->setJumpRecorder(m_jumpRecorder);
	res->setSquatRecorder(m_squatRecorder);
	res->setSlashRecorder(m_slashRecorder);
	res->setBulletRecorder(m_bulletRecorder);
	res->setDamageRecorder(m_damageRecorder);
	return res;
}

void NormalController::control() {
	// �_���[�W�̃��R�[�h�i�����L�^�ƌ��󂪈Ⴆ�Έȍ~�̃��R�[�h���폜�j
	if (m_damageRecorder != NULL) {
		bool flag = (m_characterAction->getState() == CHARACTER_STATE::DAMAGE);
		if (m_damageRecorder->existRecord()) {
			if ((bool)m_damageRecorder->checkInput() != flag) {
				m_stickRecorder->discardRecord();
				m_jumpRecorder->discardRecord();
				m_squatRecorder->discardRecord();
				m_slashRecorder->discardRecord();
				m_bulletRecorder->discardRecord();
				m_damageRecorder->discardRecord();
				m_damageRecorder->writeRecord((int)flag);
			}
		}
		else {
			m_damageRecorder->writeRecord((int)flag);
		}
		m_damageRecorder->addTime();
	}

	// �ړ� stick�Ȃǂ̓��͏�Ԃ��X�V����
	int rightStick = 0, leftStick = 0, upStick = 0, downStick = 0;
	if (m_stickRecorder != NULL) {
		if (m_stickRecorder->existRecord()) {
			int input = m_stickRecorder->checkInput();
			if (((input >> 0) & 1) == 1) { 
				rightStick = 1;
			}
			if (((input >> 1) & 1) == 1) { 
				leftStick = 1;
			}
			if (((input >> 2) & 1) == 1) { upStick = 1; }
			if (((input >> 3) & 1) == 1) { downStick = 1; }
		}
		else {
			m_brain->moveOrder(rightStick, leftStick, upStick, downStick);
			int bit = 0;
			if (rightStick > 0) { bit |= (1 << 0); }
			if (leftStick > 0) { bit |= (1 << 1); }
			if (upStick > 0) { bit |= (1 << 2); }
			if (downStick > 0) { bit |= (1 << 3); }
			m_stickRecorder->writeRecord(bit);
		}
		m_stickRecorder->addTime();
	}
	else {
		m_brain->moveOrder(rightStick, leftStick, upStick, downStick);
	}

	// stick�ɉ����Ĉړ�
	m_characterAction->move(rightStick, leftStick, upStick, downStick);

	// �W�����v
	int jump = 0;
	if (m_jumpRecorder != NULL) {
		if (m_jumpRecorder->existRecord()) {
			jump = m_jumpRecorder->checkInput();
		}
		else {
			jump = m_brain->jumpOrder();
			if (jump == 1) {
				jump;
			}
			m_jumpRecorder->writeRecord(jump);
		}
		m_jumpRecorder->addTime();
	}
	else {
		jump = m_brain->jumpOrder();
	}
	m_characterAction->jump(jump);

	// ���Ⴊ��
	int squat = 0;
	if (m_squatRecorder != NULL) {
		if (m_squatRecorder->existRecord()) {
			squat = m_squatRecorder->checkInput();
		}
		else {
			squat = m_brain->squatOrder();
			m_squatRecorder->writeRecord(squat);
		}
		m_squatRecorder->addTime();
	}
	else {
		squat = m_brain->squatOrder();
	}
	m_characterAction->setSquat(squat);
}

Object* NormalController::bulletAttack() {

	// �U���ڕW
	int targetX = 0, targetY = 0;
	m_brain->bulletTargetPoint(targetX, targetY);

	// ����
	int order = 0;
	if (m_bulletRecorder != NULL) {
		if (m_bulletRecorder->existRecord()) {
			order = m_bulletRecorder->checkInput();
			// �U���ڕW���擾
			m_bulletRecorder->getGoal(targetX, targetY);
		}
		else {
			order = m_brain->bulletOrder();
			m_bulletRecorder->writeRecord(order);
			// �U���ڕW����������
			m_bulletRecorder->setGoal(targetX, targetY);
		}
		m_bulletRecorder->addTime();
	}
	else {
		order = m_brain->bulletOrder();
	}

	// �������U���̖��߂�����Ă���Ȃ�
	if (order > 0) {
		// �ڕW�Ɍ������Ďˌ�
		return m_characterAction->bulletAttack(targetX, targetY);
	}
	return NULL;
}

Object* NormalController::slashAttack() {

	// �U���ڕW
	int targetX = 0, targetY = 0;
	m_brain->bulletTargetPoint(targetX, targetY);

	// ����
	int order = 0;
	if (m_slashRecorder != NULL) {
		if (m_slashRecorder->existRecord()) {
			order = m_slashRecorder->checkInput();
			// �U���ڕW���擾
			m_slashRecorder->getGoal(targetX, targetY);
		}
		else {
			order = m_brain->slashOrder();
			m_slashRecorder->writeRecord(order);
			// �U���ڕW����������
			m_bulletRecorder->setGoal(targetX, targetY);
		}
		m_slashRecorder->addTime();
	}
	else {
		order = m_brain->slashOrder();
	}

	// �ߋ����U���̖��߂����ꂽ���A������ō����U���^�C�~���O�Ȃ�
	if (order == 1 || m_characterAction->getSlashCnt() > 0) {
		return m_characterAction->slashAttack(targetX, targetY);
	}
	return NULL;
}

void NormalController::damage(int vx, int vy, int damageValue) {
	m_characterAction->damage(vx, vy, damageValue);
}