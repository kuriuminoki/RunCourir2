#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED


#include <string>
#include <vector>


class SoundPlayer;
class World;
class GraphHandle;
class GraphHandles;


class Conversation {
private:

	// �I�����A���������ҋ@����
	const int FINISH_COUNT = 30;
	int m_finishCnt;

	// �C�x���g�I��������
	bool m_finishFlag;

	// �����\���̑��� 1���ő�
	const unsigned int TEXT_SPEED = 5;
	unsigned int m_textSpeed;

	// �e�L�X�g���΂���悤�ɂȂ�܂ł̎���
	const unsigned int MOVE_FINAL_ABLE = 10;

	// �t�@�C���|�C���^
	int m_fp;

	World* m_world_p;

	SoundPlayer* m_soundPlayer_p;

	// �����҂̖��O
	std::string m_speakerName;

	// �����҂̊�摜
	GraphHandles* m_speakerGraph;

	// ����
	std::string m_text;

	// ���������ڂ܂Ŕ���������
	unsigned int m_textNow;

	// �J�E���g
	unsigned int m_cnt;

	// �����\�����ʉ�
	int m_displaySound;

	// ������ʉ�
	int m_nextSound;

public:
	Conversation(int textNum, World* world, SoundPlayer* soundPlayer);
	~Conversation();

	// �Q�b�^
	std::string getText() const;
	inline std::string getFullText() const { return m_text; }
	int getTextSize() const;
	GraphHandle* getGraph() const;
	inline 	std::string getSpeakerName() const { return m_speakerName; }
	inline int getFinishCnt() const { return m_finishCnt; }
	inline bool getFinishFlag() const { return m_finishFlag; }
	inline int getTextNow() const { return m_textNow; }
	inline int getCnt() const { return m_cnt; }

	// ��b���s��
	bool play();

private:
	void setNextText();
	void setSpeakerGraph(const char* faceName);
};


#endif