#ifndef EVENT_H_INCLUDED
#define EVENT_H_INCLUDED

#include <vector>
#include <string>


class World;
class SoundPlayer;
class CharacterController;
class Character;
class Conversation;


enum class EVENT_RESULT {
	NOW,
	SUCCESS,
	FAILURE
};


/*
* �C�x���g�̔��Ώ��� World�ɕω��������Ȃ�
*/
class EventFire {
protected:

	// �C�x���g���N����World
	const World* m_world_p;

public:
	EventFire(World* world);

	virtual bool fire() = 0;

	virtual void setWorld(World* world) { m_world_p = world; }
};


/*
* �C�x���g���\������v�f
*/
class EventElement {
protected:

	// �C�x���g���N���Ă���World
	World* m_world_p;

public:
	EventElement(World* world);
	virtual EVENT_RESULT play() = 0;

	// �n�[�g�̃X�L���������\���ǂ���
	virtual bool skillAble() = 0;

	virtual void setWorld(World* world) { m_world_p = world; }
};


/*
* �C�x���g
* EventElement���Ǘ�����
*/
class Event {
private:

	// ���Ό�ɂ�����g����Element����
	World* m_world_p;
	SoundPlayer* m_soundPlayer;

	// �C�x���g�ԍ�
	int m_eventNum;

	// �C�x���g�̔��Ώ���
	std::vector<EventFire*> m_eventFire;

	// �C�x���g�̗v�f
	std::vector<EventElement*> m_eventElement;

	// �C�x���g�̐i��(EventElement�̃C���f�b�N�X)
	int m_nowElement;

public:
	Event(int eventNum, World* world, SoundPlayer* soundPlayer);
	~Event();

	// ����
	bool fire();

	// �C�x���g�i�s
	EVENT_RESULT play();

	// ���n�[�g�̃X�L�������\���ǂ���
	bool skillAble();

	// World��ݒ肵�Ȃ���
	void setWorld(World* world);

private:
	void createFire(std::vector<std::string> param, World* world, SoundPlayer* soundPlayer);
	void createElement(std::vector<std::string> param, World* world, SoundPlayer* soundPlayer);
};


/*
* EventFire�̔h���N���X
*/
// ����̃L�������w�肵�����W�ɂ���
class CharacterPointFire :
	public	EventFire
{
private:
	// �p�����[�^
	std::vector<std::string> m_param;

	// �L����
	Character* m_character_p;

	// �G���A�ԍ�
	int m_areaNum;

	// �ڕW���W
	int m_x, m_y;

	// ���W�̂���̋��e
	int m_dx, m_dy;

public:
	CharacterPointFire(World* world, std::vector<std::string> param);

	bool fire();

	// ���E��ݒ肵�Ȃ���
	void setWorld(World* world);
};


/*
* EventElement�̔h���N���X
*/
// �L������AI��ς���
class ChangeBrainEvent :
	public EventElement
{
private:

	// �p�����[�^
	std::vector<std::string> m_param;

	// Brain�̃N���X��
	std::string m_brainName;

	// �Ώۂ̃L����
	CharacterController* m_controller_p;

public:
	ChangeBrainEvent(World* world, std::vector<std::string> param);

	EVENT_RESULT play();

	// �n�[�g�̃X�L���������\���ǂ���
	bool skillAble() { return false; }

	// ���E��ݒ肵�Ȃ���
	void setWorld(World* world);
};

// �L������GroupID��ς���
class ChangeGroupEvent :
	public EventElement
{
private:

	// �p�����[�^
	std::vector<std::string> m_param;

	int m_groupId;

	// �Ώۂ̃L����
	Character* m_character_p;

public:
	ChangeGroupEvent(World* world, std::vector<std::string> param);

	EVENT_RESULT play();

	// �n�[�g�̃X�L���������\���ǂ���
	bool skillAble() { return false; }

	// ���E��ݒ肵�Ȃ���
	void setWorld(World* world);
};

// ����̃L������HP��0�ɂȂ�܂Ő키
class DeadCharacterEvent :
	public EventElement
{
private:

	// �p�����[�^
	std::vector<std::string> m_param;

	// �Ώۂ̃L����
	Character* m_character_p;

public:
	DeadCharacterEvent(World* world, std::vector<std::string> param);

	EVENT_RESULT play();

	// �n�[�g�̃X�L���������\���ǂ���
	bool skillAble() { return true; }

	// ���E��ݒ肵�Ȃ���
	void setWorld(World* world);
};

// ����̃O���[�v���S�ł���܂Ő키
class DeadGroupEvent :
	public EventElement
{
private:

	// �Ώۂ̃O���[�v
	int m_groupId;

public:
	DeadGroupEvent(World* world, std::vector<std::string> param);

	EVENT_RESULT play();

	// �n�[�g�̃X�L���������\���ǂ���
	bool skillAble() { return true; }
};

// ��b�C�x���g
class TalkEvent :
	public EventElement
{
private:

	Conversation* m_conversation;

public:
	TalkEvent(World* world, SoundPlayer* soundPlayer, std::vector<std::string> param);
	~TalkEvent();

	EVENT_RESULT play();

	// �n�[�g�̃X�L���������\���ǂ���
	bool skillAble() { return false; }
};

#endif