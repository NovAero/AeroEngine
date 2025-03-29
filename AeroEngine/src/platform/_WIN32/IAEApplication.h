#pragma once

#define ENTRYAPP(x) IAEApplication* EntryApplication() { return new x; }

class AERO_API IAEApplication {
public:

	IAEApplication();
	virtual ~IAEApplication() {};

public:

	virtual VOID SetupPGS() = 0;

	virtual VOID Initialise() = 0;

	virtual VOID Update() = 0;

};

IAEApplication* EntryApplication();