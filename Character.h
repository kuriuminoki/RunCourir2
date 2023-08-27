#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED


#include<string>

class Object;
class GraphHandle;
class GraphHandles;
class CharacterGraphHandle;
class FaceGraphHandle;
class SoundPlayer;
class RunnerGraphHandle;


class CharacterInfo {
private:
	// �L�����̖��O
	std::string m_name;

	// �L�����̏����̗�
	int m_maxHp;

	// �摜�̊g�嗦
	double m_handleEx;

	// ����X�s�[�h
	int m_moveSpeed;

	// �W�����v����Y�����̏���
	int m_jumpHeight;

	// �W�����v���̉�
	int m_jumpSound;

	// �󂯐g���̉�
	int m_passiveSound;

	// ���n���̉�
	int m_landSound;

public:
	// �f�t�H���g�l�ŏ�����
	CharacterInfo();
	// csv�t�@�C����ǂݍ��݁A�L�������Ō������p�����[�^�擾
	CharacterInfo(const char* characterName);

	~CharacterInfo();

	// �Q�b�^�݂̂�����
	inline std::string name() const { return m_name; }
	inline int maxHp() const { return m_maxHp; }
	inline double handleEx() const { return m_handleEx; }
	inline int moveSpeed() const { return m_moveSpeed; }
	inline int jumpHeight() const { return m_jumpHeight; }
	inline int jumpSound() const { return m_jumpSound; }
	inline int passiveSound() const { return m_passiveSound; }
	inline int landSound() const { return m_landSound; }
};


class AttackInfo {
private:
	// �e�ۂ�HP
	int m_bulletHp;

	// �e�ۂ̃_���[�W
	int m_bulletDamage;

	// �e�ۂ̑傫��(���a)
	int m_bulletRx, m_bulletRy;

	// �e�ۂ̃X�s�[�h
	int m_bulletSpeed;

	// �e�ۂ̘A�˗�
	int m_bulletRapid;

	// �e�ۂ̔򋗗�
	int m_bulletDistance;

	// �e�ۂ̐������(X�����̏���)
	int m_bulletImpactX;

	// �e�ۂ̐������(Y�����̏���)
	int m_bulletImpactY;

	// �a����HP
	int m_slashHp;

	// �a���̃_���[�W
	int m_slashDamage;

	// �a���̑傫��(�����`�̕ӂ̒���)
	int m_slashLenX, m_slashLenY;

	// �a���̑S�̃t���[��
	int m_slashCountSum;

	// �a���̌㌄
	int m_slashInterval;

	// �a���̐������(X�����̏���)
	int m_slashImpactX;

	// �a���̐������(Y�����̏���)
	int m_slashImpactY;

	// �ˌ��U���̃G�t�F�N�g
	GraphHandles* m_bulletEffectHandles;

	// �a���U���̃G�t�F�N�g
	GraphHandles* m_slashEffectHandles;

	// �ˌ��U�������������Ƃ��̃T�E���h
	int m_bulletSoundHandle;

	// �e���ˉ�
	int m_bulletStartSoundHandle;

	// �a���U�������������Ƃ��̃T�E���h
	int m_slashSoundHandle;

	// �a���J�n�T�E���h
	int m_slashStartSoundHandle;

public:
	// �f�t�H���g�l�ŏ�����
	AttackInfo();
	// csv�t�@�C����ǂݍ��݁A�L�������Ō������p�����[�^�擾
	AttackInfo(const char* characterName, double drawEx);

	~AttackInfo();
	
	// �Q�b�^�݂̂�����
	int bulletHp() const { return m_bulletHp; }
	int bulletDamage() const { return m_bulletDamage; }
	int bulletRx() const { return m_bulletRx; }
	int bulletRy() const { return m_bulletRy; }
	int bulletSpeed() const { return m_bulletSpeed; }
	int bulletRapid() { return m_bulletRapid; }
	int bulletDistance() const { return m_bulletDistance; }
	int bulletImpactX() const { return m_bulletImpactX; }
	int bulletImpactY() const { return m_bulletImpactY; }
	int slashHp() const { return m_slashHp; }
	int slashDamage() const { return m_slashDamage; }
	int slashLenX() const { return m_slashLenX; }
	int slashLenY() const { return m_slashLenY; }
	int slashCountSum() const { return m_slashCountSum; }
	int slashInterval() const { return m_slashInterval; }
	int slashImpactX() const { return m_slashImpactX; }
	int slashImpactY() const { return m_slashImpactY; }
	GraphHandles* bulletEffectHandle() const { return m_bulletEffectHandles; }
	GraphHandles* slashEffectHandle() const { return m_slashEffectHandles; }
	int bulletSoundeHandle() const { return m_bulletSoundHandle; }
	int slashSoundHandle() const { return m_slashSoundHandle; }
	int bulletStartSoundeHandle() const { return m_bulletStartSoundHandle; }
	int slashStartSoundHandle() const { return m_slashStartSoundHandle; }
};


/*
* �v���C���[��G�l�~�[�̊��N���X
*/
class Character {
protected:
	static int characterId;

	bool m_duplicationFlag;

	// ID
	int m_id;

	// �O���[�vID �������ʗp
	int m_groupId;

	// �c��̗�
	int m_hp;

	// X���W�AY���W
	int m_x, m_y;

	// ���������Ă���
	bool m_leftDirection;

	// �L�����̏��
	CharacterInfo* m_characterInfo;

	// �U���̏��
	AttackInfo* m_attackInfo;

	// �L�����摜
	CharacterGraphHandle* m_graphHandle;

	// ��摜
	FaceGraphHandle* m_faceHandle;

public:
	// �R���X�g���N�^
	Character();
	Character(int hp, int x, int y, int groupId);
	~Character();

	virtual Character* createCopy() = 0;

	// �f�o�b�O
	void debugCharacter(int x, int y, int color) const;
	virtual void debug(int x, int y, int color) const = 0;

	// �Q�b�^
	inline int getId() const { return m_id; }
	inline int getGroupId() const { return m_groupId; }
	inline int getHp() const { return m_hp; }
	inline int getX() const { return m_x; }
	inline int getY() const { return m_y; }
	inline bool getLeftDirection() const { return m_leftDirection; }
	FaceGraphHandle* getFaceHandle() const { return m_faceHandle; }
	inline CharacterGraphHandle* getCharacterGraphHandle() const { return m_graphHandle; }
	inline AttackInfo* getAttackInfo() const { return m_attackInfo; }
	inline CharacterInfo* getCharacterInfo() const { return m_characterInfo; }

	// Runner�p�̉摜
	virtual RunnerGraphHandle* getRunnerGraphHandle() { return nullptr; }

	// �Z�b�^
	inline void setHp(int hp) { m_hp = (hp > m_characterInfo->maxHp()) ? m_characterInfo->maxHp() : hp; }
	inline void setX(int x) { m_x = x; }
	inline void setY(int y) { m_y = y; }
	inline void setId(int id) { m_id = id; }
	inline void setGroupId(int id) { m_groupId = id; }
	// �L�����̌����ύX�́A�摜�̔��]���s��
	void setLeftDirection(bool leftDirection);
	inline void setDuplicationFlag(bool flag) { m_duplicationFlag = flag; }
	inline void setAttackInfo(AttackInfo* attackInfo) { m_attackInfo = attackInfo; }
	inline void setCharacterInfo(CharacterInfo* characterInfo) { m_characterInfo = characterInfo; }

	// CharacterInfo����L�����̃X�y�b�N���擾
	inline std::string getName() const { return m_characterInfo->name(); }
	inline int getMaxHp() const { return m_characterInfo->maxHp(); }
	inline int getMoveSpeed() const { return m_characterInfo->moveSpeed(); }
	inline int getJumpHeight() const { return m_characterInfo->jumpHeight(); }
	inline int getJumpSound() const { return m_characterInfo->jumpSound(); }
	inline int getPassiveSound() const { return m_characterInfo->passiveSound(); }
	inline int getLandSound() const { return m_characterInfo->landSound(); }

	// AttackInfo����U���̃X�y�b�N���擾
	inline int getBulletRapid() const { return m_attackInfo->bulletRapid(); }
	inline int getSlashCountSum() const { return m_attackInfo->slashCountSum(); }
	inline int getSlashInterval() const { return m_attackInfo->slashInterval(); }

	// �摜�̏����擾
	int getCenterX() const;
	int getCenterY() const;
	int getWide() const;
	int getHeight() const;
	// ���`�悷��摜���擾
	GraphHandle* getGraphHandle() const;
	void getHandleSize(int& wide, int& height) const;

	// �����摜���Z�b�g
	virtual void switchStand(int cnt = 0);
	// �����ˌ��摜���Z�b�g
	virtual void switchBullet(int cnt = 0);
	// �����a���摜���Z�b�g
	virtual void switchSlash(int cnt = 0);
	// ���Ⴊ�݉摜���Z�b�g
	virtual void switchSquat(int cnt = 0);
	// ���Ⴊ�ݎˌ��摜���Z�b�g
	virtual void switchSquatBullet(int cnt = 0);
	// ����摜���Z�b�g
	virtual void switchRun(int cnt = 0);
	// ����ˌ��摜���Z�b�g
	virtual void switchRunBullet(int cnt = 0);
	// ���n�摜���Z�b�g
	virtual void switchLand(int cnt = 0);
	// �㏸�摜���Z�b�g
	virtual void switchJump(int cnt = 0);
	// �~���摜���Z�b�g
	virtual void switchDown(int cnt = 0);
	// �W�����v�O�摜���Z�b�g
	virtual void switchPreJump(int cnt = 0);
	// �_���[�W�摜���Z�b�g
	virtual void switchDamage(int cnt = 0);
	// �u�[�X�g�摜���Z�b�g
	virtual void switchBoost(int cnt = 0);
	// �󒆎ˌ��摜���Z�b�g
	virtual void switchAirBullet(int cnt = 0);
	// �󒆎a���摜���Z�b�g
	virtual void switchAirSlash(int cnt = 0);

	// HP����
	void damageHp(int value);

	// �ړ�����i���W�𓮂����j
	void moveRight(int d);
	void moveLeft(int d);
	void moveUp(int d);
	void moveDown(int d);

	// �ˌ��U��������(�L�������ƂɈႤ)
	virtual Object* bulletAttack(int gx, int gy, SoundPlayer* soundPlayer) { return NULL; }

	// �a���U��������(�L�������ƂɈႤ) ���������Ă��邩�A�����J�E���g��
	virtual Object* slashAttack(bool leftDirection, int cnt, SoundPlayer* soundPlayer) { return NULL; }
};


/*
* �n�[�g�i��l���j
*/
class Heart :
	public Character
{
private:
	//// ����A�j���̃X�s�[�h
	const int RUN_ANIME_SPEED = 6;
	
	//// �W�����v�O�A�j���̃X�s�[�h
	const int RUN_PREJUMP_SPEED = 6;
	
public:
	// �R���X�g���N�^
	Heart(const char* name, int hp, int x, int y, int groupId);
	Heart(const char* name, int hp, int x, int y, int groupId, AttackInfo* attackInfo);

	// �f�X�g���N�^
	~Heart();

	Character* createCopy();

	// �f�o�b�O
	void debug(int x, int y, int color) const;

	// �摜�ύX�֐��̃I�[�o�[���C�h
	// ����摜���Z�b�g
	void switchRun(int cnt = 0);

	// ����ˌ��摜���Z�b�g
	void switchRunBullet(int cnt = 0);

	// �W�����v�O�摜���Z�b�g
	void switchPreJump(int cnt = 0);

	// �ˌ��U��������(�L�������ƂɈႤ)
	Object* bulletAttack(int gx, int gy, SoundPlayer* soundPlayer);

	// �a���U��������(�L�������ƂɈႤ)
	Object* slashAttack(bool leftDirection, int cnt, SoundPlayer* soundPlayer);
};


/*
* �V�G�X�^
*/
class Siesta :
	public Heart
{
public:
	// �R���X�g���N�^
	Siesta(const char* name, int hp, int x, int y, int groupId);
	Siesta(const char* name, int hp, int x, int y, int groupId, AttackInfo* attackInfo);

	Character* createCopy();

	// �ˌ��U��������(�L�������ƂɈႤ)
	Object* bulletAttack(int gx, int gy, SoundPlayer* soundPlayer);

	// �a���U��������(�L�������ƂɈႤ)
	Object* slashAttack(bool leftDirection, int cnt, SoundPlayer* soundPlayer);
};


/*
* �N�[���[��
*/
class Courir :
	public Character
{
private:
	//// ����A�j���̃X�s�[�h
	const int RUN_ANIME_SPEED = 6;

	// Runner�p�̉摜
	RunnerGraphHandle* m_runnerGraphHandle;

public:
	// �R���X�g���N�^
	Courir(const char* name, int hp, int x, int y, int groupId);

	~Courir();

	Character* createCopy();

	// �f�o�b�O
	void debug(int x, int y, int color) const;

	// �摜�ύX�֐��̃I�[�o�[���C�h
	// ����摜���Z�b�g
	void switchRun(int cnt = 0);

	// Runner�p�̉摜
	RunnerGraphHandle* getRunnerGraphHandle() { return m_runnerGraphHandle; }
};


#endif