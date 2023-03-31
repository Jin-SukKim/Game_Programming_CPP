#pragma once

#include <SDL.h>		// SDL_Windows 포인터 참조
#include <SDL_log.h>
#include <SDL_scancode.h>
#include <glew.h>		// OpenGL의 GLEW
#include <SOIL2.h>
#include <document.h>	// JSON parsing

#include <fmod_studio.hpp>	// FMOD API
#include <fmod_errors.h>
#include <fmod.hpp>

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

#include "GameMath\Quaternion.hpp"

#include "Component\Component.hpp"
#include "Actor\Actor.hpp"

#include "System\KeyboardState.hpp"
#include "System\MouseState.hpp"
#include "System\ControllerState.hpp"
#include "System\InputSystem.hpp"

#include "Component\MoveComponent.hpp"
#include "Component\InputComponent.hpp"

#include "Component\SpriteComponent.hpp"
#include "Component\AnimSpriteComponent.hpp"
#include "Component\CircleComponent.hpp"
#include "Component\NavComponent.hpp"

#include "Actor\VertexArray.hpp"
#include "Actor\Shader.hpp"
#include "Actor\Texture.hpp"

#include "Actor\Mesh.hpp"
#include "Actor\Renderer.hpp"
#include "Component\MeshComponent.hpp"

#include "Component\SoundEvent.hpp"
#include "Component\AudioSystem.hpp"
#include "Component\AudioComponent.hpp"

#include "CameraComponent.hpp"
#include "FPSCamera.hpp"
#include "FollowCamera.hpp"
#include "OrbitCamera.hpp"
#include "SplineCamera.hpp"

#include "FPSActor.hpp"
#include "FollowActor.hpp"
#include "OrbitActor.hpp"
#include "SplineActor.hpp"

#include "Component\AIComponent.hpp"
#include "Component\AIState.hpp"

#include "GameObject\PlaneActor.hpp"


#include "Game.hpp"