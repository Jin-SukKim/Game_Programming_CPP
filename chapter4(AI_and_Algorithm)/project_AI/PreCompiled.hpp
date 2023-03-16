#pragma once

#include <SDL.h>		// SDL_Windows 포인터 참조
#include <SDL_image.h>	// SDL_Img

#include <cmath>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <random>
#include <queue>

#include "GameMath\MathUtils.hpp"
#include "GameMath\Vector2d.hpp"
#include "GameMath\Matrix3x3.hpp"
#include "GameMath\Vector3d.hpp"
#include "GameMath\Matrix4x4.hpp"
#include "GameMath\Random.hpp"

#include "Component\Component.hpp"
#include "GameObject\Actor.hpp"

#include "Component\MoveComponent.hpp"
#include "Component\InputComponent.hpp"

#include "Component\SpriteComponent.hpp"
#include "Component\AnimSpriteComponent.hpp"
#include "Component\BGSpriteComponent.hpp"
#include "Component\CircleCompoenent.hpp"
#include "NavComponent.hpp"

#include "AIComponent.hpp"
#include "AIState.hpp"

#include "GameObject\Laser.hpp"
#include "GameObject\Ship.hpp"
#include "GameObject\Asteroid.hpp"


#include "Game.hpp"