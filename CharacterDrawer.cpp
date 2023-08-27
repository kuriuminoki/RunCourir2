#include "CharacterDrawer.h"
#include "Camera.h"
#include "Character.h"
#include "CharacterAction.h"
#include "GraphHandle.h"
#include "DxLib.h"


// �̗̓o�[��\��
void drawHpBar(int x1, int y1, int x2, int y2, int hp, int maxHp, int damageColor, int hpColor) {
	int wide = x2 - x1;
	int damage = wide * (maxHp - hp) / maxHp;
	DrawBox(x1, y1, x1 + damage, y2, damageColor, TRUE);
	DrawBox(x1 + damage, y1, x2, y2, hpColor, TRUE);
}

int CharacterDrawer::HP_COLOR = GetColor(0, 255, 0);
int CharacterDrawer::DAMAGE_COLOR = GetColor(255, 0, 0);

CharacterDrawer::CharacterDrawer(const CharacterAction* const characterAction) {
	m_characterAction = characterAction;
	m_cnt = 0;
}

// �L������`�悷��
void CharacterDrawer::drawCharacter(const Camera* const camera, int bright) {
	// �`�悷��L�����N�^�[
	const Character* character = m_characterAction->getCharacter();

	// �摜�̏��
	const GraphHandle* graphHandle = character->getGraphHandle();

	// ���W�Ɗg�嗦�擾
	int x, y;
	double ex;
	// �摜�̒��S�����W�Ƃ���
	x = character->getX() + (character->getWide() / 2);
	y = character->getY() + (character->getHeight() / 2);
	// �摜�ŗL�̊g�嗦�擾
	ex = graphHandle->getEx();

	// �J�����Œ���
	camera->setCamera(&x, &y, &ex);

	// �`��
	const RunnerGraphHandle* runnerGraph = character->getCharacterGraphHandle()->getRunnerGraphHandle();
	// ���̔�
	if (runnerGraph != nullptr) {
		const GraphHandle* handle = runnerGraph->getBackHair();
		if (handle != nullptr) {
			const int* color = runnerGraph->getHairColor();
			handle->draw(x, y, ex, color[0], color[1], color[2]);
			SetDrawBright(bright, bright, bright);
		}
	}
	// �f��
	if (m_characterAction->getState() == CHARACTER_STATE::DAMAGE && ++m_cnt / 2 % 2 == 1) {
		int dark = max(0, bright - 100);
		SetDrawBright(dark, dark, dark);
		graphHandle->draw(x, y, ex);
		SetDrawBright(bright, bright, bright);
	}
	else {
		graphHandle->draw(x, y, ex);
	}
	// ��A���ځA���A�C�A��O�̔�
	if (runnerGraph != nullptr) {
		const GraphHandle* handle = runnerGraph->getFace();
		if (handle != nullptr) {
			handle->draw(x, y, ex);
			SetDrawBright(bright, bright, bright);
		}
		handle = runnerGraph->getKutu();
		if (handle != nullptr) {
			handle->draw(x, y, ex);
			SetDrawBright(bright, bright, bright);
		}
		handle = runnerGraph->getHuku();
		if (handle != nullptr) {
			handle->draw(x, y, ex);
			SetDrawBright(bright, bright, bright);
		}
		handle = runnerGraph->getEye();
		if (handle != nullptr) {
			const int* color = runnerGraph->getEyeColor();
			handle->draw(x, y, ex, color[0], color[1], color[2]);
			SetDrawBright(bright, bright, bright);
		}
		handle = runnerGraph->getFrontHair();
		if (handle != nullptr) {
			const int* color = runnerGraph->getHairColor();
			handle->draw(x, y, ex, color[0], color[1], color[2]);
			SetDrawBright(bright, bright, bright);
		}
		handle = runnerGraph->getAse();
		if (handle != nullptr) {
			handle->draw(x, y, ex);
			SetDrawBright(bright, bright, bright);
		}
	}

	// �̗̓o�[�̍��W���J�����Œ���
	x = character->getX() + (character->getWide() / 2);
	y = character->getY();
	ex = graphHandle->getEx();
	camera->setCamera(&x, &y, &ex);
	int wide = (int)(HP_BAR_WIDE / 2 * camera->getEx());
	int height = (int)(HP_BAR_HEIGHT * camera->getEx());
	y -= (int)(10 * camera->getEx());
	// �̗͂̕`��
	drawHpBar(x - wide, y - height, x + wide, y, character->getHp(), character->getMaxHp(), DAMAGE_COLOR, HP_COLOR);
}