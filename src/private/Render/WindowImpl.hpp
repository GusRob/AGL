/* ****************************************************************************************** */
/* Preprocessor/Namespace                                                                     */
/* ****************************************************************************************** */

#ifndef WINDOWIMPL_H
#define WINDOWIMPL_H

#include <AppDelegate.hpp>
#include <MTKViewDelegate.hpp>
#include <functional>

namespace AGL {

//tmp to remove
#define K_MAX_FRAMES_IN_FLIGHT (3)

/* ****************************************************************************************** */
/* Non-Member Variables                                                                       */
/* ****************************************************************************************** */

	/* None */

/* ****************************************************************************************** */
/* Class Declaration                                                                          */
/* ****************************************************************************************** */

	class Scene_c;
	class Window_ci {
		friend class Scene_ci;

/* ****************************************************************************************** */
/* Member Variables                                                                           */
/* ****************************************************************************************** */

	private:
		uint32_t prv_Width;
		uint32_t prv_Height;
		const char *prv_Title;

		Scene_c *prv_pScene;
		MTL::Device* prv_pDevice;

		MTK::View* prv_pMtkView;
		MTKViewDelegate_s* prv_pViewDelegate = nullptr;

		NS::Window* prv_pWindow;
		NS::Application *prv_pSharedApplication;
		AppDelegate_s *prv_pAppDelegate;

		MTL::Library* prv_pShaderLibrary;
		MTL::CommandQueue* prv_pCommandQueue;
		dispatch_semaphore_t prv_semaphore;
		MTL::RenderPipelineState* prv_pPSO;
		MTL::DepthStencilState* prv_pDepthStencilState;

		//int prv_frame = 0;
		//float prv_angle = 0;

		std::function<void(void)> prv_pStepFunction;

/* ****************************************************************************************** */
/* Member Constructor/Destructors                                                             */
/* ****************************************************************************************** */

	public:
		~Window_ci();
		Window_ci(int Width, int Height, const char *Title, std::function<void(void)> StepFunction);

/* ****************************************************************************************** */
/* Member Function Prototypes                                                                 */
/* ****************************************************************************************** */

	public:
		MTL::Device *GetMTLDevice();
		void ClearScene();
		void AddScene(Scene_c &SceneToAdd);
		void Run();
		void Quit();

	private:
		NS::Menu* CreateMenu();
		void Initialise();
		void DrawFrame();
		void BuildShaders();
		void BuildDepthStencilStates();

	public:
		friend std::ostream& operator<<(std::ostream &os, const Window_ci &Window);

/* ****************************************************************************************** */
/* Class Declaration End                                                                      */
/* ****************************************************************************************** */

	}; // class Window_ci

/* ****************************************************************************************** */
/* Non-Member Function Prototypes                                                             */
/* ****************************************************************************************** */

	/* None */

/* ****************************************************************************************** */
/* Preprocessor/Namespace End                                                                 */
/* ****************************************************************************************** */

} // namespace AGL

#endif // define WINDOWIMPL_H
