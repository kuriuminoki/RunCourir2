#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <vector>

class SoundPlayer;
class World;
class Story;
class Character;


// キャラのセーブデータ
class CharacterData {
private:
	const char* m_name;
	int m_hp;
public:
	CharacterData(const char* name);

	// ゲッタ
	inline const char* name() const { return m_name; }
	inline int hp() const { return m_hp; }

	// セッタ
	inline void setHp(int hp) { m_hp = hp; }
};


// セーブデータ
class GameData {
private:
	// セーブする場所
	const char* m_saveFilePath;

	// キャラのデータ
	std::vector<CharacterData> m_characterData;

	// 今いるエリア
	int m_areaNum;

	// 今やっているストーリー
	int m_storyNum;

	// 音量
	int m_soundVolume;

public:
	GameData();
	GameData(const char* saveFilePath);

	// ゲッタ
	inline int getAreaNum() const { return m_areaNum; }
	inline int getStoryNum() const { return m_storyNum; }
	inline int getSoundVolume() const { return m_soundVolume; }

	// セッタ
	inline void setAreaNum(int areaNum) { m_areaNum = areaNum; }
	inline void setStoryNum(int storyNum) { m_storyNum = storyNum; }
	inline void setSoundVolume(int soundVolume) { m_soundVolume = soundVolume; }

	// 自身のデータをWorldにデータ反映させる
	void asignWorld(World* world);

	// Worldのデータを自身に反映させる
	void asignedWorld(World* world);
};



class Game {
private:
	GameData* m_gameData;

	// サウンドプレイヤー
	SoundPlayer* m_soundPlayer;

	// 世界
	World* m_world;

	// ストーリー
	Story* m_story;

public:
	Game();
	~Game();

	// ゲッタ
	World* getWorld() const { return m_world; }

	// デバッグ
	void debug(int x, int y, int color) const;

	// ゲームをプレイする
	bool play();
};

#endif