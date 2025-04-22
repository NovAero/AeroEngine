#include "AEAppPCH.h"
#include "engine/Simulation.h"
#include "platform/_WIN32/WinEntry.h"

class AEApplication : public AE::Simulation {
public:
	AEApplication() {}
	~AEApplication() {}

public:
	VOID SetupPGS();
	VOID Initialise() {
		Simulation::OnInit();
	}
	VOID Update() {
		Simulation::OnUpdate();
	}
	VOID Exit() {
		Simulation::Exit();
	}
};
//Interfacing for entry point... Magic!!
ENTRYAPP(AEApplication)

VOID AEApplication::SetupPGS()
{
	PerGameSettings::SetGameName(IDS_PERGAMENAME);
	PerGameSettings::SetShortName(IDS_SHORTNAME);
	PerGameSettings::SetMainIcon(IDI_MAINICON);
	PerGameSettings::SetSplashURL(IDS_SPLASHURL);
}
