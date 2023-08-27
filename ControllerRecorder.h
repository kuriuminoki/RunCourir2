#ifndef CONTROLLER_RECORDER_H_INCLUDED
#define CONTROLLER_RECORDER_H_INCLUDED


#include <vector>


class CharacterController;



/*
* ����̋L�^�i���R�[�h�j
*/
class ControllerRecord {
private:

	// ����
	int m_input;

	// ���͊J�n�̎���
	int m_startTime;

	// ���͂��ꂽ���Ԃ̒����i�Ⴆ��1F�����̓��͂Ȃ�1�j
	int m_timeLength;

	// �U���p
	int m_gx, m_gy;

public:
	ControllerRecord(int startTime, int input);

	// �Q�b�^
	int getInput() { return m_input; }
	int getStartTime() { return m_startTime; }
	int getTimeLength() { return m_timeLength; }
	int getGx() { return m_gx; }
	int getGy() { return m_gy; }

	// �Z�b�^
	void setGx(int gx) { m_gx = gx; }
	void setGy(int gy) { m_gy = gy; }

	// ���͂̏I�������i�I���������傤�ǂł͓��͂Ȃ��j
	int getEndTime() { return m_startTime + m_timeLength; }

	// ���鎞���ł̓��̓`�F�b�N
	int checkInput(int time) { 
		if (m_startTime <= time && time < m_startTime + m_timeLength) {
			return m_input;
		}
		return 0;
	}

	// ���Ԃ����Z
	void addTime() { m_timeLength++; }

	// �I���������Z�b�g�i�I���������傤�ǂł͓��͂Ȃ��j
	void setEndTime(int time);
};



/*
* CharacterController�ɂ�鑀����L�^����N���X�B
* �ߋ��̓����̍Č��Ɏg����B
*/
class ControllerRecorder {
private:

	// ����ȍ~�̓��R�[�h���Ȃ����Ƃ�\��Record�̎���
	const int END_RECORD_TIME = -1;

	// �̓����v
	int m_time;

	// ���݌��Ă���vector�̃C���f�b�N�X
	int m_index;

	// ���R�[�h�i�L�^�j
	std::vector<ControllerRecord*> m_records;

public:
	ControllerRecorder(int startTime);
	~ControllerRecorder();

	// ���Ԃ��ŏ��ɖ߂�
	void init();

	// ���Ԃ�i�߂�
	void addTime();

	// ���R�[�h�̑��݃`�F�b�N
	bool existRecord();

	// ���͂��擾
	int checkInput();

	// ����̃��R�[�h��ǋLor�ǉ�����i1F���j
	void writeRecord(int input);

	// time�ȍ~�̃��R�[�h���폜����
	void discardRecord();

	// �U���ڕW��ݒ�
	void setGoal(int gx, int gy);

	// �U���ڕW���擾
	void getGoal(int& gx, int& gy);
};


#endif