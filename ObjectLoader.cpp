#include "ObjectLoader.h"
#include "Object.h"
#include "CsvReader.h"


using namespace std;


// csv�t�@�C������ǂݍ���OBJECT:�h���C������I�u�W�F�N�g���쐬
ObjectLoader::ObjectLoader() {

}

void ObjectLoader::addObject(map<string, string> dataMap) {
	int areaNum = -1;
	if (dataMap.find("area") != dataMap.end()) {
		areaNum = stoi(dataMap["area"]);
	}
	m_objects[areaNum].push_back(dataMap);
}

// ����̃G���A�̒ǉ��I�u�W�F�N�g��vector���擾
pair<vector<Object*>, vector<Object*> > ObjectLoader::getObjects(int areaNum) {

	pair<vector<Object*>, vector<Object*> > res;
	for (unsigned int i = 0; i < m_objects[areaNum].size(); i++) {
		string name = m_objects[areaNum][i]["name"];
		int x1 = stoi(m_objects[areaNum][i]["x1"]);
		int y1 = stoi(m_objects[areaNum][i]["y1"]);
		int x2 = stoi(m_objects[areaNum][i]["x2"]);
		int y2 = stoi(m_objects[areaNum][i]["y2"]);
		string graph = m_objects[areaNum][i]["graph"];
		string color = m_objects[areaNum][i]["color"];
		int hp = stoi(m_objects[areaNum][i]["hp"]);
		string other = m_objects[areaNum][i]["other"];

		int colorHandle = str2color(color);
		Object* object = NULL;
		if (name == "Box") {
			object = new BoxObject(x1, y1, x2, y2, colorHandle, hp);
		}
		else if (name == "Triangle") {
			bool leftDown = false;
			if (other == "leftDown") { leftDown = true; }
			object = new TriangleObject(x1, y1, x2, y2, colorHandle, leftDown, hp);
		}
		if (object != NULL) { res.first.push_back(object); }

		// ���I�u�W�F�N�g�͕ʂɕ�����
		if (name == "Door") {
			graph = "picture/stageMaterial/" + graph;
			res.second.push_back(new DoorObject(x1, y1, x2, y2, graph.c_str(), stoi(other)));
		}
	}
	
	return res;

}