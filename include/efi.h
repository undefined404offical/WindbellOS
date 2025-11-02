#ifndef EFI_H
#define EFI_H

#include <stdint.h>
#include <stddef.h>

// EFI基本类型定义
typedef uint8_t UINT8;
typedef uint16_t UINT16;
typedef uint32_t UINT32;
typedef uint64_t UINT64;
typedef int8_t INT8;
typedef int16_t INT16;
typedef int32_t INT32;
typedef int64_t INT64;
typedef uint8_t BOOLEAN;
typedef size_t UINTN;

// CHAR16类型定义 - 必须在使用前定义
typedef uint16_t CHAR16;

typedef UINT8 EFI_BOOLEAN;
typedef UINT64 EFI_LBA;
typedef UINT64 EFI_PHYSICAL_ADDRESS;
typedef UINT64 EFI_VIRTUAL_ADDRESS;

// EFI状态码
typedef UINTN EFI_STATUS;
#define EFI_SUCCESS               0
#define EFI_LOAD_ERROR            1
#define EFI_INVALID_PARAMETER     2
#define EFI_UNSUPPORTED           3
#define EFI_BAD_BUFFER_SIZE       4
#define EFI_BUFFER_TOO_SMALL      5
#define EFI_NOT_READY             6
#define EFI_DEVICE_ERROR          7
#define EFI_WRITE_PROTECTED       8
#define EFI_OUT_OF_RESOURCES      9
#define EFI_VOLUME_CORRUPTED      10
#define EFI_VOLUME_FULL           11
#define EFI_NO_MEDIA              12
#define EFI_MEDIA_CHANGED         13
#define EFI_NOT_FOUND             14
#define EFI_ACCESS_DENIED         15
#define EFI_NO_RESPONSE           16
#define EFI_NO_MAPPING            17
#define EFI_TIMEOUT               18
#define EFI_NOT_STARTED           19
#define EFI_ALREADY_STARTED       20
#define EFI_ABORTED               21
#define EFI_ICMP_ERROR            22
#define EFI_TFTP_ERROR            23
#define EFI_PROTOCOL_ERROR        24
#define EFI_INCOMPATIBLE_VERSION  25
#define EFI_SECURITY_VIOLATION    26
#define EFI_CRC_ERROR             27
#define EFI_END_OF_MEDIA          28
#define EFI_END_OF_FILE           29
#define EFI_INVALID_LANGUAGE      30
#define EFI_COMPROMISED_DATA      31
#define EFI_IP_ADDRESS_CONFLICT   32
#define EFI_HTTP_ERROR            33

// 检查EFI状态是否为错误
#define EFI_ERROR(a)              ((a) & (1ULL << 63))

// EFI句柄
typedef void* EFI_HANDLE;

// EFI事件
typedef void* EFI_EVENT;

// EFIAPI调用约定
#define EFIAPI __attribute__((ms_abi))

// EFI表头
typedef struct {
    UINT64 Signature;
    UINT32 Revision;
    UINT32 HeaderSize;
    UINT32 CRC32;
    UINT32 Reserved;
} EFI_TABLE_HEADER;

// EFI内存类型
typedef UINTN EFI_MEMORY_TYPE;
#define EfiReservedMemoryType      0
#define EfiLoaderCode              1
#define EfiLoaderData              2
#define EfiBootServicesCode        3
#define EfiBootServicesData        4
#define EfiRuntimeServicesCode     5
#define EfiRuntimeServicesData     6
#define EfiConventionalMemory      7
#define EfiUnusableMemory          8
#define EfiACPIReclaimMemory       9
#define EfiACPIMemoryNVS           10
#define EfiMemoryMappedIO          11
#define EfiMemoryMappedIOPortSpace 12
#define EfiPalCode                 13
#define EfiMaxMemoryType           14

// EFI任务优先级级别
typedef UINTN EFI_TPL;
#define TPL_APPLICATION 4
#define TPL_CALLBACK    8
#define TPL_NOTIFY      16
#define TPL_HIGH_LEVEL  31

// EFI内存分配类型
typedef enum {
    AllocateAnyPages,
    AllocateMaxAddress,
    AllocateAddress,
    MaxAllocateType
} EFI_ALLOCATE_TYPE;

// EFI接口类型
typedef enum {
    EFI_NATIVE_INTERFACE,
    EFI_PCODE_INTERFACE
} EFI_INTERFACE_TYPE;

// EFI定位搜索类型
typedef enum {
    AllHandles,
    ByRegisterNotify,
    ByProtocol
} EFI_LOCATE_SEARCH_TYPE;

// EFI设备路径协议
typedef struct _EFI_DEVICE_PATH_PROTOCOL {
    UINT8 Type;
    UINT8 SubType;
    UINT8 Length[2];
} EFI_DEVICE_PATH_PROTOCOL;

// EFI定时器延迟类型
typedef enum {
    TimerCancel,
    TimerPeriodic,
    TimerRelative
} EFI_TIMER_DELAY;

// EFI内存描述符
typedef struct {
    UINT32 Type;
    UINT32 Pad;
    EFI_PHYSICAL_ADDRESS PhysicalStart;
    EFI_VIRTUAL_ADDRESS VirtualStart;
    UINT64 NumberOfPages;
    UINT64 Attribute;
} EFI_MEMORY_DESCRIPTOR;

// EFI文件模式
#define EFI_FILE_MODE_READ    0x0000000000000001
#define EFI_FILE_MODE_WRITE   0x0000000000000002
#define EFI_FILE_MODE_CREATE  0x8000000000000000

// EFI_GUID结构
typedef struct {
    UINT32 Data1;
    UINT16 Data2;
    UINT16 Data3;
    UINT8 Data4[8];
} EFI_GUID;

// EFI时间结构
typedef struct {
    UINT16 Year;
    UINT8 Month;
    UINT8 Day;
    UINT8 Hour;
    UINT8 Minute;
    UINT8 Second;
    UINT8 Pad1;
    UINT32 Nanosecond;
    INT16 TimeZone;
    UINT8 Daylight;
    UINT8 Pad2;
} EFI_TIME;

// EFI文件信息结构
typedef struct {
    UINT64 Size;
    UINT64 FileSize;
    UINT64 PhysicalSize;
    EFI_TIME CreateTime;
    EFI_TIME LastAccessTime;
    EFI_TIME ModificationTime;
    UINT64 Attribute;
    CHAR16 FileName[1];
} EFI_FILE_INFO;

// EFI文件协议
typedef struct _EFI_FILE_PROTOCOL {
    UINT64 Revision;
    EFI_STATUS (EFIAPI *Open)(
        struct _EFI_FILE_PROTOCOL *This,
        struct _EFI_FILE_PROTOCOL **NewHandle,
        CHAR16 *FileName,
        UINT64 OpenMode,
        UINT64 Attributes
    );
    EFI_STATUS (EFIAPI *Close)(
        struct _EFI_FILE_PROTOCOL *This
    );
    EFI_STATUS (EFIAPI *Delete)(
        struct _EFI_FILE_PROTOCOL *This
    );
    EFI_STATUS (EFIAPI *Read)(
        struct _EFI_FILE_PROTOCOL *This,
        UINTN *BufferSize,
        void *Buffer
    );
    EFI_STATUS (EFIAPI *Write)(
        struct _EFI_FILE_PROTOCOL *This,
        UINTN *BufferSize,
        void *Buffer
    );
    EFI_STATUS (EFIAPI *GetPosition)(
        struct _EFI_FILE_PROTOCOL *This,
        UINT64 *Position
    );
    EFI_STATUS (EFIAPI *SetPosition)(
        struct _EFI_FILE_PROTOCOL *This,
        UINT64 Position
    );
    EFI_STATUS (EFIAPI *GetInfo)(
        struct _EFI_FILE_PROTOCOL *This,
        EFI_GUID *InformationType,
        UINTN *BufferSize,
        void *Buffer
    );
    EFI_STATUS (EFIAPI *SetInfo)(
        struct _EFI_FILE_PROTOCOL *This,
        EFI_GUID *InformationType,
        UINTN BufferSize,
        void *Buffer
    );
    EFI_STATUS (EFIAPI *Flush)(
        struct _EFI_FILE_PROTOCOL *This
    );
} EFI_FILE_PROTOCOL;

// EFI简单文件系统协议
typedef struct _EFI_SIMPLE_FILE_SYSTEM_PROTOCOL {
    UINT64 Revision;
    EFI_STATUS (EFIAPI *OpenVolume)(
        struct _EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *This,
        struct _EFI_FILE_PROTOCOL **Root
    );
} EFI_SIMPLE_FILE_SYSTEM_PROTOCOL;

// EFI事件通知函数
typedef void (EFIAPI *EFI_EVENT_NOTIFY)(
    EFI_EVENT Event,
    void *Context
);

// 前向声明
struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;
struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL;
struct _EFI_RUNTIME_SERVICES;
struct _EFI_BOOT_SERVICES;
struct _EFI_CONFIGURATION_TABLE;
struct _EFI_SIMPLE_TEXT_OUTPUT_MODE;

// EFI系统表
typedef struct _EFI_SYSTEM_TABLE {
    EFI_TABLE_HEADER Hdr;
    CHAR16 *FirmwareVendor;
    UINT32 FirmwareRevision;
    EFI_HANDLE ConsoleInHandle;
    struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL *ConIn;
    EFI_HANDLE ConsoleOutHandle;
    struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
    EFI_HANDLE StandardErrorHandle;
    struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *StdErr;
    struct _EFI_RUNTIME_SERVICES *RuntimeServices;
    struct _EFI_BOOT_SERVICES *BootServices;
    UINTN NumberOfTableEntries;
    struct _EFI_CONFIGURATION_TABLE *ConfigurationTable;
} EFI_SYSTEM_TABLE;

// EFI图像句柄
typedef EFI_HANDLE EFI_IMAGE_HANDLE;

// EFI简单文本输出协议
typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
    EFI_STATUS (EFIAPI *Reset)(
        struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
        BOOLEAN ExtendedVerification
    );
    
    EFI_STATUS (EFIAPI *OutputString)(
        struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
        CHAR16 *String
    );
    
    EFI_STATUS (EFIAPI *TestString)(
        struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
        CHAR16 *String
    );
    
    EFI_STATUS (EFIAPI *QueryMode)(
        struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
        UINTN ModeNumber,
        UINTN *Columns,
        UINTN *Rows
    );
    
    EFI_STATUS (EFIAPI *SetMode)(
        struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
        UINTN ModeNumber
    );
    
    EFI_STATUS (EFIAPI *SetAttribute)(
        struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
        UINTN Attribute
    );
    
    EFI_STATUS (EFIAPI *ClearScreen)(
        struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This
    );
    
    EFI_STATUS (EFIAPI *SetCursorPosition)(
        struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
        UINTN Column,
        UINTN Row
    );
    
    EFI_STATUS (EFIAPI *EnableCursor)(
        struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
        BOOLEAN Visible
    );
    
    struct _EFI_SIMPLE_TEXT_OUTPUT_MODE *Mode;
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

// 文本颜色常量
#define EFI_BLACK      0x00
#define EFI_BLUE       0x01
#define EFI_GREEN      0x02
#define EFI_CYAN       0x03
#define EFI_RED        0x04
#define EFI_MAGENTA    0x05
#define EFI_BROWN      0x06
#define EFI_LIGHTGRAY  0x07
#define EFI_BRIGHT     0x08
#define EFI_DARKGRAY   0x08
#define EFI_LIGHTBLUE  0x09
#define EFI_LIGHTGREEN 0x0A
#define EFI_LIGHTCYAN  0x0B
#define EFI_LIGHTRED   0x0C
#define EFI_LIGHTMAGENTA 0x0D
#define EFI_YELLOW     0x0E
#define EFI_WHITE      0x0F

#define EFI_BACKGROUND_BLACK      0x00
#define EFI_BACKGROUND_BLUE       0x10
#define EFI_BACKGROUND_GREEN      0x20
#define EFI_BACKGROUND_CYAN       0x30
#define EFI_BACKGROUND_RED        0x40
#define EFI_BACKGROUND_MAGENTA    0x50
#define EFI_BACKGROUND_BROWN      0x60
#define EFI_BACKGROUND_LIGHTGRAY  0x70

// EFI加载的镜像协议
typedef struct {
    UINT32 Revision;
    EFI_HANDLE ParentHandle;
    EFI_SYSTEM_TABLE *SystemTable;
    EFI_HANDLE DeviceHandle;
    EFI_FILE_PROTOCOL *FilePath;
    void *Reserved;
    UINT32 LoadOptionsSize;
    void *LoadOptions;
    void *ImageBase;
    UINT64 ImageSize;
    EFI_MEMORY_TYPE ImageCodeType;
    EFI_MEMORY_TYPE ImageDataType;
    EFI_STATUS (EFIAPI *Unload)(
        EFI_IMAGE_HANDLE ImageHandle
    );
} EFI_LOADED_IMAGE_PROTOCOL;

// EFI启动服务
typedef struct _EFI_BOOT_SERVICES {
    EFI_TABLE_HEADER Hdr;
    
    // 任务优先级服务
    EFI_STATUS (EFIAPI *RaiseTPL)(
        EFI_TPL NewTPL
    );
    EFI_STATUS (EFIAPI *RestoreTPL)(
        EFI_TPL OldTPL
    );
    
    // 内存分配服务
    EFI_STATUS (EFIAPI *AllocatePages)(
        EFI_ALLOCATE_TYPE Type,
        EFI_MEMORY_TYPE MemoryType,
        UINTN NumberOfPages,
        EFI_PHYSICAL_ADDRESS *Memory
    );
    EFI_STATUS (EFIAPI *FreePages)(
        EFI_PHYSICAL_ADDRESS Memory,
        UINTN NumberOfPages
    );
    EFI_STATUS (EFIAPI *GetMemoryMap)(
        UINTN *MemoryMapSize,
        EFI_MEMORY_DESCRIPTOR *MemoryMap,
        UINTN *MapKey,
        UINTN *DescriptorSize,
        UINT32 *DescriptorVersion
    );
    EFI_STATUS (EFIAPI *AllocatePool)(
        EFI_MEMORY_TYPE PoolType,
        UINTN Size,
        void **Buffer
    );
    EFI_STATUS (EFIAPI *FreePool)(
        void *Buffer
    );
    
    // 事件和时间服务
    EFI_STATUS (EFIAPI *CreateEvent)(
        UINT32 Type,
        EFI_TPL NotifyTpl,
        EFI_EVENT_NOTIFY NotifyFunction,
        void *NotifyContext,
        EFI_EVENT *Event
    );
    EFI_STATUS (EFIAPI *SetTimer)(
        EFI_EVENT Event,
        EFI_TIMER_DELAY Type,
        UINT64 TriggerTime
    );
    EFI_STATUS (EFIAPI *WaitForEvent)(
        UINTN NumberOfEvents,
        EFI_EVENT *Event,
        UINTN *Index
    );
    EFI_STATUS (EFIAPI *SignalEvent)(
        EFI_EVENT Event
    );
    EFI_STATUS (EFIAPI *CloseEvent)(
        EFI_EVENT Event
    );
    EFI_STATUS (EFIAPI *CheckEvent)(
        EFI_EVENT Event
    );
    
    // 协议处理服务
    EFI_STATUS (EFIAPI *InstallProtocolInterface)(
        EFI_HANDLE *Handle,
        EFI_GUID *Protocol,
        EFI_INTERFACE_TYPE InterfaceType,
        void *Interface
    );
    EFI_STATUS (EFIAPI *ReinstallProtocolInterface)(
        EFI_HANDLE Handle,
        EFI_GUID *Protocol,
        void *OldInterface,
        void *NewInterface
    );
    EFI_STATUS (EFIAPI *UninstallProtocolInterface)(
        EFI_HANDLE Handle,
        EFI_GUID *Protocol,
        void *Interface
    );
    EFI_STATUS (EFIAPI *HandleProtocol)(
        EFI_HANDLE Handle,
        EFI_GUID *Protocol,
        void **Interface
    );
    void *Reserved;
    EFI_STATUS (EFIAPI *RegisterProtocolNotify)(
        EFI_GUID *Protocol,
        EFI_EVENT Event,
        void **Registration
    );
    EFI_STATUS (EFIAPI *LocateHandle)(
        EFI_LOCATE_SEARCH_TYPE SearchType,
        EFI_GUID *Protocol,
        void *SearchKey,
        UINTN *BufferSize,
        EFI_HANDLE *Buffer
    );
    EFI_STATUS (EFIAPI *LocateDevicePath)(
        EFI_GUID *Protocol,
        EFI_DEVICE_PATH_PROTOCOL **DevicePath,
        EFI_HANDLE *Device
    );
    EFI_STATUS (EFIAPI *InstallConfigurationTable)(
        EFI_GUID *Guid,
        void *Table
    );
    
    // 图像服务
    EFI_STATUS (EFIAPI *LoadImage)(
        BOOLEAN BootPolicy,
        EFI_HANDLE ParentImageHandle,
        EFI_DEVICE_PATH_PROTOCOL *DevicePath,
        void *SourceBuffer,
        UINTN SourceSize,
        EFI_HANDLE *ImageHandle
    );
    EFI_STATUS (EFIAPI *StartImage)(
        EFI_HANDLE ImageHandle,
        UINTN *ExitDataSize,
        CHAR16 **ExitData
    );
    EFI_STATUS (EFIAPI *Exit)(
        EFI_HANDLE ImageHandle,
        EFI_STATUS ExitStatus,
        UINTN ExitDataSize,
        CHAR16 *ExitData
    );
    EFI_STATUS (EFIAPI *UnloadImage)(
        EFI_HANDLE ImageHandle
    );
    EFI_STATUS (EFIAPI *ExitBootServices)(
        EFI_HANDLE ImageHandle,
        UINTN MapKey
    );
    
    // 其他服务...
} EFI_BOOT_SERVICES;

#endif // EFI_H