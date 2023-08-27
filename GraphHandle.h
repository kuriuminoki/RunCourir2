#ifndef GRAPH_HANDLE_H_INCLUDED
#define GRAPH_HANDLE_H_INCLUDED

/*
* �摜�f�[�^(�n���h���A�摜�ŗL�̊g�嗦�A����)���܂Ƃ߂Ĉ������߂̃N���X
*/
class GraphHandle {
private:
	// �摜�n���h��
	int m_handle;

	// (���̉摜�ŗL��)�g�嗦
	double m_ex;

	// ����
	double m_angle;

	// �����x��L���ɂ��邩
	bool m_trans;

	// ���]���邩
	bool m_reverseX;
	bool m_reverseY;

public:
	GraphHandle(const char* filePath, double ex = 1.0, double angle = 0.0, bool trans = false, bool reverseX = false, bool reverseY = false);
	~GraphHandle();

	// �Q�b�^
	inline int getHandle() const { return m_handle; }
	inline double getEx() const { return m_ex; }
	inline double getAngle() const { return m_angle; }

	// �Z�b�^
	inline void setEx(double ex) { m_ex = ex; }
	inline void setAngle(double angle) { m_angle = angle; }
	inline void setTrans(bool trans) { m_trans = trans; }
	inline void setReverseX(bool reverse) { m_reverseX = reverse; }
	inline void setReverseY(bool reverse) { m_reverseY = reverse; }

	// �`�悷��
	void draw(int x, int y, double ex = 1.0) const;

	void extendDraw(int x1, int y1, int x2, int y2) const ;
};


/*
* GraphHandle��z��Ƃ��Ă܂Ƃ߂Ĉ����N���X
*/
class GraphHandles {
private:
	GraphHandle** m_handles;
	int m_handleSum;

public:
	//  filePath�̖����ɂ�".png"�����Ȃ����ƁB
	GraphHandles(const char* filePath, int handleSum, double ex = 1.0, double angle = 0.0, bool trans = false, bool reverseX = false, bool reverseY = false);
	~GraphHandles();

	// �Q�b�^
	inline GraphHandle* getGraphHandle(int i = 0) const { return m_handles[i]; }
	inline int getHandle(int i) { return m_handles[i]->getHandle(); }
	inline int getSize() const { return m_handleSum; }

	// �Z�b�^
	void setEx(double ex);
	void setAngle(double angle);
	void setTrans(bool trans);
	void setReverseX(bool reverse);
	void setReverseY(bool reverse);

	// �`�悷��
	void draw(int x, int y, int index);
};


/*
* Runner�p�̃O���t
*/
class RunnerGraphHandle {
private:
	// �FRGB
	int m_hairRGB[3];
	int m_eyeRGB[3];

	// �����Ă���Ƃ��̊�
	GraphHandles* runAse;

	// �����Ԃ̊�
	GraphHandles* deadAse;

	// �������
	GraphHandles* stand;
	GraphHandles* standEye;
	GraphHandles* standHair;

	// run��� �ʏ�A���A���E
	GraphHandles* run[3];
	GraphHandles* runEye[3];

	// run��Ԃ̔�
	GraphHandles* runFrontHair;
	GraphHandles* runBackHair;

	// ������
	GraphHandles* dead;
	GraphHandles* deadEye;
	GraphHandles* deadHair;

	// �C�ƕ�
	GraphHandles* standKutu;
	GraphHandles* runKutu;
	GraphHandles* standHuku;
	GraphHandles* runHuku;

public:
	RunnerGraphHandle(const char* characterName, double drawEx, char kutu, char huku, int hairRGB[3], int eyeRGB[3]);
};


/*
* �L�����̉摜���Ǘ�����N���X
*/
class CharacterGraphHandle {
private:
	// �\�������摜
	GraphHandle* m_graphHandle;

	double m_ex;

	int m_wide, m_height;

	// �L�����̃p�[�c�̉摜
	// �a���U���摜
	GraphHandles* m_slashHandles;

	// �ˌ��U���摜
	GraphHandles* m_bulletHandles;

	// �L�����{�̂̉摜
	// �����摜
	GraphHandles* m_standHandles;

	// ���Ⴊ�݉摜
	GraphHandles* m_squatHandles;

	// ���Ⴊ�ݎˌ��摜
	GraphHandles* m_squatBulletHandles;

	// �����ˌ��摜
	GraphHandles* m_standBulletHandles;

	// �����a���摜
	GraphHandles* m_standSlashHandles;

	// ����摜
	GraphHandles* m_runHandles;

	// ����ˌ��摜
	GraphHandles* m_runBulletHandles;

	// ���n�摜
	GraphHandles* m_landHandles;

	// �㏸�摜
	GraphHandles* m_jumpHandles;

	// ���~�摜
	GraphHandles* m_downHandles;

	// �W�����v�O�摜
	GraphHandles* m_preJumpHandles;

	// �_���[�W�摜
	GraphHandles* m_damageHandles;

	// �u�[�X�g�摜
	GraphHandles* m_boostHandles;

	// �󒆎ˌ��摜
	GraphHandles* m_airBulletHandles;

	// �󒆎a���摜
	GraphHandles* m_airSlashHandles;

	// ����摜
	GraphHandles* m_deadHandles;
public:
	// �f�t�H���g�l�ŏ�����
	CharacterGraphHandle();
	// csv�t�@�C����ǂݍ��݁A�L�������Ō������p�����[�^�擾
	CharacterGraphHandle(const char* characterName, double drawEx, bool runner);
	// �摜���폜
	~CharacterGraphHandle();

	// �\������摜��Ԃ�
	inline GraphHandle* getHandle() { return m_graphHandle; }
	inline int getWide() const { return m_wide; }
	inline int getHeight() const { return m_height; }

	// �摜�̃Q�b�^
	inline GraphHandles* getSlashHandle() { return m_slashHandles; }
	inline GraphHandles* getBulletHandle() { return m_bulletHandles; }
	inline GraphHandles* getStandHandle() { return m_standHandles; }
	inline GraphHandles* getStandBulletHandle() { return m_standBulletHandles; }
	inline GraphHandles* getStandSlashHandle() { return m_standSlashHandles; }
	inline GraphHandles* getSquatHandle() { return m_squatHandles; }
	inline GraphHandles* getSquatBulletHandle() { return m_squatBulletHandles; }
	inline GraphHandles* getRunHandle() { return m_runHandles; }
	inline GraphHandles* getRunBulletHandle() { return m_runBulletHandles; }
	inline GraphHandles* getLandHandle() { return m_landHandles; }
	inline GraphHandles* getJumpHandle() { return m_jumpHandles; }
	inline GraphHandles* getDownHandle() { return m_downHandles; }
	inline GraphHandles* getPreJumpHandle() { return m_preJumpHandles; }
	inline GraphHandles* getDamageHandle() { return m_damageHandles; }
	inline GraphHandles* getBoostHandle() { return m_boostHandles; }
	inline GraphHandles* getAirBulletHandle() { return m_airBulletHandles; }
	inline GraphHandles* getAirSlashHandle() { return m_airSlashHandles; }
	inline GraphHandles* getDeadHandle() { return m_deadHandles; }

	// �摜�T�C�Y���Z�b�g
	void setGraphSize();

	// �摜���Z�b�g�A���݂��Ȃ��摜�Ȃ炻�̂܂܁@�T�C�Y������
	void setGraph(const GraphHandles* graphHandles, int index);
	void setGraph(GraphHandle* graphHandle);

	// �����摜���Z�b�g
	void switchStand(int index = 0);
	// �����ˌ��摜���Z�b�g
	void switchBullet(int index = 0);
	// �����a���摜���Z�b�g
	void switchSlash(int index = 0);
	// ���Ⴊ�݉摜���Z�b�g
	void switchSquat(int index = 0);
	// ���Ⴊ�ݎˌ��摜���Z�b�g
	void switchSquatBullet(int index = 0);
	// ����摜���Z�b�g
	void switchRun(int index = 0);
	// ����ˌ��摜���Z�b�g
	void switchRunBullet(int index = 0);
	// ���n�摜���Z�b�g
	void switchLand(int index = 0);
	// �㏸�摜���Z�b�g
	void switchJump(int index = 0);
	// �~���摜���Z�b�g
	void switchDown(int index = 0);
	// �W�����v�O�摜���Z�b�g
	void switchPreJump(int index = 0);
	// �_���[�W�摜���Z�b�g
	void switchDamage(int index = 0);
	// �u�[�X�g�摜���Z�b�g
	void switchBoost(int index = 0);
	// �󒆎ˌ��摜���Z�b�g
	void switchAirBullet(int index = 0);
	// �󒆎a���摜���Z�b�g
	void switchAirSlash(int index = 0);
	// ����摜���Z�b�g
	void switchDead(int index = 0);
};


/*
* �L�����̊�摜
*/
class FaceGraphHandle {
private:
	double m_ex;
	// �ʏ�
	GraphHandles* m_normalHandles;

public:
	FaceGraphHandle();
	FaceGraphHandle(const char* characterName, double drawEx);
	~FaceGraphHandle();

	GraphHandles* getGraphHandle(const char* faceName);
};


#endif