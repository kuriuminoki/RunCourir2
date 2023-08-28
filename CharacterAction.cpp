#include "CharacterAction.h"
#include "Character.h"
#include "Sound.h"
#include "Define.h"
#include "DxLib.h"
#include <vector>


using namespace std;


/*
*  �L�����N�^�[�̊��N���X
*/
CharacterAction::CharacterAction(Character* character, SoundPlayer* soundPlayer_p) {
	m_character_p = character;
	m_soundPlayer_p = soundPlayer_p;

	//�������
	m_state = CHARACTER_STATE::STAND;
	m_grand = false;
	m_runCnt = -1;
	m_squat = false;
	m_preJumpCnt = -1;
	m_moveRight = false;
	m_moveLeft = false;
	m_moveUp = false;
	m_moveDown = false;
	m_grandLeftSlope = false;
	m_grandRightSlope = false;
	m_vx = 0;
	m_vy = 0;

	m_rightLock = false;
	m_leftLock = false;
	m_upLock = false;
	m_downLock = false;

	m_bulletCnt = 0;

	m_slashCnt = 0;

	m_attackLeftDirection = false;

	m_landCnt = 0;
	m_boostCnt = 0;
	m_damageCnt = 0;
}

CharacterAction::CharacterAction() :
	CharacterAction(NULL, NULL)
{

}

void CharacterAction::setParam(CharacterAction* action) {
	action->setState(m_state);
	action->setSimpleGrand(m_grand);
	action->setGrandLeftSlope(m_grandLeftSlope);
	action->setGrandRightSlope(m_grandRightSlope);
	action->setRunCnt(m_runCnt);

	action->setSquat(m_squat);
	action->setJumpCnt(m_preJumpCnt);
	action->setMoveRight(m_moveRight);
	action->setMoveLeft(m_moveLeft);
	action->setMoveUp(m_moveUp);
	action->setMoveDown(m_moveDown);
	action->setVx(m_vx);
	action->setVy(m_vy);
	action->setRightLock(m_rightLock);
	action->setLeftLock(m_leftLock);
	action->setUpLock(m_upLock);
	action->setDownLock(m_downLock);
	action->setBulletCnt(m_bulletCnt);
	action->setSlashCnt(m_slashCnt);
	action->setAttackLeftDirection(m_attackLeftDirection);
	action->setLandCnt(m_landCnt);
	action->setBoostCnt(m_boostCnt);
	action->setDamageCnt(m_damageCnt);
}

void CharacterAction::setState(CHARACTER_STATE state) {
	if (m_state == CHARACTER_STATE::DAMAGE) { return; }
	m_state = state;
}

// �L�����𓮂��Ȃ�����
void CharacterAction::setRightLock(bool lock) { 
	// ���b�N����
	m_rightLock = lock;
}
void CharacterAction::setLeftLock(bool lock) { 
	m_leftLock = lock;
}
void CharacterAction::setUpLock(bool lock) { 
	m_upLock = lock;
}
void CharacterAction::setDownLock(bool lock) { 
	m_downLock = lock;
}

// �L�����N�^�[�̃Z�b�^
void CharacterAction::setCharacterX(int x) {
	m_character_p->setX(x);
}
void CharacterAction::setCharacterY(int y) {
	m_character_p->setY(y);
}
void CharacterAction::setCharacterLeftDirection(bool leftDirection) {
	m_character_p->setLeftDirection(leftDirection);
}

bool CharacterAction::ableDamage() const {
	if (m_state == CHARACTER_STATE::DAMAGE) { return false; }
	return true;
}

bool CharacterAction::ableAttack() const {
	if (m_bulletCnt > 0 || m_slashCnt > 0) {
		return false;
	}
	return true;
}

// ���n
void CharacterAction::setGrand(bool grand) {
	if (m_vy > 0) { // ���n���[�V�����ɂȂ�
		m_landCnt = LAND_TIME;
		m_slashCnt = 0;
		// ���ʉ�
		if (m_soundPlayer_p != NULL) {
			m_soundPlayer_p->pushSoundQueue(m_character_p->getLandSound(),
				adjustPanSound(m_character_p->getCenterX(),
					m_soundPlayer_p->getCameraX()));
		}
	}
	m_grand = grand;
	if (m_state == CHARACTER_STATE::DAMAGE && m_damageCnt == 0) {
		m_vx = 0;
		m_vy = 0;
		m_state = CHARACTER_STATE::STAND;
	}
}

void CharacterAction::setSquat(bool squat) {
	if (squat && m_grand && m_state != CHARACTER_STATE::DAMAGE && m_state != CHARACTER_STATE::SLASH && m_state != CHARACTER_STATE::PREJUMP) {
		m_squat = true;
	}
	else {
		m_squat = false;
	}
}

// �����̂���߂�
void CharacterAction::stopMoveLeft() {
	// ���֕����̂���߂�
	if (m_moveLeft) {
		m_vx += m_character_p->getMoveSpeed();
		m_moveLeft = false;
		m_runCnt = -1;
	}
}
void CharacterAction::stopMoveRight() {
	// �E�֕����̂���߂�
	if (m_moveRight) {
		m_vx -= m_character_p->getMoveSpeed();
		m_moveRight = false;
		m_runCnt = -1;
	}
}

/*
* ����΂Ȃ����ʂ̖_�l��
*/
StickAction::StickAction(Character* character, SoundPlayer* soundPlayer_p):
	CharacterAction(character, soundPlayer_p)
{

}

CharacterAction* StickAction::createCopy(vector<Character*> characters) {
	CharacterAction* res = NULL;
	for (unsigned int i = 0; i < characters.size(); i++) {
		if (m_character_p->getId() == characters[i]->getId()) {
			res = new StickAction(characters[i], m_soundPlayer_p);
			// �R�s�[����
			setParam(res);
		}
	}
	return res;
}

// �s���O�̏���
void StickAction::init() {
	// ��������S�����ɓ�����悤�ɂ���
	m_rightLock = false;
	m_leftLock = false;
	m_upLock = false;
	m_downLock = false;

	// �������񒈂ɕ�������
	m_grand = false;
	m_grandRightSlope = false;
	m_grandLeftSlope = false;
}

void StickAction::action() {
	// ���(state)�ɉ����ĉ摜���Z�b�g
	switchHandle();

	// �ˌ��̃C���^�[�o������
	if (m_bulletCnt > 0) { m_bulletCnt--; }

	// �a���̃C���^�[�o������
	if (m_slashCnt > 0) { m_slashCnt--; }

	// �_���[�W�󂯏�Ԃ͍Œ�P�b�߂�����
	if (m_damageCnt > 0) { m_damageCnt--; }

	// �d�͂̏���
	// ���ɂ���
	if(!m_grand) { 
		// �d��
		m_vy += G;
	}

	// �A�j���[�V�����p�̃J�E���g
	if (m_landCnt > 0) { m_landCnt--; }
	if (m_boostCnt > 0) { m_boostCnt--; }

	// �ړ�
	if (m_vx > 0) {// �E
		if (m_rightLock) {
			m_vx = 0;
		}
		else {
			m_character_p->moveRight(m_vx);
		}
	}
	else if (m_vx < 0) { // ��
		if (m_leftLock) {
			m_vx = 0;
		}
		else {
			m_character_p->moveLeft(-m_vx);
		}
	}
	if (m_vy < 0) { // ��
		if (m_upLock) {
			m_vy = 0;
		}
		else {
			m_character_p->moveUp(-m_vy);
		}
	}
	else if (m_vy > 0) { // ��
		if (m_downLock) {
			m_vy = 0;
		}
		else {
			m_character_p->moveDown(m_vy);
		}
	}
}

// ��Ԃɉ����ĉ摜�Z�b�g
void StickAction::switchHandle() {
	// �Z�b�g�O�̉摜�̃T�C�Y
	int wide, height;
	m_character_p->getHandleSize(wide, height);
	if (m_grand) { // �n�ʂɂ���Ƃ�
		switch (getState()) {
		case CHARACTER_STATE::STAND: //�������
			if (m_landCnt > 0) {
				m_character_p->switchLand();
			}
			else if (m_slashCnt > 0) {
				m_character_p->switchSlash();
			}
			else if (m_bulletCnt > 0) {
				if (m_runCnt != -1) {
					m_character_p->switchRunBullet(m_runCnt);
				}
				else if (m_squat) {
					m_character_p->switchSquatBullet();
				}
				else {
					m_character_p->switchBullet();
				}
			}
			else if (m_squat) {
				m_character_p->switchSquat();
			}
			else if (m_runCnt != -1) {
				m_character_p->switchRun(m_runCnt);
			}
			else {
				m_character_p->switchStand();
			}
			break;
		case CHARACTER_STATE::PREJUMP:
			m_character_p->switchPreJump(m_preJumpCnt);
			break;
		case CHARACTER_STATE::DAMAGE:
			if (m_boostCnt > 0) {
				if (m_slashCnt > 0) {
					m_character_p->switchSlash();
				}
				else if (m_bulletCnt > 0) {
					if (m_runCnt != -1) {
						m_character_p->switchRunBullet(m_runCnt);
					}
					else if (m_squat) {
						m_character_p->switchSquatBullet();
					}
					else {
						m_character_p->switchBullet();
					}
				}
				else {
					m_character_p->switchBoost();
				}
			}
			else {
				m_character_p->switchDamage();
			}
			break;
		}
	}
	else { // ���ɂ���Ƃ�
		switch (getState()) {
		case CHARACTER_STATE::STAND: //�������(�Ȃɂ��Ȃ��̏��)
			if (m_boostCnt > 0) {
				m_character_p->switchBoost();
			}
			else if (m_slashCnt > 0) {
				m_character_p->switchAirSlash();
			}
			else if (m_bulletCnt > 0) {
				m_character_p->switchAirBullet();
			}
			else if (m_vy < 0) {
				m_character_p->switchJump();
			}
			else {
				m_character_p->switchDown();
			}
			break;
		case CHARACTER_STATE::DAMAGE:
			if (m_boostCnt > 0) {
				if (m_slashCnt > 0) {
					m_character_p->switchAirSlash();
				}
				else if (m_bulletCnt > 0) {
					m_character_p->switchAirBullet();
				}
				else {
					m_character_p->switchBoost();
				}
			}
			else {
				m_character_p->switchDamage();
			}
			break;
		}
	}
	// �Z�b�g��̉摜�̃T�C�Y
	int afterWide, afterHeight;
	m_character_p->getHandleSize(afterWide, afterHeight);

	// �T�C�Y�ύX�ɂ��ʒu����
	afterChangeGraph(wide, height, afterWide, afterHeight);

	m_character_p->setLeftDirection(m_character_p->getLeftDirection());
}

// �摜�̃T�C�Y�ύX�ɂ��ʒu���� (���W�͉摜�̍���ł��邱�Ƃɒ���)
void CharacterAction::afterChangeGraph(int beforeWide, int beforeHeight, int afterWide, int afterHeight) {
	// ���֍s���Ȃ��Ȃ�
	if (m_downLock) {
		// ��֓�����
		m_character_p->moveUp((afterHeight - beforeHeight));
	}
	// ��֍s���Ȃ��Ȃ�
	else if (m_upLock) {
		// ���֓������K�v�͂Ȃ��i�摜���������Ɋg�傳��邩��j
	}
	// �㉺�ǂ����ɂł��s����
	else {
		// �����ɍL����
		int d = afterHeight - beforeHeight;
		if (d % 2 == 1) {
			if (d < 0) {
				m_character_p->moveUp((d - 1) / 2);
			}
			else {
				m_character_p->moveUp((d + 1) / 2);
			}
		}
		else {
			m_character_p->moveUp(d / 2);
		}
	}

	// �E�֍s���Ȃ��Ȃ�
	if (m_rightLock && !m_leftLock) {
		// ���֓�����
		m_character_p->moveLeft((afterWide - beforeWide));
	}
	// ���֍s���Ȃ��Ȃ�
	else if (m_leftLock && !m_rightLock) {
		// �E�֓������K�v�͂Ȃ��i�摜���E�����Ɋg�傳��邩��j
	}
	// ���E�ǂ����ɂł��s����A�������͂����Ȃ�
	else {
		// �����ɍL����
		int d = afterWide - beforeWide;
		if (d % 2 == 1) {
			if (d < 0) {
				m_character_p->moveLeft((d - 1) / 2);
			}
			else {
				m_character_p->moveLeft((d + 1) / 2);
			}
		}
		else {
			m_character_p->moveLeft(d / 2);
		}
	}
}

// ���� �_���[�W���A���Ⴊ�ݒ��A�ǂԂ��蒆�͕s��
void StickAction::walk(bool right, bool left) {
	//// �E�֕����̂���߂�
	//if (!right || m_rightLock || m_squat || damageFlag()) {
	//	stopMoveRight();
	//}
	//// ���֕����̂���߂�
	//if (!left || m_leftLock || m_squat || damageFlag()) {
	//	stopMoveLeft();
	//}
	if (damageFlag()) {
		return;
	}
	bool walkStart = false;
	// �E�֕����n�߂�
	if (!m_rightLock && !m_moveRight && !m_moveLeft && right && (!left || !m_character_p->getLeftDirection()) && !m_squat) { // �E�֕���
		m_vx += m_character_p->getMoveSpeed();
		m_moveRight = true;
		walkStart = true;
	}
	// ���֕����n�߂�
	if (!m_leftLock && !m_moveRight && !m_moveLeft && left && (!right || m_character_p->getLeftDirection()) && !m_squat) { // ���֕���
		m_vx -= m_character_p->getMoveSpeed();
		m_moveLeft = true;
		walkStart = true;
	}
	// �A�j���[�V�����p�ɃJ�E���g
	if (m_moveLeft || m_moveRight) {
		m_runCnt++;
	}
}

// �ړ�
void StickAction::move(bool right, bool left, bool up, bool down) {
	if (getState() == CHARACTER_STATE::STAND && m_grand && m_slashCnt == 0 && m_bulletCnt == 0) {
		// �ړ������֌���
		if(left && !right){
			m_character_p->setLeftDirection(true);
		}
		if (right && !left) {
			m_character_p->setLeftDirection(false);
		}
	}
	// ���E�̈ړ�
	walk(right, left);
}

// �W�����v
void StickAction::jump(int cnt) {
	// �_���[�W��ԂȂ�W�����v�ł��Ȃ�����return
	if (damageFlag()) {
		//// �󂯐g�͂ł���
		//if (cnt == 1 || m_boostCnt > 0) {
		//	if (m_boostCnt == 0) {
		//		// �󂯐g�������u��
		//		m_vy -= m_character_p->getJumpHeight() / 2;
		//		m_grand = false;
		//		m_preJumpCnt = -1;
		//		stopMoveLeft();
		//		stopMoveRight();
		//		// ���ʉ�
		//		if (m_soundPlayer_p != NULL) {
		//			m_soundPlayer_p->pushSoundQueue(m_character_p->getPassiveSound(),
		//				adjustPanSound(m_character_p->getCenterX(),
		//					m_soundPlayer_p->getCameraX()));
		//		}
		//	}
		//	// �_���[�W��Ԃ����������܂ł͂�����Boost
		//	m_boostCnt = BOOST_TIME;
		//}
		return;
	}
	// �a�����̓W�����v�s��
	if (m_slashCnt > 0) { return; }
	// ���ɕ�������W�����v���~
	if (!m_grand) {
		m_preJumpCnt = -1;
		if (getState() == CHARACTER_STATE::PREJUMP) {
			setState(CHARACTER_STATE::STAND);
		}
	}
	// �W�����v�̏����J�n
	if (cnt > 0 && m_grand && m_preJumpCnt == -1) {
		m_preJumpCnt = 0;
		setState(CHARACTER_STATE::PREJUMP);
	}
	if (m_grand && m_preJumpCnt >= 0) {
		if (cnt == 0 || m_preJumpCnt == PRE_JUMP_MAX) {
			// �W�����v
			int rate = (100 * m_preJumpCnt) / PRE_JUMP_MAX;
			int power = (m_character_p->getJumpHeight() * rate) / 100;
			m_vy -= power;
			m_grand = false;
			m_preJumpCnt = -1;
			setState(CHARACTER_STATE::STAND);
			// ���ʉ�
			if (m_soundPlayer_p != NULL) {
				m_soundPlayer_p->pushSoundQueue(m_character_p->getJumpSound(),
					adjustPanSound(m_character_p->getCenterX(),
						m_soundPlayer_p->getCameraX()));
			}
		}
		else {
			// �W�����v�O�̗��ߒ�
			m_preJumpCnt++;
		}
	}
}

// �ˌ��U��
Object* StickAction::bulletAttack(int gx, int gy) {
	if (damageFlag() && m_boostCnt == 0) {
		m_bulletCnt = 0;
		return NULL;
	}
	// �ˌ��\��ԂȂ�
	if (ableAttack()) {
		// �ˌ��s�\��Ԃɂ���
		m_bulletCnt = m_character_p->getBulletRapid();
		// �������֌���
		m_character_p->setLeftDirection(m_character_p->getCenterX() > gx);
		// �U����Ԃ�
		return m_character_p->bulletAttack(gx, gy, m_soundPlayer_p);
	}
	return NULL;
}

// �a���U��
Object* StickAction::slashAttack(int gx, int gy) {
	if (damageFlag() && m_boostCnt == 0) {
		m_slashCnt = 0;
		return NULL;
	}
	// �U���J�n
	if (ableAttack()) {
		// �W�����v�̓L�����Z��
		m_preJumpCnt = -1;
		if (getState() == CHARACTER_STATE::PREJUMP) {
			setState(CHARACTER_STATE::STAND);
		}
		m_attackLeftDirection = m_character_p->getCenterX() > gx;
		m_slashCnt = m_character_p->getSlashCountSum() + m_character_p->getSlashInterval();
		// �U���̕����֌���
		m_character_p->setLeftDirection(m_attackLeftDirection);
	}
	// �U���̃^�C�~���O����Ȃ��Ȃ�NULL���Ԃ�
	return m_character_p->slashAttack(m_attackLeftDirection, m_slashCnt, m_soundPlayer_p);
}

// �_���[�W���󂯂�
void StickAction::damage(int vx, int vy, int damageValue) {
	setState(CHARACTER_STATE::DAMAGE);
	m_vx += vx;
	m_vy += vy;
	// �n�ʂɂ��Ă��Ă������̓_���[�W���[�V����
	if(m_vy >= 0 && m_grand){ m_damageCnt = 20; }
	m_character_p->setLeftDirection(m_vx > 0);
	m_grand = false;
	// ���ɕ�������
	m_grand = false;
	m_grandRightSlope = false;
	m_grandLeftSlope = false;
	// HP����
	m_character_p->damageHp(damageValue);
	m_boostCnt = 0;
}


RunnerAction::RunnerAction(Character* character, SoundPlayer* soundPlayer_p) :
	StickAction(character, soundPlayer_p)
{

}

// ��Ԃɉ����ĉ摜�Z�b�g
void RunnerAction::switchHandle() {
	// �Z�b�g�O�̉摜�̃T�C�Y
	int wide, height;
	int tired = 2;
	m_character_p->getHandleSize(wide, height);
	if (m_grand) { // �n�ʂɂ���Ƃ�
		switch (getState()) {
		case CHARACTER_STATE::STAND: //�������
		case CHARACTER_STATE::PREJUMP:
			if (m_runCnt != -1) {
				m_character_p->switchRun(m_runCnt, tired);
			}
			else {
				m_character_p->switchStand();
			}
			break;
		case CHARACTER_STATE::DAMAGE:
			m_character_p->switchDamage();
			break;
		}
	}
	else { // ���ɂ���Ƃ�
		switch (getState()) {
		case CHARACTER_STATE::STAND: //�������(�Ȃɂ��Ȃ��̏��)
			m_character_p->switchRun(m_runCnt, tired);
			break;
		case CHARACTER_STATE::DAMAGE:
			m_character_p->switchDamage();
			break;
		}
	}
	// �Z�b�g��̉摜�̃T�C�Y
	int afterWide, afterHeight;
	m_character_p->getHandleSize(afterWide, afterHeight);

	// �T�C�Y�ύX�ɂ��ʒu����
	afterChangeGraph(wide, height, afterWide, afterHeight);

	m_character_p->setLeftDirection(m_character_p->getLeftDirection());
}