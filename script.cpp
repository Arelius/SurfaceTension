
#include "script.h"
#include "game.h"
#include "player.h"
#include "input.h"
#include "systemmanager.h"

//For Box2D Debug Draw
#include <Render.h>

void scheme_load_file_name(scheme* sc, const char* file)
{
	FILE* script_file = fopen(file, "r");
	scheme_load_file(sc, script_file);
	fclose(script_file);
}

static const char* MakeVec2Docs =
	"(make-vec2 \"Creates a vec2 pair from two numbers.\" (vec2 \"New vec2.\") (real \"X value\") (real \"Y value\"))";

pointer MakeVec2(scheme* sc, pointer p)
{
	if(is_number(pair_car(p))
	   && is_number(pair_car(pair_cdr(p))))
	{
		return cons(sc, pair_car(p), pair_car(pair_cdr(p)));
	}
	return cons(sc, mk_real(sc, 0.0f), mk_real(sc, 0.0f));
}

b2Vec2 GetVec2(pointer p)
{
	return b2Vec2(rvalue(pair_car(p)), rvalue(pair_cdr(p)));
}

void InitPhysics(b2Vec2 upperbound, b2Vec2 lowerbound, b2Vec2 Gravity)
{
	b2AABB aabb;
	aabb.lowerBound = lowerbound;
	aabb.upperBound = upperbound;

	PhysicsWorld = new b2World(aabb, Gravity, true);
	
}

pointer InitPhysicsWorld(scheme* sc, pointer p)
{
	// (init-world (upper bound pair) (lower bound pair) (gravity pair) )
	InitPhysics(GetVec2(pair_car(p)), GetVec2(pair_car(pair_cdr(p))), GetVec2(pair_car(pair_cdr(pair_cdr(p)))));
	
	return sc->NIL;
}

static const char* InitFluidsDocs = 
"(init-fluids \"Fluid Initialization\" (nil) (real \"Effective fluid particle radius.\") (real \"Effective fluid particle mass.\") (real \"Fluid particle friction\"))";

pointer InitFluids(scheme* sc, pointer p)
{
	FluidRadius = rvalue(pair_car(p));
	FluidMass = rvalue(pair_car(pair_cdr(p)));
	FluidFriction = rvalue(pair_car(pair_cdr(pair_cdr(p))));
}

static const char* MakeFluidBoxDocs =
"(make-fluid-box \"Fills a box with fluid particles.\" (nil) (vec2 \"Near box corner.\") (vec2 \"Far box corner.\") (int \"Particle width count.\") (int \"Particle depth count.\"))";

pointer MakeFluidBox(scheme* sc, pointer p)
{
	b2Vec2 C1 = GetVec2(pair_car(p));
	b2Vec2 C2 = GetVec2(pair_car(pair_cdr(p)));
	int Pw = ivalue(pair_car(pair_cdr(pair_cdr(p))));
	int Ph = ivalue(pair_car(pair_cdr(pair_cdr(pair_cdr(p)))));
	
	float WidthStep = (C2.x - C1.x)/Pw;
	float HeightStep = (C2.y - C1.y)/Ph;

	b2BodyDef bdyDef;

	b2CircleDef shpDef;

	shpDef.radius = FluidRadius;
	shpDef.density = FluidMass;
	shpDef.friction = FluidFriction;

	for(int h = 0; h < Ph; h++)
	{
		for(int w = 0; w < Pw; w++)
		{
			bdyDef.position.Set(C1.x + WidthStep * w, C1.y + HeightStep * h);
			b2Body* Body = PhysicsWorld->CreateBody(&bdyDef);
			Body->CreateShape(&shpDef);
			Body->SetMassFromShapes();
			
		}
	}
}

pointer InitCamera(scheme* sc, pointer p)
{
	// (init-camera camera-width aspect-ratio, approach)
	CameraWidth = rvalue(pair_car(p));
	CameraAspect = rvalue(pair_car(pair_cdr(p)));
	CameraApproach = rvalue(caddr(p));
}

pointer MakeStaticBox(scheme* sc, pointer p)
{
	// (make-static-box (box location) box-width box-height)

	b2BodyDef def;
	def.position = GetVec2(pair_car(p));
	b2Body* bdy = PhysicsWorld->CreateBody(&def);

	b2PolygonDef shp;
	shp.SetAsBox(rvalue(pair_car(pair_cdr(p))), rvalue(pair_car(pair_cdr(pair_cdr(p)))));
	
	bdy->CreateShape(&shp);

	return sc->NIL;
	
}

pointer SetDebugDraw(scheme* sc, pointer p)
{
	if(PhysicsWorld)
	{
		if(pair_car(p) != sc->F)
		{
			b2DebugDraw* DD = new DebugDraw();
			DD->SetFlags(b2DebugDraw::e_shapeBit | b2DebugDraw::e_centerOfMassBit);
			PhysicsWorld->SetDebugDraw(DD);
		}
		else
			PhysicsWorld->SetDebugDraw(NULL); //TODO: not sure if the old DebugDraw gets free'd
	}
}

pointer SetGameVar(scheme* sc, pointer p)
{
	if(is_symbol(pair_car(p)))
	{
		char* sym = symname(pair_car(p));
		if(strcmp(sym, "GroundCoefficient") == 0)
		{
			GroundCoefficient = rvalue(pair_car(pair_cdr(p)));
		}
	}
	return sc->NIL;
}

extern PlayerInfo* MainPlayer;

pointer InitPlayerInfo(scheme* sc, pointer p)
{
	// (init-player-info Collision-Radius Max-Thrust Max-Speed Linear-Damping Jump-Thrust)
	MainPlayer = InitPlayerInfo();
	MainPlayer->CollisionRadius = rvalue(pair_car(p));
	MainPlayer->MaxThrust = rvalue(pair_car(pair_cdr(p)));
	MainPlayer->MaxSpeed  = rvalue(pair_car(pair_cdr(pair_cdr(p))));
	MainPlayer->Damping = rvalue(cadddr(p));
	MainPlayer->JumpThrust = rvalue(car(cddddr(p)));
	return sc->NIL;
}

const char* InitPlayerDocs =
"(init-player \"Initialises a player instance\" (nil) (vec2 \"Location of Player\"))";

extern b2Vec2 CameraFocusLocation;

pointer InitPlayer(scheme* sc, pointer p)
{
	b2Vec2 Loc = GetVec2(pair_car(p));
	InitPlayer(MainPlayer, Loc);
	CameraFocusLocation = Loc;
	return sc->NIL;
}

void RegisterSchemeFunc(scheme* sc, foreign_func func, const char* Docs)
{
	//strcpy(sc->strbuff, Docs);
	//pointer DocList = readstrexp(sc);
	//scheme_define(sc, sc->global_env, car(DocList), mk_foreign_func(sc, func));
}

void* init_script_system(SystemManager* SM)
{
	scheme* scheme_vm = scheme_init_new();
	scheme_define(scheme_vm, scheme_vm->global_env, mk_symbol(scheme_vm, "init-world"), mk_foreign_func(scheme_vm, &InitPhysicsWorld));
	//RegisterSchemeFunc(scheme_vm, &MakeVec2, MakeVec2Docs);
	scheme_define(scheme_vm, scheme_vm->global_env, mk_symbol(scheme_vm, "make-vec2"), mk_foreign_func(scheme_vm, &MakeVec2));
	scheme_define(scheme_vm, scheme_vm->global_env, mk_symbol(scheme_vm, "make-static-box"), mk_foreign_func(scheme_vm, &MakeStaticBox));
	scheme_define(scheme_vm, scheme_vm->global_env, mk_symbol(scheme_vm, "init-camera"), mk_foreign_func(scheme_vm, &InitCamera));
	scheme_define(scheme_vm, scheme_vm->global_env, mk_symbol(scheme_vm, "set-debug-draw"), mk_foreign_func(scheme_vm, &SetDebugDraw));
	scheme_define(scheme_vm, scheme_vm->global_env, mk_symbol(scheme_vm, "set-game-var"), mk_foreign_func(scheme_vm, &SetGameVar));
	scheme_define(scheme_vm, scheme_vm->global_env, mk_symbol(scheme_vm, "init-player-info"), mk_foreign_func(scheme_vm, &InitPlayerInfo));
	//RegisterSchemeFunc(scheme_vm, &InitFluids, InitFluidsDocs);
	//RegisterSchemeFunc(scheme_vm, &MakeFluidBox, MakeFluidBoxDocs);
	scheme_define(scheme_vm, scheme_vm->global_env, mk_symbol(scheme_vm, "init-fluids"), mk_foreign_func(scheme_vm, &InitFluids));
	scheme_define(scheme_vm, scheme_vm->global_env, mk_symbol(scheme_vm, "make-fluid-box"), mk_foreign_func(scheme_vm, &MakeFluidBox));
	//RegisterSchemeFunc(scheme_vm, &InitPlayer, InitPlayerDocs);
	scheme_define(scheme_vm, scheme_vm->global_env, mk_symbol(scheme_vm, "init-player"), mk_foreign_func(scheme_vm, &InitPlayer));

	return scheme_vm;
}

void destroy_script_system(void* System)
{
	scheme_deinit((scheme*)System);
}

void declare_script_system(SystemManager* SM)
{
	declare_system(SM, system_manager_symbol(SM, "script"), init_script_system, destroy_script_system);
}
