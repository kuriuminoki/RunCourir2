#ifndef CHACACTER_ACTION_H_INCLUDED
#define CHACACTER_ACTION_H_INCLUDED


#include <vector>


class Character;
class Object;
class SoundPlayer;


// �L�����N�^�[�̏��
enum class CHARACTER_STATE {
	STAND,	// �������Ă��Ȃ�
	DAMAGE,	// �_���[�W�󂯒� ���n�ŉ���
	BULLET,	// �ˌ���
	SLASH,	// �a����
	PREJUMP,	// �W�����v�O
};


/*
* �L�����N�^�[�𓮂����N���X
* Controller���g�p����B����ȊO�̃C���X�^���X����g���邱�Ƃ͂Ȃ��B
*/
class CharacterAction {
protected:
	// �T�E���h�v���C���[
	SoundPlayer* m_soundPlayer_p;

	// �L�����̏��
	CHARACTER_STATE m_state;

	// �������L�����N�^�[
	Character* m_character_p;

	// �L�������n�ʂɂ���
	bool m_grand;

	// �E��������̍�ɂ���
	bool m_grandRightSlope;
	
	// ����������̍�ɂ���
	bool m_grandLeftSlope;

	// �L�����������Ă��Ȃ��Ȃ�-1 �����łȂ��Ȃ瑖�����t���[����
	int m_runCnt;

	// ���Ⴊ�ݒ�
	bool m_squat;

	// �W�����v�O�̓���
	int m_preJumpCnt;

	// �W�����v�̂��ߎ��Ԃ̍ő�
	const int PRE_JUMP_MAX = 10;

	// ���n���[�V�����̎c�莞��
	int m_landCnt;

	// ���n���[�V�����̑�����
	const int LAND_TIME = 10;

	// �u�[�X�g�A�j���̎c�莞�� �܂��͎󂯐g���
	int m_boostCnt;
	const int BOOST_TIME = 10;

	// �ړ���
	bool m_moveRight;
	bool m_moveLeft;
	bool m_moveUp;
	bool m_moveDown;

	// �����x
	int m_vx;
	int m_vy;

	// �ړ��̃��b�N�i�I�u�W�F�N�g���œ����Ȃ�������true�j
	bool m_rightLock;
	bool m_leftLock;
	bool m_upLock;
	bool m_downLock;

	// �ˌ��p�J�E���g
	int m_bulletCnt;

	// �a���p�J�E���g
	int m_slashCnt;

	// �U���������
	bool m_attackLeftDirection;

	int m_damageCnt;

public:
	CharacterAction();
	CharacterAction(Character* character, SoundPlayer* soundPlayer_p);

	virtual CharacterAction* createCopy(std::vector<Character*> characters) = 0;
	void setParam(CharacterAction* action);

	// �f�o�b�O
	void debugAction(int x, int y, int color) const;
	virtual void debug(int x, int y, int color) const = 0;

	// �Q�b�^
	inline const Character* getCharacter() const { return m_character_p; }
	inline CHARACTER_STATE getState() const { return m_state; }
	inline bool getGrand() const { return m_grand; }
	inline bool getGrandRightSlope() const { return m_grandRightSlope; }
	inline bool getGrandLeftSlope() const { return m_grandLeftSlope; }
	inline int getVx() const { return m_vx; }
	inline int getVy() const { return m_vy; }
	inline int getSlashCnt() const { return m_slashCnt; }
	inline int getBulletCnt() const { return m_bulletCnt; }
	bool getRightLock() const { return m_rightLock; }
	bool getLeftLock() const { return m_leftLock; }
	bool getUpLock() const { return m_upLock; }
	bool getDownLock() const { return m_downLock; }

	// �Z�b�^
	void setState(CHARACTER_STATE state);
	inline void setSimpleGrand(bool grand) { m_grand = grand; }
	void setGrand(bool grand);
	void setRightLock(bool lock);
	void setLeftLock(bool lock);
	void setUpLock(bool lock);
	void setDownLock(bool lock);
	inline void setBoost() { if(!m_grand) m_boostCnt = BOOST_TIME; }
	inline void setGrandRightSlope(bool grand) { m_grandRightSlope = grand; }
	inline void setGrandLeftSlope(bool grand) { m_grandLeftSlope = grand; }
	void setRunCnt(int runCnt) { m_runCnt = runCnt; }
	void setJumpCnt(int preJumpCnt) { m_preJumpCnt = preJumpCnt; }
	void setMoveRight(bool moveRight) { m_moveRight = moveRight; }
	void setMoveLeft(bool moveLeft) { m_moveLeft = moveLeft; }
	void setMoveUp(bool moveUp) { m_moveUp = moveUp; }
	void setMoveDown(bool moveDown) { m_moveDown = moveDown; }
	void setVx(int vx) { m_vx = vx; }
	void setVy(int vy) { m_vy = vy; }
	void setBulletCnt(int bulletCnt) { m_bulletCnt = bulletCnt; }
	void setSlashCnt(int slashCnt) { m_slashCnt = slashCnt; }
	void setAttackLeftDirection(bool attackLeftDirection) { m_attackLeftDirection = attackLeftDirection; }
	void setLandCnt(int landCnt) { m_landCnt = landCnt; }
	void setBoostCnt(int boostCnt) { m_boostCnt = boostCnt; }
	void setDamageCnt(int damageCnt) { m_damageCnt = damageCnt; }

	// ���_���[�W���󂯂Ă��ē����Ȃ�
	inline bool damageFlag() const { return m_state == CHARACTER_STATE::DAMAGE; }

	// squat==true�Ȃ炵�Ⴊ�ށAfalse�Ȃ痧��
	void setSquat(bool squat);

	// �L�����N�^�[�̃Z�b�^
	void setCharacterX(int x);
	void setCharacterY(int y);
	void setCharacterLeftDirection(bool leftDirection);

	// �s���O�̏��� ���t���[���s��
	virtual void init() = 0;

	// �������Z ���t���[���s��
	virtual void action() = 0;

	// �L�����̉摜��ύX
	virtual void switchHandle() = 0;

	// �ړ� �����͂S������ �L�����ɂ���Ă͎΂߂Ɉړ��ł��邽�߁B
	virtual void move(bool right, bool left, bool up, bool down) = 0;

	// �W�����v rate%�̗͂Ŕ�яオ��B
	virtual void jump(int rate) = 0;

	// �ˌ��U��
	virtual Object* bulletAttack(int gx, int gy) = 0;

	// �a���U��
	virtual Object* slashAttack(int gx, int gy) = 0;

	// �_���[�W
	virtual void damage(int vx, int vy, int damageValue) = 0;

	// �����G���Ԃ���Ȃ�
	bool ableDamage() const;

	// ���U���\���
	bool ableAttack() const;

	// �����̂���߂�
	void stopMoveLeft();
	void stopMoveRight();

protected:
	// �摜�̃T�C�Y�ύX�ɂ��ʒu����
	void afterChangeGraph(int beforeWide, int beforeHeight, int afterWide, int afterHeight);
};


/*
* ����΂Ȃ����ʂ̖_�l��
*/
class StickAction :
	public CharacterAction 
{
private:
	// �d�͉����x
	const int G = 1;

private:
	// ���֕��� �����͉E�ƍ��̂Q��
	void walk(bool right, bool left);

	// �L�����̉摜�����(state)�ɉ����ĕύX
	void switchHandle();

public:
	StickAction(Character* character, SoundPlayer* soundPlayer_p);

	CharacterAction* createCopy(std::vector<Character*> characters);

	void debug(int x, int y, int color) const;

	//�s���O�̏��� ���t���[���s��
	void init();

	// �������Z ���t���[���s��
	void action();

	// �ړ� �����͂S������
	void move(bool right, bool left, bool up, bool down);

	// �W�����v cnt�t���[����
	void jump(int cnt);

	// �ˌ��U��
	Object* bulletAttack(int gx, int gy);

	// �a���U��
	Object* slashAttack(int gx, int gy);

	// �_���[�W
	void damage(int vx, int vy, int damageValue);
};

class RunnerAction :
	public StickAction
{
public:

	RunnerAction(Character* character, SoundPlayer* soundPlayer_p);

	// �L�����̉摜�����(state)�ɉ����ĕύX
	void switchHandle();

private:

	// �ˌ��U��
	Object* bulletAttack(int gx, int gy) { return nullptr; }

	// �a���U��
	Object* slashAttack(int gx, int gy) { return nullptr; }
};

#endif