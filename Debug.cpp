#include "CharacterController.h"
#include "CharacterAction.h"
#include "Character.h"
#include "Object.h"
#include "World.h"
#include "Game.h"
#include "GraphHandle.h"
#include "Animation.h"
#include "Define.h"
#include "Text.h"
#include "Event.h"
#include "Story.h"
#include "Brain.h"
#include "DxLib.h"

/*
* Game
*/
// Game�N���X�̃f�o�b�O
void Game::debug(int x, int y, int color) const {
	DrawFormatString(x, y, color, "**GAME**");
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE, color, "StoryNum=%d", m_gameData->getStoryNum());
	//m_story->debug(x + DRAW_FORMAT_STRING_SIZE, y + DRAW_FORMAT_STRING_SIZE * 2, color);
	m_world->debug(x + DRAW_FORMAT_STRING_SIZE, y + DRAW_FORMAT_STRING_SIZE * 3, color);
}


/*
* World
*/
void debugObjects(int x, int y, int color, std::vector<Object*> objects) {
	for (unsigned int i = 0; i < objects.size(); i++) {
		objects[i]->debug(x + 500, y + DRAW_FORMAT_STRING_SIZE * i * 4, color);
	}
}

// World�N���X�̃f�o�b�O
void World::debug(int x, int y, int color) const {
	DrawFormatString(x, y, color, "**World**");
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE, color, "CharacterSum=%d, ControllerSum=%d, anime=%d", m_characters.size(), m_characterControllers.size(), m_animations.size());
	debugObjects(x, y + DRAW_FORMAT_STRING_SIZE * 2, color, m_attackObjects);
}


/*
* Story
*/
void Story::debug(int x, int y, int color) {
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE, color, "**Story**");
}


/*
* Controller
*/
// CharacterController�N���X�̃f�o�b�O
void CharacterController::debugController(int x, int y, int color) const {
	DrawFormatString(x, y, color, "**CharacterController**");
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE, color, "slashOrder=%d, slashCnt=%d, bulletOrder=%d, bulletCnt=%d", m_brain->slashOrder(), m_characterAction->getSlashCnt(), m_brain->bulletOrder(), m_characterAction->getBulletCnt());
	m_brain->debug(x + DRAW_FORMAT_STRING_SIZE, y + DRAW_FORMAT_STRING_SIZE * 2, color);
	m_characterAction->debug(x + DRAW_FORMAT_STRING_SIZE, y + DRAW_FORMAT_STRING_SIZE * 4, color);
}

// NormalController�N���X�̃f�o�b�O
void NormalController::debug(int x, int y, int color) const {
	DrawFormatString(x, y, color, "**CharacterKeyboardController**");
	debugController(x + DRAW_FORMAT_STRING_SIZE, y + DRAW_FORMAT_STRING_SIZE * 2, color);
}

void KeyboardBrain::debug(int x, int y, int color) const {
	DrawFormatString(x, y, color, "**KeyboardBrain**");
}

void NormalAI::debug(int x, int y, int color) const {
	DrawFormatString(x, y, color, "**NormalAI**");
}

void FollowNormalAI::debug(int x, int y, int color) const {
	DrawFormatString(x, y, color, "**FollowNormalAI**");
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE, color, "follow=%d, gx,gy=%d,%d", getFollowId(), m_gx, m_gy);
}


/*
* Action
*/
// Action�N���X�̃f�o�b�O
void CharacterAction::debugAction(int x, int y, int color) const {
	DrawFormatString(x, y, color, "**CharacterAction**");
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE, color, "state=%d, grand=%d(%d%d), (vx,vy)=(%d,%d), runCnt=%d", (int)m_state, m_grand, m_grandLeftSlope, m_grandRightSlope, m_vx, m_vy, m_runCnt);
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE * 2, color, "�������F(��, ��, ��, ��)=(%d,%d,%d,%d)", m_leftLock, m_rightLock, m_upLock, m_downLock);
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE * 3, color, "�ړ����F(��, ��, ��, ��)=(%d,%d,%d,%d)", m_moveLeft, m_moveRight, m_moveUp, m_moveDown);
	m_character_p->debug(x + DRAW_FORMAT_STRING_SIZE, y + DRAW_FORMAT_STRING_SIZE * 4, color);
}

// StickAction�̃f�o�b�O
void StickAction::debug(int x, int y, int color) const {
	DrawFormatString(x, y, color, "**StickAction**");
	debugAction(x + DRAW_FORMAT_STRING_SIZE, y + DRAW_FORMAT_STRING_SIZE, color);
}


/*
* Character
*/
// Character�N���X�̃f�o�b�O
void Character::debugCharacter(int x, int y, int color) const {
	DrawFormatString(x, y, color, "**Character**");
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE, color, "(x,y)=(%d,%d), left=%d, id=%d, groupId=%d, m_handle=%d", m_x, m_y, m_leftDirection, m_id, m_groupId, m_graphHandle->getHandle());
	// DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE * 2, color, "(wide, height)=(%d,%d), handle=%d", m_wide, m_height, m_graphHandle->getHandle());
	// �摜
	// m_graphHandle->draw(GAME_WIDE - (m_wide / 2), (m_height / 2), m_graphHandle->getEx());
	// DrawBox(m_x, m_y, m_x + m_wide, m_y + m_height, color, TRUE);
	// DrawBox(m_x + 10, m_y + (m_height / 2) - 20, m_x + m_wide - 10, m_y + (m_height / 2) + 20, BLACK, TRUE);
	// DrawFormatString(m_x + 10, m_y + (m_height / 2), WHITE, "[%s]", m_characterInfo->name().c_str());
}

// Heart�N���X�̃f�o�b�O
void Heart::debug(int x, int y, int color) const {
	DrawFormatString(x, y, color, "**Heart**");
	// DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE, color, "standHandle=%d", m_standHandle);
	debugCharacter(x + DRAW_FORMAT_STRING_SIZE, y + DRAW_FORMAT_STRING_SIZE * 2, color);
}
void Courir::debug(int x, int y, int color) const {
	DrawFormatString(x, y, color, "**Courir(Runner)**");
	// DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE, color, "standHandle=%d", m_standHandle);
	debugCharacter(x + DRAW_FORMAT_STRING_SIZE, y + DRAW_FORMAT_STRING_SIZE * 2, color);
}


/*
* Object
*/
// Object�̃f�o�b�O
void Object::debugObject(int x, int y, int color) const {
	DrawFormatString(x, y, color, "**Object**");
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE, color, "((x1,y1),(x2,y2))=((%d,%d),(%d,%d))", m_x1, m_y1, m_x2, m_y2);
	// DrawBox(m_x1, m_y1, m_x2, m_y2, color, FALSE);
}

/*
* �����`�̏�Q��
*/
void BoxObject::debug(int x, int y, int color) const {
	DrawFormatString(x, y, color, "**BoxObject**");
	debugObject(x + DRAW_FORMAT_STRING_SIZE, y + DRAW_FORMAT_STRING_SIZE, color);
	// DrawBox(m_x1, m_y1, m_x2, m_y2, RED, FALSE);
}

/*
* ���p�O�p�`�̏�Q��
*/
void TriangleObject::debug(int x, int y, int color) const {
	DrawFormatString(x, y, color, "**TriangleObject**");
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE, color, "mideleY=%d", getY(m_x2 - m_x1));
	debugObject(x + DRAW_FORMAT_STRING_SIZE, y + DRAW_FORMAT_STRING_SIZE * 2, color);
	//if (m_leftDown) {
	//	DrawTriangle(m_x2, m_y1, m_x2, m_y2, m_x1, m_y2, RED, FALSE);
	//}
	//else {
	//	DrawTriangle(m_x1, m_y1, m_x2, m_y2, m_x1, m_y2, RED, FALSE);
	//}
}

/*
* �ȉ~�̒e��
*/
void BulletObject::debug(int x, int y, int color) const {
	DrawFormatString(x, y, color, "**BulletObject**");
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE, color, "(gx,gy)=(%d,%d), groupId=%d", m_gx, m_gy, m_groupId);
	debugObject(x + DRAW_FORMAT_STRING_SIZE, y + DRAW_FORMAT_STRING_SIZE * 2, color);
	// DrawOval(m_x1 + m_rx, m_y1 + m_ry, m_rx, m_ry, m_color, TRUE);
}

void ParabolaBullet::debug(int x, int y, int color) const {
	DrawFormatString(x, y, color, "**ParabolaObject**");
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE, color, "(gx,gy)=(%d,%d), r=%f", m_gx, m_gy, m_handle->getAngle());
	debugObject(x + DRAW_FORMAT_STRING_SIZE, y + DRAW_FORMAT_STRING_SIZE * 2, color);
	// DrawOval(m_x1 + m_rx, m_y1 + m_ry, m_rx, m_ry, m_color, TRUE);
}


/*
* �a���̃f�o�b�O
*/
void SlashObject::debug(int x, int y, int color) const {
	DrawFormatString(x, y, color, "**SlashObject**");
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE, color, "cnt=%d", m_cnt);
	debugObject(x + DRAW_FORMAT_STRING_SIZE, y + DRAW_FORMAT_STRING_SIZE * 2, color);
	// DrawBox(m_x1, m_y1, m_x2, m_y2, color, FALSE);
}


/*
* ���I�u�W�F�N�g�̃f�o�b�O
*/
void DoorObject::debug(int x, int y, int color) const {
	DrawFormatString(x, y, color, "**DoorObject**");
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE, color, "GoTo=%d, text=%s", m_areaNum, m_text == "" ? "" : m_text);
	debugObject(x + DRAW_FORMAT_STRING_SIZE, y + DRAW_FORMAT_STRING_SIZE * 2, color);
	DrawBox(m_x1, m_y1, m_x2, m_y2, color, FALSE);
}