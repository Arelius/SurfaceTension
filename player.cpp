#include "util.h"
#include "player.h"
#include <algorithm>
#include "input.h"
#include "game.h"
#include <OpenGL/gl.h>

float GroundCoefficient;

PlayerInfo* InitPlayerInfo()
{
	return new PlayerInfo();
}

void InitPlayer(PlayerInfo* Player, Vector Location)
{
	Player->State.Location = Location;
	Player->State.Velocity = b2Vec2(0.0f, 0.0f);
	
	b2BodyDef def;
	def.position = Location;
	def.linearDamping = Player->Damping;
	Player->State.Body = PhysicsWorld->CreateBody(&def);
	
	b2CircleDef shape;
	shape.radius = Player->CollisionRadius;
	shape.density = 1.0f;
	Player->State.Body->CreateFixture(&shape);

	Player->State.Body->SetMassFromShapes();
	Player->State.Body->AllowSleeping(false);
}

void DestroyPlayer(PlayerInfo* Player)
{
	delete Player;
}

void UpdatePlayerInput(PlayerInfo* Player, PlayerInput* Input)
{
	Player->State.Drive.x = clamp(GetAxisState(Input, Input_Move_X), -1.0f, 1.0f);
	Player->State.Drive.y = clamp(GetAxisState(Input, Input_Move_Y), -1.0f, 1.0f);

	if(abs(Player->State.Drive.x) < 0.1f)
		Player->State.Drive.x = 0.0f;
	if(abs(Player->State.Drive.y) < 0.1f)
		Player->State.Drive.y = 0.0f;

	Player->State.bJump = GetButtonState(Input, Input_Jump);
}

void UpdatePlayer(float DeltaTime, PlayerInfo* Player)
{
	Vector Thrust = Player->State.Drive;

	Thrust.Normalize();

	Thrust *= Player->MaxThrust;
	
	Vector Velocity = Player->State.Body->GetLinearVelocity();
	
	Velocity += DeltaTime * Thrust;
	
	Velocity.x = std::max(std::min(Velocity.x, Player->MaxSpeed), -Player->MaxSpeed);

	bool bCanJump = true;//Player->State.Body->GetLinearVelocity().y >= 0.0f;

	if(false)//(IsOnGround)
	{
		Player->State.Velocity.x *= GroundCoefficient;
		
		bool bBrake = Thrust.LengthSquared() == 0.0f;

		//if(abs(Player->State.Velocity.x) <= StopSpeed && bBrake)
		//	Player->State.Velocity.x = 0.0f;
		
		Vector GravityDir = PhysicsWorld->GetGravity();

        GravityDir.Normalize();

		if(b2Dot(Player->State.Velocity, PhysicsWorld->GetGravity()) > 0.0f)
			Player->State.Velocity -= Player->State.Velocity.Length() * b2Dot(Player->State.Velocity, PhysicsWorld->GetGravity()) * GravityDir;
	}
	else if(false)
	{
		Player->State.Velocity += DeltaTime * PhysicsWorld->GetGravity();
	}

	Vector PlayerCOM = Player->State.Body->GetWorldCenter();

	if(Player->State.Body->GetLinearVelocity().Length() < Player->MaxSpeed)
		Player->State.Body->ApplyForce(Thrust, PlayerCOM);

	if(bCanJump && Player->State.bJump)
	{
		Vector JumpForce = PhysicsWorld->GetGravity();
        JumpForce.Normalize();
        JumpForce *= -1.0f * Player->JumpThrust;
		Player->State.Body->ApplyForce(JumpForce, PlayerCOM);
	}
	Player->State.Location = Player->State.Body->GetWorldCenter();
	Player->State.Velocity = Player->State.Body->GetLinearVelocity();
	//Player->State.Body->SetLinearVelocity(Velocity);
}

void RenderPlayer(PlayerInfo* Player)
{
	
}
