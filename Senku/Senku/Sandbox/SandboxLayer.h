#pragma once
#include "SenkuEngine.h"

class SandBoxLayer : public Senku::Layer
{
public:
	

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(/*Timestep ts*/) override;
	
	virtual bool OnEvent(Event& event) override;
};