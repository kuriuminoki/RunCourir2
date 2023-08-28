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
	int m_maxStamina;
	int m_maxSpeed;
	int m_hairRGB[3];
	char m_kutu;
	char m_huku;
public:
	CharacterData(const char* name);

	// ゲッタ
	inline const char* name() const { return m_name; }
	inline int hp() const { return m_hp; }
	inline int maxStamina() const { return m_maxStamina; }
	inline int maxSpeed() const { return m_maxSpeed; }
	inline void hairRGB(int hairRGB[3]) const { 
		hairRGB[0] = m_hairRGB[0];
		hairRGB[1] = m_hairRGB[1];
		hairRGB[2] = m_hairRGB[2];
	}
	inline char kutu() const { return m_kutu; }
	inline char huku() const { return m_huku; }

	// セッタ
	inline void setHp(int hp) { m_hp = hp; }
	inline void setMaxStamina(int maxStamina) { m_maxStamina = maxStamina; }
	inline void setMaxSpeed(int maxSpeed) { m_maxSpeed = maxSpeed; }
	inline void set(int hairRGB[3]) { 
		m_hairRGB[0] = hairRGB[0];
		m_hairRGB[1] = hairRGB[1];
		m_hairRGB[2] = hairRGB[2];
	}
	inline void setKutu(char kutu) { m_kutu = kutu; }
	inline void setHuku(char huku) { m_huku = huku; }
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

	// 所持金
	int m_money;
	bool kutu[3];
	bool huku[3];

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