# 第十三课 ACPI基础篇

## 1.ACPI简介


###1.1 ACPI是什么
 **ACPI全称Advanced Configuration andPower Interface（高级配置电源管理接口）** 规范定义了标准的工业接口，使OS直接主导主板设备的配置和主板设备及整个系统的电源管理；原Advanced Power Management模型(APM)将电源管理几乎完全分配给BIOS控制，限制了操作系统在控制电能消耗方面的功能。

**ACPI**规范定义了硬件接口、软件接口和数据结构，既不是一个软件规范也不是一个硬件规范，ACPI是个包括软件接口、硬件接口的接口规范。

- ACPI系统框架图


![avatar](Picture13/ACPI.PNG)


![avatar](Picture13/pic1.PNG)
###1.2 ACPI语言

- **ASL**：ACPI Source Language 
- **AML**：ACPI Machine Language 
![avatar](Picture13/pic2.PNG)

###1.3 ACPI Talbes
**RSDT ：Root System Description Table**它主要描述了一组指向其他系统描述表的物理指针（32位）。 OS通过 **RSDP** 结构中的指针来定位**RSDT**。 

 从0F0000h到0FFFFFh这段只供BIOS读取的存储空间搜索**‘RSD PTR’** 字符，当搜到时，这个时候的地址即是RSDP的入口地址。

如下图所示：
**偏移16个byte**开始的4个字节数据即**RSDT**的32位物理地址，**偏移24byte**开始的8个字节数据即**XSDT**（如果存在的话）的64位物理地址。


- **RSDP：Root System Description Pointer** 


![avatar](Picture13/pic3.PNG)

- **ACPI Table Header**


![avatar](Picture13/pic4.PNG)

**Signature ：四字节的标志   Length:   整个TABLE总的字节长度**

- **Root System Description Pointer and Table**

![avatar](Picture13/pic15.PNG)

- **RSDT结构图**

偏移36byte开始，每4个字节数据指向一个table的入口地址

![avatar](Picture13/pic5.PNG)

- **XSDT结构图**

**XSDT**：Extended System Description Table 他和DSDT类似，区别在于它提供的是64位地址的物理指针


**从偏移36byte开始，每8个字节数据指向一个table的入口地址。**

**XSDT和RSDT的功能一样，都是提供其他Table的入口地址，只是XSDT是指向64位的物理地址，一个兼容ACPI的系统如果存在XSDT的话必须使用XSDT**

![avatar](Picture13/pic6.PNG)



- **FADT：Fixed ACPI Description Table 包含了 ACPI 硬件寄存器的配置详细信息和 FACS/DSDT 的物理地址。**

- **FACS ：Firmware ACPI Control Structure
   实现固件和 OS之间的信息交换，包含上次启动时的系统硬件签名、firmware waking vector和 Global Lock等**

- **DSDT：Differentiated System Description Table
   包含其他平台的实现和配置详细信息。**


FACS和DSDT这两个Table，而这两个table的地址是在FADT(FACP）中存放，所以我们要先从RSDT中读出FADT的地址，进入到FADT中，FADT的结构如图1-4，偏移36byte的4个字节即FACS的入口地址，偏移40byte的4个字节为DSDT的地址。

![avatar](Picture13/pic10.PNG)
- **SSDT ：Secondary System Description Tablez**

![avatar](Picture13/pic7.PNG)

 - **MADT ：Multiple APIC DescriptionTable 描述和中断相关的信息。**

![avatar](Picture13/pic9.PNG)


- **ACPI Table的建立**

**BIOS在把控制器交给OS之前建立ACPI Table**


![avatar](Picture13/pic8.PNG)
###1.4 ACPI的应用

1. 设备配置（Device Configuration）
2. 电源和性能管理（Power and performance management）
3. 配置/即插即用（Configuration/Plug and Play）
4. 系统事件（System Event）
5. 电池管理（Battery management）
6. 温度管理（Thermal management）
7. 嵌入式控制器接口（Embedded Controller Interface）
8. SMBus 控制器（SMBus Controller）
9. 唤醒和休眠（ Waking and Sleeping ）
10. 平台通信通道（ Platform Communications Channel ）
11. NUMA设置（ Non-Uniform Memory ）
12. APEI设置（ ACPI Platform Error Interfaces ） 



##2. 电源管理

![avatar](Picture13/pic11.PNG)

- **Global System States (Gx)**
- 
G0：正常工作状态，也是S0状态，操作系统和应用程序都在运行。 CPU执行指令。 CPU 、硬盘、 DVD 驱动器等等这些的设备可以一再的进入和从低能源状态回来，叫做 C0-Cn 和 D0-D3。

G1：睡眠状态， 细分为从 S1 到 S4 这四种状态。

G2：Soft Off，也是S5状态 ，有些部件仍然带电，使计算机仍然可以被键盘、 时钟、modem、 LAN还有 USB 设备所唤醒。

G3：Mechanical Off 

- **Sleeping States (Sx)**

S0：即G0，正常工作状态

S1：最耗电的睡眠模式。CPU 停止执行指令。 CPU 和内存的电源一直维持着，一些设备如果没有被使用那么就会被停止供电。

S2：比 S1 更深的睡眠状态，不给CPU 供电；然而，通常这种模式并不被采用。 

S3 ：Suspend to RAM/STR，也叫Standby。此时，RAM仍然有电源供给。

S4： Suspend to Disk/STD，也叫Hibernate休眠。信息存入硬盘， RAM无电源供给。

S5：即G2，Soft Off

- **Device Power States (Dx)**


D0(Fully-On)：即正常工作状态。

D1：具体含义由各设备自己定义，且不一定所以设备都支持D1状态。

D2：和D1类似。如果总线进入D2状态，则其Device会丢失环境。

D3hot：具体含义由各设备自己定义，需要留有足够的点让软件可以枚举到设备。

D3(Off)：也叫D3clod，设备完全不供电，设备环境完全丢失，不会被OS或软件枚举。重新加电时，OS需要重新初始化设备。


- **Processor power states (Cx)**

C0：只有在该状态下 CPU 才会执行指令

C1：也叫Halt，Latency必须短到OS使用CPU 的时候不会考虑到这个时间方 面的因素。除了CPU不执行指令外，没有其他软件可见的效果

C2：也叫Stop-Clock ，Latency由ACPI 固件提供，OS利用这个信息决定何时用C1取代C2 

C3： 也叫Sleep，处理器缓存保留了所有的状态信息，但是忽略所有的侦听。 OS负责保证缓存数据的一致性

	Latency: 各状态恢复到 C0 的唤醒时间
    C1E=C1+最低频率的P-State 
- **Device and Processor Performance States (Px)**

Px 状态是在 C0（对于处理器）和 D0（对于设备）下定义的电源消耗和能力的状态。 性能状态允许 OSPM 在性能和能源消耗之间取得平衡。

P0：使用最大性能并且消耗的电能最多。

P1：性能比前者要小，且消耗电能也相应少一些。

Pn：n的大小依赖于处理器和设备，处理器和设备可以定一个任意的不超过 255 的数字。 

##3. ACPI in UEFI



![avatar](Picture13/pic17.PNG)


- **UEFI ACPI Protocol **

![avatar](Picture13/pic13.PNG)


![avatar](Picture13/pic19.PNG)

- **ACPI Namespace** 

![avatar](Picture13/pic14.PNG)

- **设备配置**


![avatar](Picture13/pic20.PNG)

- **OperationRegion**

![avatar](Picture13/pic21.PNG)
![avatar](Picture13/pic22.PNG)