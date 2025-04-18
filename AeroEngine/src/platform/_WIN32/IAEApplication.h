#pragma once

#define ENTRYAPP(x) Win32::IAEApplication* EntryApplication() { return new x; }

namespace Win32 {
	class AERO_API IAEApplication {
	public:

		IAEApplication();
		virtual ~IAEApplication() {};

	public:

		virtual VOID SetupPGS() = 0;

		virtual VOID Register() = 0;
		virtual VOID Initialise() = 0;

		virtual VOID Update() = 0;

		virtual VOID Exit() = 0;

	};

	IAEApplication* EntryApplication();
}