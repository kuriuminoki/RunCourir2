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


// �F�̖��O����F�n���h�����擾
int str2color(string colorName) {
	if (colorName == "white") { return WHITE; }
	else if (colorName == "gray") { return GRAY; }
	else if (colorName == "lightBlue") { return LIGHT_BLUE; }
	return -1;
}


// ��s���̃e�L�X�g�����s���̃f�[�^�ɕϊ�
vector<string> csv2vector(string buff) {
	vector<string> data;
	string oneCell = "";

	// 1���������Ă���
	for (int i = 0; buff[i] != '\0'; i++) {
		// CSV�t�@�C���Ȃ̂ŃJ���}�ŋ�؂���oneData��push_back
		if (buff[i] == ',') {
			data.push_back(oneCell);
			oneCell = "";
		}
		else { // �J���}�ȊO�̕����Ȃ獇��
			oneCell += buff[i];
		}
	}
	// �Ō�̃Z���ɂ̓J���}���Ȃ�
	data.push_back(oneCell);

	return data;
}


// �t�@�C�������w�肵��CSV�t�@�C����ǂݍ���
CsvReader::CsvReader(const char* fileName) {

	// �t�@�C���|�C���^
	int fp;

	// �o�b�t�@
	const int size = 512;
	char buff[size];

	// �t�@�C�����J��
	fp = FileRead_open(fileName);

	// �o�b�t�@�Ɉ�s�ځi�J�������j�̃e�L�X�g������
	FileRead_gets(buff, size, fp);

	// �J�������̃��X�g���擾
	m_columnNames = csv2vector(buff);

	// �t�@�C���̏I�[�܂Ń��[�v
	while (FileRead_eof(fp) == 0) {
		// ��������o�b�t�@�Ɉ�s���̃e�L�X�g������
		FileRead_gets(buff, size, fp);

		// ��s���̃e�L�X�g���f�[�^�ɂ���Vector�ɕϊ�
		vector<string> oneDataVector;
		oneDataVector = csv2vector(buff);

		// vector�ƃJ���������g����map�𐶐�
		map<string, string> oneData;
		for (int i = 0; i < oneDataVector.size(); i++) {
			oneData[m_columnNames[i]] = oneDataVector[i];
		}

		// ��s����map�f�[�^��push_back
		m_data.push_back(oneData);
	}

	// �t�@�C�������
	FileRead_close(fp);
}


/*
* �J��������value�̃f�[�^���擾
* ��FfindOne("name", "�L������");
* ������Ȃ��������̃f�[�^��Ԃ�
*/
map<string, string> CsvReader::findOne(const char* columnName, const char* value) {

	// m_data[i][columnName] == value�ƂȂ�i�𒲂ׂ�
	map<string, string>::iterator ite;
	for (int i = 0; i < m_data.size(); i++) {

		// �J�������ɑΉ�����l���擾
		ite = m_data[i].find(columnName);

		// ���̃J�����������݂��Ȃ�
		if (ite == m_data[i].end()) { continue; }

		// �����Ɉ�v
		if (ite->second == value) {
			return m_data[i];
		}
	}

	// ��̃f�[�^��Ԃ�
	map<string, string> res;
	return res;
}


/*
* �S�f�[�^��Ԃ�
*/
vector<map<string, string>> CsvReader::getData() const {
	return m_data;
}


// �t�@�C�������w�肵��CSV�t�@�C����ǂݍ���
CsvReader2::CsvReader2(const char* fileName) {

	// �t�@�C���|�C���^
	int fp;

	// �o�b�t�@
	const int size = 512;
	char buff[size];

	// �t�@�C�����J��
	fp = FileRead_open(fileName);

	string domainName = "";
	// �t�@�C���̏I�[�܂Ń��[�v
	while (FileRead_eof(fp) == 0) {
		// ��������o�b�t�@�Ɉ�s���̃e�L�X�g������
		FileRead_gets(buff, size, fp);

		// ��s���̃e�L�X�g���f�[�^�ɂ���Vector�ɕϊ�
		vector<string> oneDataVector = csv2vector(buff);

		if (oneDataVector[1] == "") {
			domainName = oneDataVector[0];
			FileRead_gets(buff, size, fp);
			m_columnNames[domainName] = csv2vector(buff);
		}
		else {
			// vector�ƃJ���������g����map�𐶐�
			map<string, string> oneData;
			for (int i = 0; i < oneDataVector.size(); i++) {
				oneData[m_columnNames[domainName][i]] = oneDataVector[i];
			}

			// ��s����map�f�[�^��push_back
			m_data[domainName].push_back(oneData);
		}
	}

	// �t�@�C�������
	FileRead_close(fp);
}


/*
* �h���C������domainName�̃f�[�^����A
* �J��������value�̃f�[�^���擾
* ��FfindOne("name", "�L������");
* ������Ȃ��������̃f�[�^��Ԃ�
*/
map<string, string> CsvReader2::findOne(const char* domainName, const char* columnName, const char* value) {

	// m_data[i][columnName] == value�ƂȂ�i�𒲂ׂ�
	map<string, string>::iterator ite;
	for (int i = 0; i < m_data.size(); i++) {

		// �J�������ɑΉ�����l���擾
		ite = m_data[domainName][i].find(columnName);

		// ���̃J�����������݂��Ȃ�
		if (ite == m_data[domainName][i].end()) { continue; }

		// �����Ɉ�v
		if (ite->second == value) {
			return m_data[domainName][i];
		}
	}

	// ��̃f�[�^��Ԃ�
	map<string, string> res;
	return res;
}


/*
* area/area?.csv����L�����N�^�[��I�u�W�F�N�g�����[�h����
* Character����new���邽�߁A���̃N���X��new�������get���č폜���邱�ƁB
* ���̃N���X��new���ꂽCharacter���͂��̃N���X�ō폜���Ȃ��B
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

	// �t�@�C�����J��
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

// BGM�̃��[�h
void AreaReader::loadBGM(std::map<std::string, std::string> dataMap) {
	ostringstream filePath;
	filePath << "sound/bgm/" << dataMap["name"];
	m_bgmName = filePath.str().c_str();
	m_bgmVolume = stoi(dataMap["volume"]);
}

// �L�����N�^�[�̃��[�h
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

	// �L�������쐬
	Character* character = NULL;
	if (name == "�e�X�g") {
		character = new Heart(name.c_str(), 100, x, y, groupId);
	}
	else if (name == "�n�[�g") {
		character = new Heart(name.c_str(), 100, x, y, groupId);
	}
	else if (name == "�V�G�X�^") {
		character = new Siesta(name.c_str(), 100, x, y, groupId);
	}
	else { // Runner
		character = new Courir(name.c_str(), 100, x, y, groupId);
	}

	// �J�������Z�b�g
	if (cameraFlag && m_camera_p == NULL && character != NULL) {
		m_camera_p = new Camera(0, 0, 1.0);
		m_camera_p->setPoint(character->getCenterX(), character->getCenterY());
		m_focusId = character->getId();
	}

	// �v���C���[�����쒆�̃L�����Ƃ��ăZ�b�g
	if (playerFlag && m_playerId == -1 && character != NULL) {
		m_playerId = character->getId();
		m_playerCharacter_p = character;
	}

	// �A�N�V�������쐬
	CharacterAction* action = NULL;
	SoundPlayer* soundPlayer = sound ? m_soundPlayer_p : NULL;
	if (actionName == "stick") {
		action = new StickAction(character, soundPlayer);
	}
	else if (actionName == "runner") {
		action = new RunnerAction(character, soundPlayer);
	}

	if (action == NULL) { return; }

	// Brain���쐬
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

	if (brain == NULL) { return; }

	// �R���g���[�����쐬
	CharacterController* controller = NULL;
	if (controllerName == "normal") {
		controller = new NormalController(brain, action);
	}

	if (character != NULL && controller != NULL) { 
		m_characters.push_back(character);
		m_characterControllers.push_back(controller);
	}
}

// �w�i�̃��[�h
void AreaReader::loadBackGround(std::map<std::string, std::string> dataMap) {
	string graphName = dataMap["graph"];
	string color = dataMap["color"];

	// �w�i�摜
	if (graphName != "null") {
		ostringstream filePath;
		filePath << "picture/backGround/" << graphName;
		m_backGroundGraph = LoadGraph(filePath.str().c_str());
	}
	else {
		m_backGroundGraph = -1;
	}
	// �w�i�F
	m_backGroundColor = str2color(color);
}

// �v���C���[�̏����ʒu���A�O�����G���A�����ɐݒ�
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

// follow�Ώۂ��v���C���[�ɂ���
void AreaReader::setFollow() {
	for (int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getBrain()->needSearchFollow()) {
			m_characterControllers[i]->searchFollowCandidate(m_playerCharacter_p);
		}
	}
}