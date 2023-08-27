#ifndef WORLD_DRAWER_H_INCLUDED
#define WORLD_DRAWER_H_INCLUDED

class World;
class CharacterDrawer;
class ObjectDrawer;
class AnimationDrawer;
class ConversationDrawer;


// 世界を描画する
class WorldDrawer {
private:

	// 描画する世界
	const World* m_world;

	// キャラ描画用
	CharacterDrawer* m_characterDrawer;

	// オブジェクト描画用
	ObjectDrawer* m_objectDrawer;

	// アニメーション描画用
	AnimationDrawer* m_animationDrawer;

	// 会話イベント
	ConversationDrawer* m_conversationDrawer;

public:
	WorldDrawer(const World* world);

	~WorldDrawer();

	// セッタ
	void setWorld(World* world) { m_world = world; }

	// 描画する
	void draw();
};

#endif