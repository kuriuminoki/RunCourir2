#ifndef SOUND_H_INCLUDED
#define SOUND_H_INCLUDED

#include <queue>
#include <string>

// �J�����̃p���𒲐�����p
int adjustPanSound(int x, int cameraX);

class SoundPlayer {
private:
	// ���W �p���̐ݒ�Ɏg��
	int m_cameraX;

	// ����
	int m_volume;

	// BGM�̖��O
	std::string m_bgmName;

	// BGM
	int m_bgmHandle;

	// �Đ��\��̌��ʉ�
	std::queue<std::pair<int, int> > m_soundQueue;

public:
	SoundPlayer();
	~SoundPlayer();

	void setVolume(int volume);
	inline int getVolume() const { return m_volume; }
	inline int getCameraX() const { return m_cameraX; }
	inline void setCameraX(int cameraX) { m_cameraX = cameraX; }

	// BGM���Z�b�g�i�ύX�j
	void setBGM(std::string bgmName, int volume = 100);

	// �������Ă��鉹�y���`�F�b�N����
	inline bool sameBGM(std::string bgmName) const { return m_bgmName == bgmName; }

	// BGM���Đ�
	void playBGM();

	// BGM���X�g�b�v
	void stopBGM();

	// ���ʉ��̍Đ��ҋ@��փv�b�V��
	void pushSoundQueue(int soundHandle, int panPal = 0);

	// ���ʉ���炷
	void play();
};

#endif