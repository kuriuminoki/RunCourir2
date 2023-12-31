#include "CsvReader.h"
#include "Character.h"
#include "CharacterAction.h"
#include "CharacterController.h"
#include "Object.h"
#include "Camera.h"
#include "Brain.h"
#include "ObjectLoader.h"
#include "Define.h"
#include "DxLib.h"


using namespace std;


// 色の名前から色ハンドルを取得
int str2color(string colorName) {
	if (colorName == "white") { return WHITE; }
	else if (colorName == "gray") { return GRAY; }
	else if (colorName == "lightBlue") { return LIGHT_BLUE; }
	return -1;
}


// 一行分のテキストから一行分のデータに変換
vector<string> csv2vector(string buff) {
	vector<string> data;
	string oneCell = "";

	// 1文字ずつ見ていく
	for (int i = 0; buff[i] != '\0'; i++) {
		// CSVファイルなのでカンマで区切ってoneDataにpush_back
		if (buff[i] == ',') {
			data.push_back(oneCell);
			oneCell = "";
		}
		else { // カンマ以外の文字なら合体
			oneCell += buff[i];
		}
	}
	// 最後のセルにはカンマがない
	data.push_back(oneCell);

	return data;
}


// ファイル名を指定してCSVファイルを読み込む
CsvReader::CsvReader(const char* fileName) {

	// ファイルポインタ
	int fp;

	// バッファ
	const int size = 512;
	char buff[size];

	// ファイルを開く
	fp = FileRead_open(fileName);

	// バッファに一行目（カラム名）のテキストを入れる
	FileRead_gets(buff, size, fp);

	// カラム名のリストを取得
	m_columnNames = csv2vector(buff);

	// ファイルの終端までループ
	while (FileRead_eof(fp) == 0) {
		// いったんバッファに一行分のテキストを入れる
		FileRead_gets(buff, size, fp);

		// 一行分のテキストをデータにしてVectorに変換
		vector<string> oneDataVector;
		oneDataVector = csv2vector(buff);

		// vectorとカラム名を使ってmapを生成
		map<string, string> oneData;
		for (int i = 0; i < oneDataVector.size(); i++) {
			oneData[m_columnNames[i]] = oneDataVector[i];
		}

		// 一行分のmapデータをpush_back
		m_data.push_back(oneData);
	}

	// ファイルを閉じる
	FileRead_close(fp);
}


/*
* カラム名がvalueのデータを取得
* 例：findOne("name", "キャラ名");
* 見つからなかったら空のデータを返す
*/
map<string, string> CsvReader::findOne(const char* columnName, const char* value) {

	// m_data[i][columnName] == valueとなるiを調べる
	map<string, string>::iterator ite;
	for (int i = 0; i < m_data.size(); i++) {

		// カラム名に対応する値を取得
		ite = m_data[i].find(columnName);

		// そのカラム名が存在しない
		if (ite == m_data[i].end()) { continue; }

		// 条件に一致
		if (ite->second == value) {
			return m_data[i];
		}
	}

	// 空のデータを返す
	map<string, string> res;
	return res;
}


/*
* 全データを返す
*/
vector<map<string, string>> CsvReader::getData() const {
	return m_data;
}


// ファイル名を指定してCSVファイルを読み込む
CsvReader2::CsvReader2(const char* fileName) {

	// ファイルポインタ
	int fp;

	// バッファ
	const int size = 512;
	char buff[size];

	// ファイルを開く
	fp = FileRead_open(fileName);

	string domainName = "";
	// ファイルの終端までループ
	while (FileRead_eof(fp) == 0) {
		// いったんバッファに一行分のテキストを入れる
		FileRead_gets(buff, size, fp);

		// 一行分のテキストをデータにしてVectorに変換
		vector<string> oneDataVector = csv2vector(buff);

		if (oneDataVector[1] == "") {
			domainName = oneDataVector[0];
			FileRead_gets(buff, size, fp);
			m_columnNames[domainName] = csv2vector(buff);
		}
		else {
			// vectorとカラム名を使ってmapを生成
			map<string, string> oneData;
			for (int i = 0; i < oneDataVector.size(); i++) {
				oneData[m_columnNames[domainName][i]] = oneDataVector[i];
			}

			// 一行分のmapデータをpush_back
			m_data[domainName].push_back(oneData);
		}
	}

	// ファイルを閉じる
	FileRead_close(fp);
}


/*
* ドメイン名がdomainNameのデータから、
* カラム名がvalueのデータを取得
* 例：findOne("name", "キャラ名");
* 見つからなかったら空のデータを返す
*/
map<string, string> CsvReader2::findOne(const char* domainName, const char* columnName, const char* value) {

	// m_data[i][columnName] == valueとなるiを調べる
	map<string, string>::iterator ite;
	for (int i = 0; i < m_data.size(); i++) {

		// カラム名に対応する値を取得
		ite = m_data[domainName][i].find(columnName);

		// そのカラム名が存在しない
		if (ite == m_data[domainName][i].end()) { continue; }

		// 条件に一致
		if (ite->second == value) {
			return m_data[domainName][i];
		}
	}

	// 空のデータを返す
	map<string, string> res;
	return res;
}


/*
* area/area?.csvからキャラクターやオブジェクトをロードする
* Character等をnewするため、このクラスをnewした後はgetして削除すること。
* このクラスでnewされたCharacter等はこのクラスで削除しない。
*/
AreaReader::AreaReader(int fromAreaNum, int toAreaNum, SoundPlayer* soundPlayer) {
	m_fromAreaNum = fromAreaNum;
	m_soundPlayer_p = soundPlayer;

	m_camera_p = NULL;
	m_focusId = -1;
	m_playerId = -1;
	m_backGroundGraph = -1;
	m_backGroundColor = -1;
	m_bgmName = "";

	// ファイルを開く
	ostringstream fileName;
	fileName << "data/area/area" << toAreaNum << ".csv";

	CsvReader2 csvReader2(fileName.str().c_str());

	vector<map<string, string> > data;
	// BGM
	data = csvReader2.getDomainData("BGM:");
	for (unsigned int i = 0; i < data.size(); i++) {
		loadBGM(data[i]);
	}
	// CHARACTER
	data = csvReader2.getDomainData("CHARACTER:");
	for (unsigned int i = 0; i < data.size(); i++) {
		loadCharacter(data[i]);
	}
	// OBJECT
	data = csvReader2.getDomainData("OBJECT:");
	ObjectLoader objectLoader;
	for (unsigned int i = 0; i < data.size(); i++) {
		objectLoader.addObject(data[i]);
	}
	pair<vector<Object*>, vector<Object*> > p = objectLoader.getObjects();
	m_objects = p.first;
	m_doorObjects = p.second;
	// BACKGROUND
	data = csvReader2.getDomainData("BACKGROUND:");
	for (unsigned int i = 0; i < data.size(); i++) {
		loadBackGround(data[i]);
	}

	setPlayer();
	setFollow();
}

// BGMのロード
void AreaReader::loadBGM(std::map<std::string, std::string> dataMap) {
	ostringstream filePath;
	filePath << "sound/bgm/" << dataMap["name"];
	m_bgmName = filePath.str().c_str();
	m_bgmVolume = stoi(dataMap["volume"]);
}

// キャラクターのロード
void AreaReader::loadCharacter(std::map<std::string, std::string> dataMap) {
	string name = dataMap["name"];
	int x = stoi(dataMap["x"]);
	int y = stoi(dataMap["y"]);
	bool sound = (bool)stoi(dataMap["sound"]);
	int groupId = stoi(dataMap["groupId"]);
	string actionName = dataMap["action"];
	string brainName = dataMap["brain"];
	string controllerName = dataMap["controller"];
	bool cameraFlag = (bool)stoi(dataMap["camera"]);
	bool playerFlag = (bool)stoi(dataMap["player"]);

	// キャラを作成
	Character* character = NULL;
	if (name == "テスト") {
		character = new Heart(name.c_str(), 100, x, y, groupId);
	}
	else if (name == "ハート") {
		character = new Heart(name.c_str(), 100, x, y, groupId);
	}
	else if (name == "シエスタ") {
		character = new Siesta(name.c_str(), 100, x, y, groupId);
	}
	else { // Runner
		character = new Courir(name.c_str(), 100, x, y, groupId);
	}

	// カメラをセット
	if (cameraFlag && m_camera_p == NULL && character != NULL) {
		m_camera_p = new Camera(0, 0, 1.0);
		m_camera_p->setPoint(character->getCenterX(), character->getCenterY());
		m_focusId = character->getId();
	}

	// プレイヤーが操作中のキャラとしてセット
	if (playerFlag && m_playerId == -1 && character != NULL) {
		m_playerId = character->getId();
		m_playerCharacter_p = character;
	}

	// アクションを作成
	CharacterAction* action = NULL;
	SoundPlayer* soundPlayer = sound ? m_soundPlayer_p : NULL;
	if (actionName == "stick") {
		action = new StickAction(character, soundPlayer);
	}
	else if (actionName == "runner") {
		action = new RunnerAction(character, soundPlayer);
	}

	if (action == NULL) { return; }

	// Brainを作成
	Brain* brain = NULL;
	if (brainName == "Keyboard") {
		brain = new KeyboardBrain(m_camera_p);
	}
	else if (brainName == "NormalAI") {
		brain = new NormalAI();
	}
	else if (brainName == "FollowNormalAI") {
		brain = new FollowNormalAI();
	}
	else if (brainName == "FollowParabolaAI") {
		brain = new FollowParabolaAI();
	}
	else if (brainName == "Freeze") {
		brain = new Freeze();
	}
	else if (brainName == "RunnerBrain") {
		brain = new RunnerBrain();
	}

	if (brain == NULL) { return; }

	// コントローラを作成
	CharacterController* controller = NULL;
	if (controllerName == "normal") {
		controller = new NormalController(brain, action);
	}

	if (character != NULL && controller != NULL) { 
		m_characters.push_back(character);
		m_characterControllers.push_back(controller);
	}
}

// 背景のロード
void AreaReader::loadBackGround(std::map<std::string, std::string> dataMap) {
	string graphName = dataMap["graph"];
	string color = dataMap["color"];

	// 背景画像
	if (graphName != "null") {
		ostringstream filePath;
		filePath << "picture/backGround/" << graphName;
		m_backGroundGraph = LoadGraph(filePath.str().c_str());
	}
	else {
		m_backGroundGraph = -1;
	}
	// 背景色
	m_backGroundColor = str2color(color);
}

// プレイヤーの初期位置を、前いたエリアを元に設定
void AreaReader::setPlayer() {
	for (int i = 0; i < m_characters.size(); i++) {
		if (m_playerId == m_characters[i]->getId()) {
			for (int j = 0; j < m_doorObjects.size(); j++) {
				if (m_doorObjects[j]->getAreaNum() == m_fromAreaNum) {
					m_characters[i]->setX(m_doorObjects[j]->getX1());
					m_characters[i]->setY(m_doorObjects[j]->getY2() - m_characters[i]->getHeight());
				}
			}
			break;
		}
	}
}

// follow対象をプレイヤーにする
void AreaReader::setFollow() {
	for (int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getBrain()->needSearchFollow()) {
			m_characterControllers[i]->searchFollowCandidate(m_playerCharacter_p);
		}
	}
}