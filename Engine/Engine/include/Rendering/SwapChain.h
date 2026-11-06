#pragma once
#include "Engine/Prerequisites.h"


class Window;
class Device;
class Texture2D;

class
	SwapChain {
public:
	SwapChain() = default;
	~SwapChain();

	SwapChain(const SwapChain&) = delete;
	SwapChain& operator=(const SwapChain&) = delete;

	SwapChain(SwapChain&&) = delete;
	SwapChain& operator=(SwapChain&&) = delete;


	HRESULT
		init(Device device, Window window,
			unsigned int width, unsigned int height);

	void
		destroy();

	HRESULT
		Present(unsigned int syncInterval = 1,
			unsigned int flags = 0);

	HRESULT
		GetBuffer(unsigned int bufferIndex,
			Texture2D backBuffer);

	HRESULT
		ResizeBuffers(unsigned int width,
			unsigned int height);

	IDXGISwapChain*
		getSwapChain() { return m_swapChain; }

private:
	IDXGISwapChain* m_swapChain = nullptr;
};