#include "Text.h"
#include "Game.hpp"

Text::Text(Menu menu, State* state) : Entity(state), mMenu(menu)
{
	switch (menu)
	{
	//case (SplashText):
	//	mSprite = "SplashText";
	//	flash = true;
	//	break;
	case (MenuText):
		mSprite = "MenuText";
		break;

	}
}

void Text::updateCurrent(const GameTimer& gt)
{
	Text::check(gt);
	Entity::updateCurrent(gt);
}

void Text::drawCurrent() const
{
	renderer->World = getTransform();
	renderer->NumFramesDirty++;
}

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

void Text::check(const GameTimer& gt)
{
	if (GetAsyncKeyState('Q'))
		this->setPosition(0.f, -1.f, 0.f);

	if (GetAsyncKeyState(VK_UP))
	{
		show = false;

	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		show = true;
	}
}