#include "Text.h"
#include "World.h"
#include "GraphHandle.h"
#include "Control.h"
#include "Sound.h"
#include "Character.h"
#include "DxLib.h"
#include <algorithm>
#include <string>
#include <sstream>


using namespace std;


Conversation::Conversation(int textNum, World* world, SoundPlayer* soundPlayer) {

	m_finishCnt = 0;
	m_finishFlag = false;
	m_world_p = world;
	m_soundPlayer_p = soundPlayer;
	m_speakerName = "";
	m_speakerGraph = NULL;
	m_text = "";
	m_textNow = 0;
	m_cnt = 0;
	m_textSpeed = TEXT_SPEED;

	// 効果音
	m_displaySound = LoadSoundMem("sound/text/display.wav");
	m_nextSound = LoadSoundMem("sound/text/next.wav");

	// 対象のファイルを開く
	ostringstream oss;
	oss << "data/text/text" << textNum << ".txt";
	m_fp = FileRead_open(oss.str().c_str());
	setNextText();

}

Conversation::~Conversation() {
	// 効果音削除
	DeleteSoundMem(m_displaySound);
	DeleteSoundMem(m_nextSound);
	// ファイルを閉じる
	FileRead_close(m_fp);
}

// テキストを返す（描画用）
std::string Conversation::getText() const {
	return m_text.substr(0, m_textNow);
}

// 画像を返す（描画用）
GraphHandle* Conversation::getGraph() const {
	int size = (int)m_speakerGraph->getSize();
	int index = size - (m_textNow / 2 % size) - 1;
	index = m_textNow == (unsigned int)m_text.size() ? 0 : index;
	return m_speakerGraph->getGraphHandle(index);
}

int Conversation::getTextSize() const {
	return (int)m_text.size();
}

bool Conversation::play() {

	// 終了処理
	if (m_finishCnt > 0) {
		m_finishCnt++;
		if (m_finishCnt == FINISH_COUNT) {
			m_finishFlag = true;
			return true;
		}
		return false;
	}

	// プレイヤーからのアクション（スペースキー入力）
	if (controlSpace() == 1 || leftClick() == 1) {
		if (m_textNow == m_text.size()) {
			// 全ての会話が終わった
			if (FileRead_eof(m_fp) != 0) {
				m_finishCnt++;
				return false;
			}
			// 次のテキストへ移る
			setNextText();
			// 効果音
			m_soundPlayer_p->pushSoundQueue(m_nextSound);
		}
		else if(m_cnt > MOVE_FINAL_ABLE) {
			// 最後までテキストを飛ばす
			m_textNow = (unsigned int)m_text.size();
		}
	}

	// 表示文字を増やす
	m_cnt++;
	if (m_cnt % m_textSpeed == 0 && m_textNow < m_text.size()) {
		// 日本語表示は１文字がサイズ２分
		m_textNow = min(m_textNow + 2, (unsigned int)m_text.size());
		// 効果音
		m_soundPlayer_p->pushSoundQueue(m_displaySound);
	}

	return false;
}

void Conversation::setNextText() {
	m_cnt = 0;
	m_textNow = 0;
	// バッファ
	const int size = 512;
	char buff[size];
	// 発言者
	FileRead_gets(buff, size, m_fp);
	m_speakerName = buff;
	// 画像
	FileRead_gets(buff, size, m_fp);
	setSpeakerGraph(buff);
	// テキスト
	FileRead_gets(buff, size, m_fp);
	m_text = buff;

	if (FileRead_eof(m_fp) == 0) {
		FileRead_gets(buff, size, m_fp);
		string s = buff;
		if (s == "") {
			m_textSpeed = TEXT_SPEED;
		}
		else {
			m_textSpeed = stoi(s);
			FileRead_gets(buff, size, m_fp);
		}
	}
	else {
		m_textSpeed = TEXT_SPEED;
	}
}

void Conversation::setSpeakerGraph(const char* faceName) {
	Character* c = m_world_p->getCharacterWithName(m_speakerName);
	m_speakerGraph = c->getFaceHandle()->getGraphHandle(faceName);
}