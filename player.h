#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <Box2D.h>
#include "2dtypes.h"
typedef b2Vec2 Vector;

struct PlayerState
{
	Vector Location;
	Vector Velocity;
	b2Body* Body;
	// Input Variables.
	Vector Drive;
	bool bJump;
};

struct PlayerInfo
{
	float CollisionRadius;
	float MaxThrust;
	float MaxSpeed;
	float Damping;
	float JumpThrust;
	PlayerState State;
};

extern PlayerInfo* MainPlayer;
extern float GroundCoefficient;

PlayerInfo* InitPlayerInfo();
void InitPlayer(PlayerInfo* Player, Vector Location);
void DestroyPlayer(PlayerInfo* Player);
void UpdatePlayerInput(PlayerInfo* Player, struct PlayerInput* Input);
void UpdatePlayer(float DeltaTime, PlayerInfo* Player);
void RenderPlayer(PlayerInfo* Player);

#endif //_PLAYER_H_
