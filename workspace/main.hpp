#pragma once

/* vcc includes */
#include <d3d9.h>
#include <d3dx9.h>

/* c++ includes */
#include "resources/resources.hpp"

/* comment pragma libs */
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

/* main defines */
static LPDIRECT3D9 g_pd3d = NULL;
static LPDIRECT3DDEVICE9 g_device = NULL;
static D3DPRESENT_PARAMETERS g_d3d9pp = {};
static HWND g_window;