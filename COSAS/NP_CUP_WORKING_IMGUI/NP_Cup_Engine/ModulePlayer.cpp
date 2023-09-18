#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
	speed_bost = false;
	lastCheckPoint = nullptr;

	game_finished = false;
	restart_timer = 5.f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	respawn = false;

	VehicleInfo car;
	
	// Car properties ----------------------------------------
	car.chassis_size.Set(1.8, 1.1f, 4);
	car.chassis_offset.Set(0, 1.5, 0);
	car.mass = 500.0f;
	car.suspensionStiffness = 10.f;
	car.suspensionCompression = 1.f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.6f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;
	
	car.spawnPoint = {0, 1, 10};

	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(car.spawnPoint.x, car.spawnPoint.y, car.spawnPoint.z -5);
	App->camera->currVehicle = vehicle;
	
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

update_status ModulePlayer::PreUpdate(float dt)
{

	if (vehicle->rotating)
	{
		float increment = 0.15f;

		//Get vehicle transform and rotation quaternion
		btTransform vehicleTransform = vehicle->vehicle->getRigidBody()->getWorldTransform();
		btQuaternion vehicleQuaternion = vehicleTransform.getRotation();

		//Axis to rotate arround
		btVector3 rotAxis = { vehicle->axis.x, vehicle->axis.y, vehicle->axis.z };

		//Rotate the quaternion an angle arround an axis
		vehicleQuaternion.setRotation(rotAxis, vehicle->target_angle);

		//Apply the new rotation and mult with the current one
		vehicleTransform.setRotation(vehicleTransform.getRotation() * vehicleQuaternion);

		//Set new transform
		vehicle->vehicle->getRigidBody()->setWorldTransform(vehicleTransform);

		//Stop loop and remove angular velocity created by the rotation
		//Update vehicle current angle value
		vehicle->current_angle = vehicle->vehicle->getRigidBody()->getWorldTransform().getRotation().getAngle() * RADTODEG;
		vehicle->vehicle->getRigidBody()->setAngularVelocity({ 0,0,0 });
		vehicle->rotating = false;
	}

	return UPDATE_CONTINUE;
}


// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn = acceleration = brake = 0.0f;

	//Camera lerp backwards
	if (game_finished)
	{
		if (App->camera->followCar) App->camera->followCar = false;

		restart_timer -= dt;
		btVector3 cameraLerp = { App->camera->Position.x, App->camera->Position.y, App->camera->Position.z };

		vec3 a = { App->player->vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().getX(),
			App->player->vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().getY(),
			App->player->vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().getZ() };

		cameraLerp = lerp(cameraLerp, { 100, 100, 0 }, 0.01f);
		App->camera->LookAt(a);
		App->camera->Position = { cameraLerp.getX(), cameraLerp.getY(), cameraLerp.getZ() };

		if (restart_timer <= 0.f)
		{
			RespawnCar();
			App->camera->followCar = true;
			restart_timer = 5.f;
			game_finished = false;
		}
	}

	//Respawn car to last spawnPosition
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN && !respawn)
	{
		RespawnCar();
	}
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN && !respawn)
	{
		LastCheckPoint();
	}

	if(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		if (roundf(vehicle->GetKmh()) >= 0)
		{
			if (speed_bost)
			{
				acceleration = MAX_BOOST_ACCELERATION;
			}
			else
			{
				acceleration = MAX_ACCELERATION;
			}
		}
		else
		{
			brake = BRAKE_POWER;
		}
	}

	if(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		if (turn < TURN_DEGREES) 
		{
			if (App->physics->GetGravity().getY() <= 0)
			{
				turn += TURN_DEGREES;
			}
			if(App->physics->GetGravity().getY() > 0)
			{
				turn -= TURN_DEGREES;
			}
		}
	}

	if(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		if (turn > -TURN_DEGREES) 
		{
			if (App->physics->GetGravity().getY() <= 0)
			{
				turn -= TURN_DEGREES;
			}
			if(App->physics->GetGravity().getY() > 0)
			{
				turn += TURN_DEGREES;
			}
		}
	}

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		if (roundf(vehicle->GetKmh()) <= 0)
		{
			acceleration = -MAX_ACCELERATION;
		}
		else
		{
			brake = BRAKE_POWER;
		}
	}

	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	vehicle->Render();

	return UPDATE_CONTINUE;
}

void ModulePlayer::FinishGame()
{
	if (!game_finished)
	{
		game_finished = true;
	}
}

void ModulePlayer::RespawnCar()
{
	App->physics->SetGravity({GRAVITY.getX(), GRAVITY.getY(), GRAVITY.getZ()});
	mat4x4 carMatrix;
	vehicle->GetTransform(&carMatrix);

	//Correct position and rotation
	carMatrix.rotate(0, { 0, 1, 0 });
	carMatrix.translate(vehicle->info.spawnPoint.x, vehicle->info.spawnPoint.y, vehicle->info.spawnPoint.z);

	//Set corrected transform
	vehicle->SetTransform(&carMatrix.M[0]);

	//Correct velocity (set to 0)
	vehicle->vehicle->getRigidBody()->setLinearVelocity({ 0, 0, 0 });
	vehicle->vehicle->getRigidBody()->setAngularVelocity({ 0, 0, 0 });

	vehicle->rotating = false;
	vehicle->current_angle = 0;
	App->camera->cameraOffset = vec3(0.f, 4.f, 0.f);
	App->player->speed_bost = false;
	lastCheckPoint = nullptr;

	p2List_item<PhysSensor3D*>* item = App->map->map_sensors.getFirst();
	while (item)
	{
		item->data->isEnabled = true;
		item = item->next;
	}
}

void ModulePlayer::LastCheckPoint()
{
	App->physics->SetGravity({ GRAVITY.getX(), GRAVITY.getY(), GRAVITY.getZ() });
	mat4x4 carMatrix;
	vehicle->GetTransform(&carMatrix);

	if (lastCheckPoint != nullptr) 
	{
		//Correct position and rotation
		carMatrix.rotate(lastCheckPoint->targetRot.x, { lastCheckPoint->targetRot.y, lastCheckPoint->targetRot.z, lastCheckPoint->targetRot.w });
		mat4x4 checkPointMat;
		lastCheckPoint->GetTransform(&checkPointMat);
		carMatrix.translate(checkPointMat[12], checkPointMat[13], checkPointMat[14]);
	}

	//Set corrected transform
	vehicle->SetTransform(&carMatrix.M[0]);

	App->camera->cameraOffset = vec3(0.f, 6.f, 0.f);
	//Correct velocity (set to 0)
	vehicle->vehicle->getRigidBody()->setLinearVelocity({ 0, 0, 0 });
	vehicle->vehicle->getRigidBody()->setAngularVelocity({ 0, 0, 0 });

	vehicle->rotating = false;
	vehicle->current_angle = 0;
	App->player->speed_bost = false;

	p2List_item<PhysSensor3D*>* item = App->map->map_sensors.getFirst();
	while (item)
	{
		item->data->isEnabled = true;
		item = item->next;
	}
}



