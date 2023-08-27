#ifndef CHARACTER_DRAWER_H_INCLUDED
#define CHARACTER_DRAWER_H_INCLUDED

class Camera;
class CharacterAction;

// CharacterAction�����ăL������`�悷��
class CharacterDrawer {
private:
	// �L�����̓����̏�� const�֐������Ă΂Ȃ�
	const CharacterAction* m_characterAction;

	const int HP_BAR_WIDE = 200;
	const int HP_BAR_HEIGHT = 20;
	static int HP_COLOR;
	static int DAMAGE_COLOR;

	// �_�ŗp
	int m_cnt;

public:
	CharacterDrawer(const CharacterAction* const characterAction);

	// �Z�b�^
	void setCharacterAction(const CharacterAction* action) { m_characterAction = action; }

	void drawCharacter(const Camera* const camera, int bright = 255);
};

#endif