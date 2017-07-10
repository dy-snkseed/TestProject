#pragma once

#include "resource.h"

void ServiceMain(int argc, char** argv);
void ControlHandler(DWORD request);
int InstallService();
int RemoveService();
int _StartService();