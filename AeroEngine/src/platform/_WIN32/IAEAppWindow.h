#pragma once

#define ENTRYAPP(x) IAEAppWindow* EntryApplication() { return new x; }

class AERO_API IAEAppWindow {
public:

	IAEAppWindow();
	virtual ~IAEAppWindow() {};

public:

	virtual VOID SetupPGS() = 0;

	virtual VOID Initialise() = 0;

	virtual VOID Update() = 0;

};

IAEAppWindow* EntryApplication();