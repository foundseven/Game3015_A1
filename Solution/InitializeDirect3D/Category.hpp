#pragma region Step 1
#pragma once

//a category is a category of the objects in the scene
namespace Category
{
	enum Type
	{
		None = 0,
		Scene = 1 << 0,
		PlayerAircraft = 1 << 1,
		AlliedAircraft = 1 << 2,
		EnemyAircraft = 1 << 3,
	};
}

#pragma endregion