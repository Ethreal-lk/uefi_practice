# UEFI的基础服务

## 系统表

### 系统表的构成

- 表头

  **数据结构定义：**

```c++
typedef struct {
  ///
  /// A 64-bit signature that identifies the type of table that follows.
  /// Unique signatures have been generated for the EFI System Table,
  /// the EFI Boot Services Table, and the EFI Runtime Services Table.
  ///
  UINT64  Signature;
  ///
  /// The revision of the EFI Specification to which this table
  /// conforms. The upper 16 bits of this field contain the major
  /// revision value, and the lower 16 bits contain the minor revision
  /// value. The minor revision values are limited to the range of 00..99.
  ///
  UINT32  Revision;
  ///
  /// The size, in bytes, of the entire table including the EFI_TABLE_HEADER.
  ///
  UINT32  HeaderSize; //整个表的长度 等价于sizeof(EFI_ SYSTEM_TABELE)
  ///
  /// The 32-bit CRC for the entire table. This value is computed by
  /// setting this field to 0, and computing the 32-bit CRC for HeaderSize bytes.
  ///
  UINT32  CRC32;  // 表的校验码， 
  ///
  /// Reserved field that must be set to 0.
  ///
  UINT32  Reserved;
} EFI_TABLE_HEADER;
```



- 固件信息

- 标注输入控制台、标准输出控制台、标准错误控制台

  **句柄通常用来标识Windows资源(如菜单、 图标、窗口等)和设备等对象。虽然可以把句柄理解为是一个指针变量类型，但它不是对象所在的地址指针，而是作为Windows系统内部表的索引值来使用 的。调用相关文本界面控制的API函数。这些函数可分为三类。一是用于控制台窗口操作的函数(包括窗口的缓冲区大小、窗口前景字符和背景颜色、窗口标题、大小和位置等)；二是用于控制台输入输出的函数(包括字符属性操作函数)；其他的函数并为最后一类。通过调用CloseHandle函数来关闭输入输出句柄。**

- 启动时服务表

- 运行时服务表

- 系统配置表

  ```c
  typedef struct {
    ///
    /// The 128-bit GUID value that uniquely identifies the system configuration table.
    ///
    EFI_GUID                          VendorGuid; //配置表标识符
    ///
    /// A pointer to the table associated with VendorGuid.
    ///
    VOID                              *VendorTable; // 指向配置表的数据
  } EFI_CONFIGURATION_TABLE;
  ```

  ​	 

**数据结构定义清单：**

```c
typedef struct {
  ///
  /// The table header for the EFI System Table.
  ///
  EFI_TABLE_HEADER                  Hdr; // 标准UEFI 表头
  ///
  /// A pointer to a null terminated string that identifies the vendor
  /// that produces the system firmware for the platform.
  ///
  CHAR16                            *FirmwareVendor;  // 固件提供商
  ///
  /// A firmware vendor specific value that identifies the revision
  /// of the system firmware for the platform.
  ///
  UINT32                            FirmwareRevision; // 固件版本号
  ///
  /// The handle for the active console input device. This handle must support
  /// EFI_SIMPLE_TEXT_INPUT_PROTOCOL and EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL.
  ///
  EFI_HANDLE                        ConsoleInHandle;  //输入控制台设备的句柄
  ///
  /// A pointer to the EFI_SIMPLE_TEXT_INPUT_PROTOCOL interface that is
  /// associated with ConsoleInHandle.
  ///
  EFI_SIMPLE_TEXT_INPUT_PROTOCOL    *ConIn;
  ///
  /// The handle for the active console output device.
  ///
  EFI_HANDLE                        ConsoleOutHandle;   //输出控制台设备的句柄
  ///
  /// A pointer to the EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL interface
  /// that is associated with ConsoleOutHandle.
  ///
  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL   *ConOut;
  ///
  /// The handle for the active standard error console device.
  /// This handle must support the EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL.
  ///
  EFI_HANDLE                        StandardErrorHandle;   // 标准错误控制台设备
  ///
  /// A pointer to the EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL interface
  /// that is associated with StandardErrorHandle.
  ///
  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL   *StdErr;
  ///
  /// A pointer to the EFI Runtime Services Table.
  ///
  EFI_RUNTIME_SERVICES              *RuntimeServices; //运行时服务表
  ///
  /// A pointer to the EFI Boot Services Table.
  ///
  EFI_BOOT_SERVICES                 *BootServices;   // 启动服务表
  ///
  /// The number of system configuration tables in the buffer ConfigurationTable.
  ///
  UINTN                             NumberOfTableEntries;  // configuratiantable数组大小 
  ///
  /// A pointer to the system configuration tables.
  /// The number of entries in the table is NumberOfTableEntries.
  ///
  EFI_CONFIGURATION_TABLE           *ConfigurationTable; // 系统配置表数组
} EFI_SYSTEM_TABLE;
```

