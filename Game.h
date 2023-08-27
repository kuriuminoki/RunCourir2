#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <vector>

class SoundPlayer;
class World;
class Story;
class Character;


// �L�����̃Z�[�u�f�[�^
class CharacterData {
private:
	const char* m_name;
	int m_hp;
public:
	CharacterData(const char* name);

	// �Q�b�^
	inline const char* name() const { return m_name; }
	inline int hp() const { return m_hp; }

	// �Z�b�^
	inline void setHp(int hp) { m_hp = hp; }
};


// �Z�[�u�f�[�^
class GameData {
private:
	// �Z�[�u����ꏊ
	const char* m_saveFilePath;

	// �L�����̃f�[�^
	std::vector<CharacterData> m_characterData;

	// ������G���A
	int m_areaNum;

	// ������Ă���X�g�[���[
	int m_storyNum;

	// ����
	int m_soundVolume;

public:
	GameData();
	GameData(const char* saveFilePath);

	// �Q�b�^
	inline int getAreaNum() const { return m_areaNum; }
	inline int getStoryNum() const { return m_storyNum; }
	inline int getSoundVolume() const { return m_soundVolume; }

	// �Z�b�^
	inline void setAreaNum(int areaNum) { m_areaNum = areaNum; }
	inline void setStoryNum(int storyNum) { m_storyNum = storyNum; }
	inline void setSoundVolume(int soundVolume) { m_soundVolume = soundVolume; }

	// ���g�̃f�[�^��World�Ƀf�[�^���f������
	void asignWorld(World* world);

	// World�̃f�[�^�����g�ɔ��f������
	void asignedWorld(World* world);
};



class Game {
private:
	GameData* m_gameData;

	// �T�E���h�v���C���[
	SoundPlayer* m_soundPlayer;

	// ���E
	World* m_world;

	// �X�g�[���[
	Story* m_story;

public:
	Game();
	~Game();

	// �Q�b�^
	World* getWorld() const { return m_world; }

	// �f�o�b�O
	void debug(int x, int y, int color) const;

	// �Q�[�����v���C����
	bool play();
};

#endif