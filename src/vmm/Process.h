#pragma once

#include "Include.h"
#include "SrcGame.h"

bool IsLobby(const std::string& mapName);

bool FindGameProcess();

void UpdateGameScene();
void ListenGameProcessState();