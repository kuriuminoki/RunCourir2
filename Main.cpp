#include "Control.h"
#include "Define.h"
#include "Game.h"
#include "GameDrawer.h"
#include "DxLib.h"


// �t���X�N���[���Ȃ�FALSE
static int WINDOW = TRUE;
// �}�E�X��\������Ȃ�TRUE
static int MOUSE_DISP = TRUE;

///////fps�̒���///////////////
static int mStartTime;
static int mCount;
static int debug = FALSE;
static float mFps;
static const int N = 60;
static const int FPS = 60;

bool Update() {
	if (mCount == 0) {
		mStartTime = GetNowCount();
	}
	if (mCount == N) {
		int t = GetNowCount();
		mFps = 1000.f / ((t - mStartTime) / (float)N);
		mCount = 0;
		mStartTime = t;
	}
	mCount++;
	return true;
}

void Draw(int x, int y, int color) {
	DrawFormatString(0, 0, WHITE, "�f�o�b�O���[�h�F%.1f FPS", mFps);
}

void Wait() {
	int tookTime = GetNowCount() - mStartTime;
	int waitTime = mCount * 1000 / FPS - tookTime;
	if (waitTime > 0) {
		Sleep(waitTime);
	}
}

//////////���C���֐�///////////////////////
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	SetWindowSizeChangeEnableFlag(TRUE);//window�T�C�Y�ύX�\
	SetUseDirectInputFlag(TRUE);
	SetGraphMode(GAME_WIDE, GAME_HEIGHT, 16);
	ChangeWindowMode(WINDOW), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK);
	//SetAlwaysRunFlag(TRUE);//��ʂ���ɃA�N�e�B�u
	SetMainWindowText("����I�N�[���[���Q");
	////�}�E�X�֘A////
	SetMouseDispFlag(MOUSE_DISP);//�}�E�X�\��
	//SetMousePoint(320, 240);//�}�E�X�J�[�\���̏����ʒu
	SetDrawMode(DX_DRAWMODE_BILINEAR);
	//SetDrawMode(DX_DRAWMODE_NEAREST);
	//�Q�[���{��
	Game game;
	// �Q�[���`��p
	GameDrawer* gameDrawer = new GameDrawer(&game);
	bool title_flag = false;//true�Ȃ�^�C�g����ʏI��
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0)
	{
		updateKey();
		mouseClick();

		/////���C��////
		if (game.play()) {
			delete gameDrawer;
			gameDrawer = new GameDrawer(&game);
		}
		gameDrawer->draw();
		///////////////

		//FPS����
		if (controlDebug() == TRUE) {
			if (debug == FALSE) { debug = TRUE; }
			else { debug = FALSE; }
		}
		Update();
		// �f�o�b�O
		if (debug == TRUE) {
			Draw(0, 0, BLACK);
			game.debug(0, DRAW_FORMAT_STRING_SIZE, BLACK);
		}
		Wait();
		if (controlEsc() == TRUE) { DxLib_End(); }
		//FPS���삱���܂�
	}

	DxLib_End(); // DX���C�u�����I������
	return 0;
}