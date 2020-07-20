#include <stdio.h>
#include <Windows.h>
#include <Imagehlp.H>
#include<iostream>
#pragma comment(lib,"Imagehlp.lib")

HANDLE OpenPeByFileName(LPTSTR FileName)
{
    LPTSTR PortableExecutableFileName = FileName;//获取PE文件名字
    HANDLE hFile, hMapFile, hMapAddress = NULL;
    DWORD dwFileSize = 0;

    hFile = CreateFile(PortableExecutableFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);//获取程序句柄
    dwFileSize = GetFileSize(hFile, NULL);
    std::cout<<"文件大小：                                       " << dwFileSize<<std::endl;
    hMapFile = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, dwFileSize, NULL);//创建文件映象
    hMapAddress = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, dwFileSize);//在调用进程的地址空间映射一个文件视图，返回一个映射视图的起始地址
                                                                           //把B进程的文件映象存放到A中，这样好通信
    if (hMapAddress != NULL)
    {
        CloseHandle(hFile);
        CloseHandle(hMapFile);
        return hMapAddress;
    }
}
BOOL IsPEFile(HANDLE ImageBase)
{
    PIMAGE_DOS_HEADER pDosHead = NULL;
    PIMAGE_NT_HEADERS pNtHead = NULL;
    if (ImageBase == NULL) { return FALSE; }
    pDosHead = (PIMAGE_DOS_HEADER)ImageBase;
    if (IMAGE_DOS_SIGNATURE != pDosHead->e_magic) { return FALSE; }
    pNtHead = (PIMAGE_NT_HEADERS)((DWORD)pDosHead + pDosHead->e_lfanew);
    if (IMAGE_NT_SIGNATURE != pNtHead->Signature) { return FALSE; }
    return TRUE;
}
PIMAGE_NT_HEADERS GetNtHead(HANDLE ImageBase)
{
    PIMAGE_DOS_HEADER pDosHead = NULL;
    PIMAGE_NT_HEADERS pNtHead = NULL;
    pDosHead = (PIMAGE_DOS_HEADER)ImageBase;
    pNtHead = (PIMAGE_NT_HEADERS)((DWORD)pDosHead + pDosHead->e_lfanew);
    return pNtHead;
}

void DisplayDOSHeadInfo(HANDLE ImageBase)
{
    PIMAGE_DOS_HEADER pDosHead = NULL;
    pDosHead = (PIMAGE_DOS_HEADER)ImageBase;
    std::cout << "-------------------------DOSHead-------------------------" << std::endl;
    printf("DOS签名(MZSignature)：                           %x\n", pDosHead->e_magic);
    printf("文件最后页的字节数(UsedBytesInTheLastPage)：     %x\n", pDosHead->e_cblp);
    printf("文件页数(FileSizeInPages)：                      %x\n", pDosHead->e_cp);
    printf("重定义元素个数(NumberOfRelocationItems)：        %x\n", pDosHead->e_crlc);
    printf("头部尺寸(HeaderSizeInParagraphs)：               %x\n", pDosHead->e_cparhdr);
    printf("所需的最小附加段(MinimumExtraParagraphs)：       %x\n", pDosHead->e_minalloc);
    printf("所需的最大附加段(MaximumExtraParagraphs)：       %x\n", pDosHead->e_maxalloc);
    printf("初始的SS值(InitialRelativeSS)：                  %x\n", pDosHead->e_ss);
    printf("初始的SP值(InitialSP)：                          %x\n", pDosHead->e_sp);
    printf("校验和(Checksum)：                               %x\n", pDosHead->e_csum);
    printf("初始的IP值(InitialIP)：                          %x\n", pDosHead->e_ip);
    printf("初始的CS值(InitialRelativeCS)：                  %x\n", pDosHead->e_cs);
    printf("重分配表文件地址(AddressOfRelocationTable)：     %x\n", pDosHead->e_lfarlc);
    printf("覆盖号(OverlayNumber)：                          %x\n", pDosHead->e_ovno);
    printf("保留字[4](Reserved)：\n");
    printf("Reserved[0]：                                    %x\n", pDosHead->e_res[0]);
    printf("Reserved[1]：                                    %x\n", pDosHead->e_res[1]);
    printf("Reserved[2]：                                    %x\n", pDosHead->e_res[2]);
    printf("Reserved[3]：                                    %x\n", pDosHead->e_res[3]);
    printf("OEM标识符(OEMid)：                               %x\n", pDosHead->e_oemid);
    printf("OEM信息(OEMinfo)：                               %x\n", pDosHead->e_oeminfo);
    printf("保留字[10](Reserved2)：\n");
    printf("Reserved2[0]：                                   %x\n", pDosHead->e_res2[0]);
    printf("Reserved2[1]：                                   %x\n", pDosHead->e_res2[1]);
    printf("Reserved2[2]：                                   %x\n", pDosHead->e_res2[2]);
    printf("Reserved2[3]：                                   %x\n", pDosHead->e_res2[3]);
    printf("Reserved2[4]：                                   %x\n", pDosHead->e_res2[4]);
    printf("Reserved2[5]：                                   %x\n", pDosHead->e_res2[5]);
    printf("Reserved2[6]：                                   %x\n", pDosHead->e_res2[6]);
    printf("Reserved2[7]：                                   %x\n", pDosHead->e_res2[7]);
    printf("Reserved2[8]：                                   %x\n", pDosHead->e_res2[8]);
    printf("Reserved2[9]：                                   %x\n", pDosHead->e_res2[9]);
    printf("指示NT头的偏移(AddressOfNewExeHeader)：          %x\n", pDosHead->e_lfanew);
}


void DisplayFileHeaderInfo(HANDLE ImageBase)
{
    PIMAGE_NT_HEADERS pNtHead = NULL;
    pNtHead = GetNtHead(ImageBase);
    std::cout << "-------------------------FileHeader-------------------------" << std::endl;
    printf("签名(Signatur):                                  %x\n", pNtHead->Signature);
    printf("运行平台(Machine):                               %x\n", pNtHead->FileHeader.Machine);
    printf("节区数目(NumberOfSections):                      %x\n", pNtHead->FileHeader.NumberOfSections);
    printf("创建时间(TimeDateStamp):                         %x\n", pNtHead->FileHeader.TimeDateStamp);
    printf("COFF文件符号表偏移(PointerToSymbolTable):        %x\n", pNtHead->FileHeader.PointerToSymbolTable);
    printf("符号表的数量(NumberOfSymbols):                   %x\n", pNtHead->FileHeader.NumberOfSymbols);
    printf("可选头大小(SizeOfOptionalHeader):                %x\n", pNtHead->FileHeader.SizeOfOptionalHeader);
    printf("文件特性(Characteristics):                       %x\n", pNtHead->FileHeader.Characteristics);
}

void DisplayOptionalHeaderInfo(HANDLE ImageBase)
{
    PIMAGE_NT_HEADERS pNtHead = NULL;
    pNtHead = GetNtHead(ImageBase);
    std::cout << "-------------------------OptionalHeader-------------------------" << std::endl;
    printf("魔数(Magic)：                                    %x\n", pNtHead->OptionalHeader.Magic);
    printf("链接器的主版本号(MajorLinkerVersion)：           %x\n", pNtHead->OptionalHeader.MajorLinkerVersion);
    printf("链接器的次版本号(MinorLinkerVersion)：           %x\n", pNtHead->OptionalHeader.MinorLinkerVersion);
    printf("代码节大小(SizeOfCode)：                         %x\n", pNtHead->OptionalHeader.SizeOfCode);
    printf("已初始化数大小(SizeOfInitializedData)：          %x\n", pNtHead->OptionalHeader.SizeOfInitializedData);
    printf("未初始化数大小(SizeOfUninitializedData)：        %x\n", pNtHead->OptionalHeader.SizeOfUninitializedData);
    printf("最先执行代码起始地址(AddressOfEntryPoint)：      %x\n", pNtHead->OptionalHeader.AddressOfEntryPoint);
    printf("代码基址(BaseOfCode)：                           %x\n", pNtHead->OptionalHeader.BaseOfCode);
    printf("数据基址(BaseOfData)：                           %x\n", pNtHead->OptionalHeader.BaseOfData);
    printf("镜像首地址(ImageBase)：                          %x\n", pNtHead->OptionalHeader.ImageBase);
    printf("节段在内存中的最小单位(SectionAlignment)：       %x\n", pNtHead->OptionalHeader.SectionAlignment);
    printf("节段在磁盘文件中的最小单位(FileAlignment)：      %x\n", pNtHead->OptionalHeader.FileAlignment);
    printf("主系统的主版本号(MajorOperatingSystemVersion)：  %x\n", pNtHead->OptionalHeader.MajorOperatingSystemVersion);
    printf("主系统的次版本号(MinorOperatingSystemVersion)：  %x\n", pNtHead->OptionalHeader.MinorOperatingSystemVersion);
    printf("镜像的主版本号(MajorImageVersion)：              %x\n", pNtHead->OptionalHeader.MajorImageVersion);
    printf("镜像的次版本号(MinorImageVersion)：              %x\n", pNtHead->OptionalHeader.MinorImageVersion);
    printf("子系统的主版本号(MajorSubsystemVersion)：        %x\n", pNtHead->OptionalHeader.MajorSubsystemVersion);
    printf("子系统的次版本号(MinorSubsystemVersion)：        %x\n", pNtHead->OptionalHeader.MinorSubsystemVersion);
    printf("保留字段(Win32VersionValue)：                    %x\n", pNtHead->OptionalHeader.Win32VersionValue);
    printf("镜像被加载进内存时的大小(SizeOfImage)：          %x\n", pNtHead->OptionalHeader.SizeOfImage);
    printf("所有头的总大小(SizeOfHeaders)：                  %x\n", pNtHead->OptionalHeader.SizeOfHeaders);
    printf("镜像文件的校验和(CheckSum)：                     %x\n", pNtHead->OptionalHeader.CheckSum);
    printf("运行此镜像所需的子系统(Subsystem)：              %x\n", pNtHead->OptionalHeader.Subsystem);
    printf("DLL标识(DllCharacteristics)：                    %x\n", pNtHead->OptionalHeader.DllCharacteristics);
    printf("最大栈大小(SizeOfStackReserve)：                 %x\n", pNtHead->OptionalHeader.SizeOfStackReserve);
    printf("初始提交的堆栈大小(SizeOfStackCommit)：          %x\n", pNtHead->OptionalHeader.SizeOfStackCommit);
    printf("最大堆大小(SizeOfHeapReserve)：                  %x\n", pNtHead->OptionalHeader.SizeOfHeapReserve);
    printf("初始提交的局部堆空间大小(SizeOfHeapCommit)：     %x\n", pNtHead->OptionalHeader.SizeOfHeapCommit);
    printf("保留字段(LoaderFlags)：                          %x\n", pNtHead->OptionalHeader.LoaderFlags);
    printf("DataDirectory的数组个数(NumberOfRvaAndSizes)：   %x\n", pNtHead->OptionalHeader.NumberOfRvaAndSizes);
}
//IMAGE_DATA_DIRECTORY表展示写，感觉没啥用这玩意
//void DisplayDataDirectoryInfo(HANDLE ImageBase)
//{
//    PIMAGE_NT_HEADERS pNtHead = NULL;
//    pNtHead = GetNtHead(ImageBase);
//    printf("数据的相对虚拟地址(VirtualAddress):                               %x\n", pNtHead->OptionalHeader.DataDirectory->VirtualAddress);
//    printf("数据块的大小(Size):                      %x\n", pNtHead->OptionalHeader.DataDirectory->Size);
//
//}

void DisplaySectionHeaderInfo(HANDLE ImageBase)
{
    PIMAGE_NT_HEADERS pNtHead = NULL;
    PIMAGE_FILE_HEADER pFileHead = NULL;
    PIMAGE_SECTION_HEADER pSection = NULL;
    DWORD NumberOfSectinsCount = 0;
    pNtHead = GetNtHead(ImageBase);
    pSection = IMAGE_FIRST_SECTION(pNtHead);
    pFileHead = &pNtHead->FileHeader;

    NumberOfSectinsCount = pFileHead->NumberOfSections;        // 获得区段数量
    DWORD* difA = NULL;   // 虚拟地址开头
    DWORD* difS = NULL;   // 相对偏移(用于遍历)
    difA = (DWORD*)malloc(NumberOfSectinsCount * sizeof(DWORD));
    difS = (DWORD*)malloc(NumberOfSectinsCount * sizeof(DWORD));
    std::cout << "-------------------------SectionHeader-------------------------" << std::endl;
    printf("节区名称 相对偏移\t虚拟大小\tRaw数据指针\tRaw数据大小\t节区属性\n");
    for (int temp = 0; temp < NumberOfSectinsCount; temp++, pSection++)
    {
        printf("%s\t 0x%.8X \t 0x%.8X \t 0x%.8X \t 0x%.8X \t 0x%.8X\n",
            pSection->Name, pSection->VirtualAddress, pSection->Misc.VirtualSize,
            pSection->PointerToRawData, pSection->SizeOfRawData, pSection->Characteristics);
        difA[temp] = pSection->VirtualAddress;
        difS[temp] = pSection->VirtualAddress - pSection->PointerToRawData;
    }
}


//VOID DisplayExportTable(HANDLE ImageBase)
//{
//    IMAGE_IMPORT_DESCRIPTOR* pExport = 0;
//    PIMAGE_DOS_HEADER pDosHead = NULL;
//    PIMAGE_NT_HEADERS pNtHead = NULL;
//    IMAGE_SECTION_HEADER* pSectionHeader = 0;
//    pDosHead = (PIMAGE_DOS_HEADER)ImageBase;
//    pNtHead = (PIMAGE_NT_HEADERS)((DWORD)pDosHead + pDosHead->e_lfanew);
//    if (pNtHead->Signature != 0x00004550) { return; }        // 无效PE文件
//    //if (pNtHead->OptionalHeader.Magic != 0x20b){return;}  // 不是64位PE
//
////定位到第一个节的地址
//    pSectionHeader = (IMAGE_SECTION_HEADER*)((DWORD)pNtHead + sizeof(IMAGE_NT_HEADERS));
//    //ntHeader->FileHeader.NumberOfSections为节的数量，此处循环，找到引入表的节
//    for (int i = 0; i < pNtHead->FileHeader.NumberOfSections; i++)
//    {
//        //IMAGE_DIRECTORY_ENTRY_IMPORT，引入表
//        if (pSectionHeader->VirtualAddress == pNtHead->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress)
//        {
//            break;
//        }
//        //没找到，那么增加SizeOf(IMAGE_SECTION_HEADER)字节数，指向下一个节
//        pSectionHeader++;
//    }
//    //引入表入口
//    pExport = (IMAGE_IMPORT_DESCRIPTOR*)((DWORD)ImageBase + pSectionHeader->PointerToRawData);
//    //加载的DLL的名称，这里是RVA地址，需要转换成文件偏移地址，因为我们不是通过PE加载器加载，而是映射到内存
//    while (pExport->Name != 0)
//    {
//        DWORD offset = (DWORD)ImageBase + pExport->Name - (pSectionHeader->VirtualAddress - pSectionHeader->PointerToRawData);
//        char* s = (char*)offset;
//        std::cout << s << std::endl;
//        //继续读取
//        pExport++;
//    }

//
//}

void DisplayImportTable(HANDLE ImageBase)
{
    PIMAGE_DOS_HEADER pDosHead = NULL;
    PIMAGE_NT_HEADERS pNtHead = NULL;
    PIMAGE_IMPORT_DESCRIPTOR pInput = NULL;
    PIMAGE_THUNK_DATA _pThunk = NULL;
    DWORD dwThunk = NULL;
    DWORD dwOriginalFirstThunk = NULL;
    USHORT Hint;
    int iSystemBits = 0;

    pDosHead = (PIMAGE_DOS_HEADER)ImageBase;
    pNtHead = GetNtHead(ImageBase);
    //判断版本是32位还是64位
    if (pNtHead->FileHeader.Machine== IMAGE_FILE_MACHINE_I386)
    {
        iSystemBits = 32;
    }
    if (pNtHead->FileHeader.Machine == (IMAGE_FILE_MACHINE_IA64 || IMAGE_FILE_MACHINE_AMD64))
    {
        iSystemBits = 64;
    }
    if (pNtHead->OptionalHeader.DataDirectory[1].VirtualAddress == 0) { return; }  // 读取导入表RVA
    pInput = (PIMAGE_IMPORT_DESCRIPTOR)ImageRvaToVa((PIMAGE_NT_HEADERS)pNtHead, pDosHead, pNtHead->OptionalHeader.DataDirectory[1].VirtualAddress, NULL);
    std::cout << "-------------------------ImportTable-------------------------" << std::endl;
    for (; pInput->Name != NULL;)
    {
        char* szFunctionModule = (PSTR)ImageRvaToVa((PIMAGE_NT_HEADERS)pNtHead, pDosHead, (ULONG)pInput->Name, NULL);  // 遍历出模块名称
        dwThunk = pInput->FirstThunk;
        dwOriginalFirstThunk= pInput->OriginalFirstThunk;
        _pThunk = (PIMAGE_THUNK_DATA)ImageRvaToVa((PIMAGE_NT_HEADERS)pNtHead, pDosHead, (ULONG)pInput->FirstThunk, NULL);
        
        for (; _pThunk->u1.AddressOfData != NULL;)
        {
            
            char* szFunction = (PSTR)ImageRvaToVa((PIMAGE_NT_HEADERS)pNtHead, pDosHead, (ULONG)(((PIMAGE_IMPORT_BY_NAME)_pThunk->u1.AddressOfData)->Name), 0);
            if (szFunction != NULL)
            {
                memcpy(&Hint, szFunction - 2, 2);
            }
            else
            {
                Hint = -1;
            }
            if (Hint== NULL || dwThunk == NULL || dwOriginalFirstThunk == NULL || szFunctionModule == NULL || szFunction == NULL)
            {

            }
            else
            {
                std::cout << "序号：" << Hint << std::endl;
                std::cout << "Thunk RVA：" << std::hex << dwThunk << std::endl;
                std::cout << "OriginalFirstThunk：" << std::hex << dwOriginalFirstThunk << std::endl;
                std::cout << "DLL名称：" << szFunctionModule << std::endl;
                std::cout << "API名称：" << szFunction << std::endl;
                std::cout << "--------------------------------------------------" << std::endl;
            }

            //32位 +4 64位+8
            if (iSystemBits == 32)
            {
                dwThunk += 4;
            }
            else if (iSystemBits == 64)
            {
                dwThunk += 8;
            }
            _pThunk++;
        }
        pInput++;
    }
}
VOID DisplayExportTable(HANDLE ImageBase)
{

    PIMAGE_EXPORT_DIRECTORY pExport;
    PIMAGE_DOS_HEADER pDosHead = NULL;
    PIMAGE_NT_HEADERS pNtHead = NULL;
    pDosHead = (PIMAGE_DOS_HEADER)ImageBase;
    pNtHead = (PIMAGE_NT_HEADERS)((DWORD)pDosHead + pDosHead->e_lfanew);
    if (pNtHead->Signature != 0x00004550) { return; }        // 无效PE文件

    pExport = (PIMAGE_EXPORT_DIRECTORY)ImageRvaToVa((PIMAGE_NT_HEADERS)pNtHead, pDosHead, pNtHead->OptionalHeader.DataDirectory[0].VirtualAddress, NULL);
    DWORD NumberOfNames = pExport->NumberOfNames;
    ULONGLONG** ppdwNames = (ULONGLONG**)pExport->AddressOfNames;
    ppdwNames = (PULONGLONG*)ImageRvaToVa((PIMAGE_NT_HEADERS)pNtHead, pDosHead, (ULONG)ppdwNames, NULL);
    ULONGLONG** ppdwAddr = (ULONGLONG**)pExport->AddressOfFunctions;
    ppdwAddr = (PULONGLONG*)ImageRvaToVa((PIMAGE_NT_HEADERS)pNtHead, pDosHead, (DWORD)ppdwAddr, NULL);
    ULONGLONG* ppdwOrdin = (ULONGLONG*)ImageRvaToVa((PIMAGE_NT_HEADERS)pNtHead, pDosHead, (DWORD)pExport->AddressOfNameOrdinals, NULL);
    std::cout << "-------------------------ExportTable-------------------------" << std::endl;
    char* szFunction = (PSTR)ImageRvaToVa((PIMAGE_NT_HEADERS)pNtHead, pDosHead, (ULONG)*ppdwNames, NULL);
    std::cout << "特征值：" << pExport->Characteristics << std::endl;
    std::cout << "时间日期标志：" << std::hex << pExport->TimeDateStamp << std::endl;
    //std::cout  <<"大版本号"<< pExport->MajorVersion << std::endl;
    //std::cout << "小版本号" << pExport->MinorVersion << std::endl;
    std::cout << "版本号：" << std::hex << pExport->MajorVersion << "." << std::hex << pExport->MinorVersion << std::endl;
    std::cout << "名称：" << pExport->Name << std::endl;
    std::cout << "基址：" << pExport->Base << std::endl;
    std::cout << "函数个数：" << pExport->NumberOfFunctions << std::endl;
    std::cout << "名称个数：" << pExport->NumberOfNames << std::endl;
    std::cout << "函数地址：" << pExport->AddressOfFunctions << std::endl;
    std::cout << "名称地址：" << pExport->AddressOfNames << std::endl;
    std::cout << "名称序数地址：" << pExport->AddressOfNameOrdinals << std::endl;
    for (DWORD i = 0; i < NumberOfNames; i++)
    {
        std::cout << "序号：" << i << std::endl;
        std::cout << "RVA" << *ppdwAddr << std::endl;
        std::cout << "名称：" << szFunction << std::endl;
        std::cout << "-----------------------------------" << std::endl;
        szFunction = szFunction + strlen(szFunction) + 1;
        ppdwAddr++;
    }
}
int main()
{
    HANDLE lpMapAddress = NULL;
    wchar_t * wcFileName = L"C:\\Users\\ascotbe\\Desktop\\UplayInstaller.exe";//你文件的位置
    lpMapAddress = OpenPeByFileName(wcFileName);
    DisplayDOSHeadInfo(lpMapAddress);
    IsPEFile(lpMapAddress);
    //DisplayDataDirectoryInfo(lpMapAddress);
    DisplayFileHeaderInfo(lpMapAddress);
    DisplayOptionalHeaderInfo(lpMapAddress);
    DisplaySectionHeaderInfo(lpMapAddress);
    DisplayImportTable(lpMapAddress);
    DisplayExportTable(lpMapAddress);//DLL的时候再调用这个函数
    return 0;
}
