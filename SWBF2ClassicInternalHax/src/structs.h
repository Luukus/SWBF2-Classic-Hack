#pragma once
#include <Windows.h>
#include <cstdint>
#include "shared/structs/matrix.h"
#include "shared/structs/vec.h"

class NetworkedRotation
{
public:
	float rotX1; //0x0000
	char pad_0004[4]; //0x0004
	float rotZ1; //0x0008
	char pad_000C[20]; //0x000C
	float rotZ2; //0x0020
	char pad_0024[4]; //0x0024
	float rotX2; //0x0028
	char pad_002C[1664]; //0x002C
	float minRotY; //0x06AC
	float maxRotY; //0x06B0
};

class Stats
{
public:
	float rotX1; //0x0000
	char pad_0004[4]; //0x0004
	float rotZ1; //0x0008
	char pad_000C[20]; //0x000C
	float rotZ2; //0x0020
	char pad_0024[4]; //0x0024
	float rotX2; //0x0028
	char pad_002C[4]; //0x002C
	Vec3 position;
	char pad_003C[24]; //0x003C
	float health; //0x0054
	float maxHealth; //0x0058
	char pad_005C[132]; //0x005C
}; //Size: 0x00E0

class Weapon
{
public:
	char pad_0000[260]; //0x0000
}; //Size: 0x0104

class Team
{
public:
	char pad_0000[12]; //0x0000
	wchar_t* teamName; //0x000C
	char pad_0010[44]; //0x0010
	int32_t teamSize; //0x003C
	char pad_0040[196]; //0x0040
}; //Size: 0x0104

class PlayerController
{
public:
	char pad_0000[224]; //0x0000
}; //Size: 0x00E0

class Aimer
{
public:
	char pad_0000[72]; //0x0000
	Vec3 rotation; 
	char pad_0054[944]; //0x0054
}; //Size: 0x0404

class EntityClass
{
public:
	char pad_0000[32]; //0x0000
	wchar_t* name; //0x0020
	char pad_0024[736]; //0x0024
}; //Size: 0x0304

class TraceLineEntity
{
public:
	char pad_0000[260]; //0x0000
}; //Size: 0x0104

class Entity
{
public:
	char pad_0000[200]; //0x0000
	class PlayerController* controller; //0x00C8
	char pad_00CC[124]; //0x00CC
	Vec3 eyePosition; //0x0148
	char pad_0154[172]; //0x0154
	class EntityClass* entityClass; //0x0200
	char pad_0204[152]; //0x0204
	Vec3 velocity; //0x029C
	float rotationY; //0x02A8
	char pad_02AC[44]; //0x02AC
	class Aimer* aimer; //0x02D8
	char pad_02DC[8]; //0x02DC
	class Stats* stats; //0x02E4
	char pad_02E8[504]; //0x02E8
	class Weapon* primaryWeapon; //0x04E0
	class Weapon* secondaryWeapon; //0x04E4
	class Weapon* utilityWeapon; //0x04E8
	char pad_04EC[24]; //0x04EC
	int32_t isInAir; //0x0504
	int32_t isOnGround; //0x0508
	char pad_050C[712]; //0x050C
	float stamina; //0x07D4
	char pad_07D8[280]; //0x07D8
	float rotX1; //0x08F0
	char pad_08F4[4]; //0x08F4
	float rotZ1; //0x08F8
	char pad_08FC[20]; //0x08FC
	float rotZ2; //0x0910
	char pad_0914[4]; //0x0914
	float rotX2; //0x0918
	char pad_091C[524]; //0x091C
}; //Size: 0x0B28

class Character
{
public:
	char pad_0000[48]; //0x0000
	wchar_t nameStart[2]; //0x0030
	char pad_0034[260]; //0x0034
	class Team* team; //0x0138
	char pad_013C[4]; //0x013C
	class EntityClass* entityClass; //0x0140
	char pad_0144[4]; //0x0144
	class Entity* entity; //0x0148
	char pad_014C[100]; //0x014C
}; //Size: 0x01B0

class GameCamera
{
public:
	char pad_0000[48]; //0x0000
	Matrix4x4 matrix1; //0x0030
	Matrix4x4 matrix2; //0x0070
	char pad_00B0[64]; //0x00B0
	Matrix4x4 matrix3; //0x00F0
	char pad_0130[44]; //0x0130
	Matrix4x4 matrix4; //0x015C
	char pad_019C[52]; //0x019C
}; //Size: 0x01D0

class ChaseCamera
{
public:
	char pad_0000[16]; //0x0000
	Matrix4x4 matrix1; //0x0010
	char pad_0050[240]; //0x0050
}; //Size: 0x0140

class CameraManager
{
public:
	char pad_0000[36]; //0x0000
	class GameCamera* gameCamera; //0x0024
	class ChaseCamera* otherCamera; //0x0028
	char pad_002C[216]; //0x002C
}; //Size: 0x0104

class Game
{
public:
	class CameraManager* cameraManager; //0x0000
	char pad_0004[12]; //0x0004
	class Character(*entityList)[64]; //0x0010
	char pad_0014[300]; //0x0014
}; //Size: 0x0140
