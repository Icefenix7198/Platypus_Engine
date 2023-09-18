#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_BOOST_ACCELERATION 2500.0f
#define MAX_ACCELERATION 1000.0f	   
#define TURN_DEGREES 20.0f * DEGTORAD  
#define BRAKE_POWER 250.0f




class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	bool CleanUp();

	void RespawnCar();
	void LastCheckPoint();
	void FinishGame();

public:

	PhysVehicle3D* vehicle;
	PhysSensor3D* lastCheckPoint;
	float turn;
	float acceleration;
	float brake;
	bool respawn;
	bool speed_bost;
	bool super_boost;
	bool game_finished;

	Cylinder* wheels[4];

private:
	float restart_timer;
};