#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../vendor/imgui/imgui.h"
#include "..//vendor/raknet/SingleProducerConsumer.h"
#include "imgui/imgui_internal.h"

enum eTouchType
{
	TOUCH_POP = 1,
	TOUCH_PUSH = 2,
	TOUCH_MOVE = 3
};

struct BUFFERED_COMMAND_TEXTDRAW
{
    uint16_t textdrawId;
};

class CGUI
{
public:
	CGUI();
	~CGUI();

	static void Render();

	float ScaleX(float x) { return m_vecScale.x * x; }
	float ScaleY(float y) { return m_vecScale.y * y; }
	ImFont* GetFont() { return m_pFont; }
	ImFont* GetSampFont() { return m_pSampFont; }
	ImFont* LoadFont(char *font, float fontsize);
	float GetFontSize() { return m_fFontSize; }

	static bool OnTouchEvent(int &type, int &num, int &x, int &y);

	void RenderText(ImVec2& pos, ImU32 col, bool bOutline, const char* text_begin, const char* text_end = nullptr);

	static void
	CreateCircleProgressBar(ImVec2 pos, ImColor color, int value, float radius,
							float thickness);

	static inline struct RwTexture* m_pIconTruckTex;
	static inline struct RwTexture* m_pPassengerButtonTex;

	ImVec2		m_vecScreenCenter;
	static inline bool m_bAnyItemHovered;
private:
	void RenderRakNetStatistics();
	void RenderVersion();
	//void ProcessPushedTextdraws();


private:
	ImFont* 	m_pFont;
	ImFont*		m_pSampFont;

	ImVec2		m_vecScale;
	float 		m_fFontSize;

    static DataStructures::SingleProducerConsumer<BUFFERED_COMMAND_TEXTDRAW> m_BufferedCommandTextdraws;

public:
	static inline ImVec2 GegPosFromGta(const ImVec2 &pos) {
		ImGuiIO &io = ImGui::GetIO();

		auto xPercent = (pos.x / 640) * 100;
		auto yPercent = (pos.y / 480) * 100;

		auto xSize = io.DisplaySize.x * xPercent / 100;
		auto ySize = io.DisplaySize.y * yPercent / 100;

		return ImVec2(xSize, ySize);
	}
	static inline ImVec2 GetCenterScreen(const ImVec2 &size) {
		ImGuiIO &io = ImGui::GetIO();
		return ImVec2( (io.DisplaySize.x - size.x) / 2, (io.DisplaySize.y - size.y) / 2 );
	}
	static inline ImVec2 GetPercent(float x, float y, ImVec2 &size) {
		ImGuiIO &io = ImGui::GetIO();
		return ImVec2( io.DisplaySize.x * (x / 100) - size.x, io.DisplaySize.y * (y / 100) - size.y);
	}
	static inline ImVec2 GetCenterOf(const ImVec2 &parentPos, const ImVec2 &parentSize, const ImVec2 &size) {
		return ImVec2( (parentPos.x + (parentSize.x / 2)) - (size.x / 2), (parentPos.y + (parentSize.y / 2)) - (size.y / 2) );
	}
	static inline ImVec2 CenterOfWindow(const ImVec2 &size) {
		//ImGuiIO &io = ImGui::GetIO();
		auto winSize = &ImGui::GetCurrentWindow()->Size;
		return ImVec2( (winSize->x - size.x) / 2, (winSize->y - size.y) / 2 );
	}

	static void RainbowText(const char* text, int fontSize);

	static void RenderMechanicHook();
	static void RenderPassengerButton();

    static void PushToBufferedQueueTextDrawPressed(uint16_t textdrawId);
    static void ProcessPushedTextdraws();
};

namespace ImGui {

	IMGUI_API void StyleColorsLiveRussia(ImGuiStyle *dst = NULL);
	IMGUI_API bool ButtonLr(const char* label, const ImVec2& size_arg, ImGuiButtonFlags flags = ImGuiButtonFlags_None);
	IMGUI_API bool ImageButtonLr(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), int frame_padding = -1, const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1));
}