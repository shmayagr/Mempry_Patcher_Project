#include"stafx.h"
#include"accessMem.h"
#include<map>
#include<conio.h>
#include"PEDeatails.h"
PROCESS_INFORMATION getInjectedProcessInformation(const TCHAR* exepath)
{
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	CreateProcess(exepath,NULL,NULL,NULL,FALSE,NULL,NULL,NULL,&si,&pi);
	return pi;
	//return OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, pi.dwProcessId);
}

HANDLE getInectedProcessHandle(DWORD pid)
{
	return OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, pid);
}

HANDLE getInectedThreadHandle(DWORD tid) 
{
	return OpenThread(THREAD_SET_CONTEXT | THREAD_SUSPEND_RESUME | THREAD_GET_CONTEXT, FALSE, tid);
}

char* pProcessVirtualAlloc(HANDLE hProcess,SIZE_T page_count) 
{
	SIZE_T page_size = 1 << 12;// depepnds on the size you want to alloc 
	SIZE_T total_pages_alloc = page_size*page_count;
	char* buffer = (char*)VirtualAllocEx(hProcess, NULL, total_pages_alloc, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	/*char* buffer = (char*)VirtualAlloc( NULL, total_pages_alloc, MEM_COMMIT | MEM_RESERVE, PAGE_WRITECOMBINE);*/

	return buffer;
}


BYTE* getAssEmblyCode()
{
	BYTE Scode[] = { 0x60,
	0x68, 0x11, 0x11, 0x11, 0x11,
	0xb8, 0x22, 0x22, 0x22, 0x22,
	0xff, 0xd0,
	0x61,
	0x68, 0x33, 0x33, 0x33, 0x33,
	0xc3 };
	BYTE* code = (BYTE*)malloc(sizeof(Scode));
	memcpy(code, Scode, sizeof(Scode));
	return code;
}

HANDLE getFunctionAddrFromKernel32(const char* funcName)
{
	return GetProcAddress(GetModuleHandleW(L"kernel32.dll"), funcName);
}

//void writeToinjectedMemoryProcess()
void basicAssemblyInject(HANDLE hProcess,HANDLE hthread, void* allocatedBuffer, SIZE_T nameWriteOffset, const TCHAR* dllpath,BYTE* assemblyCode,SIZE_T assemblyCodeSize) 
{
	SuspendThread(hthread);
	CONTEXT tContext;
	tContext.ContextFlags = CONTEXT_FULL;
	GetThreadContext(hthread, &tContext);
	DWORD loadLybAddress = (DWORD)getFunctionAddrFromKernel32("LoadLibraryA");
	void* nameOffsetP = ((char*)allocatedBuffer + nameWriteOffset);
	memcpy((void*)(assemblyCode + 2), &nameOffsetP, sizeof(char*));
	memcpy((void*)(assemblyCode + 7), (void*)&(loadLybAddress), sizeof(DWORD));
	memcpy((void*)(assemblyCode + 0xf), (void*)&(tContext.Eip), sizeof(DWORD));
	WriteProcessMemory(hProcess, nameOffsetP, dllpath, strlen(dllpath) + 1, NULL);
	WriteProcessMemory(hProcess, allocatedBuffer, assemblyCode, assemblyCodeSize, NULL);
	tContext.Eip = (DWORD)allocatedBuffer;
	SetThreadContext(hthread, &tContext);
	ResumeThread(hthread);
}
void examp()
{
	printf("w\n");
	if (2 == 3)
	{
		printf("How do you got here?\n");
	}
	else
		printf("of course\n");
}
int main(int argc,char**argv) {
	char executabinjectedpathlepath[] = "simplecheck.exe";//argv[1];
	char dllpath[] = "injectedDLL.dll";//argv[2];
	PROCESS_INFORMATION pi = getInjectedProcessInformation(executabinjectedpathlepath);
	//handle currentp = (handle)getmodulehandle(null);
	HANDLE hProcess = getInectedProcessHandle(pi.dwProcessId);
	HANDLE hThread = getInectedThreadHandle(pi.dwThreadId);
	BYTE assemblycode[] = { 
	0x60,
	0x68, 0x11, 0x11, 0x11, 0x11,
	0xb8, 0x22, 0x22, 0x22, 0x22,
	0xff, 0xd0,
	0x61,
	0x68, 0x33, 0x33, 0x33, 0x33,
	0xc3 };
	void* buffer = (void*)pProcessVirtualAlloc(hProcess, 1);
	basicAssemblyInject(hProcess, hThread, buffer, 2048, dllpath, assemblycode, sizeof(assemblycode));

	//char* virtualAllocated = pProcessVirtualAlloc(pi.hProcess, 10);
	//printf("addr %d alloc %d", getFunctionAddrFromKernel32("LoadLibraryA"), virtualAllocated);
	//printf("f = %d", f(3));
	/*char welcome[28] = "welcome to our program!!!!!";
	printf("%s", welcome);
	getFirstSectionAddress();*/
	/*std::map<std::string, int>m;
	
	std::string a = "ff";
	m[a] = 5;
	char as[4] = "ff";
	std::string ma(as);
	printf("%s",&ma[0]);
	
	printf("mval = %d", m[as]);*/
//	printf("%s",GetCurrentProcess());
	/*HMODULE currentHM = GetModuleHandle(NULL);
	PEDeatis ped(currentHM);*/
	//std::string secName(".text");
	//Section sec = ped[".text"];
	//DWORD textH = sec.virtualAddressAbs;
	//DWORD CurrentProtect,dontcare;
	////printf(".text = %d", textH);
	//examp();
	////printf("examp() = %d", &examp);
	//DWORD funAddr = (DWORD)&examp;
	//DWORD pageaddr = ped.getPageAddressByAbsAddr(".text",funAddr);

	//VirtualProtect((void*)pageaddr,ped.getPageSize(),PAGE_EXECUTE_READWRITE,&CurrentProtect);
	//for(unsigned int i = 0;(i + funAddr)<(pageaddr + ped.getPageSize());i++)
	//{
	//	BYTE*target = ((BYTE*)funAddr + i);
	//	if (*target == 0x75)
	//	{
	//		*target = 0x74;
	//		//break;
	//	}
	//	else if(*target == 0x74)
	//	{
	//		*target = 0x75;
	//		//break;
	//	}
	//}
	////*(unsigned char*)(textH + 0x10887) = 0x75;
	////memset((char*)texth + 0x10887, 0x75, 1);
	////virtualprotect((char*)(texth + (ped.getoptheader()->sectionalignment) * 16), ped.getoptheader()->sectionalignment, currentprotect, &dontcare);
	//VirtualProtect((void*)pageaddr, ped.getPageSize(),  CurrentProtect,&dontcare);
	//examp();
}