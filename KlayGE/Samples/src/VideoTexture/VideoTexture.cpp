#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/Util.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/Font.hpp>
#include <KlayGE/Renderable.hpp>
#include <KlayGE/RenderableHelper.hpp>
#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/RenderEffect.hpp>
#include <KlayGE/FrameBuffer.hpp>
#include <KlayGE/SceneManager.hpp>
#include <KlayGE/Context.hpp>
#include <KlayGE/ResLoader.hpp>
#include <KlayGE/RenderSettings.hpp>
#include <KlayGE/Mesh.hpp>
#include <KlayGE/RenderableHelper.hpp>
#include <KlayGE/Light.hpp>
#include <KlayGE/SceneObjectHelper.hpp>
#include <KlayGE/Show.hpp>
#include <KlayGE/UI.hpp>
#include <KlayGE/Camera.hpp>

#include <KlayGE/RenderFactory.hpp>
#include <KlayGE/InputFactory.hpp>
#include <KlayGE/ShowFactory.hpp>

#include <vector>
#include <sstream>
#include <boost/tuple/tuple.hpp>
#include <boost/bind.hpp>

#include "VideoTexture.hpp"

using namespace std;
using namespace KlayGE;

#ifdef KLAYGE_COMPILER_MSVC
extern "C"
{
	_declspec(dllexport) uint32_t NvOptimusEnablement = 0x00000001;
}
#endif

namespace
{
	class RenderTeapot : public StaticMesh
	{
	public:
		RenderTeapot(RenderModelPtr model, std::wstring const & /*name*/)
			: StaticMesh(model, L"Teapot")
		{
			RenderFactory& rf = Context::Instance().RenderFactoryInstance();

			technique_ = rf.LoadEffect("VideoTexture.fxml")->TechniqueByName("Object");
		}

		void BuildMeshInfo()
		{
		}

		void OnRenderBegin()
		{
			App3DFramework const & app = Context::Instance().AppInstance();

			float4x4 const & view = app.ActiveCamera().ViewMatrix();
			float4x4 const & proj = app.ActiveCamera().ProjMatrix();

			*(technique_->Effect().ParameterByName("mvp")) = view * proj;
			*(technique_->Effect().ParameterByName("mv")) = view;
			*(technique_->Effect().ParameterByName("eye_pos")) = app.ActiveCamera().EyePos();
		}

		void VideoTexture(TexturePtr const & video_tex)
		{
			*(technique_->Effect().ParameterByName("video_tex")) = video_tex;
		}

		void LightPos(float3 const & light_pos)
		{
			*(technique_->Effect().ParameterByName("light_pos")) = light_pos;
		}

		void LightColor(float3 const & light_color)
		{
			*(technique_->Effect().ParameterByName("light_color")) = light_color;
		}

		void LightFalloff(float3 const & light_falloff)
		{
			*(technique_->Effect().ParameterByName("light_falloff")) = light_falloff;
		}
	};

	class TeapotObject : public SceneObjectHelper
	{
	public:
		TeapotObject()
			: SceneObjectHelper(SOA_Cullable)
		{
			renderable_ = SyncLoadModel("teapot.meshml", EAH_GPU_Read | EAH_Immutable, CreateModelFactory<RenderModel>(), CreateMeshFactory<RenderTeapot>())->Mesh(0);
		}

		void VideoTexture(TexturePtr const & video_tex)
		{
			checked_pointer_cast<RenderTeapot>(renderable_)->VideoTexture(video_tex);
		}

		void LightPos(float3 const & light_pos)
		{
			checked_pointer_cast<RenderTeapot>(renderable_)->LightPos(light_pos);
		}

		void LightColor(float3 const & light_color)
		{
			checked_pointer_cast<RenderTeapot>(renderable_)->LightColor(light_color);
		}

		void LightFalloff(float3 const & light_falloff)
		{
			checked_pointer_cast<RenderTeapot>(renderable_)->LightFalloff(light_falloff);
		}
	};

	enum
	{
		Exit
	};

	InputActionDefine actions[] =
	{
		InputActionDefine(Exit, KS_Escape)
	};
}


int main()
{
	ResLoader::Instance().AddPath("../../Samples/media/Common");

	Context::Instance().LoadCfg("KlayGE.cfg");

	VideoTextureApp app;
	app.Create();
	app.Run();

	return 0;
}

VideoTextureApp::VideoTextureApp()
					: App3DFramework("Video Texture")
{
	ResLoader::Instance().AddPath("../../Samples/media/VideoTexture");
}

bool VideoTextureApp::ConfirmDevice() const
{
	RenderEngine& re = Context::Instance().RenderFactoryInstance().RenderEngineInstance();
	RenderDeviceCaps const & caps = re.DeviceCaps();
	if (caps.max_shader_model < 1)
	{
		return false;
	}
	return true;
}

void VideoTextureApp::InitObjects()
{
	// ��������
	font_ = Context::Instance().RenderFactoryInstance().MakeFont("gkai00mp.kfont");

	this->LookAt(float3(-0.2f, 0.3f, -0.2f), float3(0, 0, 0));
	this->Proj(0.01f, 100);

	fpcController_.AttachCamera(this->ActiveCamera());
	fpcController_.Scalers(0.05f, 0.1f);

	light_ = MakeSharedPtr<PointLightSource>();
	light_->Attrib(0);
	light_->Color(float3(2, 2, 2));
	light_->Falloff(float3(1, 0, 1.0f));
	light_->Position(float3(0.25f, 0.5f, -1.0f));
	light_->AddToSceneManager();

	light_proxy_ = MakeSharedPtr<SceneObjectLightSourceProxy>(light_);
	checked_pointer_cast<SceneObjectLightSourceProxy>(light_proxy_)->Scaling(0.01f, 0.01f, 0.01f);
	light_proxy_->AddToSceneManager();

	InputEngine& inputEngine(Context::Instance().InputFactoryInstance().InputEngineInstance());
	InputActionMap actionMap;
	actionMap.AddActions(actions, actions + sizeof(actions) / sizeof(actions[0]));

	action_handler_t input_handler = MakeSharedPtr<input_signal>();
	input_handler->connect(boost::bind(&VideoTextureApp::InputHandler, this, _1, _2));
	inputEngine.ActionMap(actionMap, input_handler, true);

	ground_ = MakeSharedPtr<TeapotObject>();
	ground_->AddToSceneManager();

	ShowEngine& se = Context::Instance().ShowFactoryInstance().ShowEngineInstance();
	se.Load(ResLoader::Instance().Locate("big_buck_bunny.avi"));
	se.Play();

	UIManager::Instance().Load(ResLoader::Instance().Open("VideoTexture.uiml"));
}

void VideoTextureApp::OnResize(uint32_t width, uint32_t height)
{
	App3DFramework::OnResize(width, height);

	UIManager::Instance().SettleCtrls(width, height);
}

void VideoTextureApp::InputHandler(InputEngine const & /*sender*/, InputAction const & action)
{
	switch (action.first)
	{
	case Exit:
		this->Quit();
		break;
	}
}

void VideoTextureApp::DoUpdateOverlay()
{
	std::wostringstream stream;
	stream.precision(2);
	stream << std::fixed << this->FPS() << " FPS";

	font_->RenderText(0, 0, Color(1, 1, 0, 1), L"Video Texture", 16);
	font_->RenderText(0, 18, Color(1, 1, 0, 1), stream.str(), 16);

	UIManager::Instance().Render();
}

uint32_t VideoTextureApp::DoUpdate(uint32_t /*pass*/)
{
	RenderEngine& re = Context::Instance().RenderFactoryInstance().RenderEngineInstance();
	ShowEngine& se = Context::Instance().ShowFactoryInstance().ShowEngineInstance();

	Color clear_clr(0.2f, 0.4f, 0.6f, 1);
	if (Context::Instance().Config().graphics_cfg.gamma)
	{
		clear_clr.r() = 0.029f;
		clear_clr.g() = 0.133f;
		clear_clr.b() = 0.325f;
	}		
	re.CurFrameBuffer()->Clear(FrameBuffer::CBM_Color | FrameBuffer::CBM_Depth, clear_clr, 1.0f, 0);

	checked_pointer_cast<TeapotObject>(ground_)->VideoTexture(se.PresentTexture());
	checked_pointer_cast<TeapotObject>(ground_)->LightPos(light_->Position());
	checked_pointer_cast<TeapotObject>(ground_)->LightColor(light_->Color());
	checked_pointer_cast<TeapotObject>(ground_)->LightFalloff(light_->Falloff());

	return App3DFramework::URV_Need_Flush | App3DFramework::URV_Finished;
}
