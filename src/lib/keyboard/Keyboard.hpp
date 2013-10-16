#pragma once

#include <common/WindowsWrapper.hpp>
#include <climits>

namespace Keyboard
{
	HINSTANCE loadKeyboardLayout();
	int unloadKeyboardLayout(HINSTANCE kbdLibrary);
	int getKeyboardLayoutFile(char* layoutFile, DWORD bufferSize);
	int convertVirtualKeyToWChar(int virtualKey, PWCHAR out, PWCHAR buffer);

	#define KBD_LONG_POINTER64 __ptr64
	#define BUILD_WOW6432

	#if defined(BUILD_WOW6432)
	#define KBD_LONG_POINTER __ptr64
	#else
	#define KBD_LONG_POINTER
	#endif

	#define CAPLOK 0x01
	#define WCH_NONE 0xF000
	#define WCH_DEAD 0xF001

	typedef struct {
		BYTE Vk;
		BYTE ModBits;
	} VK_TO_BIT, *KBD_LONG_POINTER PVK_TO_BIT;

	typedef struct {
		PVK_TO_BIT pVkToBit;
		WORD wMaxModBits;
		BYTE * ModNumber;
	} MODIFIERS, *KBD_LONG_POINTER PMODIFIERS;

	typedef struct _VSC_VK {
		BYTE Vsc;
		USHORT Vk;
	} VSC_VK, *KBD_LONG_POINTER PVSC_VK;

	typedef struct _VK_VSC {
		BYTE Vk;
		BYTE Vsc;
	} VK_VSC, *KBD_LONG_POINTER PVK_VSC;

	#define TYPEDEF_VK_TO_WCHARS(n) typedef struct _VK_TO_WCHARS##n { \
	BYTE VirtualKey; \
	BYTE Attributes; \
	WCHAR wch[n]; \
	} VK_TO_WCHARS##n, *KBD_LONG_POINTER PVK_TO_WCHARS##n;

	TYPEDEF_VK_TO_WCHARS(1)
	TYPEDEF_VK_TO_WCHARS(2)
	TYPEDEF_VK_TO_WCHARS(3)
	TYPEDEF_VK_TO_WCHARS(4)
	TYPEDEF_VK_TO_WCHARS(5)
	TYPEDEF_VK_TO_WCHARS(6)
	TYPEDEF_VK_TO_WCHARS(7)
	TYPEDEF_VK_TO_WCHARS(8)
	TYPEDEF_VK_TO_WCHARS(9)
	TYPEDEF_VK_TO_WCHARS(10)

	typedef struct _VK_TO_WCHAR_TABLE {
		PVK_TO_WCHARS1 pVkToWchars;
		BYTE nModifications;
		BYTE cbSize;
	} VK_TO_WCHAR_TABLE, *KBD_LONG_POINTER PVK_TO_WCHAR_TABLE;

	typedef struct {
		DWORD dwBoth;
		WCHAR wchComposed;
		USHORT uFlags;
	} DEADKEY, *KBD_LONG_POINTER PDEADKEY;

	#define TYPEDEF_LIGATURE(n) typedef struct _LIGATURE##n { \
	BYTE VirtualKey; \
	WORD ModificationNumber; \
	WCHAR wch[n]; \
	} LIGATURE##n, *KBD_LONG_POINTER PLIGATURE##n;
 
	TYPEDEF_LIGATURE(1)
	TYPEDEF_LIGATURE(2)
	TYPEDEF_LIGATURE(3)
	TYPEDEF_LIGATURE(4)
	TYPEDEF_LIGATURE(5)
 
	typedef struct {
		BYTE vsc;
		WCHAR *KBD_LONG_POINTER pwsz;
	} VSC_LPWSTR, *KBD_LONG_POINTER PVSC_LPWSTR;

	typedef WCHAR *KBD_LONG_POINTER DEADKEY_LPWSTR;

	typedef struct tagKbdLayer {
		PMODIFIERS pCharModifiers;
		PVK_TO_WCHAR_TABLE pVkToWcharTable;
		PDEADKEY pDeadKey;
		PVSC_LPWSTR pKeyNames;
		PVSC_LPWSTR pKeyNamesExt;
		WCHAR *KBD_LONG_POINTER *KBD_LONG_POINTER pKeyNamesDead;
		USHORT *KBD_LONG_POINTER pusVSCtoVK;
		BYTE bMaxVSCtoVK;
		PVSC_VK pVSCtoVK_E0;
		PVSC_VK pVSCtoVK_E1;
		DWORD fLocaleFlags;
		BYTE nLgMax;
		BYTE cbLgEntry;
		PLIGATURE1 pLigature;
		DWORD dwType;
		DWORD dwSubType;
	} KBDTABLES, *KBD_LONG_POINTER PKBDTABLES;
 
	typedef struct _VK_FUNCTION_PARAM {
		BYTE NLSFEProcIndex;
		ULONG NLSFEProcParam;
	} VK_FPARAM, *KBD_LONG_POINTER PVK_FPARAM;
 
	typedef struct _VK_TO_FUNCTION_TABLE {
		BYTE Vk;
		BYTE NLSFEProcType;
		BYTE NLSFEProcCurrent;
		BYTE NLSFEProcSwitch;
		VK_FPARAM NLSFEProc[8];
		VK_FPARAM NLSFEProcAlt[8];
	} VK_F, *KBD_LONG_POINTER PVK_F;
 
	typedef struct tagKbdNlsLayer {
		USHORT OEMIdentifier;
		USHORT LayoutInformation;
		UINT NumOfVkToF;
		PVK_F pVkToF;
		INT NumOfMouseVKey;
		USHORT *KBD_LONG_POINTER pusMouseVKey;
	} KBDNLSTABLES, *KBD_LONG_POINTER PKBDNLSTABLES;

} // Keyboard
