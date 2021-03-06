#include "InternetExplorer.hpp"

#include <system/Process.hpp>
#include <common/Exception.hpp>
#include <log/Log.hpp>

namespace Malicious {
#pragma pack(push, 1)
	typedef struct tagCOGETSERVERPID_OBJREFHDR {
		DWORD  signature;  /* Should be 'MEOW'. */
		BYTE  padding[48];
		USHORT  pid;
	} COGETSERVERPID_OBJREFHDR;
#pragma pack(pop)

	// https://github.com/kimgr/cogetserverpid
	void CoGetServerPID(IUnknown* punk, DWORD* pdwPID) {
		HRESULT hr;
		IUnknown* pProxyManager = NULL;
		IStream* pMarshalStream = NULL;
		HGLOBAL hg = NULL;
		COGETSERVERPID_OBJREFHDR *pObjRefHdr = NULL;
		LARGE_INTEGER zero = {0};

		if(pdwPID == NULL) {
			return;
		}
		if(punk == NULL) {
			return;
		}

		/* Make sure this is a standard proxy, otherwise we can't make any
		   assumptions about OBJREF wire format. */
		hr = punk->QueryInterface(IID_IProxyManager, (void**)&pProxyManager);
		if(FAILED(hr)) {
			return;
		}

		pProxyManager->Release();

		/* Marshal the interface to get a new OBJREF. */
		hr = ::CreateStreamOnHGlobal(NULL, TRUE, &pMarshalStream);
		if(SUCCEEDED(hr)) {
			hr = ::CoMarshalInterface(pMarshalStream, IID_IUnknown, punk, MSHCTX_INPROC, NULL, MSHLFLAGS_NORMAL);
			if(SUCCEEDED(hr)) {
				/* We just created the stream so it's safe to go back to a raw pointer. */
				hr = ::GetHGlobalFromStream(pMarshalStream, &hg);
				if(SUCCEEDED(hr)) {
					/* Start out pessimistic. */
					hr = RPC_E_INVALID_OBJREF;

					pObjRefHdr = (COGETSERVERPID_OBJREFHDR*)GlobalLock(hg);
					if(pObjRefHdr != NULL) {
						/* Verify that the signature is MEOW. */
						if(pObjRefHdr->signature == 0x574f454d) {
							/* We got the remote PID! */
							*pdwPID = pObjRefHdr->pid;
							hr = S_OK;
						}
						GlobalUnlock(hg);
					}
				}
				/* Rewind stream and release marshal data to keep refcount in order. */
				pMarshalStream->Seek(zero, SEEK_SET, NULL);
				CoReleaseMarshalData(pMarshalStream);
			}
			pMarshalStream->Release();
		}
	}

	InternetExplorer::InternetExplorer(bool killOnDestroyArg)
		: pid(0), killOnDestroy(killOnDestroyArg) {
		HRESULT CoInitializeResult = CoInitialize(NULL);
		if(CoInitializeResult != S_OK) {
			throw Common::Exception("COM Initialization failed : " + Common::toString(CoInitializeResult));
		}

		HRESULT CoCreateInstanceResult = CoCreateInstance(CLSID_InternetExplorer, NULL, CLSCTX_LOCAL_SERVER, IID_IWebBrowser2, (void**)&browser);
		if(CoCreateInstanceResult != S_OK || !browser) {
			throw Common::Exception("CoCreateInstance failed : " + Common::toString(CoCreateInstanceResult));
		}

		HRESULT PutVisibleResult = browser->put_Visible(false);
		if(PutVisibleResult != S_OK) {
			throw Common::Exception("IWebBrowser2::PutVisible failed : " + Common::toString(PutVisibleResult));
		}

		CoGetServerPID(browser, &pid);
	}

	InternetExplorer::~InternetExplorer() {
		if(killOnDestroy) {
			LOG << "Killing IE with PID : "+Common::toString(pid);
			kill();
		}
	}

	DWORD InternetExplorer::getPid() const {
		return pid;
	}

	void InternetExplorer::kill() {
		HRESULT quitResult = browser->Quit();
		if(quitResult != S_OK) {
			throw Common::Exception("IWebBrowser2::Quit failed : " + Common::toString(quitResult));
		}
	}
}
