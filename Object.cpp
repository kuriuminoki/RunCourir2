#include "Object.h"
#include "Character.h"
#include "CharacterAction.h"
#include "CharacterController.h"
#include "Define.h"
#include "GraphHandle.h"
#include "Animation.h"
#include "DxLib.h"
#include <algorithm>
#include <cmath>
#include <sstream>

using namespace std;


Object::Object() :
	Object(0, 0, 0, 0, -1)
{

}

Object::Object(int x1, int y1, int x2, int y2, int hp) {
	m_x1 = x1;
	m_y1 = y1;
	m_x2 = x2;
	m_y2 = y2;
	// �召�֌W�� 1 <= 2
	if (m_x1 > m_x2) { std::swap(m_x1, m_x2); }
	if (m_y1 > m_y2) { std::swap(m_y1, m_y2); }

	m_hp = hp;
	m_damageCnt = 0;

	m_deleteFlag = false;

	m_effectHandles_p = NULL;
	m_soundHandle_p = -1;
}

void Object::setParam(Object* object) {
	object->setX1(m_x1);
	object->setY1(m_y1);
	object->setX2(m_x2);
	object->setY2(m_y2);
	object->setHp(m_hp);
	object->setDamageCnt(m_damageCnt);
	object->setDeleteFlag(m_deleteFlag);
	object->setEffectHandles(m_effectHandles_p);
	object->setSoundHandle(m_soundHandle_p);
}

// HP�����炷
void Object::decreaseHp(int damageValue) {
	m_hp = max(0, m_hp - damageValue);
	if (m_hp == 0) { setDeleteFlag(true); }
	m_damageCnt = DAMAGE_CNT_SUM;
}

// �A�j���[�V�����쐬
Animation* BulletObject::createAnimation(int x, int y, int flameCnt) {
	if (m_effectHandles_p == NULL) {
		return NULL;
	}
	return new Animation((m_x1 + m_x2) / 2, (m_y1 + m_y2) / 2, 3, m_effectHandles_p);
}

// �A�j���[�V�����쐬
Animation* SlashObject::createAnimation(int x, int y, int flameCnt) {
	if (m_effectHandles_p == NULL) {
		return NULL;
	}
	// ���W�̕��ς��Ƃ�
	x = (x + (m_x1 + m_x2) / 2) / 2;
	y = (y + (m_y1 + m_y2) / 2) / 2;
	return new Animation(x, y, flameCnt, m_effectHandles_p);
}

/*
* �l�p�`�̃I�u�W�F�N�g
*/
BoxObject::BoxObject(int x1, int y1, int x2, int y2, int color, int hp) :
	Object(x1, y1, x2, y2, hp)
{
	m_color = color;
}

// �L�����N�^�[�Ƃ̓����蔻��
bool BoxObject::atari(CharacterController* characterController) {
	// �L�����̏��@���W�ƈړ��X�s�[�h
	int characterX1 = characterController->getAction()->getCharacter()->getX();
	int characterY1 = characterController->getAction()->getCharacter()->getY();
	int characterWide = characterController->getAction()->getCharacter()->getWide();
	int characterHeight = characterController->getAction()->getCharacter()->getHeight();
	int characterX2 = characterX1 + characterWide;
	int characterY2 = characterY1 + characterHeight;
	int characterVx = characterController->getAction()->getVx();
	int characterVy = characterController->getAction()->getVy();

	// �L�������㉺�ړ��œ������Ă��邩����
	if (characterX2 > m_x1 && characterX1 < m_x2) {
		// ���Ɉړ����̃L�������ォ�瓖�����Ă��邩����
		if (characterY2 <= m_y1 && characterY2 + characterVy >= m_y1) {
			// ���n
			characterController->setCharacterGrand(true);
			// �L�����͉��ֈړ��ł��Ȃ�
			characterController->setActionDownLock(true);
			// ������Ԃ܂ł͈ړ�������
			characterController->setCharacterY(m_y1 - characterHeight);
		}
		// ��Ɉړ����̃L�����������瓖�����Ă��邩����
		else if (characterY1 >= m_y2 && characterY1 + characterVy <= m_y2) {
			// �L�����͏�ֈړ��ł��Ȃ�
			characterController->setActionUpLock(true);
			// ������Ԃ܂ł͈ړ�������
			characterController->setCharacterY(m_y2);
		}
	}

	// �L���������E�ړ��œ������Ă��邩����
	if (characterY2 + characterVy > m_y1 && characterY1 + characterVy < m_y2) {
		// �E�Ɉړ����̃L�����������瓖�����Ă��邩����
		if (characterX2 <= m_x1 && characterX2 + characterVx >= m_x1) {
			// �i���Ƃ݂Ȃ��ď��z����
			if (characterY2 - STAIR_HEIGHT <= m_y1) {
				// �K�؂ȍ��W��
				characterController->setCharacterX(m_x1 - characterWide / 2 - characterVx);
				characterController->setCharacterY(m_y1 - characterHeight);
				// ���n
				characterController->setCharacterGrand(true);
				characterController->setActionBoost();
				// �L�����͉��ֈړ��ł��Ȃ�
				characterController->setActionDownLock(true);
			}
			else {
				// �L�����͉E�ֈړ��ł��Ȃ�
				characterController->setActionRightLock(true);
				// ������Ԃ܂ł͈ړ�������
				characterController->setCharacterX(m_x1 - characterWide);
			}
		}
		// ���Ɉړ����̃L�������E���瓖�����Ă��邩����
		else if (characterX1 >= m_x2 && characterX1 + characterVx <= m_x2) {
			if (characterY2 - STAIR_HEIGHT <= m_y1) {
				// �K�؂ȍ��W��
				characterController->setCharacterX(m_x2 - characterWide / 2 + characterVx);
				characterController->setCharacterY(m_y1 - characterHeight);
				// ���n
				characterController->setCharacterGrand(true);
				characterController->setActionBoost();
				// �L�����͉��ֈړ��ł��Ȃ�
				characterController->setActionDownLock(true);
			}
			else {
				// �L�����͍��ֈړ��ł��Ȃ�
				characterController->setActionLeftLock(true);
				// ������Ԃ܂ł͈ړ�������
				characterController->setCharacterX(m_x2);
			}
		}
	}
	return false;
}

// �L�������I�u�W�F�N�g�ɓ��荞��ł���Ƃ��̏���
void BoxObject::penetration(CharacterController* characterController) {
	// �L�����̏��@���W�ƈړ��X�s�[�h
	int characterX1 = characterController->getAction()->getCharacter()->getX();
	int characterY1 = characterController->getAction()->getCharacter()->getY();
	int characterWide = characterController->getAction()->getCharacter()->getWide();
	int characterHeight = characterController->getAction()->getCharacter()->getHeight();
	int characterX2 = characterX1 + characterWide;
	int characterY2 = characterY1 + characterHeight;
	// ������I�u�W�F�N�g�̒��ɓ��荞��ł��܂�����
	bool slope = characterController->getAction()->getGrandLeftSlope() || characterController->getAction()->getGrandRightSlope();
	if (!slope && characterY2 > m_y1 && characterY1 < m_y2 && characterX2 > m_x1 && characterX1 < m_x2) {
		// �L���������ɂ͂ݏo���Ă���Ȃ�
		if (characterX1 < m_x1 || characterX2 > m_x2) {
			if ((characterX1 + characterX2) < (m_x1 + m_x2)) {
				// ������Ԃ܂ňړ�������
				characterController->setCharacterX(m_x1 - characterWide);
				// �L�����͉E�ֈړ��ł��Ȃ�
				characterController->setActionRightLock(true);
			}
			else {
				// ������Ԃ܂ňړ�������
				characterController->setCharacterX(m_x2);
				// �L�����͍��ֈړ��ł��Ȃ�
				characterController->setActionLeftLock(true);
			}
		}
		else if (characterY1 < m_y1 || characterY2 > m_y2) {
			if ((characterY1 + characterY2) < (m_y1 + m_y2)) {
				// �^���
				characterController->setCharacterY(m_y1 - characterHeight);
				// ���n
				characterController->setCharacterGrand(true);
				// �L�����͉��ֈړ��ł��Ȃ�
				characterController->setActionDownLock(true);
			}
			else {
				// �^����
				characterController->setCharacterY(m_y2);
				// �L�����͏�ֈړ��ł��Ȃ�
				characterController->setActionUpLock(true);
			}
		}
	}
}

// �U���I�u�W�F�N�g�Ƃ̓����蔻��
bool BoxObject::atariObject(Object* object) {
	// �j��s�\�I�u�W�F�N�g
	if (!object->getAbleDelete()) { return false; }
	// �������Ă���Ȃ�
	if (m_x2 > object->getX1() && m_x1 < object->getX2() && m_y2 > object->getY1() && m_y1 < object->getY2()) {
		object->setDeleteFlag(true);
		// �����̗̑͂����炷
		if (getAbleDelete()) {
			decreaseHp(object->getDamage());
			return true;
		}
	}
	return false;
}

void BoxObject::action() {
	if (m_damageCnt > 0) { m_damageCnt--; }
}

TriangleObject::TriangleObject(int x1, int y1, int x2, int y2, int color, bool leftDown, int hp):
	Object(x1, y1, x2, y2, hp)
{
	m_color = color;
	m_leftDown = leftDown;
}

// ���WX�ɂ�����Y���W�i�X������Z�o����j
int TriangleObject::getY(int x) const {
	if (x < m_x1) {
		if (m_leftDown) {
			return m_y2;
		}
		else {
			return m_y1;
		}
	}
	if (x > m_x2) {
		if (m_leftDown) {
			return m_y1;
		}
		else {
			return m_y2;
		}
	}
	// �X��
	// ���q
	double a1 = (double)(m_y1 - m_y2);
	if (!m_leftDown) {
		a1 *= -1;
	}
	// ����
	double a2 = (double)(m_x2 - m_x1);

	// �ؕ�
	double b;
	if (m_leftDown) {
		b = m_y1 - (a1 * m_x2 / a2);
	}
	else {
		b = m_y1 - (a1 * m_x1 / a2);
	}

	// ���W
	double y = a1 * x / a2 + b;

	return (int)y;
}

// �L�����N�^�[�Ƃ̓����蔻��
bool TriangleObject::atari(CharacterController* characterController) {
	// �L�����̏��@���W�ƈړ��X�s�[�h
	int characterX1 = characterController->getAction()->getCharacter()->getX();
	int characterY1 = characterController->getAction()->getCharacter()->getY();
	int characterWide = characterController->getAction()->getCharacter()->getWide();
	int characterHeight = characterController->getAction()->getCharacter()->getHeight();
	int characterX2 = characterX1 + characterWide;
	int characterY2 = characterY1 + characterHeight;
	int characterX1_5 = characterController->getAction()->getCharacter()->getCenterX();
	int characterY1_5 = characterController->getAction()->getCharacter()->getCenterY();
	int characterVx = characterController->getAction()->getVx();
	int characterVy = characterController->getAction()->getVy();

	// �L�������㉺�ړ��œ������Ă��邩����
	if (characterX2 > m_x1 && characterX1 < m_x2) {
		// ����Ă���Ƃ��͂��̏�������true�ɂȂ�
		if (characterY2 == getY(characterX1_5 - characterVx)) {
			// �O�̃t���[���ł͒��n���Ă����̂ŁA�����������n
			characterController->setCharacterGrand(true);
			if (m_leftDown) {
				characterController->setCharacterGrandRightSlope(true);
			}
			else {
				characterController->setCharacterGrandLeftSlope(true);
			}
			// �L�����͉��ֈړ��ł��Ȃ�
			characterController->setActionDownLock(true);
			// ������Ԃ܂ł͈ړ�������
			characterController->setCharacterY(getY(characterX1_5) - characterHeight);
		}
		// ���Ɉړ����̃L�������ォ�瓖�����Ă��邩����
		else if (characterY2 <= getY(characterX1_5) && characterY2 + characterVy >= getY(characterX1_5)) {
			// ���n
			characterController->setCharacterGrand(true);
			if (m_leftDown) {
				characterController->setCharacterGrandRightSlope(true);
			}
			else {
				characterController->setCharacterGrandLeftSlope(true);
			}
			// �L�����͉��ֈړ��ł��Ȃ�
			characterController->setActionDownLock(true);
			// ������Ԃ܂ł͈ړ�������
			characterController->setCharacterY(getY(characterX1_5) - characterHeight);
		}
		// ��Ɉړ����̃L�����������瓖�����Ă��邩����
		else if (characterY1 >= m_y2 && characterY1 + characterVy <= m_y2) {
			// �L�����͏�ֈړ��ł��Ȃ�
			characterController->setActionUpLock(true);
			// ������Ԃ܂ł͈ړ�������
			characterController->setCharacterY(m_y2);
		}
	}

	// ����ړ�
	if (characterX2 > m_x1 && characterX1 < m_x2 && characterY2 <= m_y2 && characterY2 >= getY(characterX1_5)) {
		// ���n
		characterController->setCharacterGrand(true);
		if (m_leftDown) {
			characterController->setCharacterGrandRightSlope(true);
		}
		else {
			characterController->setCharacterGrandLeftSlope(true);
		}
		// �L�����͉��ֈړ��ł��Ȃ�
		characterController->setActionDownLock(true);
		// �K�؂ȍ����ֈړ�
		characterController->setCharacterY(getY(characterX1_5) - characterHeight);
	}

	// ��̉s�p�i��[�j�̓����蔻��
	if (m_y2 > characterY1 && m_y2 < characterY2) {
		if (m_leftDown) {// ����������
			// �E�Ɉړ����̃L�������E���瓖�����Ă��邩����
			if (characterX2 <= m_x1 && characterX2 + characterVx >= m_x1) {
				// �L�����͉E�ֈړ��ł��Ȃ�
				characterController->setActionRightLock(true);
				// ������Ԃ܂ł͈ړ�������
				characterController->setCharacterX(m_x1 - characterWide);
			}
		}
		else {
			// ���Ɉړ����̃L�����������瓖�����Ă��邩����
			if (characterX1 >= m_x2 && characterX1 + characterVx <= m_x2) {
				// �L�����͍��ֈړ��ł��Ȃ�
				characterController->setActionLeftLock(true);
				// ������Ԃ܂ł͈ړ�������
				characterController->setCharacterX(m_x2);
			}
		}
	}

	// �L���������E�ړ��œ������Ă��邩����
	if (characterY2 > m_y1 && characterY1 < m_y2) {
		if (m_leftDown) {// ����������
			// ���Ɉړ����̃L�������E���瓖�����Ă��邩����
			if (characterX1 >= m_x2 && characterX1 + characterVx <= m_x2) {
				// �L�����͍��ֈړ��ł��Ȃ�
				characterController->setActionLeftLock(true);
				// ������Ԃ܂ł͈ړ�������
				characterController->setCharacterX(m_x2);
			}
		}
		else {
			// �E�Ɉړ����̃L�����������瓖�����Ă��邩����
			if (characterX2 <= m_x1 && characterX2 + characterVx >= m_x1) {
				// �L�����͉E�ֈړ��ł��Ȃ�
				characterController->setActionRightLock(true);
				// ������Ԃ܂ł͈ړ�������
				characterController->setCharacterX(m_x1 - characterWide);
			}
		}
	}
	return false;
}

// �L�������I�u�W�F�N�g�ɓ��荞��ł���Ƃ��̏���
void TriangleObject::penetration(CharacterController* characterController) {
	// �L�����̏��@���W�ƈړ��X�s�[�h
	int characterX1 = characterController->getAction()->getCharacter()->getX();
	int characterY1 = characterController->getAction()->getCharacter()->getY();
	int characterWide = characterController->getAction()->getCharacter()->getWide();
	int characterHeight = characterController->getAction()->getCharacter()->getHeight();
	int characterX1_5 = characterController->getAction()->getCharacter()->getCenterX();
	int characterY1_5 = characterController->getAction()->getCharacter()->getCenterY();
	int characterX2 = characterX1 + characterWide;
	int characterY2 = characterY1 + characterHeight;
	int slopeY = getY(characterX1_5);
	// ������I�u�W�F�N�g�̒��ɓ��荞��ł��܂�����
	if (characterY2 > slopeY && characterY1 < m_y2 && characterX2 > m_x1 && characterX1 < m_x2) {
		if (characterY1 < slopeY || characterY2 > m_y2) {
			if ((characterY1 + characterY2) < (slopeY + m_y2)) {
				// �^���
				characterController->setCharacterY(slopeY - characterHeight);
				// ���n
				characterController->setCharacterGrand(true);
				// �L�����͉��ֈړ��ł��Ȃ�
				characterController->setActionDownLock(true);
			}
			else {
				// �^����
				characterController->setCharacterY(m_y2);
				// �L�����͏�ֈړ��ł��Ȃ�
				characterController->setActionUpLock(true);
			}
		}
		// �L���������ɂ͂ݏo���Ă���Ȃ�
		else if (characterX1 < m_x1 || characterX2 > m_x2) {
			if ((characterX1 + characterX2) < (m_x1 + m_x2)) {
				// ������Ԃ܂ňړ�������
				characterController->setCharacterX(m_x1 - characterWide);
				// �L�����͉E�ֈړ��ł��Ȃ�
				characterController->setActionRightLock(true);
			}
			else {
				// ������Ԃ܂ňړ�������
				characterController->setCharacterX(m_x2);
				// �L�����͍��ֈړ��ł��Ȃ�
				characterController->setActionLeftLock(true);
			}
		}
	}
}

// ���I�u�W�F�N�g�Ƃ̓����蔻��
bool TriangleObject::atariObject(Object* object) {
	// �j��s�\�I�u�W�F�N�g
	if (!object->getAbleDelete()) { return false; }
	// �Εӂ��l�����ē����蔻����v�Z
	int y = object->getY1();
	if (m_leftDown) {
		y = getY(object->getX2());
	}
	else {
		y = getY(object->getX1());
	}
	// �������Ă���Ȃ�
	if (m_x2 > object->getX1() && m_x1 < object->getX2() && m_y2 > object->getY1() && y < object->getY2()) {
		object->setDeleteFlag(true);
		// �����̗̑͂����炷
		if (getAbleDelete()) {
			decreaseHp(object->getDamage());
			return true;
		}
	}
	return false;
}

void TriangleObject::action() {
	if (m_damageCnt > 0) { m_damageCnt--; }
}


BulletObject::BulletObject(int x, int y, int color, int gx, int gy, AttackInfo* attackInfo) :
	Object(x - attackInfo->bulletRx(), y - attackInfo->bulletRx(), x + attackInfo->bulletRx(), y + attackInfo->bulletRy())
{
	// �K�v�Ȃ�ォ��Z�b�^�Őݒ�
	m_characterId = -1;
	m_groupId = -1;

	// �e�p�����[�^�̐ݒ�
	m_color = color;
	m_gx = gx;
	m_gy = gy;
	m_rx = attackInfo->bulletRx();
	m_ry = attackInfo->bulletRy();
	m_damage = attackInfo->bulletDamage();
	m_d = attackInfo->bulletDistance();
	m_hp = attackInfo->bulletHp();

	// �p�x���v�Z���AVX��VY������
	int dx = gx - x;
	int dy = gy - y;
	double r = std::atan2((double)dy, (double)dx);
	m_v = attackInfo->bulletSpeed();
	m_vx = (int)(m_v * std::cos(r));
	m_vy = (int)(m_v * std::sin(r));

	// �G�t�F�N�g�̉摜
	m_effectHandles_p = attackInfo->bulletEffectHandle();

	// �T�E���h
	m_soundHandle_p = attackInfo->bulletSoundeHandle();
}

BulletObject::BulletObject(int x, int y, int color, int gx, int gy) :
	Object()
{
	m_characterId = -1;
	m_groupId = -1;
	m_color = color;
	m_gx = gx;
	m_gy = gy;
	m_rx = 0;
	m_ry = 0;
	m_damage = 0;
	m_d = 0;
	m_hp = 0;
	m_v = 0;
	m_vx = 0;
	m_vy = 0;
	m_effectHandles_p = NULL;
	m_soundHandle_p = -1;
}

// �L�����Ƃ̓����蔻��
// �������Ă���Ȃ�L�����𑀍삷��B
bool BulletObject::atari(CharacterController* characterController) {
	// ���Ŗh�~
	if (m_characterId == characterController->getAction()->getCharacter()->getId()) {
		return false;
	}
	// �`�[���L���h�~ && �����L�����ɂ͓�����Ȃ�
	int groupId = characterController->getAction()->getCharacter()->getGroupId();
	if (m_groupId == groupId || groupId == -1) {
		return false;
	}

	// �L�����̏��@���W�ƈړ��X�s�[�h
	int characterX1 = characterController->getAction()->getCharacter()->getX();
	int characterY1 = characterController->getAction()->getCharacter()->getY();
	int characterX2 = characterX1 + characterController->getAction()->getCharacter()->getWide();
	int characterY2 = characterY1 + characterController->getAction()->getCharacter()->getHeight();

	// �����蔻��
	if (characterX2 > m_x1 && characterX1 < m_x2 && characterY2 > m_y1 && characterY1 < m_y2 && characterController->getAction()->ableDamage()) {
		// �ђʒe����Ȃ��Ȃ����
		m_deleteFlag = true;
		characterController->damage(m_vx / 2, m_vy / 2, m_damage);
		return true;
	}
	return false;
}

// ���U���I�u�W�F�N�g�Ƃ̓����蔻��
bool BulletObject::atariObject(Object* object) {
	// �ǂ��炩���j��s�\�I�u�W�F�N�g
	if (!object->getAbleDelete() || !getAbleDelete() || m_groupId == object->getGroupId()) { 
		return false;
	}
	// �������Ă���Ȃ�
	if (m_x2 > object->getX1() && m_x1 < object->getX2() && m_y2 > object->getY1() && m_y1 < object->getY2()) {
		object->decreaseHp(m_damage);
		decreaseHp(object->getDamage());
		return true;
	}
	return false;
}

void BulletObject::action() {
	if (m_damageCnt > 0) { m_damageCnt--; }
	m_x1 += m_vx;
	m_x2 += m_vx;
	m_y1 += m_vy;
	m_y2 += m_vy;
	m_d -= m_v;
	// �򋗗������������
	if (m_d <= 0) {
		m_deleteFlag = true;
	}
}


ParabolaBullet::ParabolaBullet(int x, int y, GraphHandle* handle, int gx, int gy, AttackInfo* attackInfo):
	BulletObject(x, y, -1, gx, gy, attackInfo)
{
	m_handle = handle;
	// �U���͈͂ɍ��킹�ĉ摜�̊g�嗦��ݒ�
	int attackSize = max(attackInfo->bulletRx(), attackInfo->bulletRy());
	int graphX = 0, graphY = 0;
	GetGraphSize(handle->getHandle(), &graphX, &graphY);
	int graphSize = min(graphX, graphY);
	m_handle->setEx((double)attackSize / (double)graphSize);
}
ParabolaBullet::ParabolaBullet(int x, int y, GraphHandle* handle, int gx, int gy) :
	BulletObject(x, y, -1, gx, gy)
{
	m_handle = handle;
}

void ParabolaBullet::action() {
	if (m_damageCnt > 0) { m_damageCnt--; }
	m_x1 += m_vx;
	m_x2 += m_vx;
	m_vy += G;
	m_y1 += m_vy;
	m_y2 += m_vy;
}

// �摜�n���h����Ԃ�
GraphHandle* ParabolaBullet::getHandle() const { 
	double r = atan2((double)m_vy, (double)m_vx);
	if (m_vy == 0) { r = 0; }
	m_handle->setAngle(r);
	return m_handle;
}


SlashObject::SlashObject(int x1, int y1, int x2, int y2, GraphHandle* handle, int slashCountSum, AttackInfo* attackInfo) :
	Object(x1, y1, x2, y2, attackInfo->slashHp())
{
	// �K�v�Ȃ�ォ��Z�b�^�Őݒ�
	m_characterId = -1;
	m_groupId = -1;

	// �摜
	m_handle = handle;
	
	// �_���[�W
	m_damage = attackInfo->slashDamage();

	// �S�̃t���[��
	m_slashCountSum = slashCountSum;

	// �J�E���g
	m_cnt = 0;

	// �������(X�����̏���)
	m_slashImpactX = attackInfo->slashImpactX();

	// �������(Y�����̏���)
	m_slashImpactY = attackInfo->slashImpactY();


	// �G�t�F�N�g�̉摜
	m_effectHandles_p = attackInfo->slashEffectHandle();

	// �T�E���h
	m_soundHandle_p = attackInfo->slashSoundHandle();
}

SlashObject::SlashObject(int x1, int y1, int x2, int y2, GraphHandle* handle, int slashCountSum) :
	Object(x1, y1, x2, y2, 0)
{
	m_characterId = -1;
	m_groupId = -1;
	m_handle = handle;
	m_damage = 0;
	m_slashCountSum = slashCountSum;
	m_cnt = 0;
	m_slashImpactX = 0;
	m_slashImpactY = 0;
	m_effectHandles_p = NULL;
	m_soundHandle_p = -1;
}

// �傫�����w�肵�Ȃ��ꍇ�B�摜����T�C�Y�擾�B�������ԁAAttackInfo
SlashObject::SlashObject(int x, int y, GraphHandle* handle, int slashCountSum, AttackInfo* attackInfo) {
	int x2 = 0;
	int y2 = 0;
	GetGraphSize(handle->getHandle(), &x2, &y2);
	x2 += x;
	y2 = y;
	m_hp = attackInfo->slashHp();
	SlashObject(x, y, x2, y2, handle, slashCountSum, attackInfo);
}

// �L�����Ƃ̓����蔻��
// �������Ă���Ȃ�L�����𑀍삷��B
bool SlashObject::atari(CharacterController* characterController) {
	// ���Ŗh�~
	if (m_characterId == characterController->getAction()->getCharacter()->getId()) {
		return false;
	}
	// �`�[���L���h�~
	int groupId = characterController->getAction()->getCharacter()->getGroupId();
	if (m_groupId == groupId || groupId == -1) {
		return false;
	}

	// �L�����̏��@���W�ƈړ��X�s�[�h
	int characterX1 = characterController->getAction()->getCharacter()->getX();
	int characterY1 = characterController->getAction()->getCharacter()->getY();
	int characterX2 = characterX1 + characterController->getAction()->getCharacter()->getWide();
	int characterY2 = characterY1 + characterController->getAction()->getCharacter()->getHeight();

	// �����蔻��
	if (characterX2 > m_x1 && characterX1 < m_x2 && characterY2 > m_y1 && characterY1 < m_y2 && characterController->getAction()->ableDamage()) {
		// �ђʒe����Ȃ��Ȃ����
		 // m_deleteFlag = true;
		if (characterX1 + characterX2 < m_x1 + m_x2) {
			characterController->damage(-m_slashImpactX, -m_slashImpactY, m_damage);
		}
		else {
			characterController->damage(m_slashImpactX, -m_slashImpactY, m_damage);
		}
		return true;
	}
	return false;
}

// ���U���I�u�W�F�N�g�Ƃ̓����蔻��
bool SlashObject::atariObject(Object* object) {
	// �ǂ��炩���j��s�\�I�u�W�F�N�g
	if (!object->getAbleDelete() || !getAbleDelete() || m_groupId == object->getGroupId()) {
		return false;
	}
	// �������Ă���Ȃ�
	if (m_x2 > object->getX1() && m_x1 < object->getX2() && m_y2 > object->getY1() && m_y1 < object->getY2()) {
		object->decreaseHp(m_damage);
		decreaseHp(object->getDamage());
		return true;
	}
	return false;
}

void SlashObject::action() {
	if (m_damageCnt > 0) { m_damageCnt--; }
	// ���Ԍo��
	m_cnt++;

	// ���Ԃ������̂ŏ���
	if (m_cnt == m_slashCountSum) {
		m_deleteFlag = true;
	}
}

DoorObject::DoorObject(int x1, int y1, int x2, int y2, const char* fileName, int areaNum) :
	Object(x1, y1, x2, y2)
{
	m_fileName = fileName;
	m_graph = new GraphHandle(fileName, 1.0, 0.0, true);
	m_areaNum = areaNum;
	m_text = "";
}

DoorObject::~DoorObject() {
	delete m_graph;
}

bool DoorObject::atari(CharacterController* characterController) {
	if (!characterController->getAction()->ableDamage() || !characterController->getAction()->getGrand()) {
		m_text = "";
		return false;
	}
	// �L�����̏��@���W�ƈړ��X�s�[�h
	int characterX1 = characterController->getAction()->getCharacter()->getX();
	int characterY1 = characterController->getAction()->getCharacter()->getY();
	int characterX2 = characterX1 + characterController->getAction()->getCharacter()->getWide();
	int characterY2 = characterY1 + characterController->getAction()->getCharacter()->getHeight();

	// �����蔻��
	if (characterX2 > m_x1 && characterX1 < m_x2 && characterY2 > m_y1 && characterY1 < m_y2) {
		m_text = "�v�L�[�œ���";
		return true;
	}
	m_text = "";
	return false;
}


// �R�s�[�쐬
Object* BoxObject::createCopy() {
	Object* res = new BoxObject(m_x1, m_y1, m_x2, m_y2, m_color, m_hp);
	setParam(res);
	return res;
}
Object* TriangleObject::createCopy() {
	Object* res = new TriangleObject(m_x1, m_y1, m_x2, m_y2, m_color, m_leftDown, m_hp);
	setParam(res);
	return res;
}
Object* BulletObject::createCopy() {
	BulletObject* res = new BulletObject(m_x1, m_y1, m_color, m_gx, m_gy);
	setParam(res);
	setBulletParam(res);
	return res;
}
void BulletObject::setBulletParam(BulletObject* object) {
	object->setCharacterId(m_characterId);
	object->setGroupId(m_groupId);
	object->setColor(m_color);
	object->setRx(m_rx);
	object->setRy(m_ry);
	object->setV(m_v);
	object->setVx(m_vx);
	object->setVy(m_vy);
	object->setGx(m_gx);
	object->setGy(m_gy);
	object->setD(m_d);
	object->setDamage(m_damage);
	object->setEffectHandles(m_effectHandles_p);
}
Object* ParabolaBullet::createCopy() {
	ParabolaBullet* res = new ParabolaBullet(m_x1, m_y1, m_handle, m_gx, m_gy);
	setParam(res);
	setBulletParam(res);
	res->setGraphHandle(m_handle);
	return res;
}
Object* SlashObject::createCopy() {
	SlashObject* res = new SlashObject(m_x1, m_y1, m_x2, m_y2, m_handle, m_slashCountSum);
	setParam(res);
	setSlashParam(res);
	return res;
}
void SlashObject::setSlashParam(SlashObject* object) {
	object->setCharacterId(m_characterId);
	object->setGroupId(m_groupId);
	object->setDamage(m_damage);
	object->setGraphHandle(m_handle);
	object->setCnt(m_cnt);
	object->setSlashImpactX(m_slashImpactX);
	object->setSlashImpactY(m_slashImpactY);
	object->setEffectHandles(m_effectHandles_p);
}
Object* DoorObject::createCopy() {
	DoorObject* res = new DoorObject(m_x1, m_y1, m_x2, m_y2, m_fileName, m_areaNum);
	setParam(res);
	res->setText(m_text);
	return res;
}


// �`��p
// �I�u�W�F�N�g�`��i�摜���Ȃ��Ƃ��Ɏg���j
void BoxObject::drawObject(int x1, int y1, int x2, int y2) const {
	if (m_damageCnt > 0) {
		DrawBox(x1, y1, x2, y2, RED, TRUE);
	}
	else {
		DrawBox(x1, y1, x2, y2, m_color, TRUE);
	}
}
// �I�u�W�F�N�g�`��i�摜���Ȃ��Ƃ��Ɏg���j
void TriangleObject::drawObject(int x1, int y1, int x2, int y2) const {
	if (m_leftDown) {
		if (m_damageCnt > 0) {
			DrawTriangle(x2, y1, x2, y2, x1, y2, RED, TRUE);
		}
		else {
			DrawTriangle(x2, y1, x2, y2, x1, y2, m_color, TRUE);
		}
	}
	else {
		if (m_damageCnt > 0) {
			DrawTriangle(x1, y1, x2, y2, x1, y2, RED, TRUE);
		}
		else {
			DrawTriangle(x1, y1, x2, y2, x1, y2, m_color, TRUE);
		}
	}
}
// �I�u�W�F�N�g�`��i�摜���Ȃ��Ƃ��Ɏg���j
void BulletObject::drawObject(int x1, int y1, int x2, int y2) const {
	int rx = abs(x2 - x1) / 2;
	int ry = abs(y2 - y1) / 2;
	if (m_damageCnt > 0) {
		DrawOval(x1 + rx, y1 + ry, rx, ry, RED, TRUE);
	}
	else {
		DrawOval(x1 + rx, y1 + ry, rx, ry, m_color, TRUE);
	}
}