#pragma once

#include "lcd.h"

class CLCD :
	public CComObjectRootEx<CComObjectThreadModel>,
	public IDispatchImpl<ILCD>
{
public:
	BEGIN_COM_MAP(CLCD)
		COM_INTERFACE_ENTRY(ILCD)
		COM_INTERFACE_ENTRY(IDispatch)
	END_COM_MAP()

	void Initialize(LCDBase_t *lcd)
	{
		m_lcd = lcd;
	}

	// ILCD methods
	STDMETHOD (get_Display)(LPSAFEARRAY *ppsa)
	{
		SAFEARRAYBOUND sab = {0};
		sab.cElements = m_lcd->width * m_lcd->height;
		sab.lLbound = 0;
		LPSAFEARRAY psa = SafeArrayCreate(VT_UI1, 1, &sab);

		LPBYTE lpData = NULL;
		SafeArrayAccessData(psa, (LPVOID *) &lpData);
		u_char *image = m_lcd->image(m_lcd);
		memcpy(lpData, image, GRAY_DISPLAY_SIZE);
		free(image);
		SafeArrayUnaccessData(psa);

		*ppsa = psa;
		return S_OK;
	}

	STDMETHOD (get_Width)(LPWORD lpWidth) 
	{
		*lpWidth = m_lcd->width;
		return S_OK;
	}

	STDMETHOD (get_Height)(LPWORD lpHeight)
	{
		*lpHeight = LCD_HEIGHT;
		return S_OK;
	}

	STDMETHOD(Draw)(BYTE Display[8192])
	{
		u_char *image = m_lcd->image(m_lcd);
		memcpy(Display, image, GRAY_DISPLAY_SIZE);
		free(image);
		return S_OK;
	}

	STDMETHOD(GetByteArray)(BYTE Display[8192])
	{
		memcpy(Display, m_lcd->image(m_lcd), m_lcd->width * m_lcd->height);
		return S_OK;
	}

private:
	LCDBase_t *m_lcd;
};