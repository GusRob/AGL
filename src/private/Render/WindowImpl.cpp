/* ****************************************************************************************** */
/* Preprocessor/Namespace                                                                     */
/* ****************************************************************************************** */

#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION

#include <iostream>
#include <WindowImpl.hpp>
#include <Scene.hpp>
#include <SceneImpl.hpp>

#include <Metal/Metal.hpp>
#include <AppKit/AppKit.hpp>
#include <MetalKit/MetalKit.hpp>

#include <AGLTypes.hpp>
#include <ShaderLib.hpp>

namespace AGL {

/* ****************************************************************************************** */
/* Private Non-Member Variables                                                               */
/* ****************************************************************************************** */

	/* None */

/* ****************************************************************************************** */
/* Private Non-Member Function Prototypes                                                     */
/* ****************************************************************************************** */

	/* None */

/* ****************************************************************************************** */
/* Member Constructor/Destructor Implementations                                              */
/* ****************************************************************************************** */

	Window_ci::~Window_ci() {
		prv_pShaderLibrary->release();
		prv_pPSO->release();
		prv_pCommandQueue->release();
		prv_pDevice->release();
		prv_pMtkView->release();
		//prv_pAppDelegate->release();
		prv_pWindow->release();
	}

/* ------------------------------------------------------------------------------------------ */

	Window_ci::Window_ci(int Width, int Height, const char *Title, std::function<void(void)> StepFunction)
	: prv_Width(Width), prv_Height(Height), prv_Title(Title), prv_pStepFunction(StepFunction) {
		prv_pSharedApplication = NS::Application::sharedApplication();
		prv_pAppDelegate = new AppDelegate_s([this](){this->Initialise();}, [this](){return this->CreateMenu();});
		prv_pSharedApplication->setDelegate(prv_pAppDelegate);
		prv_pDevice = MTL::CreateSystemDefaultDevice();
		BuildShaders();
		BuildDepthStencilStates();
		ClearScene();
	}

/* ****************************************************************************************** */
/* Member Function Implementations                                                            */
/* ****************************************************************************************** */

	MTL::Device *Window_ci::GetMTLDevice() {
		return prv_pDevice;
	}

/* ------------------------------------------------------------------------------------------ */

	void Window_ci::ClearScene() {
		prv_pScene = nullptr;
	}

/* ------------------------------------------------------------------------------------------ */

	void Window_ci::AddScene(Scene_c &SceneToAdd) {
		prv_pScene = &SceneToAdd;
	}

/* ------------------------------------------------------------------------------------------ */

	void Window_ci::Run() {
		prv_pSharedApplication->run();
	}

/* ------------------------------------------------------------------------------------------ */

	void Window_ci::Quit() {
		return;
	}

/* ------------------------------------------------------------------------------------------ */

	NS::Menu* Window_ci::CreateMenu() {
		using NS::StringEncoding::UTF8StringEncoding;

		NS::Menu* pMainMenu = NS::Menu::alloc()->init();
		NS::MenuItem* pAppMenuItem = NS::MenuItem::alloc()->init();
		NS::Menu* pAppMenu = NS::Menu::alloc()->init( NS::String::string( "Appname", UTF8StringEncoding ) );

		NS::String* appName = NS::RunningApplication::currentApplication()->localizedName();
		NS::String* quitItemName = NS::String::string( "Quit ", UTF8StringEncoding )->stringByAppendingString( appName );
		SEL quitCb = NS::MenuItem::registerActionCallback( "appQuit", [](void*,SEL,const NS::Object* pSender){
			auto pApp = NS::Application::sharedApplication();
			pApp->terminate( pSender );
		} );

		NS::MenuItem* pAppQuitItem = pAppMenu->addItem( quitItemName, quitCb, NS::String::string( "q", UTF8StringEncoding ) );
		pAppQuitItem->setKeyEquivalentModifierMask( NS::EventModifierFlagCommand );
		pAppMenuItem->setSubmenu( pAppMenu );

		NS::MenuItem* pWindowMenuItem = NS::MenuItem::alloc()->init();
		NS::Menu* pWindowMenu = NS::Menu::alloc()->init( NS::String::string( "Window", UTF8StringEncoding ) );

		SEL closeWindowCb = NS::MenuItem::registerActionCallback( "windowClose", [](void*, SEL, const NS::Object*){
			auto pApp = NS::Application::sharedApplication();
				pApp->windows()->object< NS::Window >(0)->close();
		} );
		NS::MenuItem* pCloseWindowItem = pWindowMenu->addItem( NS::String::string( "Close Window", UTF8StringEncoding ), closeWindowCb, NS::String::string( "w", UTF8StringEncoding ) );
		pCloseWindowItem->setKeyEquivalentModifierMask( NS::EventModifierFlagCommand );

		pWindowMenuItem->setSubmenu( pWindowMenu );

		pMainMenu->addItem( pAppMenuItem );
		pMainMenu->addItem( pWindowMenuItem );

		pAppMenuItem->release();
		pWindowMenuItem->release();
		pAppMenu->release();
		pWindowMenu->release();

		return pMainMenu->autorelease();
	}

/* ------------------------------------------------------------------------------------------ */

	void Window_ci::Initialise() {
		CGRect frame = (CGRect) {
			{100.0, 100.0},
			{CGFloat(prv_Width), CGFloat(prv_Height)}
		};

		prv_pWindow = NS::Window::alloc()->init(
			frame,
			NS::WindowStyleMaskClosable|NS::WindowStyleMaskTitled|NS::WindowStyleMaskResizable,
			NS::BackingStoreBuffered,
			false );

		prv_pMtkView = MTK::View::alloc()->init( frame, prv_pDevice );
		prv_pMtkView->setColorPixelFormat( MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB );
		prv_pMtkView->setClearColor( MTL::ClearColor::Make( 1.0, 1.0, 1.0, 1.0 ) );

		prv_pViewDelegate = new MTKViewDelegate_s([this](){this->DrawFrame();});
		prv_pMtkView->setDelegate( prv_pViewDelegate );

		prv_pCommandQueue = prv_pDevice->newCommandQueue();
		prv_semaphore = dispatch_semaphore_create(K_MAX_FRAMES_IN_FLIGHT);

		prv_pWindow->setContentView( prv_pMtkView );
		prv_pWindow->setTitle( NS::String::string( prv_Title, NS::StringEncoding::UTF8StringEncoding ) );

		prv_pWindow->makeKeyAndOrderFront( nullptr );
	}

/* ------------------------------------------------------------------------------------------ */

	void Window_ci::DrawFrame() {
		if(prv_pScene) {
			NS::AutoreleasePool* pPool = NS::AutoreleasePool::alloc()->init();

			MTL::CommandBuffer* pCmd = prv_pCommandQueue->commandBuffer();
			dispatch_semaphore_wait( prv_semaphore, DISPATCH_TIME_FOREVER );
			pCmd->addCompletedHandler( ^void( MTL::CommandBuffer* pCmd ){
				dispatch_semaphore_signal( prv_semaphore );
			});

			MTL::RenderPassDescriptor* pRpd = prv_pMtkView->currentRenderPassDescriptor();
			MTL::RenderCommandEncoder* pEnc = pCmd->renderCommandEncoder( pRpd );

			pEnc->setRenderPipelineState( prv_pPSO );
			pEnc->setDepthStencilState( prv_pDepthStencilState );

			prv_pScene->prv_pImpl->DrawScene(*pEnc);

			pEnc->endEncoding();
			pCmd->presentDrawable( prv_pMtkView->currentDrawable() );
			pCmd->commit();

			pPool->release();

			if(prv_pStepFunction != NULL){
				prv_pStepFunction();
			}
		}
	}

/* ------------------------------------------------------------------------------------------ */

	void Window_ci::BuildShaders() {
		NS::Error* pError = nullptr;
		dispatch_data_t library_data = dispatch_data_create(ShaderLibRawData, ShaderLibRawData_len, NULL, DISPATCH_DATA_DESTRUCTOR_DEFAULT);
		MTL::Library* pLibrary = prv_pDevice->newLibrary(library_data, &pError);
		if ( !pLibrary ) {
			__builtin_printf( "%s", pError->localizedDescription()->utf8String() );
			assert( false );
		}

		MTL::Function* pVertexFn = pLibrary->newFunction( NS::String::string("vertexMain", NS::StringEncoding::UTF8StringEncoding) );
		MTL::Function* pFragFn = pLibrary->newFunction( NS::String::string("fragmentMain", NS::StringEncoding::UTF8StringEncoding) );

		MTL::RenderPipelineDescriptor* pDesc = MTL::RenderPipelineDescriptor::alloc()->init();
		pDesc->setVertexFunction( pVertexFn );
		pDesc->setFragmentFunction( pFragFn );
		pDesc->colorAttachments()->object(0)->setPixelFormat( MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB );
		pDesc->colorAttachments()->object(0)->setBlendingEnabled(true);
		pDesc->colorAttachments()->object(0)->setRgbBlendOperation(MTL::BlendOperationAdd);
		pDesc->colorAttachments()->object(0)->setAlphaBlendOperation(MTL::BlendOperationAdd);
		pDesc->colorAttachments()->object(0)->setSourceRGBBlendFactor(MTL::BlendFactorSourceAlpha);
		pDesc->colorAttachments()->object(0)->setDestinationRGBBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);
		pDesc->colorAttachments()->object(0)->setSourceAlphaBlendFactor(MTL::BlendFactorOne);
		pDesc->colorAttachments()->object(0)->setDestinationAlphaBlendFactor(MTL::BlendFactorZero);

		pDesc->setDepthAttachmentPixelFormat( MTL::PixelFormat::PixelFormatDepth32Float );

		prv_pPSO = prv_pDevice->newRenderPipelineState( pDesc, &pError );
		if ( !prv_pPSO ) {
			__builtin_printf( "%s", pError->localizedDescription()->utf8String() );
			assert( false );
		}

		pVertexFn->release();
		pFragFn->release();
		pDesc->release();
		prv_pShaderLibrary = pLibrary;
	}

/* ------------------------------------------------------------------------------------------ */

	void Window_ci::BuildDepthStencilStates()
	{
		MTL::DepthStencilDescriptor* pDsDesc = MTL::DepthStencilDescriptor::alloc()->init();
		pDsDesc->setDepthCompareFunction( MTL::CompareFunction::CompareFunctionLess );
		pDsDesc->setDepthWriteEnabled( true );

		prv_pDepthStencilState = prv_pDevice->newDepthStencilState( pDsDesc );

		pDsDesc->release();
	}

/* ****************************************************************************************** */
/* Non-Member Function Implementations                                                        */
/* ****************************************************************************************** */

	std::ostream& operator<<(std::ostream &os, const Window_ci &Window) {
		os << "Window_ci(" << Window.prv_Title << ", " << Window.prv_Width << ", " << Window.prv_Height << ", ";
		if(Window.prv_pScene) {
			os << *Window.prv_pScene;
		} else {
			os << "No Scene";
		}
		os << ")";
		return os;
	}

/* ****************************************************************************************** */
/* Private Non-Member Function Implementations                                                */
/* ****************************************************************************************** */

	/* None */

/* ****************************************************************************************** */
/* Preprocessor/Namespace End                                                                 */
/* ****************************************************************************************** */

} // namespace AGL
