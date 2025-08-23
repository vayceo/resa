#include "../main.h"
#include "game.h"
#include "../net/netgame.h"
#include <cmath>
#include "../gui/gui.h"

extern CGUI* pGUI;
extern CNetGame *pNetGame;

CWidget::CWidget(const char* szTexture)
{
	m_dwLastTickClicked = 0;
	m_pSprite = new CSprite2d();
	m_pSprite->m_pTexture = CUtil::LoadTextureFromDB("mobile", szTexture);
	m_bShouldBeDrawn = false;
	m_bColoured = false;
}

CWidget::~CWidget()
{
	delete m_pSprite;
}

void CWidget::SetDrawState(bool bState)
{
	m_bShouldBeDrawn = bState;
}

void CWidget::SetWidth(float w)
{
	this->w = pGUI->ScaleX(w);
}

void CWidget::SetHeight(float h)
{
	this->h = pGUI->ScaleX(h);
}

void CWidget::SetPos(float x, float y)
{
	this->x = pGUI->ScaleX(x);
	this->y = pGUI->ScaleY(y);

}

void CWidget::SetPosWithoutScale(float x, float y)
{
	this->x = x;
	this->y = y;
}

void CWidget::SetTexture(const char* szDB, const char* szTexture)
{
	m_pSprite->m_pTexture = CUtil::LoadTextureFromDB(szDB, szTexture);
}

void CWidget::Draw()
{
	if (!m_pSprite) return;
	if (!m_bShouldBeDrawn) return;
	CRGBA col;
	col.a = 255;
	col.b = 255;
	col.g = 255;
	col.r = 255;
	if (m_iState == 2)
	{
		if (GetTickCount() - m_dwLastTickClicked >= 75)
		{
			m_iState = 0;
		}
	}
	if (m_iState == 1)
	{
		//9CCF9C
		col.a = 255;
		col.b = 0x9C;
		col.g = 0xCF;
		col.r = 0x9C;
	}
	if (m_bColoured)
	{
		m_pSprite->Draw(x, y, w, h, &color);
		m_bShouldBeDrawn = false;
		return;
	}
	m_pSprite->Draw(x, y, w, h, &col);
	m_bShouldBeDrawn = false;
}

bool CWidget::IsXYInRect(float x, float y)
{
	return (x >= this->x && x <= this->x + w &&
		y >= this->y && y <= this->y + h);
}

void CWidget::SetColor(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
{
	color.a = a;
	color.r = r;
	color.g = g;
	color.b = b;
	m_bColoured = true;
}

void CWidget::ResetColor()
{
	m_bColoured = false;
}

void CWidget::SetState(int iState)
{
	if (iState == 2)
	{
		m_dwLastTickClicked = GetTickCount();
	}
	m_iState = iState;
}

int CWidget::GetState() const
{
	return m_iState;
}
