#ifndef ANIMATION_H_INCLUDED
#define ANIMATION_H_INCLUDED

class GraphHandle;
class GraphHandles;

class Animation {
private:
	// �摜�n���h��
	GraphHandles* m_handles;

	// ���W
	int m_x, m_y;
	
	// �J�E���g
	int m_cnt;

	// �I���܂ł̃J�E���g
	int m_finishCnt;

	// �摜�P���̕\������
	int m_flameCnt;

	// �A�j���[�V�������I��
	bool m_finishFlag;

public:
	Animation(int x, int y, int flameCnt, GraphHandles* graphHandles);

	Animation* createCopy();

	// �Q�b�^
	inline int getX() const { return m_x; }
	inline int getY() const { return m_y; }
	inline bool getFinishFlag() const { return m_finishFlag; }

	// �Z�b�^
	inline void setCnt(int cnt) { m_cnt = cnt; }
	inline void setFinishCnt(int finishCnt) { m_finishCnt = finishCnt; }
	inline void setFinishFlag(int finishFlag) { m_finishFlag = finishFlag; }

	// �J�E���g
	void count();

	// �`��p
	GraphHandle* getHandle() const;
};

#endif