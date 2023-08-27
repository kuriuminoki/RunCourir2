#ifndef CHACACTER_CONTROLLER_H_INCLUDED
#define CHACACTER_CONTROLLER_H_INCLUDED


#include <vector>


class Character;
class CharacterAction;
class Object;
class Camera;
class Brain;
class ControllerRecorder;


/*
* �R���g���[���̊��N���X�i�L�����N�^�[�𑀍삷��N���X�j
*/
class CharacterController {
protected:
	// �����Ȃ�true Recorder���f���[�g���Ȃ�����
	bool m_duplicationFlag;

	// ����������𖽗߂��Ă��� Controller���f���[�g����
	Brain* m_brain;

	// ����Ώ� Controller���f���[�g����
	CharacterAction* m_characterAction;

	// ����̋L�^ �g��Ȃ��Ȃ�NULL
	ControllerRecorder* m_stickRecorder;
	ControllerRecorder* m_jumpRecorder;
	ControllerRecorder* m_squatRecorder;
	ControllerRecorder* m_slashRecorder;
	ControllerRecorder* m_bulletRecorder;

	// �_���[�W�̋L�^ �ω������炻��ȍ~�̃��R�[�h���폜����
	ControllerRecorder* m_damageRecorder;

public:
	CharacterController();
	CharacterController(Brain* brain, CharacterAction* characterAction);
	~CharacterController();

	virtual CharacterController* createCopy(std::vector<Character*> characters, const Camera* camera) = 0;

	// �f�o�b�O
	void debugController(int x, int y, int color) const;
	virtual void debug(int x, int y, int color) const = 0;

	// �Q�b�^
	inline const CharacterAction* getAction() const { return m_characterAction; }
	inline const Brain* getBrain() const { return m_brain; }
	inline const ControllerRecorder* getStickRecorder() const { return m_stickRecorder; }
	inline const ControllerRecorder* getJumpRecorder() const { return m_jumpRecorder; }
	inline const ControllerRecorder* getSquatRecorder() const { return m_squatRecorder; }
	inline const ControllerRecorder* getSlashRecorder() const { return m_slashRecorder; }
	inline const ControllerRecorder* getBulletRecorder() const { return m_bulletRecorder; }
	inline const ControllerRecorder* getDamageRecorder() const { return m_damageRecorder; }

	// �Z�b�^
	void setAction(CharacterAction* action);
	void setBrain(Brain* brain);
	void setStickRecorder(ControllerRecorder* recorder);
	void setJumpRecorder(ControllerRecorder* recorder);
	void setSquatRecorder(ControllerRecorder* recorder);
	void setSlashRecorder(ControllerRecorder* recorder);
	void setBulletRecorder(ControllerRecorder* recorder);
	void setDamageRecorder(ControllerRecorder* recorder);
	void setTarget(Character* character);
	void setDuplicationFlag(bool flag) { m_duplicationFlag = flag; }

	// ���R�[�_��������
	void initRecorder();
	// ���R�[�h����߂�
	void eraseRecorder();

	// �b������������ɓ������肷��{�^����true��
	virtual bool getActionKey() const;

	// �A�N�V�����̃Z�b�^
	void setCharacterGrand(bool grand);
	void setCharacterGrandRightSlope(bool grand);
	void setCharacterGrandLeftSlope(bool grand);
	void setActionRightLock(bool lock);
	void setActionLeftLock(bool lock);
	void setActionUpLock(bool lock);
	void setActionDownLock(bool lock);
	void setActionBoost();

	// �L�����N�^�[�̃Z�b�^
	void setCharacterX(int x);
	void setCharacterY(int y);

	// �s���O�̏��� ���t���[���s��
	void init();

	// �U���Ώۂ�ύX
	void searchTargetCandidate(Character* character);

	// �ǐՑΏۂ�ύX
	void searchFollowCandidate(Character* character);

	// ����ⓖ���蔻��̌��ʂ𔽉f�i���ۂɃL�����𓮂����j���t���[���s��
	void action();

	// �L�����̑���
	virtual void control() = 0;

	// �ˌ��U��
	virtual Object* bulletAttack() = 0;

	// �a���U��
	virtual Object* slashAttack() = 0;

	// �_���[�W
	virtual void damage(int vx, int vy, int damageValue) = 0;
};

/*
* ���ʂ̃R���g���[��
*/
class NormalController :
	public CharacterController {

private:
	// �W�����v�L�[�𒷉�������ő厞��
	const int JUMP_KEY_LONG = 10;
public:
	NormalController(Brain* brain, CharacterAction* characterAction);

	CharacterController* createCopy(std::vector<Character*> characters, const Camera* camera);

	void debug(int x, int y, int color) const;

	// �L�����̈ړ��₵�Ⴊ��(;�U���ȊO��)���� ���t���[���s��
	void control();

	// �ˌ��U��
	Object* bulletAttack();

	// �a���U��
	Object* slashAttack();

	// �_���\�W
	void damage(int vx, int vy, int damageValue);
};

#endif
