#include "Text.h"
#include "Game.hpp"

/**
 * @brief Constructs a Text entity with specified menu configuration
 * @param menu Menu type from Text::Menu enum
 * @param state Pointer to parent State context
 *
 * Initializes text properties based on menu type:
 * - MenuText: Uses "MenuText" sprite material
 * - Note: SplashText case commented out but available for expansion
 */
Text::Text(Menu menu, State* state) : Entity(state), mMenu(menu)
{
	switch (menu)
	{
	//case (SplashText):  // Reserved for future expansion
	//    mSprite = "SplashText";
	//    flash = true;
	//    break;
	case (MenuText):
		mSprite = "MenuText"; // Default menu text material
		break;

	}
}

/**
 * @brief Updates text state each frame
 * @param gt GameTimer with frame timing information
 *
 * Update sequence:
 * 1. Performs custom text checks (input handling)
 * 2. Calls base class update for standard entity behavior
 */
void Text::updateCurrent(const GameTimer& gt)
{
	Text::check(gt);
	Entity::updateCurrent(gt);
}

/**
 * @brief Submits text for rendering
 *
 * Updates renderer properties:
 * - World transform from current entity state
 * - Marks render item as dirty for constant buffer update
 */
void Text::drawCurrent() const
{
	renderer->World = getTransform();
	renderer->NumFramesDirty++;
}

/**
 * @brief Initializes render resources for the text
 *
 * Creates and configures RenderItem:
 * - Uses "boxGeo" geometry (placeholder for text quads)
 * - Applies material specified by mSprite
 * - Sets primitive type and draw parameters
 * - Registers with state's render item list
 */
void Text::buildCurrent()
{
	Game* game = mState->GetContext()->game;

	auto render = std::make_unique<RenderItem>();
	renderer = render.get();
	renderer->World = getTransform();
	renderer->ObjCBIndex = (UINT)mState->getRenderItems().size();
	renderer->Mat = game->getMaterials()[mSprite].get();
	renderer->Geo = game->getGeometries()["boxGeo"].get();
	renderer->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	renderer->IndexCount = renderer->Geo->DrawArgs["box"].IndexCount;
	renderer->StartIndexLocation = renderer->Geo->DrawArgs["box"].StartIndexLocation;
	renderer->BaseVertexLocation = renderer->Geo->DrawArgs["box"].BaseVertexLocation;
	mState->getRenderItems().push_back(std::move(render));
}

/**
 * @brief Handles text-specific input and state checks
 * @param gt GameTimer with frame timing information
 *
 * Key bindings:
 * - Q: Resets text position to (0, -1, 0)
 * - Up Arrow: Hides text (show = false)
 * - Down Arrow: Shows text (show = true)
 *
 * @note Uses direct key state checks for demonstration.
 *       Consider moving to centralized input system for production.
 */
void Text::check(const GameTimer& gt)
{
	// Debug position reset
	if (GetAsyncKeyState('Q'))
		this->setPosition(0.f, -1.f, 0.f);

	// Visibility controls
	if (GetAsyncKeyState(VK_UP))
	{
		show = false;

	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		show = true;
	}
}