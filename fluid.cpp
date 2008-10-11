#include "fluid.h"

const MaxFluidParticles = 1024;

class b2FluidPairCallback : public b2PairCallback
{
	b2PairCallback* m_passThrough;
	void* FluidMemStart;
	void* FluidMemEnd;
	b2FluidPairCallback(b2PairCallback* PassThrough, void* MemStart, void* MemEnd) : m_passThrough(PassThrough), FluidMemStart(MemStart), FluidMemEnd(MemEnd)
	{}
	bool IsFluid(void* proxyUserData)
	{
		return (proxyUserData >= FluidMemStart && proxyUserData < FluidMemEnd);
	}
	virtual void* PairAdded(void* proxyUserData1, void* proxyUserData2)
	{
		if(IsFluid(proxyUserData1))
		{
			
		}
		else if(IsFluid(proxyUserData2))
		{
		}
		else
			return m_passThrough->PairAdded(proxyUserData1, proxyUserData2);
	}
	virtual void PairRemoved(void* proxyUserData1, void* proxyUserData2, void* pairUserData)
	{
		if(IsFluid(proxyUserData1))
		{
			
		}
		else if(IsFluid(proxyUserData2))
		{
		}
		else
			m_passThrough->PairRemoved(proxyUserData1, proxyUserData2, pairUserData);
	}
};

b2FluidManager::b2FluidManager(b2World* world, float Radius, float Mass) : m_world(world), m_radius(Radius), m_mass(Mass)
{}

void b2FluidManager::CreateParticle(b2Vec2& Loc)
{
	int ProxyID = m_world->m_broadPhase->CreateProxy(
}
