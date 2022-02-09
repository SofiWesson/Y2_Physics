#pragma once

class GameState
{
public:
	GameState() {}
	virtual ~GameState() {}
	
	virtual bool startup() { return true; }
	virtual void shutdown() {}
	
	virtual void update(float deltaTime) {}
	virtual void draw() {}
};