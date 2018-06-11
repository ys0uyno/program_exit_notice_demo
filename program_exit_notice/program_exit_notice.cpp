// program_exit_notice.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <TlHelp32.h>

int get_process_id(const TCHAR *process_name)
{
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (TRUE == Process32First(snapshot, &entry))
	{
		while (TRUE == Process32Next(snapshot, &entry))
		{
			if (0 == _tcsicmp(entry.szExeFile, process_name))
			{
				_tprintf(L"%s pid: %d\n", entry.szExeFile, entry.th32ProcessID);
				CloseHandle(snapshot);
				return entry.th32ProcessID;
			}
		}
	}

	CloseHandle(snapshot);
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	DWORD target_pid = get_process_id(L"mfc_dialog.exe");
	if (0 == target_pid)
	{
		printf("target pid is ZERO\n");
		return -1;
	}

	// get process handle from process id
	HANDLE target_handle = OpenProcess(SYNCHRONIZE, TRUE, target_pid);
	if (NULL == target_handle)
	{
		printf("OpenProcess pid: %d failed, error: %d\n", target_pid, GetLastError());
		return -1;
	}

	WaitForSingleObject(target_handle, INFINITE);
	CloseHandle(target_handle);
	printf("WaitForSingleObject get signal\n");

	return 0;
}

