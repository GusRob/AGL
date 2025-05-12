/* ****************************************************************************************** */
/* Preprocessor/Namespace                                                                     */
/* ****************************************************************************************** */

#include <AppDelegate.hpp>

#include <iostream>

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

	AppDelegate_s::AppDelegate_s(std::function<void(void)> WindowInitialise, std::function<NS::Menu*(void)> WindowGetMenuBar)
	: prv_WindowInitialise(WindowInitialise), prv_WindowGetMenuBar(WindowGetMenuBar) {} 

/* ****************************************************************************************** */
/* Member Function Implementations                                                            */
/* ****************************************************************************************** */

	void AppDelegate_s::applicationWillFinishLaunching( NS::Notification* pNotification ) {
		NS::Menu* pMenu = prv_WindowGetMenuBar();
		NS::Application* pApp = reinterpret_cast< NS::Application* >( pNotification->object() );
		pApp->setMainMenu( pMenu );
		pApp->setActivationPolicy( NS::ActivationPolicy::ActivationPolicyRegular );
	}

/* ------------------------------------------------------------------------------------------ */

	void AppDelegate_s::applicationDidFinishLaunching( NS::Notification* pNotification ) {
		prv_WindowInitialise();
		NS::Application* pApp = reinterpret_cast< NS::Application* >( pNotification->object() );
		pApp->activateIgnoringOtherApps( true );
	}

/* ------------------------------------------------------------------------------------------ */

	bool AppDelegate_s::applicationShouldTerminateAfterLastWindowClosed( NS::Application* pSender ) {
		return true;
	}

/* ****************************************************************************************** */
/* Non-Member Function Implementations                                                        */
/* ****************************************************************************************** */

	/* None */

/* ****************************************************************************************** */
/* Private Non-Member Function Implementations                                                */
/* ****************************************************************************************** */

	/* None */

/* ****************************************************************************************** */
/* Preprocessor/Namespace End                                                                 */
/* ****************************************************************************************** */

} // namespace AGL
