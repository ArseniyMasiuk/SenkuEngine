#include "SandboxLayer.h"



void SandBoxLayer::OnAttach()
{
}

void SandBoxLayer::OnDetach()
{
}

void SandBoxLayer::OnUpdate()
{
	LOG_TRACE("SandboxLayerUpdating");
}

bool SandBoxLayer::OnEvent(Event & event)
{
	return false;
}
