/** @file
debugchecker library functions.


Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#if defined(EDK2)
#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/IoLib.h>
#include <Protocol/LoadedImage.h>
#include <Library/UefiBootServicesTableLib.h>

#elif defined(GNU_EFI)
#include <efi.h>
#endif

#if defined(EDK2_LIN) || defined(EDK2)
#include <Register/Intel/Cpuid.h>
#endif


#define BIT_MASK_VALID                0b00000000000000001000000000000000
#define BIT_MASK_UFP2DFP              0b00000000000000000100000000000000
#define BIT_MASK_ENUM_CFG             0b00000000000000000011110000000000
#define BIT_MASK_EGRANT               0b00000000000000000000001000000000
#define BIT_MASK_DBGENABLE            0b00000000000000000000000100000000
#define BIT_MASK_PWRBRK2              0b00000000000000000000000010000000
#define BIT_MASK_USB3DBCEN            0b00000000000000000000000001000000
#define BIT_MASK_USB2DBCEN            0b00000000000000000000000000100000
#define BIT_MASK_HEEN                 0b00000000000000000000000000010000
#define BIT_MASK_USER_FIFO_BLOCKING   0b00000000000000000000000000000100
#define BIT_MASK_HOST_EXI_EN_LOCK     0b00000000000000000000000000000001
#define      MAX_CONFIG_INDEX   14
#define      DEBUG_PRIVACY_MSR  0xC80
#define      IA32_RTIT_CTL 0x570
#define      THREE_STRIKE_DISABLE 0x1A4

// CPU ID information
extern UINT32 gCPU_Family;
extern UINT32 gCPU_Model;

// Call-to-action variables
extern UINT8 u8Run_Control;
extern UINT8 u8Processor_Trace;
extern UINT8 u8Three_Strike;
extern UINT16 u16SelectedIndex;




void getDbgInfo();