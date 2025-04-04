#pragma once

namespace AE {

	class AERO_API Simulation : public Win32::IAEApplication, public Win32::AEWindow {
	public:
		Simulation();
		~Simulation();
		
		virtual VOID PreInitialise() override;
		virtual VOID Update() override;

		virtual LRESULT MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override;

	public: //Direct3D 12

	};
}