#pragma once

class SamplerDesc;
class CommandSigniature;
class RootSigniature;
class ComputePSO;
class GraphicsPSO;

namespace AE::Graphics {

	void InitialiseCommonState(void);
	void DestroyCommonState(void);

    enum eDefaultTexture
    {
        Magenta2D,  // Useful for indicating missing textures
        BlackOpaque2D,
        BlackTransparent2D,
        WhiteOpaque2D,
        WhiteTransparent2D,
        DefaultNormalMap,
        BlackCubeMap,

        NumDefaultTextures
    };

}