#include "../include/efi.h"

extern "C" {

// EFI系统表全局变量
EFI_SYSTEM_TABLE* gST;

// 内核入口点类型定义
typedef void (*KernelEntry)();

// 定义协议GUID
EFI_GUID LoadedImageProtocol = { 0x5B1B31A1, 0x9562, 0x11D2, {0x8E, 0x3F, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B} };
EFI_GUID FileSystemProtocol = { 0x964E5B22, 0x6459, 0x11D2, {0x8E, 0x39, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B} };
EFI_GUID FileInfoGuid = { 0x09576E92, 0x6D3F, 0x11D2, {0x8E, 0x39, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B} };

// 将ASCII字符串转换为Unicode字符串
void AsciiToUnicode(const char* ascii, CHAR16* unicode) {
    while (*ascii) {
        *unicode++ = (CHAR16)*ascii++;
    }
    *unicode = 0;
}

// 加载内核文件
EFI_STATUS LoadKernel(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable, void** KernelImage, UINTN* KernelSize) {
    EFI_STATUS Status;
    EFI_LOADED_IMAGE_PROTOCOL* LoadedImage;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FileSystem;
    EFI_FILE_PROTOCOL* Root;
    EFI_FILE_PROTOCOL* KernelFile;
    
    // 获取加载的镜像协议
    Status = SystemTable->BootServices->HandleProtocol(
        ImageHandle, 
        &LoadedImageProtocol, 
        (void**)&LoadedImage);
    if (EFI_ERROR(Status)) {
        return Status;
    }
    
    // 获取文件系统协议
    Status = SystemTable->BootServices->HandleProtocol(
        LoadedImage->DeviceHandle, 
        &FileSystemProtocol, 
        (void**)&FileSystem);
    if (EFI_ERROR(Status)) {
        return Status;
    }
    
    // 打开根目录
    Status = FileSystem->OpenVolume(FileSystem, &Root);
    if (EFI_ERROR(Status)) {
        return Status;
    }
    
    // 打开内核文件
    CHAR16 KernelPath[] = { '\\', 'K', 'E', 'R', 'N', 'E', 'L', '.', 'B', 'I', 'N', '\0' };
    Status = Root->Open(Root, &KernelFile, KernelPath, EFI_FILE_MODE_READ, 0);
    if (EFI_ERROR(Status)) {
        CHAR16 open_error_msg[] = { 'E', 'r', 'r', 'o', 'r', ':', ' ', 'C', 'a', 'n', 'n', 'o', 't', ' ', 'o', 'p', 'e', 'n', ' ', 'k', 'e', 'r', 'n', 'e', 'l', ' ', 'f', 'i', 'l', 'e', '.', '\r', '\n', 0 };
        gST->ConOut->OutputString(gST->ConOut, open_error_msg);
        Root->Close(Root);
        return Status;
    }
    
    // 获取文件大小
    EFI_FILE_INFO* FileInfo = NULL;
    UINTN InfoSize = sizeof(EFI_FILE_INFO) + 200; // 初始分配足够的空间
    
    // 分配初始缓冲区
    Status = SystemTable->BootServices->AllocatePool(
        EfiLoaderData, 
        InfoSize, 
        (void**)&FileInfo);
    if (EFI_ERROR(Status)) {
        KernelFile->Close(KernelFile);
        Root->Close(Root);
        return Status;
    }
    
    // 获取文件信息
    Status = KernelFile->GetInfo(KernelFile, &FileInfoGuid, &InfoSize, FileInfo);
    if (Status == EFI_BUFFER_TOO_SMALL) {
        // 缓冲区太小，重新分配
        SystemTable->BootServices->FreePool(FileInfo);
        Status = SystemTable->BootServices->AllocatePool(
            EfiLoaderData, 
            InfoSize, 
            (void**)&FileInfo);
        if (EFI_ERROR(Status)) {
            CHAR16 alloc_error_msg[] = { 'E', 'r', 'r', 'o', 'r', ':', ' ', 'C', 'a', 'n', 'n', 'o', 't', ' ', 'a', 'l', 'l', 'o', 'c', 'a', 't', 'e', ' ', 'b', 'u', 'f', 'f', 'e', 'r', '.', '\r', '\n', 0 };
            gST->ConOut->OutputString(gST->ConOut, alloc_error_msg);
            KernelFile->Close(KernelFile);
            Root->Close(Root);
            return Status;
        }
        
        // 再次获取文件信息
        Status = KernelFile->GetInfo(KernelFile, &FileInfoGuid, &InfoSize, FileInfo);
    }
    
    if (EFI_ERROR(Status)) {
        CHAR16 info_error_msg[] = { 'E', 'r', 'r', 'o', 'r', ':', ' ', 'C', 'a', 'n', 'n', 'o', 't', ' ', 'g', 'e', 't', ' ', 'f', 'i', 'l', 'e', ' ', 'i', 'n', 'f', 'o', '.', '\r', '\n', 0 };
        gST->ConOut->OutputString(gST->ConOut, info_error_msg);
        SystemTable->BootServices->FreePool(FileInfo);
        KernelFile->Close(KernelFile);
        Root->Close(Root);
        return Status;
    }
    
    // 分配内存
    *KernelSize = FileInfo->FileSize;
    Status = SystemTable->BootServices->AllocatePool(
        EfiLoaderData, 
        *KernelSize, 
        KernelImage);
    if (EFI_ERROR(Status)) {
        SystemTable->BootServices->FreePool(FileInfo);
        KernelFile->Close(KernelFile);
        Root->Close(Root);
        return Status;
    }
    
    // 读取文件
    Status = KernelFile->Read(KernelFile, KernelSize, *KernelImage);
    if (EFI_ERROR(Status)) {
        SystemTable->BootServices->FreePool(FileInfo);
        SystemTable->BootServices->FreePool(*KernelImage);
        KernelFile->Close(KernelFile);
        Root->Close(Root);
        return Status;
    }
    
    // 关闭文件
    SystemTable->BootServices->FreePool(FileInfo);
    KernelFile->Close(KernelFile);
    Root->Close(Root);
    
    return EFI_SUCCESS;
}

// 主函数
EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
    // 保存系统表指针
    gST = SystemTable;
    
    // 清除屏幕
    gST->ConOut->ClearScreen(gST->ConOut);
    
    // 设置文本颜色为白色
    gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
    
    // 显示启动成功消息
    CHAR16 message[] = { 'W', 'i', 'n', 'd', 'b', 'e', 'l', 'l', 'O', 'S', ' ', 
                         'S', 't', 'a', 'r', 't', 'e', 'd', ' ', 
                         'S', 'u', 'c', 'c', 'e', 's', 's', 'f', 'u', 'l', 'l', 'y', '!', '\r', '\n', 0 };
    gST->ConOut->OutputString(gST->ConOut, message);
    
    // 加载内核
    CHAR16 loading_msg[] = { 'L', 'o', 'a', 'd', 'i', 'n', 'g', ' ', 'k', 'e', 'r', 'n', 'e', 'l', '.', '.', '.', '\r', '\n', 0 };
    gST->ConOut->OutputString(gST->ConOut, loading_msg);
    
    void* KernelImage = NULL;
    UINTN KernelSize = 0;
    EFI_STATUS Status = LoadKernel(ImageHandle, SystemTable, &KernelImage, &KernelSize);
    if (EFI_ERROR(Status)) {
        CHAR16 error_msg[] = { 'E', 'r', 'r', 'o', 'r', ':', ' ', 'C', 'a', 'n', 'n', 'o', 't', ' ', 'l', 'o', 'a', 'd', ' ', 'k', 'e', 'r', 'n', 'e', 'l', '.', '\r', '\n', 0 };
        gST->ConOut->OutputString(gST->ConOut, error_msg);
        return Status;
    }
    
    CHAR16 success_msg[] = { 'K', 'e', 'r', 'n', 'e', 'l', ' ', 'l', 'o', 'a', 'd', 'e', 'd', ' ', 's', 'u', 'c', 'c', 'e', 's', 's', 'f', 'u', 'l', 'l', 'y', '.', '\r', '\n', 0 };
    gST->ConOut->OutputString(gST->ConOut, success_msg);
    
    // 退出UEFI启动服务
    UINTN MapKey, DescriptorSize;
    UINT32 DescriptorVersion;
    EFI_MEMORY_DESCRIPTOR* MemoryMap = NULL;
    UINTN MemoryMapSize = 0;
    
    // 分配一个足够大的初始缓冲区
    MemoryMapSize = 8192; // 初始分配8KB
    Status = SystemTable->BootServices->AllocatePool(
        EfiLoaderData, 
        MemoryMapSize, 
        (void**)&MemoryMap);
    if (EFI_ERROR(Status)) {
        CHAR16 alloc_error_msg[] = { 'E', 'r', 'r', 'o', 'r', ':', ' ', 'C', 'a', 'n', 'n', 'o', 't', ' ', 'a', 'l', 'l', 'o', 'c', 'a', 't', 'e', ' ', 'i', 'n', 'i', 't', 'i', 'a', 'l', ' ', 'm', 'e', 'm', 'o', 'r', 'y', ' ', 'm', 'a', 'p', '.', '\r', '\n', 0 };
        gST->ConOut->OutputString(gST->ConOut, alloc_error_msg);
        return Status;
    }
    
    // 第一次调用，获取所需的内存映射大小
    Status = SystemTable->BootServices->GetMemoryMap(&MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
    if (Status == EFI_BUFFER_TOO_SMALL) {
        // 释放旧的缓冲区
        SystemTable->BootServices->FreePool(MemoryMap);
        
        // 分配足够大小的内存映射缓冲区
        Status = SystemTable->BootServices->AllocatePool(
            EfiLoaderData, 
            MemoryMapSize, 
            (void**)&MemoryMap);
        if (EFI_ERROR(Status)) {
            CHAR16 alloc_error_msg[] = { 'E', 'r', 'r', 'o', 'r', ':', ' ', 'C', 'a', 'n', 'n', 'o', 't', ' ', 'a', 'l', 'l', 'o', 'c', 'a', 't', 'e', ' ', 'm', 'e', 'm', 'o', 'r', 'y', ' ', 'm', 'a', 'p', '.', '\r', '\n', 0 };
            gST->ConOut->OutputString(gST->ConOut, alloc_error_msg);
            return Status;
        }
        
        // 获取内存映射
        Status = SystemTable->BootServices->GetMemoryMap(&MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
        if (EFI_ERROR(Status)) {
            CHAR16 map_error_msg[] = { 'E', 'r', 'r', 'o', 'r', ':', ' ', 'C', 'a', 'n', 'n', 'o', 't', ' ', 'g', 'e', 't', ' ', 'm', 'e', 'm', 'o', 'r', 'y', ' ', 'm', 'a', 'p', '.', '\r', '\n', 0 };
            gST->ConOut->OutputString(gST->ConOut, map_error_msg);
            SystemTable->BootServices->FreePool(MemoryMap);
            return Status;
        }
    } else if (EFI_ERROR(Status)) {
        CHAR16 exit_error_msg[] = { 'E', 'r', 'r', 'o', 'r', ':', ' ', 'C', 'a', 'n', 'n', 'o', 't', ' ', 'g', 'e', 't', ' ', 'm', 'e', 'm', 'o', 'r', 'y', ' ', 'm', 'a', 'p', ' ', 's', 'i', 'z', 'e', '.', '\r', '\n', 0 };
        gST->ConOut->OutputString(gST->ConOut, exit_error_msg);
        SystemTable->BootServices->FreePool(MemoryMap);
        return Status;
    }
    // 如果Status == EFI_SUCCESS，说明初始缓冲区足够大，可以继续使用
    
    // 退出启动服务
    Status = SystemTable->BootServices->ExitBootServices(ImageHandle, MapKey);
    if (EFI_ERROR(Status)) {
        CHAR16 exit_error_msg[] = { 'E', 'r', 'r', 'o', 'r', ':', ' ', 'C', 'a', 'n', 'n', 'o', 't', ' ', 'e', 'x', 'i', 't', ' ', 'b', 'o', 'o', 't', ' ', 's', 'e', 'r', 'v', 'i', 'c', 'e', 's', '.', '\r', '\n', 0 };
        gST->ConOut->OutputString(gST->ConOut, exit_error_msg);
        SystemTable->BootServices->FreePool(MemoryMap);
        return Status;
    }
    
    // 将内核复制到正确的内存位置 (0x100000)
    void* kernel_dest = (void*)0x100000;
    for (UINTN i = 0; i < KernelSize; i++) {
        ((char*)kernel_dest)[i] = ((char*)KernelImage)[i];
    }
    
    // 跳转到内核入口点
    KernelEntry kernel_entry = (KernelEntry)0x100000; // 内核加载地址
    kernel_entry();
    
    // 如果内核返回，则无限循环
    while (1) {
        // 可以在这里添加系统空闲任务
    }
    
    return EFI_SUCCESS;
}

} // extern "C"