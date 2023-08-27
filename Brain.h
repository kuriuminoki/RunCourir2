#ifndef BRAIN_H_INCLUDED
#define BRAIN_H_INCLUDED

#include <vector>


class Character;
class CharacterAction;
class Camera;


// Controller�ɖ��߂���N���X�i�L�[�{�[�h���}�E�X��`�h�j
class Brain {
protected:
	// ���̃L�����̏�Ԃ��l�����Ďˌ��̖ڕW���W�⎟�̍s�������߂邽�ߕK�v
	const CharacterAction* m_characterAction_p;

public:
	Brain();

	virtual Brain* createCopy(std::vector<Character*> characters, const Camera* camera) = 0;

	virtual void debug(int x, int y, int color) const = 0;

	// �b�������������������
	virtual bool actionOrder() { return false; }

	// �Z�b�^
	virtual void setCharacterAction(const CharacterAction* characterAction) = 0;

	// �������U���̖ڕW���W
	virtual void bulletTargetPoint(int& x, int& y) = 0;

	// �ړ��i�㉺���E�̓��́j
	virtual void moveOrder(int& right, int& left, int& up, int& down) = 0;

	// �W�����v�̐���
	virtual int jumpOrder() = 0;

	// ���Ⴊ�݂̐���
	virtual int squatOrder() = 0;

	// �ߋ����U��
	virtual int slashOrder() = 0;

	// �������U��
	virtual int bulletOrder() = 0;

	// �U���Ώۂ����߂�(AI�N���X�ŃI�[�o���C�h����B)
	virtual void searchTarget(const Character* character) { }

	// �U���Ώۂ�ύX����K�v������Ȃ�true�ŃA�s�[������(AI�N���X�ŃI�[�o���C�h����)�B
	virtual bool needSearchTarget() const { return false; }

	// �ǐՑΏۂ����߂�(AI�N���X�ŃI�[�o���C�h����B)
	virtual void searchFollow(const Character* character) { }

	// �ǐՑΏۂ�ύX����K�v������Ȃ�true�ŃA�s�[������(AI�N���X�ŃI�[�o���C�h����)�B
	virtual bool needSearchFollow() const { return false; }

	virtual int getTargetId() const { return -1; }

	virtual void setTarget(Character* character) {  }
};


/*
* �L�[�{�[�h�ŃL�����̑���𖽗߂���N���X
*/
class KeyboardBrain :
	public Brain
{
private:
	// �J����
	const Camera* m_camera_p;

public:
	KeyboardBrain(const Camera* camera);
	Brain* createCopy(std::vector<Character*> characters, const Camera* camera) { return new KeyboardBrain(camera); }
	void debug(int x, int y, int color) const;
	inline void setCharacterAction(const CharacterAction* characterAction) { m_characterAction_p = characterAction; }
	void bulletTargetPoint(int& x, int& y);
	bool actionOrder();
	void moveOrder(int& right, int& left, int& up, int& down);
	int jumpOrder();
	int squatOrder();
	int slashOrder();
	int bulletOrder();
};


/*
* �S�������Ȃ�AI
*/
class Freeze :
	public Brain
{
public:
	Freeze() { }
	Brain* createCopy(std::vector<Character*> characters, const Camera* camera) { return new Freeze(); }
	void debug(int x, int y, int color) const { }
	bool actionOrder() { return false; }
	void setCharacterAction(const CharacterAction* characterAction) {  }
	void bulletTargetPoint(int& x, int& y) {  }
	void moveOrder(int& right, int& left, int& up, int& down) { right = 0; left = 0; up = 0; down = 0; }
	int jumpOrder() { return 0; }
	int squatOrder() { return 0; }
	int slashOrder() { return 0; }
	int bulletOrder() { return 0; }
};


/*
*  ���ʂɓG�Ɛ키�悤���߂���`�h�̃N���X
*/
class NormalAI :
	public Brain
{
private:

	// �U���Ώۂ�F�m���鋗��
	const int TARGET_DISTANCE = 2000;

	// �ړ��p
	int m_rightKey, m_leftKey, m_upKey, m_downKey;

	// �W�����v�̒���
	int m_jumpCnt;

	// ���Ⴊ�ޒ���
	int m_squatCnt;

protected:
	// �U���Ώ�
	const Character* m_target_p;

	// �ˌ��̐��x
	const int BULLET_ERROR = 400;

	// ���ɈӖ��̂Ȃ��ړ�������m��
	const int MOVE_RAND = 59;

	// �ړ��ڕW
	int m_gx, m_gy;

	// �ړ��ڕW�B���Ƃ݂Ȃ��덷 �}GX_ERROR
	const int GX_ERROR = 100;

	// �ړ�����
	int m_moveCnt;

	// �ړ�����߂�܂ł̎���
	const int GIVE_UP_MOVE_CNT = 300;

public:
	NormalAI();
	Brain* createCopy(std::vector<Character*> characters, const Camera* camera);
	void setParam(NormalAI* brain);

	void setRightKey(int rightKey) { m_rightKey = rightKey; }
	void setLeftKey(int leftKey) { m_leftKey = leftKey; }
	void setUpKey(int upKey) { m_upKey = upKey; }
	void setDownKey(int downKey) { m_downKey = downKey; }
	void setJumpCnt(int cnt) { m_jumpCnt = cnt; }
	void setSquatCnt(int cnt) { m_squatCnt = cnt; }
	void setGx(int gx) { m_gx = gx; }
	void setGy(int gy) { m_gy = gy; }
	void setMoveCnt(int cnt) { m_moveCnt = cnt; }
	void setTarget(Character* character) { m_target_p = character; }

	void debug(int x, int y, int color) const;
	void setCharacterAction(const CharacterAction* characterAction);
	void bulletTargetPoint(int& x, int& y);
	void moveOrder(int& right, int& left, int& up, int& down);
	int jumpOrder();
	int squatOrder();
	int slashOrder();
	int bulletOrder();

	// �U���Ώۂ����߂�(target�̂܂܂��Acharacter�ɕύX���邩)
	void searchTarget(const Character* character);

	// �U���Ώۂ�ύX����K�v������Ȃ�true�ŃA�s�[������B
	bool needSearchTarget() const;

	int getTargetId() const;

protected:
	// �X�e�B�b�N����
	void stickOrder(int& right, int& left, int& up, int& down);
};


/*
* ParabolaBullet���g��AI
*/
class ParabolaAI :
	public NormalAI
{
	void bulletTargetPoint(int& x, int& y);
};


class FollowNormalAI :
	public NormalAI
{
private:
	// ���Ă����L����
	const Character* m_follow_p;

	// �ǐՑΏۂ̋߂��ɂ���Ƃ݂Ȃ��덷 �}GX_ERROR
	const int FOLLOW_X_ERROR = 500;

public:
	FollowNormalAI();

	Brain* createCopy(std::vector<Character*> characters, const Camera* camera);

	void debug(int x, int y, int color) const;

	int getFollowId() const;

	void setFollow(Character* character) { m_follow_p = character; }

	// �ړ��̖ڕW�n�_�ݒ�
	void moveOrder(int& right, int& left, int& up, int& down);

	// �ǐՑΏۂ����߂�(AI�N���X�ŃI�[�o���C�h����B)
	void searchFollow(const Character* character);

	// �ǐՑΏۂ�ύX����K�v������Ȃ�true�ŃA�s�[������(AI�N���X�ŃI�[�o���C�h����)�B
	bool needSearchFollow() const;
};

/*
* ParabolaBullet���g��AI
*/
class FollowParabolaAI :
	public FollowNormalAI
{
	void bulletTargetPoint(int& x, int& y);
};


#endif