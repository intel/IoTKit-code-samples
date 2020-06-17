/**
 * @file    debugchecker.c
 * @brief   functinos to check if debugging is enabled
 *
 *
Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent
 */

#include "debugchecker.h"
#include "dbgapp.h"
#include "asmutils.h"
#include "functions.h"


UINT32 gCPU_Family;
UINT32 gCPU_Model;

// Call-to-action variables
UINT8 u8Run_Control;
UINT8 u8Processor_Trace;
UINT8 u8Three_Strike;
UINT16 u16SelectedIndex;

/**
// Display Processor Version Information
*/
static void ProcessorVersionInfo() {
  CPUID_VERSION_INFO_EAX Eax;
  _cpuid(CPUID_VERSION_INFO, &Eax.Uint32, 0, 0,
           0);

  gCPU_Family = Eax.Bits.FamilyId;
  gCPU_Model = Eax.Bits.Model;
  if (Eax.Bits.FamilyId == 0x06 || Eax.Bits.FamilyId == 0x0F) {
    gCPU_Model |= (Eax.Bits.ExtendedModelId << 4);
  }

    print_str(L"Family:");
    print_uint64((uint64_t) gCPU_Family);
    print_str(L"Model:");
    print_uint64((uint64_t) gCPU_Model);
    print_str(L"Stepping:");
    print_uint64((uint64_t) Eax.Bits.SteppingId);

}

static void CheckRunControl() {
  UINT64 u64Debug = read_msr(DEBUG_PRIVACY_MSR);
  u8Run_Control = u64Debug & 0x1;

  print_str(L"Phase-2: Platform Debug Enabling checking\n" );

  if (u8Run_Control) {
    print_str(L"Run Control is enabled\n");
  } else {
    print_str(L"Run Control is disabled\n");
  }
}


static void CheckProcessorTrace() {
  UINT64 u64Debug = read_msr(IA32_RTIT_CTL);
  u8Processor_Trace = u64Debug & 0x1;

  print_str(L"Phase-3: Intel(r) Process Trace Enabling checking\n" );

  if (u8Processor_Trace) {
    print_str(L"Intel(R) Processor Trace is enabled\n");
  } else {
    print_str(L"Intel(R) Processor Trace is disabled\n");
  }
}

VOID CheckThreeStrike() {
  UINT64 u64Debug = read_msr(THREE_STRIKE_DISABLE);
  u8Three_Strike = (u64Debug & 0x0080) ? 1 : 0; // 0000 1000 0000 0000 - bit 11

  print_str(L"Phase-4: Crash Log configuration checking\n" );
  if (u8Three_Strike) {
      print_str(L"Three Strike is enabled\n");
  } else {
      print_str(L"Three Strike is disabled\n");
  }
}



static void checkDCIStatus(UINT32 u32Enabling_Status, UINT32 u32Connection_Established, UINT32 u32Debug_Enable) {

  print_str(L"Phase-1: Host-Target connectivity checking\n" );

  if (u32Enabling_Status) {
    print_str(L"Platform Debug Consent is enabled, ");
  } else {
    print_str(L"Platform Debug Consent is disabled, ");
  }

  if (u16SelectedIndex == 0) {
    if (u32Debug_Enable) {
      print_str(L"and Intel(R) DCI is enabled\n");
    } else {
      print_str(L"and Intel(R) DCI is disabled\n");
    }
  } else {
    if (u32Debug_Enable) {
      print_str(L"Intel(R) DCI is enabled, ");
    } else {
      print_str(L"Intel(R) DCI is disabled, ");
    }
    if (u32Connection_Established) {
      print_str(L"and a debug connection established.\n");
    } else {
      print_str(L"and a debug connection not established.\n");
    }
  }
}

static void CheckUSBConnection(){

  #ifdef DUMP_USB_CONFIG
  UINT16  u32USB_Config_register[0x10];
  UINT32  u32USB_CFG_register;
  #endif

  #ifdef DUMP_USB_CONFIG

  print_str(L"  Phase-5: USB Port configuration checking\n" );

  if (gDebugConfigurationTable[u16SelectedIndex].u32USB_CFG_Address != 0) {
    u32USB_CFG_register = MmioRead32(gDebugConfigurationTable[u16SelectedIndex].u32USB_CFG_Address);
    /*
      USB port configuration
      0x0    - Host
      0x20  - Disconnected
      0x160  - Device
      0x180  - DbC2
    */
    for (UINT16 i=0; i < gDebugConfigurationTable[u16SelectedIndex].u16Number_of_USB; i++) {
      u32USB_Config_register[i] = MmioRead16(gDebugConfigurationTable[u16SelectedIndex].u32USB_CFG_Address + i*0x10);

      switch(u32USB_Config_register[i]) {
        case 0x0:
          print_str(L"Host");
          break;
        case 0x20:
          print_str(L"Disconnected");
          break;
        case 0x160:
          print_str(L"Device");
          break;
        case 0x180:
          print_str(L"DbC2");
          break;
      }
      print_str(L"\n");

    }
  }
  #endif

  return;
}


void getDbgInfo() {

    UINT32 u32ECTRL_register;


    u32ECTRL_register = MmioReadEfi(0xD0A80004);

    UINT32 u32Connection_Established = (u32ECTRL_register & BIT_MASK_EGRANT) >> 9;
    UINT32 u32Debug_Enable = (u32ECTRL_register & BIT_MASK_DBGENABLE) >> 8;
    UINT32 u32Enabling_Status = (u32ECTRL_register & BIT_MASK_HEEN) >> 4;
    UINT32 u32Debug_ConsentEnum = (u32ECTRL_register & BIT_MASK_ENUM_CFG) >> 10;
    ProcessorVersionInfo();
    checkDCIStatus(u32Enabling_Status, u32Connection_Established, u32Debug_Enable);
    CheckRunControl();
    CheckProcessorTrace();
    CheckThreeStrike();

}