#pragma once
#include "SenkuEngine\Core\Input.h"
namespace Senku
{

	class WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;

		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual double GetMouseXImpl() override;
		virtual double GetMouseYImpl() override;
		virtual std::pair<double, double> GetMousePositionImpl() override;
	};
}