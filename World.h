#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDE


#include <vector>
#include <string>


class CharacterController;
class CharacterAction;
class Character;
class Object;
class DoorObject;
class Camera;
class Animation;
class SoundPlayer;
class Conversation;
class Brain;
class ObjectLoader;


class World {
private:
	// �����Ȃ�true �w�i���f���[�g���Ȃ�����
	bool m_duplicationFlag;

	// �T�E���h�v���C���[
	SoundPlayer* m_soundPlayer_p;

	// ��b�C�x���g EventElement�N���X������炤
	Conversation* m_conversation_p;

	// �X�L���������̓G���A�Ԃ̈ړ��ł��Ȃ�
	bool m_skillFlag;

	// ��ʂ̖��邳
	int m_brightValue;

	// �J�����Ō��Ă���L������ID
	int m_focusId;

	// �v���C���[�i�n�[�g�j��ID
	int m_playerId;

	// ���ܐ��E�̂ǂ̃G���A�ɂ��邩�i�������ߖ�̂��߃v���C���[�̕t�߂݂̂�ǂݍ��ށj
	int m_areaNum;

	// �`��p�̃J���� World���f���[�g����
	Camera* m_camera;

	// ���E�ɑ��݂���L�����N�^�[ World���f���[�g����
	std::vector<Character*> m_characters;

	// �퓬�̂��߂ɃL�����𓮂����R���g���[�� World���f���[�g����
	std::vector<CharacterController*> m_characterControllers;

	// �ǂ⏰�̃I�u�W�F�N�g World���f���[�g����
	std::vector<Object*> m_stageObjects;

	// �G���A�Ԃ��Ȃ���� World���f���[�g����
	std::vector<Object*> m_doorObjects;

	// �U���̂����蔻��̃I�u�W�F�N�g World���f���[�g����
	std::vector<Object*> m_attackObjects;

	// �G�t�F�N�g���̃A�j���[�V���� World���f���[�g����
	std::vector<Animation*> m_animations;

	// �w�i
	int m_backGroundGraph;
	int m_backGroundColor;

public:
	World(int fromAreaNum, int toAreaNum, SoundPlayer* soundPlayer);
	~World();

	World(const World* original);

	// �Q�b�^
	inline int getFocusId() const { return m_focusId; }
	inline int getPlayerId() const { return m_playerId; }
	inline int getBrightValue() const { return m_brightValue; }
	inline int getAreaNum() const { return m_areaNum; }
	inline const Camera* getCamera() const { return m_camera; }
	std::vector<CharacterController*> getCharacterControllers() const;
	std::vector<const CharacterAction*> getActions() const;
	std::vector<Character*> getCharacters() const;
	std::vector<Object*> getStageObjects() const;
	std::vector<Object*> getDoorObjects() const;
	std::vector<Object*> getAttackObjects() const;
	std::vector<Animation*> getAnimations() const;
	std::vector<const Object*> getFrontObjects() const;
	std::vector<const Object*> getBackObjects() const;
	std::vector<const Animation*> getConstAnimations() const;
	inline const int getBackGroundGraph() const { return m_backGroundGraph; }
	inline const int getBackGroundColor() const { return m_backGroundColor; }
	inline const Conversation* getConversation() const { return m_conversation_p; }
	inline SoundPlayer* getSoundPlayer() const { return m_soundPlayer_p; }

	// �Z�b�^
	inline void setSkillFlag(bool skillFlag) { m_skillFlag = skillFlag; }
	inline void setFocusId(int id) { m_focusId = id; }

	// �X�g�[���[��C�x���g�ɂ��ǉ��I�u�W�F�N�g
	void addObject(ObjectLoader* objectLoader);

	//�f�o�b�O
	void debug(int x, int y, int color) const;

	// �L�����ɐ�킹��
	void battle();

	// �L�����ɉ�b������
	void talk();

	// �L�����̏�Ԃ�ύX
	void asignedCharacterData(const char* name, CharacterData* data);

	// �L�����̏�Ԃ�������
	void asignCharacterData(const char* name, CharacterData* data);

	/*
	* �C�x���g�p
	*/
	Character* getCharacterWithName(std::string characterName) const;
	CharacterController* getControllerWithName(std::string characterName) const;
	Character* getCharacterWithId(int id) const;
	void setBrainWithId(int id, Brain* brain);
	inline void setConversation(Conversation* conversation){ m_conversation_p = conversation; }
	void pushCharacter(Character* character, CharacterController* controller);
	void popCharacter(int id);
	void createRecorder();
	void initRecorder();
	void eraseRecorder();

private:
	// �L�����N�^�[�ƃI�u�W�F�N�g�̓����蔻��
	void atariCharacterAndObject(CharacterController* controller, std::vector<Object*>& objects);

	// �L�����N�^�[�Ɣ��̓����蔻��
	void atariCharacterAndDoor(CharacterController* controller, std::vector<Object*>& objects);

	// �A�j���[�V�����̍X�V
	void updateAnimation();

	// �g�o�O�̃L�����폜
	void cleanCharacterController();

	// �L�����̍X�V�i�U���Ώۂ̕ύX�j
	void updateCharacter();

	// �J�����̍X�V
	void updateCamera();

	// �L�����N�^�[�̓���
	void controlCharacter();

	// �I�u�W�F�N�g�̓���
	void controlObject();

	// �ǂ⏰<->�U���̓����蔻��
	void atariStageAndAttack();

	// �U��<->�U���̓����蔻��
	void atariAttackAndAttack();
};

#endif