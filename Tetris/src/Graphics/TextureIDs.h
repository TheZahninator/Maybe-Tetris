#pragma once

//+++++++++++++++++++++++++++++++++++++++++++++
//Block IDs
//+++++++++++++++++++++++++++++++++++++++++++++

#define TEX_BLOCK_WHITE					0x0001

//+++++++++++++++++++++++++++++++++++++++++++++
//Background IDs
//+++++++++++++++++++++++++++++++++++++++++++++

#define TEX_BACKGROUND_FIELD_TILE		0x0100
#define TEX_BACKGROUND_FIELD_BORDER		0x0101
#define TEX_BACKGROUND_1				0x0102
#define TEX_BACKGROUND_OVERLAY			0x0103



#define TEX_KEY_OVERLAY					0x0002

#define TEX_KEY_OVERLAY_SIZE	DirectX::SimpleMath::Vector2(64, 64)

#define TEX_KEY_OVERLAY_UP_RELEASED_RECT DirectX::SimpleMath::Vector2(0, 0)
#define TEX_KEY_OVERLAY_UP_PRESSED_RECT	 DirectX::SimpleMath::Vector2(0, 64)

#define TEX_KEY_OVERLAY_DOWN_RELEASED_RECT  DirectX::SimpleMath::Vector2(64, 0)
#define TEX_KEY_OVERLAY_DOWN_PRESSED_RECT	DirectX::SimpleMath::Vector2(64, 64) 
											 
#define TEX_KEY_OVERLAY_LEFT_RELEASED_RECT  DirectX::SimpleMath::Vector2(64 * 2, 0)
#define TEX_KEY_OVERLAY_LEFT_PRESSED_RECT	DirectX::SimpleMath::Vector2(64 * 2, 64) 
											 
#define TEX_KEY_OVERLAY_RIGHT_RELEASED_RECT DirectX::SimpleMath::Vector2(64 * 3, 0)
#define TEX_KEY_OVERLAY_RIGHT_PRESSED_RECT	DirectX::SimpleMath::Vector2(64 * 3, 64) 
										    
#define TEX_KEY_OVERLAY_CCW_RELEASED_RECT   DirectX::SimpleMath::Vector2(0, 64 * 2)
#define TEX_KEY_OVERLAY_CCW_PRESSED_RECT	DirectX::SimpleMath::Vector2(64 * 2, 64 * 2) 
										    
#define TEX_KEY_OVERLAY_CW_RELEASED_RECT    DirectX::SimpleMath::Vector2(64, 64 * 2)
#define TEX_KEY_OVERLAY_CW_PRESSED_RECT	    DirectX::SimpleMath::Vector2(64 * 3, 64 * 2)