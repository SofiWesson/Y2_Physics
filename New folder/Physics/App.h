#pragma once
#include "Renderer2D.h"

#include "Application.h"

class GameStateManager;

class App : public aie::Application
{
public:
	App();
	~App();

	virtual bool startup();
	virtual void shutdown();

	void update(float deltaTime);
	void draw();

	GameStateManager* GetGSM() { return m_GSM; };
	aie::Renderer2D* Get2DRenderer() { return m_2dRenderer; }
	aie::Font* GetFont() { return m_font; }

protected:
	aie::Renderer2D* m_2dRenderer;
	aie::Font* m_font;

	const float m_extents = 100;
	const float m_aspectRatio = 16.f / 9.f;

private:
	GameStateManager* m_GSM = nullptr;

};