/* ****************************************************************************************** */
/* Preprocessor                                                                               */
/* ****************************************************************************************** */

#include <iostream>

#include <Window.hpp>
#include <Scene.hpp>
#include <Math.hpp>

#define C_WIDTH  (600)
#define C_HEIGHT (600)

/* ****************************************************************************************** */
/* Variables                                                                                  */
/* ****************************************************************************************** */

AGL::Window_c *pWindow;
AGL::Scene_c *pScene;

/* ****************************************************************************************** */
/* Function Prototypes                                                                        */
/* ****************************************************************************************** */

/* None */

/* ****************************************************************************************** */
/* Main Function                                                                              */
/* ****************************************************************************************** */

int main( int argc, char* argv[] ) {
	pWindow = new AGL::Window_c(C_WIDTH, C_HEIGHT, "Example Program 2: Object", nullptr);
	pScene = new AGL::Scene_c(*pWindow);

	uint32_t ObjectIndex = pScene->AddObjectFromFile("examples/assets/HeadModel/Head.obj");
	pScene->SetObjectScale(ObjectIndex, simd::float3(500));
	pScene->SetObjectPosition(ObjectIndex, simd::float3{0, 30, -200});

	pWindow->AddScene(*pScene);

	pWindow->Run();
}

/* ****************************************************************************************** */
/* Function Implementations                                                                   */
/* ****************************************************************************************** */

/* None */
