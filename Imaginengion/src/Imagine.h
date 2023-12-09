#pragma once

//For use by applications only. Not used by engine
//-----------CORE-------------------
#include "Core/Application.h"
#include "Core/Layer.h"
#include "Core/Log.h"
#include "Core/Input.h"
#include "Core/Keycodes.h"
#include "Core/Mousecodes.h"
#include "Core/Keycodes.h"
#include "Core/Timer.h"
//-----------------------------------

//------------EVENTS-----------------
#include "Events/Event.h"
//-----------------------------------



//----------RENDERING-------------------
#include "Renderer/Renderer.h"
#include "Renderer/RenderCommand.h"

#include "Renderer/IndexBuffer.h"
#include "Renderer/BufferLayout.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/VertexArray.h"

#include "Renderer/OrthographicCamera.h"
#include "Renderer/OrthographicCameraController.h"
//---------------------------------------


//---------------ENTITY COMPONENT SYSTEM------------
#include "ECS/Components.h"