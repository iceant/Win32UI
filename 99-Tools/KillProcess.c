#include <KillProcess.h>
#include <stdio.h>
#include <windows.h>
#include <tlhelp32.h>
#include <string.h>

static DWORD FindProcessByName(const char* processName){
    HANDLE hProcessSnap;
//    HANDLE hProcess;
    PROCESSENTRY32 pe32;
//    DWORD dwPriorityClass;
    DWORD dwTargetProcessID = 0;
    
    // Take a snapshot of all processes in the system.
    hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
    if( hProcessSnap == INVALID_HANDLE_VALUE )
    {
        return( 0 );
    }
    
    // Set the size of the structure before using it.
    pe32.dwSize = sizeof( PROCESSENTRY32 );
    
    // Retrieve information about the first process,
    // and exit if unsuccessful
    if( !Process32First( hProcessSnap, &pe32 ) )
    {
        CloseHandle( hProcessSnap );  // clean the snapshot object
        return( 0 );
    }
    
    // Now walk the snapshot of processes
    do
    {
        if(strstr(pe32.szExeFile, processName)) // put the name of your process you want to kill
        {
            dwTargetProcessID = pe32.th32ProcessID;
            break;
        }
    } while( Process32Next( hProcessSnap, &pe32 ) );
    
    CloseHandle( hProcessSnap );
    
    return dwTargetProcessID;
}

BOOL TerminateMyProcess(DWORD dwProcessId, UINT uExitCode)
{
    DWORD dwDesiredAccess = PROCESS_TERMINATE;
    BOOL  bInheritHandle  = FALSE;
    HANDLE hProcess = OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId);
    if (hProcess == NULL)
        return FALSE;
    
    BOOL result = TerminateProcess(hProcess, uExitCode);
    
    CloseHandle(hProcess);
    
    return result;
}

int main(int argc, char** argv){
    
    if(argc<2){
        fprintf(stderr, "%s process_name\n", argv[0]);
        return 1;
    }
    
    DWORD processId = FindProcessByName(argv[1]);
    if(!processId){
        fprintf(stderr, "%s don't exist!!!\n", argv[1]);
        return 1;
    }
    
    BOOL Result = TerminateMyProcess(processId, 0);
    if(Result){
        printf("%s Terminated!\n", argv[1]);
    }
    
    return 0;
}