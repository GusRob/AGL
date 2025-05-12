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
simd::float4x4 CurrentRotation = simd_matrix4x4(simd_quaternion(0.0f, 0.0f, 0.0f, 1.0f));
simd::float4x4 RotationMatrix = simd_matrix4x4(simd_quaternion(0.0f, 0.0f, 0.0f, 1.0f));
uint32_t ObjectIndex;

/* ****************************************************************************************** */
/* Function Prototypes                                                                        */
/* ****************************************************************************************** */

void MoveObject();

/* ****************************************************************************************** */
/* Main Function                                                                              */
/* ****************************************************************************************** */

int main( int argc, char* argv[] ) {
	CurrentRotation = simd_matrix4x4(simd_quaternion(0.0f, 0.0f, 0.0f, 1.0f));
	float angle = 0.01f; // radians per frame
	RotationMatrix = math::makeYRotate(angle);

	pWindow = new AGL::Window_c(C_WIDTH, C_HEIGHT, "Example Program 3: Animation", MoveObject);

	pScene = new AGL::Scene_c(*pWindow);

	ObjectIndex = pScene->AddObjectFromFile("examples/assets/Dragon/dragon.obj");
	pScene->SetObjectScale(ObjectIndex, simd::float3(300));
	pScene->SetObjectPosition(ObjectIndex, simd::float3{0, 0, -200});

	pWindow->AddScene(*pScene);

	pWindow->Run();
}

/* ****************************************************************************************** */
/* Function Implementations                                                                   */
/* ****************************************************************************************** */

void MoveObject() {
	CurrentRotation = simd_mul(CurrentRotation, RotationMatrix);
	pScene->SetObjectRotation(ObjectIndex, CurrentRotation);
}
