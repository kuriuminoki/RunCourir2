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
	// csvファイルから読み込んだOBJECT:ドメインからオブジェクトを作成
	ObjectLoader();

	void addObject(std::map<std::string, std::string> dataMap);

	// 特定のエリアの追加オブジェクトのvectorを取得
	std::pair<std::vector<Object*>, std::vector<Object*> > getObjects(int areaNum = -1);
};


#endif