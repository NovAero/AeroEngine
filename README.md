<head> 
    <h1>AeroEngine (AE)</h1> 
    <h3><i>Version 1.1.2a</i></h3>
</head>

<body>
    <h4>Overview:</h4>
    <p>
        AeroEngine is a Direct3D 12 game engine, and is currently in early development.<br>
    </p>
    <h4>Changelist 1.1.2a:</h4>
    <small>10/04/2025</small>
    <p>
        -Added D3D12 initalisation and pipeline setup<br>
        -Fixed error caused from Simulation::PreInititalise not calling AEWindow::Initialise where the HWND is set, causing flow on error during swap chain creation<br>
        -Added AEApplication::Exit() for interface overloading<br>
        -Simulation::Exit() now flushes the Dx12 command queues, and clears all GPU processes that it is currently calling<br>
        -Added update loop functionality to Simulation, creating an fps and delta value - to be used later
    </p>
</body>
