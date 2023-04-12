#pragma once

#include <cstdint>
#include <WTypesbase.h>

namespace ntapi
{
    struct UNICODE_STRING {
        uint16_t Length;
        uint16_t MaximumLength;
        wchar_t* Buffer;
    };

    typedef struct _LDR_MODULE {
        LIST_ENTRY InLoadOrderModuleList;
        LIST_ENTRY InMemoryOrderModuleList;
        LIST_ENTRY InInitializationOrderModuleList;
        PVOID BaseAddress;
        PVOID EntryPoint;
        ULONG SizeOfImage;
        UNICODE_STRING FullDllName;
        UNICODE_STRING BaseDllName;
        ULONG Flags;
        SHORT LoadCount;
        SHORT TlsIndex;
        LIST_ENTRY HashTableEntry;
        ULONG TimeDateStamp;

    } LDR_MODULE, * PLDR_MODULE;

    typedef struct _PEB_LDR_DATA {
        ULONG Length;
        BOOLEAN Initialized;
        PVOID SsHandle;
        LIST_ENTRY InLoadOrderModuleList;
        LIST_ENTRY InMemoryOrderModuleList;
        LIST_ENTRY InInitializationOrderModuleList;
    } PEB_LDR_DATA, * PPEB_LDR_DATA;

    typedef struct _PEB_FREE_BLOCK {
        _PEB_FREE_BLOCK* Next;
        ULONG Size;
    } PEB_FREE_BLOCK, * PPEB_FREE_BLOCK;

    typedef struct _RTL_DRIVE_LETTER_CURDIR {
        USHORT Flags;
        USHORT Length;
        ULONG TimeStamp;
        UNICODE_STRING DosPath;
    } RTL_DRIVE_LETTER_CURDIR, * PRTL_DRIVE_LETTER_CURDIR;

    typedef struct _RTL_USER_PROCESS_PARAMETERS {
        ULONG MaximumLength;
        ULONG Length;
        ULONG Flags;
        ULONG DebugFlags;
        PVOID ConsoleHandle;
        ULONG ConsoleFlags;
        HANDLE StdInputHandle;
        HANDLE StdOutputHandle;
        HANDLE StdErrorHandle;
        UNICODE_STRING CurrentDirectoryPath;
        HANDLE CurrentDirectoryHandle;
        UNICODE_STRING DllPath;
        UNICODE_STRING ImagePathName;
        UNICODE_STRING CommandLine;
        PVOID Environment;
        ULONG StartingPositionLeft;
        ULONG StartingPositionTop;
        ULONG Width;
        ULONG Height;
        ULONG CharWidth;
        ULONG CharHeight;
        ULONG ConsoleTextAttributes;
        ULONG WindowFlags;
        ULONG ShowWindowFlags;
        UNICODE_STRING WindowTitle;
        UNICODE_STRING DesktopName;
        UNICODE_STRING ShellInfo;
        UNICODE_STRING RuntimeData;
        RTL_DRIVE_LETTER_CURDIR DLCurrentDirectory[0x20];
    } RTL_USER_PROCESS_PARAMETERS, * PRTL_USER_PROCESS_PARAMETERS;

    typedef struct _PEB {
        BOOLEAN InheritedAddressSpace;
        BOOLEAN ReadImageFileExecOptions;
        BOOLEAN BeingDebugged;
        BOOLEAN Spare;
        HANDLE Mutant;
        PVOID ImageBaseAddress;
        PPEB_LDR_DATA LoaderData;
        RTL_USER_PROCESS_PARAMETERS ProcessParameters;
        PVOID SubSystemData;
        PVOID ProcessHeap;
        PVOID FastPebLock;
        uintptr_t FastPebLockRoutine;
        uintptr_t FastPebUnlockRoutine;
        ULONG EnvironmentUpdateCount;
        uintptr_t KernelCallbackTable;
        PVOID EventLogSection;
        PVOID EventLog;
        PPEB_FREE_BLOCK FreeList;
        ULONG TlsExpansionCounter;
        PVOID TlsBitmap;
        ULONG TlsBitmapBits[0x2];
        PVOID ReadOnlySharedMemoryBase;
        PVOID ReadOnlySharedMemoryHeap;
        uintptr_t ReadOnlyStaticServerData;
        PVOID AnsiCodePageData;
        PVOID OemCodePageData;
        PVOID UnicodeCaseTableData;
        ULONG NumberOfProcessors;
        ULONG NtGlobalFlag;
        BYTE Spare2[0x4];
        LARGE_INTEGER CriticalSectionTimeout;
        ULONG HeapSegmentReserve;
        ULONG HeapSegmentCommit;
        ULONG HeapDeCommitTotalFreeThreshold;
        ULONG HeapDeCommitFreeBlockThreshold;
        ULONG NumberOfHeaps;
        ULONG MaximumNumberOfHeaps;
        uintptr_t* ProcessHeaps;
        PVOID GdiSharedHandleTable;
        PVOID ProcessStarterHelper;
        PVOID GdiDCAttributeList;
        PVOID LoaderLock;
        ULONG OSMajorVersion;
        ULONG OSMinorVersion;
        ULONG OSBuildNumber;
        ULONG OSPlatformId;
        ULONG ImageSubSystem;
        ULONG ImageSubSystemMajorVersion;
        ULONG ImageSubSystemMinorVersion;
        ULONG GdiHandleBuffer[0x22];
        ULONG PostProcessInitRoutine;
        ULONG TlsExpansionBitmap;
        BYTE TlsExpansionBitmapBits[0x80];
        ULONG SessionId;
    } PEB, * PPEB;

    typedef BOOLEAN(NTAPI
        * PLDR_INIT_ROUTINE)(
            _In_ PVOID DllHandle,
            _In_ ULONG
            Reason,
            _In_opt_ PVOID Context
            );

    typedef struct _LDRP_CSLIST {
        PSINGLE_LIST_ENTRY Tail;
    } LDRP_CSLIST, * PLDRP_CSLIST;

    typedef struct _LDR_SERVICE_TAG_RECORD {
        struct _LDR_SERVICE_TAG_RECORD* Next;
        ULONG ServiceTag;
    } LDR_SERVICE_TAG_RECORD, * PLDR_SERVICE_TAG_RECORD;

    typedef enum _LDR_DDAG_STATE {
        LdrModulesMerged = -5,
        LdrModulesInitError = -4,
        LdrModulesSnapError = -3,
        LdrModulesUnloaded = -2,
        LdrModulesUnloading = -1,
        LdrModulesPlaceHolder = 0,
        LdrModulesMapping = 1,
        LdrModulesMapped = 2,
        LdrModulesWaitingForDependencies = 3,
        LdrModulesSnapping = 4,
        LdrModulesSnapped = 5,
        LdrModulesCondensed = 6,
        LdrModulesReadyToInit = 7,
        LdrModulesInitializing = 8,
        LdrModulesReadyToRun = 9
    } LDR_DDAG_STATE;

    typedef struct _LDR_DDAG_NODE {
        LIST_ENTRY Modules;
        PLDR_SERVICE_TAG_RECORD ServiceTagList;
        ULONG LoadCount;
        ULONG LoadWhileUnloadingCount;
        ULONG LowestLink;
        union {
            LDRP_CSLIST Dependencies;
            SINGLE_LIST_ENTRY RemovalLink;
        };
        LDRP_CSLIST IncomingDependencies;
        LDR_DDAG_STATE State;
        SINGLE_LIST_ENTRY CondenseLink;
        ULONG PreorderNumber;
    } LDR_DDAG_NODE, * PLDR_DDAG_NODE;

    typedef struct _RTL_BALANCED_NODE {
        union {
            struct _RTL_BALANCED_NODE* Children[2];
            struct {
                struct _RTL_BALANCED_NODE* Left;
                struct _RTL_BALANCED_NODE* Right;
            };
        };
        union {
            UCHAR Red : 1;
            UCHAR Balance : 2;
            ULONG_PTR ParentValue;
        };
    } RTL_BALANCED_NODE, * PRTL_BALANCED_NODE;

    typedef enum _LDR_DLL_LOAD_REASON {
        LoadReasonStaticDependency,
        LoadReasonStaticForwarderDependency,
        LoadReasonDynamicForwarderDependency,
        LoadReasonDelayloadDependency,
        LoadReasonDynamicLoad,
        LoadReasonAsImageLoad,
        LoadReasonAsDataLoad,
        LoadReasonEnclavePrimary,// since REDSTONE3
        LoadReasonEnclaveDependency,
        LoadReasonPatchImage,// since WIN11
        LoadReasonUnknown = -1
    } LDR_DLL_LOAD_REASON,
        * PLDR_DLL_LOAD_REASON;

    typedef struct _LDR_DATA_TABLE_ENTRY {
        LIST_ENTRY InLoadOrderLinks;
        LIST_ENTRY InMemoryOrderLinks;
        union {
            LIST_ENTRY InInitializationOrderLinks;
            LIST_ENTRY InProgressLinks;
        };
        PVOID DllBase;
        PLDR_INIT_ROUTINE EntryPoint;
        ULONG SizeOfImage;
        UNICODE_STRING FullDllName;
        UNICODE_STRING BaseDllName;
        union {
            UCHAR FlagGroup[4];
            ULONG Flags;
            struct {
                ULONG PackagedBinary : 1;
                ULONG MarkedForRemoval : 1;
                ULONG ImageDll : 1;
                ULONG LoadNotificationsSent : 1;
                ULONG TelemetryEntryProcessed : 1;
                ULONG ProcessStaticImport : 1;
                ULONG InLegacyLists : 1;
                ULONG InIndexes : 1;
                ULONG ShimDll : 1;
                ULONG InExceptionTable : 1;
                ULONG ReservedFlags1 : 2;
                ULONG LoadInProgress : 1;
                ULONG LoadConfigProcessed : 1;
                ULONG EntryProcessed : 1;
                ULONG ProtectDelayLoad : 1;
                ULONG ReservedFlags3 : 2;
                ULONG DontCallForThreads : 1;
                ULONG ProcessAttachCalled : 1;
                ULONG ProcessAttachFailed : 1;
                ULONG CorDeferredValidate : 1;
                ULONG CorImage : 1;
                ULONG DontRelocate : 1;
                ULONG CorILOnly : 1;
                ULONG ChpeImage : 1;
                ULONG ChpeEmulatorImage : 1;
                ULONG ReservedFlags5 : 1;
                ULONG Redirected : 1;
                ULONG ReservedFlags6 : 2;
                ULONG CompatDatabaseProcessed : 1;
            };
        };
        USHORT ObsoleteLoadCount;
        USHORT TlsIndex;
        LIST_ENTRY HashLinks;
        ULONG TimeDateStamp;
        struct _ACTIVATION_CONTEXT* EntryPointActivationContext;
        PVOID Lock;// RtlAcquireSRWLockExclusive
        PLDR_DDAG_NODE DdagNode;
        LIST_ENTRY NodeModuleLink;
        struct _LDRP_LOAD_CONTEXT* LoadContext;
        PVOID ParentDllBase;
        PVOID SwitchBackContext;
        RTL_BALANCED_NODE BaseAddressIndexNode;
        RTL_BALANCED_NODE MappingInfoIndexNode;
        ULONG_PTR OriginalBase;
        LARGE_INTEGER LoadTime;
        ULONG BaseNameHashValue;
        LDR_DLL_LOAD_REASON LoadReason;// since WIN8
        ULONG ImplicitPathOptions;
        ULONG ReferenceCount;// since WIN10
        ULONG DependentLoadFlags;
        UCHAR SigningLevel;// since REDSTONE2
        ULONG CheckSum;    // since 22H1
        PVOID ActivePatchImageBase;
        uintptr_t HotPatchState;
    } LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;
}// namespace ntapi
