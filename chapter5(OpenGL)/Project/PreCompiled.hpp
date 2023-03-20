#pragma once

#include <SDL.h>		// SDL_Windows 포인터 참조
#include <glew.h>		// OpenGL의 GLEW
#include <SOIL2.h>

#include <cmath>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <random>
#include <queue>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

#include "GameMath\MathUtils.hpp"
#include "GameMath\Vector2d.hpp"
#include "GameMath\Matrix3x3.hpp"
#include "GameMath\Vector3d.hpp"
#include "GameMath\Matrix4x4.hpp"
#include "GameMath\Random.hpp"

#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#include "Component\Component.hpp"
#include "GameObject\Actor.hpp"

#include "Component\MoveComponent.hpp"
#include "Component\InputComponent.hpp"

#include "Component\SpriteComponent.hpp"
#include "Component\AnimSpriteComponent.hpp"
#include "Component\CircleComponent.hpp"
#include "Component\NavComponent.hpp"

#include "Component\AIComponent.hpp"
#include "Component\AIState.hpp"

#include "GameObject\Laser.hpp"
#include "GameObject/Asteroid.hpp"
#include "GameObject/Ship.hpp"

#include "Game.hpp"