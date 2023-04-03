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
#include <array>
#include <functional>

#include "GameMath\MathUtils.hpp"
#include "GameMath\Vector2d.hpp"
#include "GameMath\Matrix3x3.hpp"
#include "GameMath\Vector3d.hpp"
#include "GameMath\Matrix4x4.hpp"
#include "GameMath\Random.hpp"

#include "GameMath\Quaternion.hpp"

#include "Component\Component.hpp"
#include "Actor\Actor.hpp"

#include "System\Input\KeyboardState.hpp"
#include "System\Input\MouseState.hpp"
#include "System\Input\ControllerState.hpp"
#include "System\Input\InputSystem.hpp"

#include "Component\Move\MoveComponent.hpp"
#include "Component\Move\InputComponent.hpp"
#include "Component\Move\NavComponent.hpp"

#include "Component\Mesh\SpriteComponent.hpp"
#include "Component\Mesh\AnimSpriteComponent.hpp"

#include "Component\CircleComponent.hpp"
#include "Collision.hpp"

#include "Actor\Render\VertexArray.hpp"
#include "Actor\Render\Shader.hpp"
#include "Actor\Render\Texture.hpp"
#include "Actor\Render\Mesh.hpp"
#include "Actor\Render\Renderer.hpp"

#include "Component\Mesh\MeshComponent.hpp"

#include "BoxComponent.hpp"
#include "PhysWorld.hpp"

#include "System\Audio\SoundEvent.hpp"
#include "System\Audio\AudioSystem.hpp"
#include "Component\Audio\AudioComponent.hpp"

#include "Component\Camera\CameraComponent.hpp"
#include "Component\Camera\FPSCamera.hpp"
#include "Component\Camera\FollowCamera.hpp"
#include "Component\Camera\OrbitCamera.hpp"
#include "Component\Camera\SplineCamera.hpp"

#include "Actor\Camera\CameraActor.hpp"
#include "Actor\Camera\FPSActor.hpp"
#include "Actor\Camera\FollowActor.hpp"
#include "Actor\Camera\OrbitActor.hpp"
#include "Actor\Camera\SplineActor.hpp"

#include "Component\AI\AIComponent.hpp"
#include "Component\AI\AIState.hpp"

#include "GameObject\PlaneActor.hpp"

#include "BallMove.hpp"
#include "BallActor.hpp"

#include "TargetActor.hpp"


#include "Game.hpp"