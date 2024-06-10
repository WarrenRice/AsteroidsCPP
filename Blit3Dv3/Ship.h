#pragma once
#include<Blit3D.h>

class Shot
{
public:
	glm::vec2 velocity, position;
	Sprite* sprite = NULL;
	float timeToLive = 3.14f; //shots live for 2 seconds
	void Draw();
	bool Update(float seconds); //return false if shot dead (timeToLive <= 0)
};

class Ship
{
public:
	Sprite* shieldSprite = NULL;
	Sprite* shotSprite = NULL;
	std::vector<Sprite *> spriteList;
	glm::vec2 velocity, position;
	float angle = 0;
	float shotTimer = 0;
	float radius = 27.f;
	float radius2 = radius * radius;
	int lives = 3;
	int frameNumber = 0;
	float thrustTimer = 0;
	bool thrusting = false;
	bool turningLeft = false;
	bool turningRight = false;
	float shieldTimer = 0;
	float blinkTimer = 0.f;
	bool blink = true;

	void Draw();
	void Update(float seconds);
	bool Shoot(std::vector<Shot> &shotList);
};

enum class EnemySize {S, M, L};

class Enemy {
public: 
	glm::vec2 velocity, position;
	Sprite* sprite = NULL;
	float radius2;
	float spin;
	float angle = 0;
	EnemySize size;

	void Draw();
	void Update(float seconds);
};

Enemy EnermyGenerator(EnemySize size);
void InitializeRNG();

EnemySize RandomEnermySize();

float DistanceSquared(glm::vec2 pos1, glm::vec2 pos2);

//check collide overload functions
bool Collide(Shot& s, Enemy& e);
bool Collide(Ship* s, Enemy& e);

extern std::vector<Sprite*> explosionSpriteList;

class Explosion {
public:
	int frameNum = 0;
	float frameSpeed = 1.f / 10.f;
	float frameTimer = 0.f;
	glm::vec2 position;
	float scale = 3.f;

	void Draw();
	bool Update(float seconds);

	Explosion(glm::vec2 location, float size);
};