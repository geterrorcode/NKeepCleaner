#include "utils.h"


#include <QApplication>
#include <tlhelp32.h>

namespace updaterUtils {

	INT64 getPID(const std::wstring processName)
	{
        INT64 pid = -1;
        // 프로세스 이름으로 프로세스 ID 찾기
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnapshot != INVALID_HANDLE_VALUE) {
            PROCESSENTRY32 pe32 = { 0 };
            pe32.dwSize = sizeof(PROCESSENTRY32);

            if (Process32First(hSnapshot, &pe32)) {
                do {
                    std::wstring exeFile = pe32.szExeFile;

                    if (exeFile == processName) {
                        pid = pe32.th32ProcessID;
                        break;
                    }
                } while (Process32Next(hSnapshot, &pe32));
            }

            CloseHandle(hSnapshot);
        }

        return pid;
	}
}
