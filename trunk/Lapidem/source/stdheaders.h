#pragma once

#include <windows.h>
#include <stdlib.h>  
#include <cmath>
#include <ctime>
#include <vector>                                                   
#include <fstream>
#include <string>
using std::string;
using std::vector;
using std::ios_base;
using std::ifstream;

#include "CGame.h"
#include "CGameplayState.h"

#include "CLevel.h"
#include "CCamera.h"
#include "CSpellFactory.h"
#include "CParticleManager.h"

#include "Corona_ObjectManager.h"
#include "Corona_EventHandler.h"

#include "Wrappers/CSGD_Direct3D.h"      
#include "Wrappers/CSGD_TextureManager.h"
#include "Wrappers/CSGD_DirectSound.h"   
#include "Wrappers/CSGD_WaveManager.h"   
#include "Wrappers/CSGD_DirectInput.h"   
