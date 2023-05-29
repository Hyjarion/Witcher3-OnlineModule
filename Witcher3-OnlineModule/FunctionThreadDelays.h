#pragma once

namespace FunctionThreadDelayer
{
	struct func_SpawnPlayer
	{
		uint32 ID;
		std::u16string ResName;
		Vector pos;
		bool isDepotPath;
	};
	std::vector<func_SpawnPlayer> func_SpawnPlayer_delays;

	struct func_SpawnNpc
	{
		uint32 ID;
		char16_t ResName[10];
		Vector pos;
		float health;
	};
	std::vector<func_SpawnNpc> func_SpawnNpc_delays;

	struct func_MoveActorTo
	{
		uintptr_t CActor; 
		Vector* pos; 
		uint32 EMoveType; 
		float absSpeed; 
		float radius; 
		uint32 EMoveFailureAction; 
		uint16 unk; 
		float unk2;
	};
	std::vector<func_MoveActorTo> func_MoveActorTo_delays;

	bool func_ExecGameConsoleCommand_locked = false;
	struct func_ExecGameConsoleCommand
	{
		std::string command;
	};
	std::vector<func_ExecGameConsoleCommand> func_ExecGameConsoleCommand_delays;
}
