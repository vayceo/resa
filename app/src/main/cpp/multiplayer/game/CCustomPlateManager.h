#pragma once

#include "CRenderTarget.h"
#include <list>
#include "gui/gui.h"


enum ePlateType
{
	NUMBERPLATE_TYPE_NONE,
	NUMBERPLATE_TYPE_RUS,		//1
	NUMBERPLATE_TYPE_UA,		//2
	NUMBERPLATE_TYPE_BY,		//3
	NUMBERPLATE_TYPE_KZ,		//4
	NUMBERPLATE_TYPE_RU_POLICE,	//5
	NUMBERPLATE_TYPE_TAX,		//6
	NUMBERPLATE_TYPE_GOS		//7
};

struct CSprite2d;
struct RwTexture;

class CCustomPlateManager
{
    static inline RwTexture* pNoPlateTex;

	static inline CSprite2d* m_pRuSprite;
	static inline CSprite2d* m_pUaSprite;
	static inline CSprite2d* m_pBuSprite;
	static inline CSprite2d* m_pKzSprite;
	static inline CSprite2d* m_pRuPoliceSprite;

    static inline ImFont* m_pRuFont;

public:
	static void Initialise();	// call this once in CGame__Process
	static void Shutdown();

    static inline RwTexture* GetNoPlateTex() { return pNoPlateTex; }

	static RwTexture* createTexture(ePlateType type, std::string& szNumber, std::string& szRegion);
    static RwTexture* createRuPlate(std::string& szNumber, std::string& szRegion);
    static RwTexture* createRuPolicePlate(std::string& szNumber, std::string& szRegion);
	static RwTexture* createKzPlate(std::string& szNumber, std::string& szRegion);
	static RwTexture* createUaPlate(std::string& szNumber, std::string& szRegion);
	static RwTexture* createBuPlate(std::string& szNumber, std::string& szRegion);
};