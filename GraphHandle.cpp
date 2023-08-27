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

// 画像のデリート
GraphHandle::~GraphHandle() {
	DeleteGraph(m_handle);
}

// 描画する
void GraphHandle::draw(int x, int y, double ex, int r, int g, int b) const {
	SetDrawBright(r, g, b);
	DrawRotaGraph(x, y, ex, m_angle, m_handle, m_trans, m_reverseX, m_reverseY);
	SetDrawBright(255, 255, 255);
}

// 範囲を指定して描画する
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
* 複数の画像をまとめて扱うクラス
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

// 画像のデリート
GraphHandles::~GraphHandles() {
	for (int i = 0; i < m_handleSum; i++) {
		delete m_handles[i];
	}
	delete m_handles;
}

// セッタ
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

// 描画する
void GraphHandles::draw(int x, int y, int index) {
	m_handles[index]->draw(x, y);
}


/*
* キャラの画像
*/
// 画像ロード用
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
// デフォルト値で初期化
CharacterGraphHandle::CharacterGraphHandle() :
	CharacterGraphHandle("test", 1.0, false)
{

}
// csvファイルを読み込み、キャラ名で検索しパラメータ取得
CharacterGraphHandle::CharacterGraphHandle(const char* characterName, double drawEx, bool runner) {
	m_ex = drawEx;

	CsvReader reader("data/characterGraph.csv");

	// キャラ名でデータを検索
	map<string, string> data = reader.findOne("name", characterName);
	if (data.size() == 0) { data = reader.findOne("name", "テスト"); }

	// ロード
	const char* dir = "picture/stick/";
	if (runner) {
		loadCharacterGraph(dir, "", m_standHandles, "stand", data, m_ex);
		loadCharacterGraph(dir, "", m_runHandles, "run", data, m_ex);
		// Runner用画像
		string nameStr = characterName;
		if (nameStr == "クーリール") {
			int hairRGB[3] = { 255, 255, 255 };
			int eyeRGB[3] = { 255, 255, 255 };
			m_runnerGraphHandle = new RunnerGraphHandle(characterName, drawEx, 'A', 'B', hairRGB, eyeRGB);
		}
		else if (nameStr == "クーリールカスタム") {
			int hairRGB[3] = { GetRand(255), GetRand(255), GetRand(255) };
			int eyeRGB[3] = { 255, 255, 255 };
			m_runnerGraphHandle = new RunnerGraphHandle(characterName, drawEx, 'A', 'B', hairRGB, eyeRGB);
		}
		else {
			int hairRGB[3] = { GetRand(255), GetRand(255), GetRand(255) };
			int eyeRGB[3] = { GetRand(255), GetRand(255), GetRand(255) };
			m_runnerGraphHandle = new RunnerGraphHandle(characterName, drawEx, 'A', 'B', hairRGB, eyeRGB);
		}
	}
	else {
		loadCharacterGraph(dir, characterName, m_standHandles, "stand", data, m_ex);
		loadCharacterGraph(dir, characterName, m_runHandles, "run", data, m_ex);
		m_runnerGraphHandle = nullptr;
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
// 画像を削除
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

// 画像のサイズをセット
void CharacterGraphHandle::setGraphSize() {
	GetGraphSize(m_graphHandle->getHandle(), &m_wide, &m_height);
	// 画像の拡大率も考慮してサイズを決定
	m_wide = (int)(m_wide * m_ex);
	m_height = (int)(m_height * m_ex);
}

// 画像をセットする 存在しないならそのまま
void CharacterGraphHandle::setGraph(const GraphHandles* graphHandles, int index) {
	if (index >= graphHandles->getSize() || index < 0) { return; }
	m_graphHandle = graphHandles == NULL ? m_graphHandle : graphHandles->getGraphHandle(index);
	setGraphSize();
}
void CharacterGraphHandle::setGraph(GraphHandle* graphHandle) {
	m_graphHandle = graphHandle;
	setGraphSize();
}

// 立ち画像をセット
void CharacterGraphHandle::switchStand(int index){
	setGraph(m_standHandles, index);
	if (m_runnerGraphHandle != nullptr) {
		m_runnerGraphHandle->switchStand();
	}
}
// 立ち射撃画像をセット
void CharacterGraphHandle::switchBullet(int index){
	setGraph(m_standBulletHandles, index);
}
// 立ち斬撃画像をセット
void CharacterGraphHandle::switchSlash(int index){
	setGraph(m_standSlashHandles, index);
}
// しゃがみ画像をセット
void CharacterGraphHandle::switchSquat(int index){
	setGraph(m_squatHandles, index);
}
// しゃがみ射撃画像をセット
void CharacterGraphHandle::switchSquatBullet(int index) {
	setGraph(m_squatBulletHandles, index);
}
// 走り画像をセット
void CharacterGraphHandle::switchRun(int index, int tired){
	setGraph(m_runHandles, index);
	if (m_runnerGraphHandle != nullptr) {
		m_runnerGraphHandle->switchRun(index, tired);
	}
}
// 走り射撃画像をセット
void CharacterGraphHandle::switchRunBullet(int index) {
	setGraph(m_runBulletHandles, index);
}
// 着地画像をセット
void CharacterGraphHandle::switchLand(int index){
	setGraph(m_landHandles, index);
}
// 上昇画像をセット
void CharacterGraphHandle::switchJump(int index){
	setGraph(m_jumpHandles, index);
}
// 降下画像をセット
void CharacterGraphHandle::switchDown(int index){
	setGraph(m_downHandles, index);
}
// ジャンプ前画像をセット
void CharacterGraphHandle::switchPreJump(int index){
	setGraph(m_preJumpHandles, index);
}
// ダメージ画像をセット
void CharacterGraphHandle::switchDamage(int index){
	setGraph(m_damageHandles, index);
}
// ブースト画像をセット
void CharacterGraphHandle::switchBoost(int index){
	setGraph(m_boostHandles, index);
}
// 空中射撃画像をセット
void CharacterGraphHandle::switchAirBullet(int index){
	setGraph(m_airBulletHandles, index);
}
// 空中斬撃画像をセット
void CharacterGraphHandle::switchAirSlash(int index){
	setGraph(m_airSlashHandles, index);
}
// やられ画像をセット
void CharacterGraphHandle::switchDead(int index) {
	setGraph(m_deadHandles, index);
	if (m_runnerGraphHandle != nullptr) {
		m_runnerGraphHandle->switchDead();
	}
}



FaceGraphHandle::FaceGraphHandle():
	FaceGraphHandle("テスト", 1.0)
{

}
FaceGraphHandle::FaceGraphHandle(const char* characterName, double drawEx) {
	m_ex = drawEx;
	
	CsvReader reader("data/faceGraph.csv");
	// キャラ名でデータを検索
	map<string, string> data = reader.findOne("name", characterName);
	if (data.size() == 0) { data = reader.findOne("name", "テスト"); }

	// ロード
	const char* dir = "picture/face/";
	loadCharacterGraph(dir, characterName, m_normalHandles, "通常", data, m_ex);

}
FaceGraphHandle::~FaceGraphHandle() {
	delete m_normalHandles;
}
GraphHandles* FaceGraphHandle::getGraphHandle(const char* faceName) {
	return m_normalHandles;
}


// 画像ロード用
void loadRunnerGraph(const char* dir, const char* characterName, GraphHandles*& handles, string stateName, map<string, string>& data, double ex) {
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
RunnerGraphHandle::RunnerGraphHandle(const char* characterName, double drawEx, char kutu, char huku, int hairRGB[3], int eyeRGB[3]) {
	m_ex = drawEx;
	for (int i = 0; i < 3; i++) {
		m_hairRGB[i] = hairRGB[i];
		m_eyeRGB[i] = eyeRGB[i];
	}

	// 画像をロード
	const char* dir = "picture/stick/";
	ostringstream oss;
	oss << dir;
	// 走っているときの汗
	runAse = new GraphHandles((oss.str() + "run汗").c_str(), 3, m_ex, 0.0, true);

	// やられ状態の汗
	deadAse = new GraphHandles((oss.str() + "dead汗").c_str(), 1, m_ex, 0.0, true);

	oss << characterName << "/";
	// 立ち状態
	stand = new GraphHandles((oss.str() + "stand").c_str(), 1, m_ex, 0.0, true);
	standEye = new GraphHandles((oss.str() + "standEye").c_str(), 1, m_ex, 0.0, true);
	standFrontHair = new GraphHandles((oss.str() + "standHair手前").c_str(), 1, m_ex, 0.0, true);
	standBackHair = new GraphHandles((oss.str() + "standHair奥").c_str(), 1, m_ex, 0.0, true);

	// run状態 通常、疲れ、限界
	run[0] = new GraphHandles((oss.str() + "run通常").c_str(), 3, m_ex, 0.0, true);
	run[1] = new GraphHandles((oss.str() + "run疲労").c_str(), 3, m_ex, 0.0, true);
	run[2] = new GraphHandles((oss.str() + "run限界").c_str(), 3, m_ex, 0.0, true);
	runEye[0] = new GraphHandles((oss.str() + "runEye通常").c_str(), 3, m_ex, 0.0, true);
	runEye[1] = new GraphHandles((oss.str() + "runEye疲労").c_str(), 3, m_ex, 0.0, true);
	runEye[2] = new GraphHandles((oss.str() + "runEye限界").c_str(), 3, m_ex, 0.0, true);

	// run状態の髪
	runFrontHair = new GraphHandles((oss.str() + "runHair手前").c_str(), 3, m_ex, 0.0, true);
	runBackHair = new GraphHandles((oss.str() + "runHair奥").c_str(), 3, m_ex, 0.0, true);

	// やられ状態
	dead = new GraphHandles((oss.str() + "dead").c_str(), 1, m_ex, 0.0, true);
	deadEye = new GraphHandles((oss.str() + "deadEye").c_str(), 1, m_ex, 0.0, true);
	deadHair = new GraphHandles((oss.str() + "deadHair").c_str(), 1, m_ex, 0.0, true);

	// 靴と服
	if (kutu == 'A' || kutu == 'B' || kutu == 'C') {
		ostringstream oss2;
		oss2 << dir << "靴/";
		standKutu = new GraphHandles((oss2.str() + "kutu" + kutu + "stand").c_str(), 1, m_ex, 0.0, true);
		runKutu = new GraphHandles((oss2.str() + "kutu" + kutu).c_str(), 3, m_ex, 0.0, true);
	}
	else {
		standKutu = nullptr;
		runKutu = nullptr;
	}
	if (huku == 'A' || huku == 'B' || huku == 'C') {
		ostringstream oss2;
		oss2 << dir << "服/";
		standHuku = new GraphHandles((oss2.str() + "huku" + huku + "stand").c_str(), 1, m_ex, 0.0, true);
		runHuku = new GraphHandles((oss2.str() + "huku" + huku).c_str(), 3, m_ex, 0.0, true);
	}
	else {
		standHuku = nullptr;
		runHuku = nullptr;
	}
	// 汗
	ostringstream oss3;
	oss3 << dir;
	deadAse = new GraphHandles((oss3.str() + "dead汗").c_str(), 1, m_ex, 0.0, true);
	runAse = new GraphHandles((oss3.str() + "run汗").c_str(), 3, m_ex, 0.0, true);

	// 描画する画像
	m_backHair = nullptr;
	m_face = nullptr;
	m_eye = nullptr;
	m_frontHair = nullptr;
	m_kutu = nullptr;
	m_huku = nullptr;
	m_ase = nullptr;
}
RunnerGraphHandle::~RunnerGraphHandle() {
	// 走っているときの汗
	delete runAse;

	// やられ状態の汗
	delete deadAse;

	// 立ち状態
	delete stand;
	delete standEye;
	delete standFrontHair;
	delete standBackHair;

	// run状態 通常、疲れ、限界
	delete run[0];
	delete run[1];
	delete run[2];
	delete runEye[0];
	delete runEye[1];
	delete runEye[2];

	// run状態の髪
	delete runFrontHair;
	delete runBackHair;

	// やられ状態
	delete dead;
	delete deadEye;
	delete deadHair;

	// 靴と服
	delete standKutu;
	delete runKutu;
	delete standHuku;
	delete runHuku;
}

// 画像を設定
void RunnerGraphHandle::switchStand() {
	m_backHair = standBackHair->getGraphHandle();
	m_face = stand->getGraphHandle();
	m_eye = standEye->getGraphHandle();
	m_frontHair = standFrontHair->getGraphHandle();
	if (standKutu != nullptr) {
		m_kutu = standKutu->getGraphHandle();
	}
	if (standHuku != nullptr) {
		m_huku = standHuku->getGraphHandle();
	}
	m_ase = nullptr;
}
void RunnerGraphHandle::switchRun(int runIndex, int tired) {
	m_backHair = runBackHair->getGraphHandle(runIndex % 3);
	m_face = run[tired]->getGraphHandle(runIndex % 3);
	m_eye = runEye[tired]->getGraphHandle(runIndex % 3);
	m_frontHair = runFrontHair->getGraphHandle(runIndex % 3);
	if (standKutu != nullptr) {
		m_kutu = runKutu->getGraphHandle(runIndex % 3);
	}
	if (standHuku != nullptr) {
		m_huku = runHuku->getGraphHandle(runIndex % 3);
	}
	m_ase = runAse->getGraphHandle(runIndex % 3);
}
void RunnerGraphHandle::switchDead() {
	m_backHair = nullptr;
	m_face = dead->getGraphHandle();
	m_eye = deadEye->getGraphHandle();
	m_frontHair = deadHair->getGraphHandle();
	m_kutu = nullptr;
	m_huku = nullptr;
	m_ase = deadAse->getGraphHandle();
}