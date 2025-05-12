/* ****************************************************************************************** */
/* Preprocessor/Namespace                                                                     */
/* ****************************************************************************************** */

#ifndef APPDELEGATE_H
#define APPDELEGATE_H

#include <Metal/Metal.hpp>
#include <AppKit/AppKit.hpp>
#include <MetalKit/MetalKit.hpp>

#include <functional>

namespace AGL {

/* ****************************************************************************************** */
/* Non-Member Variables                                                                       */
/* ****************************************************************************************** */

	/* None */

/* ****************************************************************************************** */
/* Class Declaration                                                                          */
/* ****************************************************************************************** */

	struct AppDelegate_s : public NS::ApplicationDelegate {

/* ****************************************************************************************** */
/* Member Variables                                                                           */
/* ****************************************************************************************** */

	private:
		std::function<void(void)>  prv_WindowInitialise;
		std::function<NS::Menu*(void)> prv_WindowGetMenuBar;

/* ****************************************************************************************** */
/* Member Constructor/Destructors                                                             */
/* ****************************************************************************************** */

	public:
		AppDelegate_s(std::function<void(void)> WindowInitialise, std::function<NS::Menu*(void)> WindowGetMenuBar);

/* ****************************************************************************************** */
/* Member Function Prototypes                                                                 */
/* ****************************************************************************************** */

	public:
		void applicationWillFinishLaunching( NS::Notification* pNotification ) override;
		void applicationDidFinishLaunching( NS::Notification* pNotification ) override;
		bool applicationShouldTerminateAfterLastWindowClosed( NS::Application* pSender ) override;

		/*void keyDown(NS::Event* Event);
		void keyUp(NS::Event* Event);
		void mouseDown(NS::Event* Event);
		void mouseUp(NS::Event* Event);
		void mouseMoved(NS::Event* Event);
		void mouseDragged(NS::Event* Event);
		void scrollWheel(NS::Event* Event);
		void windowDidResize(NS::Event* Event);*/

/* ****************************************************************************************** */
/* Class Declaration End                                                                      */
/* ****************************************************************************************** */

	}; // class AppDelegate_s

/* ****************************************************************************************** */
/* Non-Member Function Prototypes                                                             */
/* ****************************************************************************************** */

	/* None */

/* ****************************************************************************************** */
/* Preprocessor/Namespace End                                                                 */
/* ****************************************************************************************** */

} // namespace AGL

#endif // define APPDELEGATE_H
