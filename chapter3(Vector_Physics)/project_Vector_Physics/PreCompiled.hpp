#pragma once

#include <SDL.h>		// SDL_Windows 포인터 참조
#include <SDL_image.h>	// SDL_Img

#include <cmath>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>

#include "GameMath\MathUtils.hpp"
#include "GameMath\Vector2d.hpp"
#include "GameMath\Matrix3x3.hpp"
#include "Vector3d.hpp"
#include "Matrix4x4.hpp"

#include "Component\Component.hpp"
#include "GameObject\Actor.hpp"

#include "MoveComponent.hpp"

#include "Component\SpriteComponent.hpp"
#include "Component\AnimSpriteComponent.hpp"
#include "Component\BGSpriteComponent.hpp"

#include "GameObject\Ship.hpp"
#include "Asteroid.hpp"

#include "Game.hpp"