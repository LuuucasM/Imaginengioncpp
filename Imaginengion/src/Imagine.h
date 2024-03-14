#pragma once

//For use by applications only. Not used by engine
//-----------CORE-------------------
#include "Core/Core.h"

#include "Core/Application.h"
#include "Core/Layer.h"
#include "Core/Log.h"
#include "Core/Assert.h"

#include "Core/Input.h"
#include "Core/Keycodes.h"
#include "Core/Mousecodes.h"

#include "Imgui/ImguiLayer.h"
//-----------END CORE----------------

//------------EVENTS-----------------
#include "Events/Delegate.h"
//-------------END EVENTS------------

//-------------ECS-------------------
#include "ECS/Components.h"
#include "ECS/Components/ScriptClass.h"
#include "ECS/Entity.h"
//------------END ECS----------------

//------------SCENE------------------
#include "Scene/Scene.h"
//------------END SCENE--------------

//------------RENDERING--------------
#include "Renderer/Renderer.h"

#include "Renderer/IndexBuffer.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/Shader.h"
#include "Renderer/FrameBuffer.h"
#include "Renderer/Texture.h"
#include "Renderer/Subtexture2D.h"
#include "Renderer/VertexArray.h"

#include "Renderer/OrthographicCamera.h"
#include "Renderer/OrthographicCameraController.h"
//-----------END RENDERING------------


//--------ENTITY COMPONENT SYSTEM------------
#include "ECS/Components.h"
//---------------------------------------