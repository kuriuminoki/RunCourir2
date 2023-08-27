#include "GraphHandle.h"
#include "CsvReader.h"
#include "DxLib.h"
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;


GraphHandle::GraphHandle(const char* filePath, double ex, double angle, bool trans, bool reverseX, bool reverseY) {
	m_handle = LoadGraph(filePath);
	m_ex = ex;
	m_angle = angle;
	m_trans = trans;
	m_reverseX = reverseX;
	m_reverseY = reverseY;
}

// �摜�̃f���[�g
GraphHandle::~GraphHandle() {
	DeleteGraph(m_handle);
}

// �`�悷��
void GraphHandle::draw(int x, int y, double ex) const {
	DrawRotaGraph(x, y, ex, m_angle, m_handle, m_trans, m_reverseX, m_reverseY);
}

// �͈͂��w�肵�ĕ`�悷��
void GraphHandle::extendDraw(int x1, int y1, int x2, int y2) const {
	if (m_reverseX) {
		swap(x1, x2);
	}
	if (m_reverseY) {
		swap(y1, y2);
	}
	DrawExtendGraph(x1, y1, x2, y2, m_handle, m_trans);
}


/*
* �����̉摜���܂Ƃ߂Ĉ����N���X
*/
GraphHandles::GraphHandles(const char* filePath, int handleSum, double ex, double angle, bool trans, bool reverseX, bool reverseY) {
	m_handleSum = handleSum;
	m_handles = new GraphHandle* [m_handleSum];
	if (m_handleSum == 1) {
		ostringstream oss;
		oss << filePath << ".png";
		m_handles[0] = new GraphHandle(oss.str().c_str(), ex, angle, trans, reverseX, reverseY);
	}
	else {
		for (int i = 0; i < m_handleSum; i++) {
			ostringstream oss;
			oss << filePath << i + 1 << ".png";
			m_handles[i] = new GraphHandle(oss.str().c_str(), ex, angle, trans, reverseX, reverseY);
		}
	}
}

// �摜�̃f���[�g
GraphHandles::~GraphHandles() {
	for (int i = 0; i < m_handleSum; i++) {
		delete m_handles[i];
	}
	delete m_handles;
}

// �Z�b�^
void GraphHandles::setEx(double ex) {
	for (int i = 0; i < m_handleSum; i++) {
		m_handles[i]->setEx(ex);
	}
}
void GraphHandles::setAngle(double angle) {
	for (int i = 0; i < m_handleSum; i++) {
		m_handles[i]->setAngle(angle);
	}
}
void GraphHandles::setTrans(bool trans) {
	for (int i = 0; i < m_handleSum; i++) {
		m_handles[i]->setTrans(trans);
	}
}
void GraphHandles::setReverseX(bool reverse) {
	for (int i = 0; i < m_handleSum; i++) {
		m_handles[i]->setReverseX(reverse);
	}
}
void GraphHandles::setReverseY(bool reverse) {
	for (int i = 0; i < m_handleSum; i++) {
		m_handles[i]->setReverseY(reverse);
	}
}

// �`�悷��
void GraphHandles::draw(int x, int y, int index) {
	m_handles[index]->draw(x, y);
}


/*
* �L�����̉摜
*/
// �摜���[�h�p
void loadCharacterGraph(const char* dir, const char* characterName, GraphHandles*& handles, string stateName, map<string, string>& data, double ex) {
	int n = stoi(data[stateName]);
	if (n > 0) {
		ostringstream oss;
		oss << dir << characterName << "/" << stateName;
		handles = new GraphHandles(oss.str().c_str(), n, ex, 0.0, true);
	}
	else {
		handles = NULL;
	}
}
// �f�t�H���g�l�ŏ�����
CharacterGraphHandle::CharacterGraphHandle() :
	CharacterGraphHandle("test", 1.0, false)
{

}
// csv�t�@�C����ǂݍ��݁A�L�������Ō������p�����[�^�擾
CharacterGraphHandle::CharacterGraphHandle(const char* characterName, double drawEx, bool runner) {
	m_ex = drawEx;

	CsvReader reader("data/characterGraph.csv");

	// �L�������Ńf�[�^������
	map<string, string> data = reader.findOne("name", characterName);
	if (data.size() == 0) { data = reader.findOne("name", "�e�X�g"); }

	// ���[�h
	const char* dir = "picture/stick/";
	if (runner) {
		loadCharacterGraph(dir, "", m_standHandles, "stand", data, m_ex);
		loadCharacterGraph(dir, "", m_runHandles, "run", data, m_ex);
	}
	else {
		loadCharacterGraph(dir, characterName, m_standHandles, "stand", data, m_ex);
		loadCharacterGraph(dir, characterName, m_runHandles, "run", data, m_ex);
	}
	loadCharacterGraph(dir, characterName, m_slashHandles, "slash", data, m_ex);
	loadCharacterGraph(dir, characterName, m_bulletHandles, "bullet", data, m_ex);
	loadCharacterGraph(dir, characterName, m_squatHandles, "squat", data, m_ex);
	loadCharacterGraph(dir, characterName, m_squatBulletHandles, "squatBullet", data, m_ex);
	loadCharacterGraph(dir, characterName, m_standBulletHandles, "standBullet", data, m_ex);
	loadCharacterGraph(dir, characterName, m_standSlashHandles, "standSlash", data, m_ex);
	loadCharacterGraph(dir, characterName, m_runBulletHandles, "runBullet", data, m_ex);
	loadCharacterGraph(dir, characterName, m_landHandles, "land", data, m_ex);
	loadCharacterGraph(dir, characterName, m_jumpHandles, "jump", data, m_ex);
	loadCharacterGraph(dir, characterName, m_downHandles, "down", data, m_ex);
	loadCharacterGraph(dir, characterName, m_preJumpHandles, "preJump", data, m_ex);
	loadCharacterGraph(dir, characterName, m_damageHandles, "damage", data, m_ex);
	loadCharacterGraph(dir, characterName, m_boostHandles, "boost", data, m_ex);
	loadCharacterGraph(dir, characterName, m_airBulletHandles, "airBullet", data, m_ex);
	loadCharacterGraph(dir, characterName, m_airSlashHandles, "airSlash", data, m_ex);
	loadCharacterGraph(dir, characterName, m_deadHandles, "dead", data, m_ex);

	switchStand();
}
// �摜���폜
CharacterGraphHandle::~CharacterGraphHandle() {
	delete m_standHandles;
	delete m_slashHandles;
	delete m_bulletHandles;
	delete m_squatHandles;
	delete m_squatBulletHandles;
	delete m_standBulletHandles;
	delete m_standSlashHandles;
	delete m_runHandles;
	delete m_runBulletHandles;
	delete m_landHandles;
	delete m_jumpHandles;
	delete m_downHandles;
	delete m_preJumpHandles;
	delete m_damageHandles;
	delete m_boostHandles;
	delete m_airBulletHandles;
	delete m_airSlashHandles;
}

// �摜�̃T�C�Y���Z�b�g
void CharacterGraphHandle::setGraphSize() {
	GetGraphSize(m_graphHandle->getHandle(), &m_wide, &m_height);
	// �摜�̊g�嗦���l�����ăT�C�Y������
	m_wide = (int)(m_wide * m_ex);
	m_height = (int)(m_height * m_ex);
}

// �摜���Z�b�g���� ���݂��Ȃ��Ȃ炻�̂܂�
void CharacterGraphHandle::setGraph(const GraphHandles* graphHandles, int index) {
	if (index >= graphHandles->getSize() || index < 0) { return; }
	m_graphHandle = graphHandles == NULL ? m_graphHandle : graphHandles->getGraphHandle(index);
	setGraphSize();
}
void CharacterGraphHandle::setGraph(GraphHandle* graphHandle) {
	m_graphHandle = graphHandle;
	setGraphSize();
}

// �����摜���Z�b�g
void CharacterGraphHandle::switchStand(int index){
	setGraph(m_standHandles, index);
}
// �����ˌ��摜���Z�b�g
void CharacterGraphHandle::switchBullet(int index){
	setGraph(m_standBulletHandles, index);
}
// �����a���摜���Z�b�g
void CharacterGraphHandle::switchSlash(int index){
	setGraph(m_standSlashHandles, index);
}
// ���Ⴊ�݉摜���Z�b�g
void CharacterGraphHandle::switchSquat(int index){
	setGraph(m_squatHandles, index);
}
// ���Ⴊ�ݎˌ��摜���Z�b�g
void CharacterGraphHandle::switchSquatBullet(int index) {
	setGraph(m_squatBulletHandles, index);
}
// ����摜���Z�b�g
void CharacterGraphHandle::switchRun(int index){
	setGraph(m_runHandles, index);
}
// ����ˌ��摜���Z�b�g
void CharacterGraphHandle::switchRunBullet(int index) {
	setGraph(m_runBulletHandles, index);
}
// ���n�摜���Z�b�g
void CharacterGraphHandle::switchLand(int index){
	setGraph(m_landHandles, index);
}
// �㏸�摜���Z�b�g
void CharacterGraphHandle::switchJump(int index){
	setGraph(m_jumpHandles, index);
}
// �~���摜���Z�b�g
void CharacterGraphHandle::switchDown(int index){
	setGraph(m_downHandles, index);
}
// �W�����v�O�摜���Z�b�g
void CharacterGraphHandle::switchPreJump(int index){
	setGraph(m_preJumpHandles, index);
}
// �_���[�W�摜���Z�b�g
void CharacterGraphHandle::switchDamage(int index){
	setGraph(m_damageHandles, index);
}
// �u�[�X�g�摜���Z�b�g
void CharacterGraphHandle::switchBoost(int index){
	setGraph(m_boostHandles, index);
}
// �󒆎ˌ��摜���Z�b�g
void CharacterGraphHandle::switchAirBullet(int index){
	setGraph(m_airBulletHandles, index);
}
// �󒆎a���摜���Z�b�g
void CharacterGraphHandle::switchAirSlash(int index){
	setGraph(m_airSlashHandles, index);
}
// ����摜���Z�b�g
void CharacterGraphHandle::switchDead(int index) {
	setGraph(m_deadHandles, index);
}



FaceGraphHandle::FaceGraphHandle():
	FaceGraphHandle("�e�X�g", 1.0)
{

}
FaceGraphHandle::FaceGraphHandle(const char* characterName, double drawEx) {
	m_ex = drawEx;
	
	CsvReader reader("data/faceGraph.csv");
	// �L�������Ńf�[�^������
	map<string, string> data = reader.findOne("name", characterName);
	if (data.size() == 0) { data = reader.findOne("name", "�e�X�g"); }

	// ���[�h
	const char* dir = "picture/face/";
	loadCharacterGraph(dir, characterName, m_normalHandles, "�ʏ�", data, m_ex);

}
FaceGraphHandle::~FaceGraphHandle() {
	delete m_normalHandles;
}
GraphHandles* FaceGraphHandle::getGraphHandle(const char* faceName) {
	return m_normalHandles;
}


RunnerGraphHandle::RunnerGraphHandle(const char* characterName, double drawEx, char kutu, char huku, int hairRGB[3], int eyeRGB[3]) {
	for (int i = 0; i < 3; i++) {
		m_hairRGB[i] = hairRGB[i];
		m_eyeRGB[i] = eyeRGB[i];
	}

	// �摜�����[�h

}