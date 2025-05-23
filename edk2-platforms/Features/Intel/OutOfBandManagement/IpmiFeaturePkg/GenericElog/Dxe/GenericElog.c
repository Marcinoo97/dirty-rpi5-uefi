/** @file
  Generic Event Log functions of DXE driver.

Copyright (c) 2023, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "GenericElog.h"

ELOG_MODULE_GLOBAL  *mElogModuleGlobal;

//
// Define module globals used to register for notification of when
// the ELOG REDIR protocol has been produced.
//
EFI_EVENT  mEfiElogRedirProtocolEvent;

/**
  Sends the Event-Log data to the destination.

  @param[in] ElogData Pointer to the Event-Log data that needs to be recorded.
  @param[in] DataType Type of Elog Data that is being recorded.
  @param[in] AlertEvent This is an indication that the input data type is an Alert.
  @param[in] DataSize
  @param[out] RecordId Record ID sent by the target.
  @param[in] Global The module global variable pointer.
  @param[in] Virtual If this function is called in virtual mode or physical mode

  @retval EFI_SUCCESS Event-Log was recorded successfully.
  @retval EFI_OUT_OF_RESOURCES Not enough resources to record data.
  @retval EFI_UNSUPPORTED The Data Type is unsupported.

**/
EFI_STATUS
EfiLibSetElogData (
  IN  UINT8               *ElogData,
  IN  EFI_SM_ELOG_TYPE    DataType,
  IN  BOOLEAN             AlertEvent,
  IN  UINTN               DataSize,
  OUT UINT64              *RecordId,
  IN  ELOG_MODULE_GLOBAL  *Global,
  IN  BOOLEAN             Virtual
  )
{
  UINT8       Index;
  VOID        *ElogRedirCommand;
  EFI_STATUS  Status;
  EFI_STATUS  RetStatus;

  RetStatus = EFI_UNSUPPORTED;

  if (DataType >= EfiSmElogMax) {
    RetStatus = EFI_INVALID_PARAMETER;
    return RetStatus;
  }

  for (Index = 0; Index < Global->MaxDescriptors; Index++) {
    if (Global->Redir[Index].Valid) {
      ElogRedirCommand = Global->Redir[Index].Command[Virtual].SetEventLogData.Function;
      Status           = (*((EFI_SET_ELOG_DATA *)&ElogRedirCommand))(Global->Redir[Index].Command[Virtual].This, ElogData, DataType, AlertEvent, DataSize, RecordId);

      if (!EFI_ERROR (Status)) {
        RetStatus = EFI_SUCCESS;
        break;
      } else if (Status != EFI_UNSUPPORTED) {
        RetStatus = Status;
        break;
      }
    }
  }

  return RetStatus;
}

/**
  Add function description.

  @param This        -  add argument description
  @param ElogData    -  add argument description
  @param DataType    -  add argument description
  @param AlertEvent  -  add argument description
  @param DataSize    -  add argument description
  @param RecordId    -  add argument description

  @retval EFI_SUCCESS

**/
EFI_STATUS
EfiSetElogData (
  IN  EFI_SM_ELOG_PROTOCOL  *This,
  IN  UINT8                 *ElogData,
  IN  EFI_SM_ELOG_TYPE      DataType,
  IN  BOOLEAN               AlertEvent,
  IN  UINTN                 DataSize,
  OUT UINT64                *RecordId
  )
{
  if (DataType >= EfiSmElogMax) {
    return EFI_INVALID_PARAMETER;
  }

  return EfiLibSetElogData (
           ElogData,
           DataType,
           AlertEvent,
           DataSize,
           RecordId,
           mElogModuleGlobal,
           FALSE
           );
}

/**
  Gets the Event-Log data from the destination.

  @param[in] ElogData Pointer to the Event-Log data buffer that will contain the data to be retrieved.
  @param[in] DataType Type of Elog Data that is being recorded.
  @param[in, out] DataSize
  @param[in, out] RecordId This is the RecordId of the next record. If ElogData is NULL,
                       this gives the RecordId of the first record available in the database with the correct DataSize.
                        A value of 0 on return indicates the last record if the EFI_STATUS indicates a success
  @param[in] Global The module global variable pointer.
  @param[in] Virtual If this function is called in virtual mode or physical mode

  @retval EFI_SUCCESS Event-Log was retrieved successfully.
  @retval EFI_NOT_FOUND Event-Log target not found.
  @retval EFI_BUFFER_TOO_SMALL Target buffer is too small to retrieve the data.
  @retval EFI_UNSUPPORTED The Data Type is unsupported

**/
EFI_STATUS
EfiLibGetElogData (
  IN UINT8               *ElogData,
  IN EFI_SM_ELOG_TYPE    DataType,
  IN OUT UINTN           *DataSize,
  IN OUT UINT64          *RecordId,
  IN ELOG_MODULE_GLOBAL  *Global,
  IN BOOLEAN             Virtual
  )
{
  UINT8       Index;
  VOID        *ElogRedirCommand;
  EFI_STATUS  Status;
  EFI_STATUS  RetStatus;

  RetStatus = EFI_UNSUPPORTED;

  if (DataType >= EfiSmElogMax) {
    RetStatus = EFI_INVALID_PARAMETER;
    return RetStatus;
  }

  for (Index = 0; Index < Global->MaxDescriptors; Index++) {
    if (Global->Redir[Index].Valid) {
      ElogRedirCommand = Global->Redir[Index].Command[Virtual].GetEventLogData.Function;
      Status           = (*((EFI_GET_ELOG_DATA *)&ElogRedirCommand))(Global->Redir[Index].Command[Virtual].This, ElogData, DataType, DataSize, RecordId);

      if (!EFI_ERROR (Status)) {
        RetStatus = EFI_SUCCESS;
        break;
      } else if (Status != EFI_UNSUPPORTED) {
        RetStatus = Status;
        break;
      }
    }
  }

  return RetStatus;
}

/**
  Add function description.

  @param This      - Protocol instance pointer.
  @param ElogData  - Pointer to the Event-Log data buffer that will contain the data to be retrieved.
  @param DataType  - Type of Elog Data that is being recorded.
  @param DataSize  - Size of Elog Data in bytes.
  @param RecordId  - This is the RecordId of the next record. If ElogData is NULL,
                     this gives the RecordId of the first record available in the database with the correct DataSize.
                     A value of 0 on return indicates the last record if the EFI_STATUS indicates a success

  @retval EFI_SUCCESS add return values

**/
EFI_STATUS
EfiGetElogData (
  IN EFI_SM_ELOG_PROTOCOL  *This,
  IN UINT8                 *ElogData,
  IN EFI_SM_ELOG_TYPE      DataType,
  IN OUT UINTN             *DataSize,
  IN OUT UINT64            *RecordId
  )
{
  if (DataType >= EfiSmElogMax) {
    return EFI_INVALID_PARAMETER;
  }

  return EfiLibGetElogData (ElogData, DataType, DataSize, RecordId, mElogModuleGlobal, FALSE);
}

/**
  Erases the Event-Log data from the destination.

  @param DataType              - Type of Elog Data that is being Erased.
  @param RecordId              - This is the RecordId of the data to be erased. If RecordId is NULL, all
                                 the records on the database are erased if permitted by the target.
                                 Contains the deleted RecordId on return
  @param Global                - The module global variable pointer.
  @param Virtual               - If this function is called in virtual mode or physical mode

  @retval EFI_SUCCESS           - Event-Log was erased successfully
  @retval EFI_UNSUPPORTED       - The Data Type is unsupported
  @retval EFI_NOT_FOUND         - Event-Log target not found

**/
EFI_STATUS
EfiLibEraseElogData (
  IN EFI_SM_ELOG_TYPE    DataType,
  IN OUT UINT64          *RecordId,
  IN ELOG_MODULE_GLOBAL  *Global,
  IN BOOLEAN             Virtual
  )
{
  UINT8       Index;
  VOID        *ElogRedirCommand;
  EFI_STATUS  Status;
  EFI_STATUS  RetStatus;

  RetStatus = EFI_UNSUPPORTED;

  if (DataType >= EfiSmElogMax) {
    RetStatus = EFI_INVALID_PARAMETER;
    return RetStatus;
  }

  for (Index = 0; Index < Global->MaxDescriptors; Index++) {
    if (Global->Redir[Index].Valid) {
      ElogRedirCommand = Global->Redir[Index].Command[Virtual].EraseEventLogData.Function;
      Status           = (*((EFI_ERASE_ELOG_DATA *)&ElogRedirCommand))(Global->Redir[Index].Command[Virtual].This, DataType, RecordId);

      if (!EFI_ERROR (Status)) {
        RetStatus = EFI_SUCCESS;
        break;
      } else if (Status != EFI_UNSUPPORTED) {
        RetStatus = Status;
        break;
      }
    }
  }

  return RetStatus;
}

/**
  Efi Erase Elog Data.

  @param This
  @param DataType
  @param RecordId

  @retval EFI_SUCCESS

**/
EFI_STATUS
EfiEraseElogData (
  IN EFI_SM_ELOG_PROTOCOL  *This,
  IN EFI_SM_ELOG_TYPE      DataType,
  IN OUT UINT64            *RecordId
  )
{
  return EfiLibEraseElogData (DataType, RecordId, mElogModuleGlobal, FALSE);
}

/**
  This API enables/Disables Event Log.

  @param DataType              - Type of Elog Data that is being Activated.
  @param EnableElog            - Enables (TRUE) / Disables (FALSE) Event Log. If NULL just returns the
                                 Current ElogStatus.
  @param ElogStatus            - Current (New) Status of Event Log. Enabled (TRUE), Disabled (FALSE).
  @param Global                - The module global variable pointer.
  @param Virtual               - If this function is called in virtual mode or physical mode

  @retval EFI_SUCCESS           - Event-Log was recorded successfully
  @retval EFI_UNSUPPORTED       - The Data Type is unsupported

**/
EFI_STATUS
EfiLibActivateElog (
  IN  EFI_SM_ELOG_TYPE  DataType,
  IN BOOLEAN            *EnableElog,
  OUT BOOLEAN           *ElogStatus,
  ELOG_MODULE_GLOBAL    *Global,
  BOOLEAN               Virtual
  )
{
  UINT8       Index;
  VOID        *ElogRedirCommand;
  EFI_STATUS  Status;
  EFI_STATUS  RetStatus;

  RetStatus = EFI_UNSUPPORTED;
  if (DataType >= EfiSmElogMax) {
    RetStatus = EFI_INVALID_PARAMETER;
    return RetStatus;
  }

  for (Index = 0; Index < Global->MaxDescriptors; Index++) {
    if (Global->Redir[Index].Valid) {
      ElogRedirCommand = Global->Redir[Index].Command[Virtual].ActivateEventLog.Function;
      Status           = (*((EFI_ACTIVATE_ELOG *)&ElogRedirCommand))(Global->Redir[Index].Command[Virtual].This, DataType, EnableElog, ElogStatus);

      if (!EFI_ERROR (Status)) {
        RetStatus = EFI_SUCCESS;
        break;
      } else if (Status != EFI_UNSUPPORTED) {
        RetStatus = Status;
        break;
      }
    }
  }

  return RetStatus;
}

/**
  Efi Activate Elog.

  @param This
  @param DataType
  @param EnableElog
  @param ElogStatus

  @retval EFI_SUCCESS

**/
EFI_STATUS
EfiActivateElog (
  IN EFI_SM_ELOG_PROTOCOL  *This,
  IN  EFI_SM_ELOG_TYPE     DataType,
  IN BOOLEAN               *EnableElog,
  OUT BOOLEAN              *ElogStatus
  )
{
  if (DataType >= EfiSmElogMax) {
    return EFI_INVALID_PARAMETER;
  }

  return EfiLibActivateElog (DataType, EnableElog, ElogStatus, mElogModuleGlobal, FALSE);
}

/**
  SetElogRedirInstances.

  @retval EFI_SUCCESS - The Redir instances were successfully set.
  @retval Other       - Failed to set Redir instances.

**/
EFI_STATUS
SetElogRedirInstances (
  VOID
  )
{
  UINTN                       NumHandles;
  UINTN                       Index;
  UINTN                       Instance;
  UINTN                       EmptyIndex;
  EFI_HANDLE                  *Buffer;
  EFI_STATUS                  Status;
  EFI_SM_ELOG_REDIR_PROTOCOL  *Redir;
  REDIR_MODULE_PROC           *RedirProc;

  Buffer = NULL;

  //
  // Check for all IPMI Controllers
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiRedirElogProtocolGuid,
                  NULL,
                  &NumHandles,
                  &Buffer
                  );

  if (EFI_ERROR (Status) || (NumHandles == 0)) {
    return EFI_SUCCESS;
  }

  for (Index = 0; ((Index < NumHandles) && (Index < mElogModuleGlobal->MaxDescriptors)); Index++) {
    EmptyIndex = mElogModuleGlobal->MaxDescriptors;

    Status = gBS->HandleProtocol (
                    Buffer[Index],
                    &gEfiRedirElogProtocolGuid,
                    (VOID *)&Redir
                    );
    if (EFI_ERROR (Status) || (Redir == NULL)) {
      continue;
    }

    for (Instance = 0; Instance < mElogModuleGlobal->MaxDescriptors; Instance++) {
      if (mElogModuleGlobal->Redir[Instance].Valid == FALSE) {
        if (EmptyIndex >= mElogModuleGlobal->MaxDescriptors) {
          EmptyIndex = Instance;
        }
      } else {
        if (Redir == mElogModuleGlobal->Redir[Instance].Command->This) {
          EmptyIndex = mElogModuleGlobal->MaxDescriptors;
          break;
          //
          // FIX: changed continue to break.
          //
        }
      }
    }

    if (EmptyIndex < mElogModuleGlobal->MaxDescriptors) {
      RedirProc                                  = (REDIR_MODULE_PROC *)mElogModuleGlobal->Redir[EmptyIndex].Command;
      mElogModuleGlobal->Redir[EmptyIndex].Valid = TRUE;

      EfiSetFunctionEntry (&RedirProc->ActivateEventLog, *((VOID **)&Redir->ActivateEventLog));
      EfiSetFunctionEntry (&RedirProc->EraseEventLogData, *((VOID **)&Redir->EraseEventlogData));
      EfiSetFunctionEntry (&RedirProc->GetEventLogData, *((VOID **)&Redir->GetEventLogData));
      EfiSetFunctionEntry (&RedirProc->SetEventLogData, *((VOID **)&Redir->SetEventLogData));
      RedirProc->This = Redir;

      CopyMem (&RedirProc[EFI_ELOG_VIRTUAL], &RedirProc[EFI_ELOG_PHYSICAL], sizeof (REDIR_MODULE_PROC));
    }
  }

  if (Buffer != NULL) {
    FreePool (Buffer);
  }

  return EFI_SUCCESS;
}

/**
  This notification function is invoked when an instance of the
  ELOG REDIR protocol is produced.

  @param Event - The event that occurred
  @param Context - For EFI compatibility.  Not used.

**/
VOID
EFIAPI
NotifyElogRedirEventCallback (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  SetElogRedirInstances ();
}

/**
  Initialize the generic Elog driver of server management.

  @param ImageHandle  - The image handle of this driver
  @param SystemTable  - The pointer of EFI_SYSTEM_TABLE

  @retval EFI_SUCCESS - The driver initialized successfully

**/
EFI_STATUS
EFIAPI
InitializeElogLayer (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_HANDLE            NewHandle;
  EFI_STATUS            Status;
  EFI_SM_ELOG_PROTOCOL  *ElogProtocol;
  EFI_EVENT             Event;

  mElogModuleGlobal = AllocateZeroPool (sizeof (ELOG_MODULE_GLOBAL));
  ASSERT (mElogModuleGlobal != NULL);
  if (mElogModuleGlobal == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  ElogServiceInitialize (ImageHandle, SystemTable);

  mElogModuleGlobal->MaxDescriptors = MAX_REDIR_DESCRIPTOR;

  //
  // Check for all IPMI Controllers
  //
  SetElogRedirInstances ();

  ElogProtocol = AllocatePool (sizeof (EFI_SM_ELOG_PROTOCOL));
  ASSERT (ElogProtocol != NULL);
  if (ElogProtocol == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  ElogProtocol->ActivateEventLog  = EfiActivateElog;
  ElogProtocol->EraseEventlogData = EfiEraseElogData;
  ElogProtocol->GetEventLogData   = EfiGetElogData;
  ElogProtocol->SetEventLogData   = EfiSetElogData;

  NewHandle = NULL;
  Status    = gBS->InstallProtocolInterface (
                     &NewHandle,
                     &gEfiGenericElogProtocolGuid,
                     EFI_NATIVE_INTERFACE,
                     ElogProtocol
                     );

  ASSERT_EFI_ERROR (Status);
  if (Status != EFI_SUCCESS) {
    return Status;
  }

  //
  // Register to be notified when the ELOG REDIR protocol has been
  // produced.
  //
  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  NotifyElogRedirEventCallback,
                  NULL,
                  &Event
                  );

  if (!EFI_ERROR (Status)) {
    Status = gBS->RegisterProtocolNotify (
                    &gEfiRedirElogProtocolGuid,
                    Event,
                    &mEfiElogRedirProtocolEvent
                    );
  }

  return Status;
}

/**
  Set the function entry.

  @param FunctionPointer - The destination function pointer
  @param Function        - The source function pointer

  @retval EFI_SUCCESS - Set the function pointer successfully

**/
EFI_STATUS
EfiSetFunctionEntry (
  IN  FUNCTION_PTR  *FunctionPointer,
  IN  VOID          *Function
  )
{
  FunctionPointer->Function = (EFI_PLABEL *)Function;
  return EFI_SUCCESS;
}

/**
  Initialize Dxe generic event log.

  @param ImageHandle         - The Image handle of this driver.
  @param SystemTable         - The pointer of EFI_SYSTEM_TABLE.

  @retval EFI_SUCCESS - The driver successfully initialized

**/
EFI_STATUS
ElogServiceInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  return EFI_SUCCESS;
}
