#ifndef JABRADEVICECONFIG_H
#define JABRADEVICECONFIG_H

/* --------------------------------------------------------------------
 *
 *  GGGGGG  NN    N
 *  G       N N   N
 *  G  GGG  N  N  N - Audio
 *  G    G  N   N N
 *  GGGGGG  N    NN
 *
 *  Copyright (c) 2017, GN-Audio
 * -------------------------------------------------------------------- */

/**
 * @file JabraDeviceConfig.h
 * @brief Defines the dynamic setting interface.
 * Developer information can be found in the universal SDK documentation.
 */

/****************************************************************************/
/*                              INCLUDE FILES                               */
/****************************************************************************/

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Common.h"

/****************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                       */
/****************************************************************************/

/* This enum represents each setting data type */
typedef enum  _DataType{
  settingByte = 0,
  settingString
} DataType;

/* This enum represents each setting control type */
typedef enum  _ControlType{
  cntrlRadio = 0,
  cntrlToggle,
  cntrlComboBox,
  cntrlDrpDown,
  cntrlLabel,
  cntrlTextBox,
  cntrlButton,
  cntrlUnknown
} ControlType;

/* This structure represents Validation Rule */
typedef struct _ValidationRule{
  int  minLength;
  int  maxLength;
  char *regExp;
  char *errorMessage;
} ValidationRule;

typedef struct _DependencySetting{
  char *GUID;
  bool enableFlag;
}DependencySetting;

/* This structure contains setting values in key value pair */
typedef struct _ListKeyValue{
  unsigned short key;
  void *value;

  int dependentcount;
  DependencySetting* dependents;
}ListKeyValue;

/* This structure represents each setting info */
typedef struct _SettingInfo{
  /* Setting GUID*/
  char *guid;

  /* Setting Name*/
  char *name;

  /* setting Help Text */
  char *helpText;

  /* Current Device value for this setting */
  void *currValue;

  /* Number of values for this setting */
  int  listSize;

  /* Structure for all key-value pairs for this setting */
  ListKeyValue *listKeyValue;

  /* If validation rule is supported or not */
  bool isValidationSupport;

  /* This structure contains the validation rule */
  ValidationRule* validationRule;

  /*If device restart is required for this setting */
  bool isDeviceRestart;

  /* Is setting protected */
  bool isSettingProtected;

  /* Is setting protection enabled */
  bool isSettingProtectionEnabled;

  /*When wireless headset is connected to its base/dongle */
  bool isWirelessConnect;

  /* This represents what type of control for setting */
  ControlType cntrlType;

  /* This represents datatype of setting value */
  DataType settingDataType;

  /* This represents group name of the setting */
  char *groupName;

  /* This represents group help text of the setting */
  char *groupHelpText;

  /*whether dependency setting is present or not*/
  bool isDepedentsetting;

  /* default dependent value when setting is disabled */
  void *dependentDefaultValue;

  /*PCsetting or not*/
  bool isPCsetting;

  /* child device setting or not */
  bool isChildDeviceSetting;

} SettingInfo;

/* This structure represents all settings available for the device */
typedef struct _DeviceSettings{
  /* number of settings for the device */
  unsigned int settingCount;
  /* Setting information of all settings of the device */
  SettingInfo *settingInfo;
  /* manifest file download status */
  Jabra_ErrorStatus errStatus;
} DeviceSettings;

/* This structure represents the product registration info*/
typedef struct _PrdctRegDetails{
  char *userName;
  char *userPassword;
  char *firstName;
  char *lastName;
  char *email;
  bool mktPermission;
} PrdctRegDetails;

/* This structure represents the failed settings info */
typedef struct _FailedSettings{
  unsigned int count;
  char ** settingNames;
}FailedSettings;

/* This enum represents the settings load mode for the API */
typedef enum  _SettingsLoadMode{
    expressMode = 0,
    retrieveMode
} SettingsLoadMode;

typedef enum _SettingFailType{
    guid = 0,
    value
} SettingFailType;

typedef enum _ValueType{
    success = 0,
    guidfail,
    valuefail
} ValueType;

typedef struct _InvalidInfo{
    char* guid;
    char* settingName;
    char* failMessage;
} InvalidInfo;

/* This structure represents the list of Invalid settings from the file/cloud*/
typedef struct _InvalidList{
    unsigned int invalidCount;
    char* fileDeviceName;
    InvalidInfo *invalidinfo;
    Jabra_ErrorStatus errStatus;
} InvalidList;

/* This structure represents the product registration info*/
typedef struct _ConfigInfo{
    char *configName;
    char *configId;
} ConfigInfo;

/* This structure represents the list of configurations available from the cloud*/
typedef struct _ConfigList{
    /* no of setting for the device */
    unsigned int configCount;
    /*Config Info*/
    ConfigInfo *configinfo;
    /* config file list download status */
    Jabra_ErrorStatus errStatus;
} ConfigList;

/**
 * Metadata for an asset. Read as name->Value pairs, actual values may vary - see developer documentation
 */
typedef struct _CAssetMetadata
{
    char* name;
    char* value;
} CAssetMetadata;

/**
 * An asset element (assets may be composed of 1 or or more of these)
 */
typedef struct _CAssetElement
{
    char* url;
    char* mime;
}CAssetElement;

/**
 * A named asset, requestable by Jabra_GetNamedAsset()
 */
typedef struct _CNamedAsset
{
    CAssetMetadata* metadata;
    unsigned metadata_count; // number of items in metadata
    CAssetElement* elements;
    unsigned element_count; // number of items in elements
} CNamedAsset;


/****************************************************************************/
/*                          EXPORTED DYNAMIC CONFIG - APIs                           */
/****************************************************************************/
/** Note The proposed design is to have the memory allocated at the SDK level as
 *  the user may not aware of how much memory needs to be allocated. Settings vary up on connected devices.
 *  So it is good SDK to decide how much memory to allocate for settings.

 *  E.g.  openSSL library
 */

/**
 * Gets the unique setting identified by a GUID of a device.
 * @param[in] deviceID device ID of the device to read/get the setting from.
 * @param[in] guid the unique setting identifier, the string must be zero
 * terminated.
 * @return pointer to the structure containing the requested setting. In case
 * the device is not found or could not be accessed a NULL pointer is returned.
 * @note As Memory is allocated through the SDK it needs to be freed by calling
 * the Jabra_FreeDeviceSettings API.
 */
LIBRARY_API DeviceSettings* Jabra_GetSetting(unsigned short deviceID, const char* guid);

/** Gets the complete settings details (all groups and its settings) for a device.
 *  @param[in] : deviceID: Id for specific device.
 *  @return    : pointer to the structure containing all settings for the deviceID. In case device is not found/ could not be access, NULL pointer is returned.
 *  Note       : As Memory is allocated through SDK, need to be freed by calling Jabra_FreeDeviceSettings API.
 */
LIBRARY_API DeviceSettings* Jabra_GetSettings(unsigned short deviceID);

/** Sets all the settings( including all groups and its settings) for a device.
 *  @param[in] : deviceID : id for a specific device.
 *  @param[in] : setting : Dynamic settings for the device.
 *  @return    : Return_Ok if setting is successful.
				 Device_Unknown if deviceID is wrong.
				 Device_NotLock if device is not locked.
         Return_ParameterFail if setting parameter is wrong.
         Device_Rebooted if the device rebooted after applying settings that required rebooting.
         Device_WriteFail if it fails to write to the device.
 */
LIBRARY_API Jabra_ReturnCode Jabra_SetSettings(unsigned short deviceID, DeviceSettings* setting);

/** Restore factory settings to device.
 *  @param[in] : deviceID: id for a specific device.
 *  @param[in] : bluetooth: flag to identify, whether called from BT pairing or not.
 *  @return    : Return_Ok if successful.
				 Device_Unknown if deviceID is wrong.
				 Device_NotLock if device is not locked.
				 No_FactorySupported if device does not support factory reset.
				 Device_WriteFail if it fails to write to the device.
				 ProtectedSetting_Write if a setting is write protected
 */
LIBRARY_API Jabra_ReturnCode Jabra_FactoryReset(unsigned short deviceID, bool bluetooth);

/** Free DeviceSettings structure
 *  @param[in] : setting: DeviceSettings structure pointer, which needs to be freed.
 *  @return    : void.
 */
LIBRARY_API void Jabra_FreeDeviceSettings(DeviceSettings* setting);

/** Free char pointer
 *  @param[in] : strPtr: char pointer, which needs to be freed.
 *  @return    : void.
 */
LIBRARY_API void Jabra_FreeString(char* strPtr);

/** Saves the device settings to a local file.
 *  @param[in] : deviceID : id for a specific device
 *  @param[in] : filePath : path to local settings file
 *  @return    : Return_Ok if saving the setting to file is successful
 *               Device_Unknown if deviceID is wrong
 *               Return_ParameterFail if setting parameter is wrong
 *               FileWrite_Fail if accessing the file fails
 */
LIBRARY_API Jabra_ReturnCode Jabra_SaveSettingsToFile(unsigned short deviceID, const char* filePath);

/** Loads the device settings from local file.
 *  @param[in] : deviceID : id for a specific device
 *  @param[in] : filePath : path to local settings file
 *  @param[in] : mode : mode to be used for updating the settings
 *  @return    : If success, returns valid data for DeviceSettings pointer and error status as NoError
 *               else, returns valid pointer but with proper error status updated.
 *               Parameter_fail, if parameters are incorrect
 *               FileNotAccessible, if unable to access the file
 *               Device_NotFound if device id passed is incorrect or device is not found
 *               FileNotCompatible if none of the GUIDs/Settings are matching with device
 *               ParseError if there are XML parsing issue of file
 *  Note       : As Memory is allocated through SDK, need to be freed by calling Jabra_FreeDeviceSettings API.
 */
LIBRARY_API DeviceSettings* Jabra_LoadSettingsFromFile(unsigned short deviceID, const char* filePath, SettingsLoadMode mode);

/** Saves the device settings to cloud.
 *  @param[in] : deviceID : id for a specific device
 *  @param[in] : filePath : path to local settings file
 *  @return    : Return_Ok if saving the setting to file is successful
 *               Device_Unknown if deviceID is wrong
 *               Return_ParameterFail if setting parameter is wrong
 *               NetworkRequest_Fail if there is network issue
 *               File_AlreadyExists if file with same name already exists
 */
LIBRARY_API Jabra_ReturnCode Jabra_SaveSettingsToCloud(unsigned short deviceID, const char* authorization, const char* configName);

/** This method gets the list of configs for given authorisations.
 *  @param[in] : authorisation string for which the configs are needed
 *  @return    : If success, returns valid data for ConfigList pointer and error status as NoError
 *               else, returns valid pointer but with proper error status updated.
 *               NetworkError incase of network issue
 *               ParseError if data is downloaded but there are XML parse errors
 *               Parameter_fail if input parameters are incorrect
 *  Note       : As Memory is allocated through SDK, need to be freed by calling Jabra_FreeConfigList API.
 */
LIBRARY_API ConfigList* Jabra_GetCloudListOfConfigs(const char* authorization);

/** This method free the config list info.
 *  @param[in] : pConfigList pointer to config list to be cleared
 *  @return    : No return parameters(Void function)
 */
LIBRARY_API void Jabra_FreeConfigList(ConfigList* pConfigList);

/** This method free the invalid setting list info.
 *  @param[in] : pInvalidList pointer to invalid list to be cleared
 *  @return    : No return parameters(Void function)
 */
LIBRARY_API void Jabra_FreeInvalidList(InvalidList* pInvalidList);

/** Loads the device settings from local cloud.
 *  @param[in] : deviceID : id for a specific device
 *  @param[in] : authorization : authorization id
 *  @param[in] : configID : id of the config file that needs to be loaded
 *  @param[in] : mode : mode to be used for updating the settings
 *  @return    : If success, returns valid data for DeviceSettings pointer and error status as NoError
 *               else, returns valid pointer but with proper error status updated.
 *               Parameter_fail, if parameters are incorrect
 *               NetworkError incase of network issue
 *               Device_NotFound if device id passed is incorrect or device is not found
 *               FileNotCompatible if none of the GUIDs/Settings are matching with device
 *  Note       : As Memory is allocated through SDK, need to be freed by calling Jabra_FreeDeviceSettings API.
 */
LIBRARY_API DeviceSettings* Jabra_LoadSettingsFromCloud(unsigned short deviceID, const char* authorization, const char* configID, SettingsLoadMode mode);

/** Updates the settings data of a cloud file already present in cloud.
 *  @param[in] : deviceID : id of device from which settings needs to be updated
 *  @param[in] : authorization : authorization id
 *  @param[in] : configID : id of the config file that needs to be updated
 *  @param[in] : configName : Name of the config file to be used for update
 *  @return    : Return_Ok if saving the setting to file is successful
 *               Device_Unknown if deviceID is wrong
 *               NetworkRequest_Fail if there is network issue
 *               Return_ParameterFail if setting parameter is wrong
 */
LIBRARY_API Jabra_ReturnCode Jabra_UpdateSettingsOfCloud(unsigned short deviceID, const char* authorization, const char* configID, const char* configName);

/** Deletes the settings file already present in cloud.
 *  @param[in] : authorization : authorization id
 *  @param[in] : configID : id of the config file that needs to be updated
 *  @return    : Return_Ok if saving the setting to file is successful
 *               NetworkRequest_Fail if there is network issue
 *               Return_ParameterFail if setting parameter is wrong
 */
LIBRARY_API Jabra_ReturnCode Jabra_DeleteSettingsOfCloud(const char* authorization, const char* configID);

/**
 Returns error description for the error code
 *  @param[in] : deviceID: Id for specific device
 *  @return    : pointer to FailedSettings if one or more settings are failed while writing to device,
				 Null Pointer if all settings are written successfully.
 *  Note: This API should be called if Jabra_SetSettings does not return Return_Ok.
	As Memory is allocated through SDK, needs to be freed by calling Jabra_FreeFailedSettings API.
 */
LIBRARY_API FailedSettings* Jabra_GetFailedSettingNames(unsigned short deviceID);

/** Free FailedSettings structure
 *  @param[in] : FailedSettings structure pointer, which needs to be freed.
 *  @return    : void.
 */
LIBRARY_API void Jabra_FreeFailedSettings(FailedSettings* setting);

/** Checks if supports factory reset.
 *  @param[in] : deviceID: id for a specific device.
 *  @param[in] : bluetooth: flag to identify, whether called from BT pairing or not.
 *  @return    : true if device supports factory reset.
				 false if device does not support factory reset.
 */
LIBRARY_API bool Jabra_IsFactoryResetSupported(unsigned short deviceID, bool bluetooth);

/** Get list of invlaid settings
 *  @param[in] : deviceID
 *  @return    : InvalidListb.
 */

LIBRARY_API InvalidList* Jabra_GetInvalidSettings(unsigned short deviceID);


/**
 * Request a named asset for the specified device
 * @param deviceID - the device
 * @param name - the name of the requested asset - see developer doc for a catalogue of possibly available assets. Note that availability may vary across devices!
 * @param asset - address of a pointer to the returned asset. Caller must free the allocated asset by calling Jabra_FreeAsset
 * @return Return_Ok if a valid asset was available (is then available through (*asset)->...)
 */
LIBRARY_API Jabra_ReturnCode Jabra_GetNamedAsset(unsigned short deviceID, const char* name, CNamedAsset** asset);

/**
 * Release an asset previously obtained with Jabra_GetNamedAsset()
 * @param asset - the asset to release.
 */
LIBRARY_API void Jabra_FreeAsset(CNamedAsset* asset);

#endif /* JABRADEVICECONFIG_H */
