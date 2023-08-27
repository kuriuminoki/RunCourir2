#include "ControllerRecorder.h"
#include <algorithm>


using namespace std;


/*
* ���R�[�h
*/
ControllerRecord::ControllerRecord(int startTime, int input) {
	m_startTime = startTime;
	m_timeLength = 1;
	m_input = input;
	m_gx = 0; m_gy = 0;
}


// �I���������Z�b�g�i�I���������傤�ǂł͓��͂Ȃ��j
void ControllerRecord::setEndTime(int time) {
	if (time > m_startTime) {
		m_timeLength = time - m_startTime;
	}
	else {
		m_startTime = time;
		m_timeLength = 0;
	}
}



/*
* ���R�[�_�[
*/
ControllerRecorder::ControllerRecorder(int startTime) {
	m_time = startTime;
	m_index = 0;
	if (m_time > 0) {
		ControllerRecord* r = new ControllerRecord(0, false);
		r->setEndTime(startTime);
		m_records.push_back(r);
		m_index++;
	}
}


// �f�X�g���N�^�̓��R�[�h��S�폜
ControllerRecorder::~ControllerRecorder() {
	for (unsigned int i = 0; i < m_records.size(); i++) {
		delete m_records[i];
	}
}


// ���Ԃ�i�߂�
void ControllerRecorder::addTime() {
	m_time++;
	//if (m_records.back()->getEndTime() <= m_time) {
	//	m_index = (int)m_records.size();
	//}
	if (m_records.size() > 0 && m_index < m_records.size() && m_records[m_index]->getEndTime() <= m_time) {
		m_index = min((int)m_records.size() + 1, m_index + 1);
	}
}


// ���Ԃ��ŏ��ɖ߂�
void ControllerRecorder::init() {
	m_time = 0;
	m_index = 0;
}


// ���R�[�h�̑��݃`�F�b�N
bool ControllerRecorder::existRecord() {
	if (m_records.size() == 0) {
		return false;
	}
	// �ŐV���R�[�h�̏I�������ƌ��ݎ������r
	return m_records.back()->getEndTime() > m_time;
}


// ���͂��擾
int ControllerRecorder::checkInput() {
	if (!existRecord()) { return false; }
	return m_records[m_index]->checkInput(m_time);
}


// ����̃��R�[�h��ǋLor�ǉ�����i1F���j
void ControllerRecorder::writeRecord(int input) {
	if (m_records.size() != 0 && m_records.back()->getInput() == input) {
		m_records.back()->addTime();
	}
	else {
		m_records.push_back(new ControllerRecord(m_time, input));
		m_index++;
	}
}


// m_time�ȍ~�̃��R�[�h���폜����
void ControllerRecorder::discardRecord() {
	while (m_records.size() > 0 && m_records.back()->getStartTime() > m_time) {
		delete m_records.back();
		m_records.pop_back();
	}
	if (m_records.size() > 0) {
		m_records.back()->setEndTime(m_time);
	}
}

// �U���ڕW��ݒ�
void ControllerRecorder::setGoal(int gx, int gy) {
	if (!existRecord()) { return; }
	m_records.back()->setGx(gx);
	m_records.back()->setGy(gy);
}

// �U���ڕW���擾
void ControllerRecorder::getGoal(int& gx, int& gy) {
	if (!existRecord()) { return; }
	gx = m_records[m_index]->getGx();
	gy = m_records[m_index]->getGy();
}