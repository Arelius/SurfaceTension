#ifndef _FLUID_H_
#define _FLUID_H_

#include <Box2D.h>



class b2FluidManager
{
public:
	b2FluidManager(b2World* world, float Radius, float Mass);
	void CreateParticle(b2Vec2& Loc);
protected:
	b2World* m_world;
	float m_radius;
	float m_mass;
	b2Vec2* ParticleLocations;
	b2Vec2
};

#endif //_FLUID_H_
