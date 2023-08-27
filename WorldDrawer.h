#ifndef WORLD_DRAWER_H_INCLUDED
#define WORLD_DRAWER_H_INCLUDED

class World;
class CharacterDrawer;
class ObjectDrawer;
class AnimationDrawer;
class ConversationDrawer;


// ���E��`�悷��
class WorldDrawer {
private:

	// �`�悷�鐢�E
	const World* m_world;

	// �L�����`��p
	CharacterDrawer* m_characterDrawer;

	// �I�u�W�F�N�g�`��p
	ObjectDrawer* m_objectDrawer;

	// �A�j���[�V�����`��p
	AnimationDrawer* m_animationDrawer;

	// ��b�C�x���g
	ConversationDrawer* m_conversationDrawer;

public:
	WorldDrawer(const World* world);

	~WorldDrawer();

	// �Z�b�^
	void setWorld(World* world) { m_world = world; }

	// �`�悷��
	void draw();
};

#endif