#include "Ship.h"
#include <random>

std::mt19937 rng;

void Shot::Draw()
{
	sprite->Blit(position.x, position.y);
}

bool Shot::Update(float seconds)
{
	//TODO:
	// determine if we are past the end of our life time,
	//if so return false
	timeToLive -= seconds;
	if (timeToLive < 0) {
		return false;
	}
	
	//move the shot
	position += velocity * seconds;

	//TODO:
	//wrap around
	//bounds check position
	if (position.x < 0) position.x += 1920.f;
	if (position.x > 1920) position.x -= 1920.f;
	if (position.y < 0) position.y += 1080.f;
	if (position.y > 1080) position.y -= 1080.f;

	return true;
}

void Ship::Draw()
{
	//change ship angle because my graphics face "up", not "right"
	spriteList[frameNumber]->angle = angle - 90;
	
	
	for (float xoffset = -1920; xoffset <= 1920; xoffset += 1920) {
		for (float yoffset = -1080; yoffset <= 1080; yoffset += 1080) {
			spriteList[frameNumber]->Blit(position.x + xoffset, position.y + yoffset);

			if (shieldTimer > 0) {
				if (blink) shieldSprite->Blit(position.x + xoffset, position.y + yoffset, 1.5f, 1.5f, 0.75f);
			}
		}
	}
	
	
	//draw main ship sprite
	//spriteList[frameNumber]->Blit(position.x, position.y);

	//redraw if too close to an edge
	//left
	//if(position.x < radius + 10.f) spriteList[frameNumber]->Blit(position.x + 1920.f, position.y);
	//right
	//if (position.x > 1920.f - (radius + 10.f)) spriteList[frameNumber]->Blit(position.x - 1920.f, position.y);
	//down
	//if (position.y < radius + 10.f) spriteList[frameNumber]->Blit(position.x, position.y + 1080.f);
	//up
	//if (position.y > 1080.f - (radius + 10.f)) spriteList[frameNumber]->Blit(position.x, position.y - 1080.f);

	//copies for 4 diagonal corners
	//spriteList[frameNumber]->Blit(position.x + 1920.f, position.y + 1080.f);
	//spriteList[frameNumber]->Blit(position.x - 1920.f, position.y - 1080.f);
	//spriteList[frameNumber]->Blit(position.x - 1920.f, position.y + 1080.f);
	//spriteList[frameNumber]->Blit(position.x + 1920.f, position.y - 1080.f);
}

void Ship::Update(float seconds)
{
	//handle turning
	if (turningLeft) {
		angle += 180.f * seconds;
	}

	if (turningRight) {
		angle -= 180.f * seconds;
	}

	if (thrusting) {
		//calculate facing vector
		float radians = angle * (M_PI / 180);
		glm::vec2 facing;
		facing.x = std::cos(radians);
		facing.y = std::sin(radians);

		facing *= seconds * 400.f;

		velocity += facing;

		//check if over max speed
		if (velocity.length() > 600.f) {
			velocity = glm::normalize(velocity) * 600.f;
		}

		thrustTimer += seconds;
		
		//animation timing
		if (thrustTimer >= 1.f / 20.f){
			//change frames
			frameNumber++;
			if (frameNumber > 3)
				frameNumber = 1;

			thrustTimer -= 1.f / 20.f;
		}
	}
	else frameNumber = 0;

	//update position
	position += velocity * seconds;

	//bounds check position
	if (position.x < 0) position.x += 1920.f;
	if (position.x > 1920) position.x -= 1920.f;
	if (position.y < 0) position.y += 1080.f;
	if (position.y > 1080) position.y -= 1080.f;

	//reduce velocity due to "space friction"
	float scale = 1.f - seconds * 0.5f;
	velocity *= scale;

	//velocity could potentialy get very small:we should
	//cap it to zero when it gets really close,
	//but that is a more advanced topic that
	//requires techniques like std::isnan()
	float length = glm::length(velocity);
	if(std::isnan(length) || length < 0.00001f)
		velocity = glm::vec2(0, 0);

	//TODO:
	//handle shot timer
	if (shotTimer > 0) shotTimer -= seconds;
	if (shieldTimer > 0) shieldTimer -= seconds;
}

bool Ship::Shoot(std::vector<Shot> &shotList)
{
	if (shotTimer > 0) return false;
	if (shieldTimer > 0) return false;

	//TODO:
	//time to shoot!
	//reset shot timer
	shotTimer = .314f;

	//make a new shot
	Shot shotObj;

	//set the shot's sprite and position using the ship's variables
	shotObj.sprite = shotSprite;
	shotObj.position = position;

	//build a vector from the ship angle
	float radians = angle * (M_PI / 180);
	shotObj.velocity.x = std::cos(radians);
	shotObj.velocity.y = std::sin(radians);

	//scale up the shot velocity
	shotObj.velocity *= 500.f;

	//add the ship velocity 
	shotObj.velocity += velocity;

	//add the shot to the shotList
	shotList.push_back(shotObj);

	return true;
}

void Enemy::Draw() {
	sprite->angle = angle;

	for (float xoffset = -1920; xoffset <= 1920; xoffset += 1920) {
		for (float yoffset = -1080; yoffset <= 1080; yoffset += 1080) {
			sprite->Blit(position.x + xoffset, position.y + yoffset);
		}
	}
}

void Enemy::Update(float seconds) {
	position += velocity * seconds; 

	//bounds check position
	if (position.x < 0) position.x += 1920.f;
	if (position.x > 1920) position.x -= 1920.f;
	if (position.y < 0) position.y += 1080.f;
	if (position.y > 1080) position.y -= 1080.f;
}

extern Sprite* largeEnermy;
extern Sprite* mediumEnermy;
extern Sprite* smallEnermy;

std::uniform_real_distribution<float> posDistX(0.f, 1920.f);
std::uniform_real_distribution<float> posDistY(0.f, 1080.f);
std::uniform_real_distribution<float> spinDist(-45.f, 45.f);
std::uniform_real_distribution<float> directionDist(0.f, 360.f);
std::uniform_real_distribution<float> largeSpeedDist(20.f, 150.f);
std::uniform_real_distribution<float> mediumSpeedDist(200.f, 300.f);
std::uniform_real_distribution<float> smallSpeedDist(350.f, 450.f);

Enemy EnermyGenerator(EnemySize size) {
	Enemy E;
	E.angle = directionDist(rng);

	float dirAngle = directionDist(rng);
	E.velocity.x = cos(glm::radians(dirAngle));
	E.velocity.y = sin(glm::radians(dirAngle));

	switch (size) {
	case EnemySize::L:
		E.sprite = largeEnermy;
		E.velocity *= largeSpeedDist(rng);
		E.radius2 = (288.f / 2) * (288.f / 2);
		break;
	case EnemySize::M:
		E.sprite = mediumEnermy;
		E.velocity += mediumSpeedDist(rng);
		E.radius2 = (74.f / 2) * (74.f / 2);
		break;

	case EnemySize::S:
		E.sprite = smallEnermy;
		E.velocity += smallSpeedDist(rng);
		E.radius2 = (38.f / 2) * (38.f / 2);
		break;
	}

	E.size = size;

	E.position.x = posDistX(rng);
	E.position.y = posDistY(rng);

	E.spin = spinDist(rng);

	return E;
}

void InitializeRNG() {
	std::random_device rd;
	rng.seed(rd());
}

std::uniform_int_distribution<int> EnermySizeDist(0, 2);

EnemySize RandomEnemySize() {
	int size = EnermySizeDist(rng);

	switch (size) {
	case 0:
		return EnemySize::S;
		break;
	case 1:
		return EnemySize::M;
		break;
	default:
		return EnemySize::L;
		break;
	}
}

float DistanceSquared(glm::vec2 pos1, glm::vec2 pos2) {
	return (pos1.x - pos2.x) * (pos1.x - pos2.x) + (pos1.y - pos2.y) * (pos1.y - pos2.y);
}

bool Collide(Shot& s, Enemy& e) {
	if (DistanceSquared(s.position, e.position) > e.radius2) return false;
	return true;
}

bool Collide(Ship* s, Enemy& e) {
	if (s->shieldTimer > 0) return false;
	if (DistanceSquared(s->position, e.position) > e.radius2) return false;
	return true;
}

void Explosion::Draw() {
	explosionSpriteList[frameNum]->Blit(position.x, position.y, scale, scale);
}

bool Explosion::Update(float seconds) {
	frameTimer += seconds;

	if (frameTimer >= frameSpeed) {
		if (frameNum > explosionSpriteList.size() - 2) return false;
		frameNum++;

		frameTimer = frameTimer - frameSpeed;
	}
	return true;
}

Explosion::Explosion(glm::vec2 location, float size) {
	position = location;
	scale = size; 
}