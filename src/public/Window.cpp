/* ****************************************************************************************** */
/* Preprocessor/Namespace                                                                     */
/* ****************************************************************************************** */

#include <iostream>
#include <Window.hpp>
#include <WindowImpl.hpp>

namespace AGL {

/* ****************************************************************************************** */
/* Private Non-Member Variables                                                               */
/* ****************************************************************************************** */

	#define C_DEFAULT_WIDTH  (600)
	#define C_DEFAULT_HEIGHT (600)

/* ****************************************************************************************** */
/* Private Non-Member Function Prototypes                                                     */
/* ****************************************************************************************** */

	/* None */

/* ****************************************************************************************** */
/* Member Constructor/Destructor Implementations                                              */
/* ****************************************************************************************** */

	Window_c::~Window_c() = default;

/* ------------------------------------------------------------------------------------------ */

	Window_c::Window_c(int Width, int Height, const char *Title, std::function<void(void)> StepFunction) {
		//Create the pointer to the implementation class
		prv_pImpl = std::unique_ptr<Window_ci>(new Window_ci(Width, Height, Title, StepFunction));
	}

/* ****************************************************************************************** */
/* Member Function Implementations                                                            */
/* ****************************************************************************************** */

	void Window_c::ClearScene() {
		prv_pImpl->ClearScene();
	}

/* ------------------------------------------------------------------------------------------ */

	void Window_c::AddScene(Scene_c &SceneToAdd) {
		prv_pImpl->AddScene(SceneToAdd);
	}

/* ------------------------------------------------------------------------------------------ */

	void Window_c::Run() {
		prv_pImpl->Run();
	}

/* ------------------------------------------------------------------------------------------ */

	void Window_c::Quit() {
		prv_pImpl->Quit();
	}

/* ****************************************************************************************** */
/* Non-Member Function Implementations                                                        */
/* ****************************************************************************************** */

	std::ostream& operator<<(std::ostream &os, const Window_c &Window) {
		os << "Window_c(" << *(Window.prv_pImpl) << ")";
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
