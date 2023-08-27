#include "DxLib.h"
#include "Control.h"


//////////�L�[�{�[�h�̓��͏�Ԃ��i�[����////////////////

int Key[256]; // �L�[��������Ă���t���[�������i�[����

			  // �L�[�̓��͏�Ԃ��X�V����
int updateKey() {
	char tmpKey[256]; // ���݂̃L�[�̓��͏�Ԃ��i�[����
	GetHitKeyStateAll(tmpKey); // �S�ẴL�[�̓��͏�Ԃ𓾂�
	for (int i = 0; i < 256; i++) {
		if (tmpKey[i] != 0) { // i�Ԃ̃L�[�R�[�h�ɑΉ�����L�[��������Ă�����
			Key[i]++;     // ���Z
		}
		else {              // ������Ă��Ȃ����
			Key[i] = 0;   // 0�ɂ���
		}
	}
	return 0;
}

void mouseLimit(int& hand_x, const int hand_y) {
	if (hand_x < 0 && hand_y > 0 && hand_y < 480) {
		hand_x = 0;
		SetMousePoint(0, hand_y);//�}�E�X�J�[�\���̈ʒu
	}
	else if (hand_x > 640 && hand_y > 0 && hand_y < 480) {
		hand_x = 640;
		SetMousePoint(640, hand_y);//�}�E�X�J�[�\���̈ʒu
	}
}

///////////�}�E�X�̓��͏�Ԃ�Ԃ�//////////////
static int left_cnt = 0;
static int right_cnt = 0;
void mouseClick() {
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) { left_cnt++; }
	else { left_cnt = 0; }
	if ((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0) { right_cnt++; }
	else { right_cnt = 0; }
}
int leftClick() {
	return left_cnt;
}
int rightClick() {
	return right_cnt;
}


// W�L�[�i��L�[�j
int controlW() {
	return Key[KEY_INPUT_W];
}

// S�L�[�i���L�[�j
int controlS() {
	return Key[KEY_INPUT_S];
}

// A�L�[�i���L�[�j
int controlA() {
	return Key[KEY_INPUT_A];
}

// D�L�[�i�E�L�[�j
int controlD() {
	return Key[KEY_INPUT_D];
}

// F�L�[�i�X�L�������j
int controlF() {
	return Key[KEY_INPUT_F];
}

//�f�o�b�O���[�h�N���p
int controlDebug() {
	if (Key[KEY_INPUT_P] == 1) { // P�L�[��������Ă�����
		return TRUE;
	}
	return FALSE;
}

//space�L�[
int controlSpace()
{
	return Key[KEY_INPUT_SPACE];
}

//��Shift�L�[
int controlLeftShift()
{
	return Key[KEY_INPUT_LSHIFT];
}

//�Q�[���I���p
int controlEsc() {
	if (Key[KEY_INPUT_ESCAPE] == 1) { //ESC�L�[��1�J�E���g������Ă�����
		return TRUE;
	}
	return FALSE;
}