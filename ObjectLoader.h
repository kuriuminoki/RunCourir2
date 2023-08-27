#ifndef OBJECT_LOADER_H_INCLUDED
#define OBJECT_LOADER_H_INCLUDED


#include <map>
#include <vector>
#include <string>
#include <utility>


class Object;


class ObjectLoader {
private:
	std::map<int, std::vector<std::map<std::string, std::string> > > m_objects;

public:
	// csv�t�@�C������ǂݍ���OBJECT:�h���C������I�u�W�F�N�g���쐬
	ObjectLoader();

	void addObject(std::map<std::string, std::string> dataMap);

	// ����̃G���A�̒ǉ��I�u�W�F�N�g��vector���擾
	std::pair<std::vector<Object*>, std::vector<Object*> > getObjects(int areaNum = -1);
};


#endif