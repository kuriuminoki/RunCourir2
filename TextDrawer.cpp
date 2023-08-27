#include "TextDrawer.h"
#include "Text.h"
#include "GraphHandle.h"
#include "Define.h"
#include "DxLib.h"
#include <string>


using namespace std;


ConversationDrawer::ConversationDrawer(Conversation* conversation) {

	m_conversation = conversation;

	// �t�H���g�f�[�^
	m_textHandle = CreateFontToHandle(NULL, TEXT_SIZE, 3);
	m_nameHandle = CreateFontToHandle(NULL, NAME_SIZE, 5);

	// �摜
	m_frameHandle = LoadGraph("picture/textMaterial/frame.png");
}

ConversationDrawer::~ConversationDrawer() {
	// �t�H���g�f�[�^�폜
	DeleteFontToHandle(m_textHandle);
	DeleteFontToHandle(m_nameHandle);
	// �摜�f�[�^�폜
	DeleteGraph(m_frameHandle);
}

void ConversationDrawer::draw() {
	string text = m_conversation->getText();
	string name = m_conversation->getSpeakerName();
	GraphHandle* graph = m_conversation->getGraph();
	int graphSize = 0;
	GetGraphSize(graph->getHandle(), &graphSize, &graphSize);

	// �[�̗]��
	static const int EDGE = 52;

	// �t�L�_�V�̃t�`�̕�
	static const int TEXT_GRAPH_EDGE = 32;

	// ��[
	static const int Y1 = 551;

	// ��b�I����
	int finishCnt = m_conversation->getFinishCnt() * 8;
	if (finishCnt > 0) {
		// �t�L�_�V
		DrawExtendGraph(EDGE, Y1 + finishCnt, GAME_WIDE - EDGE, GAME_HEIGHT - EDGE - finishCnt, m_frameHandle, TRUE);
		return;
	}

	// �t�L�_�V
	DrawExtendGraph(EDGE, Y1, GAME_WIDE - EDGE, GAME_HEIGHT - EDGE, m_frameHandle, TRUE);

	// ���O
	DrawStringToHandle(EDGE * 2 + graphSize, GAME_HEIGHT - EDGE - NAME_SIZE - TEXT_GRAPH_EDGE, name.c_str(), BLACK, m_nameHandle);

	// �e�L�X�g
	int now = 0;
	int i = 0;
	while (now < text.size()) {
		int next = now + min(MAX_TEXT_LEN, (int)text.size() - now);
		string disp = text.substr(now, next - now);
		DrawStringToHandle(EDGE * 3 + graphSize, Y1 + 5 + EDGE + (i * (TEXT_SIZE + 10)), disp.c_str(), BLACK, m_textHandle);
		now = next;
		i++;
	}

	// �L�����̊�摜
	graph->draw(EDGE + TEXT_GRAPH_EDGE + graphSize / 2, Y1 + TEXT_GRAPH_EDGE + graphSize / 2, 1.0);

}