#include "Event.h"
#include "World.h"
#include "Sound.h"
#include "CsvReader.h"
#include "CharacterController.h"
#include "CharacterAction.h"
#include "Character.h"
#include "Text.h"
#include "Brain.h"
#include <sstream>

using namespace std;


// csv����擾����param1�`paramN��map��vector�ɕϊ�
vector<string> mapParam2vector(map<string, string> paramMap) {
	vector<string> res;
	int paramNum = 0;
	while (true) {
		ostringstream oss;
		oss << "param" << paramNum;
		if (paramMap.find(oss.str()) != paramMap.end()) {
			res.push_back(paramMap[oss.str()]);
			paramNum++;
		}
		else {
			break;
		}
	}
	return res;
}


/*
* �C�x���g
*/
Event::Event(int eventNum, World* world, SoundPlayer* soundPlayer) {

	m_eventNum = eventNum;
	m_nowElement = 0;

	m_world_p = world;
	m_soundPlayer = soundPlayer;

	ostringstream oss;
	oss << "data/event/event" << eventNum << ".csv";
	CsvReader2 csvReader2(oss.str().c_str());

	// ���Ώ���
	vector<map<string, string> > fireData = csvReader2.getDomainData("FIRE:");
	for (unsigned int i = 0; i < fireData.size(); i++) {
		createFire(mapParam2vector(fireData[i]), world, soundPlayer);
	}

}

bool Event::skillAble() {
	if (m_eventElement.size() == 0) { return true; }
	return m_eventElement[m_nowElement]->skillAble();
}

// World��ݒ肵�Ȃ���
void Event::setWorld(World* world) {
	m_world_p = world;
	for (unsigned int i = 0; i < m_eventFire.size(); i++) {
		m_eventFire[i]->setWorld(m_world_p);
	}
	for (unsigned int i = 0; i < m_eventElement.size(); i++) {
		m_eventElement[i]->setWorld(m_world_p);
	}
}

// Fire�̍쐬
void Event::createFire(vector<string> param, World* world, SoundPlayer* soundPlayer) {
	string param0 = param[0];
	EventFire* fire = NULL;

	if (param0 == "CharacterPoint") {
		fire = new CharacterPointFire(world, param);
	}

	if (fire != NULL) { m_eventFire.push_back(fire); }
}

// Element�̍쐬
void Event::createElement(vector<string> param, World* world, SoundPlayer* soundPlayer) {
	string param0 = param[0];
	EventElement* element = NULL;

	if (param0 == "ChangeBrain") {
		element = new ChangeBrainEvent(world, param);
	}
	else if (param0 == "ChangeGroup") {
		element = new ChangeGroupEvent(world, param);
	}
	else if (param0 == "DeadCharacter") {
		element = new DeadCharacterEvent(world, param);
	}
	else if (param0 == "DeadGroup") {
		element = new DeadGroupEvent(world, param);
	}
	else if (param0 == "Talk") {
		element = new TalkEvent(world, soundPlayer, param);
	}

	if (element != NULL) { m_eventElement.push_back(element); }
}

Event::~Event() {
	for (unsigned int i = 0; i < m_eventFire.size(); i++) {
		delete m_eventFire[i];
	}
	for (unsigned int i = 0; i < m_eventElement.size(); i++) {
		delete m_eventElement[i];
	}
}

bool Event::fire() {
	for (unsigned int i = 0; i < m_eventFire.size(); i++) {
		if (!m_eventFire[i]->fire()) {
			return false;
		}
	}
	ostringstream oss;
	oss << "data/event/event" << m_eventNum << ".csv";
	CsvReader2 csvReader2(oss.str().c_str());
	// �C�x���g�̓��e
	vector<map<string, string> > elementsData = csvReader2.getDomainData("ELEMENT:");
	// ���΂��ď��߂ăC�x���g�����
	for (unsigned int i = 0; i < elementsData.size(); i++) {
		createElement(mapParam2vector(elementsData[i]), m_world_p, m_soundPlayer);
	}
	return true;
}

EVENT_RESULT Event::play() {

	EVENT_RESULT result = m_eventElement[m_nowElement]->play();

	// Element�����������
	if (result == EVENT_RESULT::SUCCESS) {
		// ���̃G�������g��
		m_nowElement++;
		if (m_nowElement == m_eventElement.size()) { 
			// �C�x���g�����
			return EVENT_RESULT::SUCCESS;
		}
		else { 
			// �܂��C�x���g����
			return EVENT_RESULT::NOW;
		}
	}

	return result;
}



/*
* �C�x���g�̔��Ώ���
*/
EventFire::EventFire(World* world) {
	m_world_p = world;
}

// ����̃L�������w�肵�����W�ɂ���
CharacterPointFire::CharacterPointFire(World* world, std::vector<std::string> param) :
	EventFire(world)
{
	m_param = param;
	m_character_p = m_world_p->getCharacterWithName(param[1]);
	m_areaNum = stoi(param[2]);
	m_x = stoi(param[3]);
	m_y = stoi(param[4]);
	m_dx = stoi(param[5]);
	m_dy = stoi(param[6]);
}
bool CharacterPointFire::fire() {
	// ����̃L�������w�肵���ꏊ�ɂ��邩����
	if (m_world_p->getAreaNum() != m_areaNum) { return false; }
	int x = m_character_p->getCenterX();
	int y = m_character_p->getY() + m_character_p->getHeight();
	if (x > m_x - m_dx && x < m_x + m_dx && y > m_y - m_dy && y < m_y + m_dy) {
		return true;
	}
	return false;
}
void CharacterPointFire::setWorld(World* world) {
	EventFire::setWorld(world);
	m_character_p = m_world_p->getCharacterWithName(m_param[1]);
}



/*
* �C�x���g�̍\���v�f
*/
EventElement::EventElement(World* world) {
	m_world_p = world;
}


// �L������Brain��ύX����
ChangeBrainEvent::ChangeBrainEvent(World* world, vector<string> param) :
	EventElement(world)
{
	m_brainName = param[1];
	m_controller_p = m_world_p->getControllerWithName(param[2]);
	m_param = param;
}
EVENT_RESULT ChangeBrainEvent::play() {
	// �Ώۂ̃L������Brain��ύX����
	Brain* brain = NULL;
	if (m_brainName == "NormalAI") {
		brain = new NormalAI();
		m_controller_p->setBrain(brain);
	}
	else if (m_brainName == "ParabolaAI") {
		brain = new ParabolaAI();
		m_controller_p->setBrain(brain);
	}
	else if (m_brainName == "FollowNormalAI") {
		brain = new FollowNormalAI();
		m_controller_p->setBrain(brain);
		Character* follow = m_world_p->getCharacterWithName(m_param[3]);
		brain->searchFollow(follow);
	}
	else if (m_brainName == "FollowParabolaAI") {
		brain = new FollowParabolaAI();
		m_controller_p->setBrain(brain);
		Character* follow = m_world_p->getCharacterWithName(m_param[3]);
		brain->searchFollow(follow);
	}
	return EVENT_RESULT::SUCCESS;
}
void ChangeBrainEvent::setWorld(World* world) {
	EventElement::setWorld(world);
	m_controller_p = m_world_p->getControllerWithName(m_param[2]);
}

// �L������GroupID��ύX����
ChangeGroupEvent::ChangeGroupEvent(World* world, std::vector<string> param) :
	EventElement(world)
{
	m_param = param;
	m_groupId = stoi(param[1]);
	m_character_p = m_world_p->getCharacterWithName(param[2]);
}
EVENT_RESULT ChangeGroupEvent::play() {
	// �Ώۂ̃L������Brain��ύX����
	m_character_p->setGroupId(m_groupId);
	return EVENT_RESULT::SUCCESS;
}
void ChangeGroupEvent::setWorld(World* world) {
	EventElement::setWorld(world);
	m_character_p = m_world_p->getCharacterWithName(m_param[2]);
}

// ����̃L������HP��0�ɂȂ�܂Ő키
DeadCharacterEvent::DeadCharacterEvent(World* world, std::vector<std::string> param) :
	EventElement(world)
{
	m_character_p = m_world_p->getCharacterWithName(param[1]);
}
EVENT_RESULT DeadCharacterEvent::play() {
	m_world_p->battle();
	// �Ώۂ̃L������HP���`�F�b�N����
	if (m_character_p->getHp() == 0) {
		return EVENT_RESULT::SUCCESS;
	}
	return EVENT_RESULT::NOW;
}
void DeadCharacterEvent::setWorld(World* world) {
	EventElement::setWorld(world);
	m_character_p = m_world_p->getCharacterWithName(m_param[1]);
}

// ����̃O���[�v���S�ł���܂Ő키
DeadGroupEvent::DeadGroupEvent(World* world, std::vector<std::string> param) :
	EventElement(world)
{
	m_groupId = stoi(param[1]);
}
EVENT_RESULT DeadGroupEvent::play() {
	m_world_p->battle();
	vector<const CharacterAction*> actions = m_world_p->getActions();
	for (unsigned int i = 0; i < actions.size(); i++) {
		if (actions[i]->getCharacter()->getGroupId() == m_groupId && actions[i]->getCharacter()->getHp() > 0) {
			return EVENT_RESULT::NOW;
		}
	}
	return EVENT_RESULT::SUCCESS;
}


TalkEvent::TalkEvent(World* world, SoundPlayer* soundPlayer, std::vector<std::string> param):
	EventElement(world)
{
	int textNum = stoi(param[1]);
	m_conversation = new Conversation(textNum, world, soundPlayer);
}

TalkEvent::~TalkEvent() {
	delete m_conversation;
}

EVENT_RESULT TalkEvent::play() {
	m_world_p->setConversation(m_conversation);
	m_world_p->talk();
	if (m_conversation->getFinishFlag()) {
		return EVENT_RESULT::SUCCESS;
	}
	return EVENT_RESULT::NOW;
}