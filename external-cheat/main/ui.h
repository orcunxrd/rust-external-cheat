#pragma once
#include "..\mainrenderer.h"
#include "..\direct3d.h"
#include "..\stdafx.h"
#include "..\cheat\xor.h"

#pragma region Draw Funcs
typedef struct
{
	DWORD R;
	DWORD G;
	DWORD B;
	DWORD A;
}RGBA;
static void DrawRectImGui(int x, int y, int w, int h, ImVec4 color, float thickness)
{
	ImGui::GetOverlayDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(color), 0, 0, thickness);
}
static void ExternDrawLineImGui(int x1, int y1, int x2, int y2, RGBA color, int thickness)
{
	if (x2 != 0 && y2 != 0)
	{
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), ImGui::ColorConvertFloat4ToU32(ImVec4(color.R / 255.0, color.G / 255.0, color.B / 255.0, color.A / 255.0)), thickness);
	}
}
static void ExternDrawFilledRectImGui(int x, int y, int w, int h, RGBA color)
{
	ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color.R / 255.0, color.G / 255.0, color.B / 255.0, color.A / 255.0)), 0, 0);
}
static void ExternDrawCornerBoxImGui(int x, int y, int w, int h, int borderPx, RGBA color)
{
	ExternDrawFilledRectImGui(x + borderPx, y, w / 3, borderPx, color); //top 
	ExternDrawFilledRectImGui(x + w - w / 3 + borderPx, y, w / 3, borderPx, color); //top 
	ExternDrawFilledRectImGui(x, y, borderPx, h / 3, color); //left 
	ExternDrawFilledRectImGui(x, y + h - h / 3 + borderPx * 2, borderPx, h / 3, color); //left 
	ExternDrawFilledRectImGui(x + borderPx, y + h + borderPx, w / 3, borderPx, color); //bottom 
	ExternDrawFilledRectImGui(x + w - w / 3 + borderPx, y + h + borderPx, w / 3, borderPx, color); //bottom 
	ExternDrawFilledRectImGui(x + w + borderPx, y, borderPx, h / 3, color);//right 
	ExternDrawFilledRectImGui(x + w + borderPx, y + h - h / 3 + borderPx * 2, borderPx, h / 3, color);//right 
}
static RGBA HealthGradient(float Percent)
{
	if (Percent < 0 || Percent > 1) return RGBA{ 1, 1, 1, 1 };

	int Red, Green;
	if (Percent < 0.5)
	{
		Red = 255;
		Green = (int)(255 * Percent);
	}
	else
	{
		Green = 255;
		Red = 255 - (int)(255 * (Percent - 0.5) / 0.5);
	}

	return RGBA{ (DWORD)Red, (DWORD)Green, (DWORD)0 , (DWORD)255 };
}
static std::string UnicodeToUTF8(unsigned int codepoint)
{
	std::string out;

	if (codepoint <= 0x7f)
		out.append(1, static_cast<char>(codepoint));
	else if (codepoint <= 0x7ff)
	{
		out.append(1, static_cast<char>(0xc0 | ((codepoint >> 6) & 0x1f)));
		out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
	}
	else if (codepoint <= 0xffff)
	{
		out.append(1, static_cast<char>(0xe0 | ((codepoint >> 12) & 0x0f)));
		out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
		out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
	}
	else
	{
		out.append(1, static_cast<char>(0xf0 | ((codepoint >> 18) & 0x07)));
		out.append(1, static_cast<char>(0x80 | ((codepoint >> 12) & 0x3f)));
		out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
		out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
	}
	return out;
}
static std::string string_To_UTF8(const std::string& str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t* pwBuf = new wchar_t[nwLen + 1];
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char* pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr(pBuf);

	delete[]pwBuf;
	delete[]pBuf;

	pwBuf = NULL;
	pBuf = NULL;

	return retStr;
}
static void DrawFilledRectImGui(int x, int y, int w, int h, ImVec4 color)
{
	ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(color), 0, 0);
}
static void DrawNewTextImGui(int x, int y, RGBA color, const char* str)
{
	if (x != 0 && y != 0)
	{
		ImFont a;
		std::string utf_8_1 = std::string(str);
		std::string utf_8_2 = string_To_UTF8(utf_8_1);
		ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(ImVec4(color.R / 255.0, color.G / 255.0, color.B / 255.0, color.A / 255.0)), utf_8_2.c_str());
	}
}
static void DrawTextImVec4(int x, int y, ImVec4 color, const char* str)
{
	ImGui::PushFontShadow(0xFF000000);
	if (x != 0 && y != 0)
	{
		ImFont a;
		std::string utf_8_1 = std::string(str);
		std::string utf_8_2 = string_To_UTF8(utf_8_1);
		ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(color), utf_8_2.c_str());
	}
	ImGui::PopFontShadow();
}
static void DrawNewTextImGuiInt(int x, int y, RGBA color, int str)
{
	if (x != 0 && y != 0)
	{
		ImFont a;
		// std::string utf_8_1 = (int)str;
		std::string utf_8_2 = UnicodeToUTF8(str);
		ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(ImVec4(color.R / 255.0, color.G / 255.0, color.B / 255.0, color.A / 255.0)), utf_8_2.c_str());
	}
}
static void RectFilledImGui(int x0, int y0, int x1, int y1, ImColor color, float rounding, int rounding_corners_flags)
{
	ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(x0, y0), ImVec2(x1, y1), color, rounding, rounding_corners_flags);
}
static void DrawCircleFilledImGui(int x, int y, int radius, RGBA color, int segments)
{
	ImGui::GetOverlayDrawList()->AddCircleFilled(ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(ImVec4(color.R / 255.0, color.G / 255.0, color.B / 255.0, color.A / 255.0)), segments);
}
static void DrawCircleImGui(int x, int y, int radius, ImVec4 color, int segments)
{
	ImGui::GetOverlayDrawList()->AddCircle(ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(color), segments);
}
static void DrawTriangleImGui(int x1, int y1, int x2, int y2, int x3, int y3, RGBA color, float thickne)
{
	ImGui::GetOverlayDrawList()->AddTriangle(ImVec2(x1, y1), ImVec2(x2, y2), ImVec2(x3, y3), ImGui::ColorConvertFloat4ToU32(ImVec4(color.R / 255.0, color.G / 255.0, color.B / 255.0, color.A / 255.0)), thickne);
}
static void DrawTriangleFilledImGui(int x1, int y1, int x2, int y2, int x3, int y3, RGBA color)
{
	ImGui::GetOverlayDrawList()->AddTriangleFilled(ImVec2(x1, y1), ImVec2(x2, y2), ImVec2(x3, y3), ImGui::ColorConvertFloat4ToU32(ImVec4(color.R / 255.0, color.G / 255.0, color.B / 255.0, color.A / 255.0)));
}
static void DrawLineImGui(int x1, int y1, int x2, int y2, ImVec4 color, int thickness)
{
	if (x2 != 0 && y2 != 0)
	{
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), ImGui::ColorConvertFloat4ToU32(color), thickness);
	}
}
static void DrawBoxImGui(int x1, int y1, int bx, int by, ImVec4 color, int thickness)
{
	DrawLineImGui(x1 - (bx / 2), y1 - by, x1 - (bx / 2) + bx, y1 - by, color, thickness);
	DrawLineImGui(x1 - (bx / 2) + bx, y1 - by, x1 - (bx / 2) + bx, y1 - by + by, color, thickness);
	DrawLineImGui(x1 - (bx / 2) + bx, y1 - by + by, x1 - (bx / 2), y1 - by + by, color, thickness);
	DrawLineImGui(x1 - (bx / 2), y1 - by + by, x1 - (bx / 2), y1 - by, color, thickness);
}
static void DrawBottomHealthImGui(int x, int y, int w, int h, int borderPx, ImVec4 color)
{
	DrawFilledRectImGui(x + borderPx, y + h + borderPx, w, borderPx, color); //bottom 
	DrawFilledRectImGui(x + w - w + borderPx, y + h + borderPx, w, borderPx, color); //bottom 
}
static void DrawBottomHealth2ImGui(int x, int y, RGBA color, int value) //by exploder
{
	int width = 60;
	int height = 8;
	int pad = 1;
	uint32_t backcolor = ImColor(0.0f, 0.0f, 0.0f, 1.0f);

	ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x - (width / 2), y), ImVec2(x - (width / 2) + width, y + height), backcolor);
	ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x - (width / 2) + pad, y + pad), ImVec2(x - (width / 2) + pad + (value * width / 100) - (pad * 2), y + pad + height - (pad * 2)), ImGui::ColorConvertFloat4ToU32(ImVec4(color.R / 255.0, color.G / 255.0, color.B / 255.0, color.A / 255.0)));
}
static void DrawCornerBoxImGui(int x, int y, int w, int h, int borderPx, ImVec4 color)
{
	DrawFilledRectImGui(x + borderPx, y, w / 4.5, borderPx, color); //top 
	DrawFilledRectImGui(x + w - w / 4.5 + borderPx, y, w / 4.5, borderPx, color); //top 
	DrawFilledRectImGui(x, y, borderPx, h / 5, color); //left 
	DrawFilledRectImGui(x, y + h - h / 5 + borderPx * 2, borderPx, h / 5, color); //left 
	DrawFilledRectImGui(x + borderPx, y + h + borderPx, w / 4.5, borderPx, color); //bottom 
	DrawFilledRectImGui(x + w - w / 4.5 + borderPx, y + h + borderPx, w / 4.5, borderPx, color); //bottom 
	DrawFilledRectImGui(x + w + borderPx, y, borderPx, h / 5, color);//right 
	DrawFilledRectImGui(x + w + borderPx, y + h - h / 5 + borderPx * 2, borderPx, h / 5, color);//right 
}
static void DrawNormalBoxImGui(int x, int y, int w, int h, int borderPx, ImVec4 color)
{
	DrawFilledRectImGui(x + borderPx, y, w, borderPx, color); //top 
	DrawFilledRectImGui(x + w - w + borderPx, y, w, borderPx, color); //top 
	DrawFilledRectImGui(x, y, borderPx, h, color); //left 
	DrawFilledRectImGui(x, y + h - h + borderPx * 2, borderPx, h, color); //left 
	DrawFilledRectImGui(x + borderPx, y + h + borderPx, w, borderPx, color); //bottom 
	DrawFilledRectImGui(x + w - w + borderPx, y + h + borderPx, w, borderPx, color); //bottom 
	DrawFilledRectImGui(x + w + borderPx, y, borderPx, h, color);//right 
	DrawFilledRectImGui(x + w + borderPx, y + h - h + borderPx * 2, borderPx, h, color);//right 
}
static void DrawFilledBoxImGui(int x, int y, int w, int h, int borderPx, ImVec4 color)
{
	DrawFilledRectImGui(x + borderPx, y, w, borderPx, color); //top 
	DrawFilledRectImGui(x + w - w + borderPx, y, w, borderPx, color); //top 
	DrawFilledRectImGui(x, y, borderPx, h, color); //left 
	DrawFilledRectImGui(x, y + h - h + borderPx * 2, borderPx, h, color); //left 
	DrawFilledRectImGui(x + borderPx, y + h + borderPx, w, borderPx, color); //bottom 
	DrawFilledRectImGui(x + w - w + borderPx, y + h + borderPx, w, borderPx, color); //bottom 
	DrawFilledRectImGui(x + w + borderPx, y, borderPx, h, color);//right 
	DrawFilledRectImGui(x + w + borderPx, y + h - h + borderPx * 2, borderPx, h, color);//right 
}
static std::string Externstring_To_UTF8(const std::string& str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t* pwBuf = new wchar_t[nwLen + 1];
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char* pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr(pBuf);

	delete[]pwBuf;
	delete[]pBuf;

	pwBuf = NULL;
	pBuf = NULL;

	return retStr;
}
static void ExternDrawNewTextImGui(int x, int y, RGBA color, const char* str)
{
	if (x != 0 && y != 0)
	{


		ImFont a;
		std::string utf_8_1 = std::string(str);
		std::string utf_8_2 = Externstring_To_UTF8(utf_8_1);
		ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(ImVec4(color.R / 255.0, color.G / 255.0, color.B / 255.0, color.A / 255.0)), utf_8_2.c_str());
	}
}
#pragma endregion

class P_O render_base p_o {
public:
	void P_O Draw_Render_Menu p_o(IDirect3DDevice9* pDevice);
	~render_base();
	render_base(std::string path);
	void P_O Game_Loop p_o();
};

extern render_base* P_O m_render p_o;