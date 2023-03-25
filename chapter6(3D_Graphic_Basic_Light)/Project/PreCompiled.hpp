#pragma once

#include <SDL.h>		// SDL_Windows 포인터 참조
#include <glew.h>		// OpenGL의 GLEW
#include <SOIL2.h>
#include <document.h>	// JSON parsing

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

#include "Quaternion.hpp"

#include "Component\Component.hpp"
#include "GameObject\Actor.hpp"

#include "Component\MoveComponent.hpp"
#include "Component\InputComponent.hpp"

#include "Component\SpriteComponent.hpp"
#include "Component\AnimSpriteComponent.hpp"
#include "Component\CircleComponent.hpp"
#include "Component\NavComponent.hpp"

#include "GameObject\VertexArray.hpp"
#include "GameObject\Shader.hpp"
#include "GameObject\Texture.hpp"

#include "Mesh.hpp"
#include "Renderer.hpp"
#include "MeshComponent.hpp"

#include "CameraActor.hpp"

#include "Component\AIComponent.hpp"
#include "Component\AIState.hpp"

#include "PlaneActor.hpp"

#include "Game.hpp"