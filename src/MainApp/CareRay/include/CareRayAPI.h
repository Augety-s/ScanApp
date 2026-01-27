#pragma once

#ifndef __CARERAY_API_H__
#define __CARERAY_API_H__

#include "CrStructure.h"
#include "IntlMsgAliasID.h"

#ifdef __cplusplus
extern "C" {
#endif

#if (defined WIN32 || defined _WIN32 || defined _W64 || defined _WIN64 || defined WINCE)

#ifdef _CARERAY_API_
#define CARERAY_API_PORT __declspec(dllexport)
#else
#define CARERAY_API_PORT __declspec(dllimport)
#endif

#else
#define CARERAY_API_PORT
#endif
	
	// ************************ Common API ************************ //
	CARERAY_API_PORT int CrInitializeLibrary();
	CARERAY_API_PORT int CrDeinitializeLibrary();
	CARERAY_API_PORT void CrGetLastIntlMsg(char* message_buffer, int buffer_length);
	CARERAY_API_PORT int CrRegisterEventCallbackFun(CrCallbackFunPtr callback_fun);
	CARERAY_API_PORT int CrGetConfigItemValue(const char *name, char *value);
	CARERAY_API_PORT int CrSetConfigItemValue(const char *name, const char *value);
	CARERAY_API_PORT int CrGetDetectorIndexAndIPAddress(CrDetrIdxAndIPAddr* detr_idx_and_ip_array, int* valid_detr_num);
	CARERAY_API_PORT int CrConnect(int detr_index);
	CARERAY_API_PORT int CrDisconnect(int detr_index);
	CARERAY_API_PORT int CrSwitchNetworkConnection(int detr_index);
	CARERAY_API_PORT int CrResetDetector(int detr_index, int need_reboot);
	CARERAY_API_PORT int CrGetSystemInformation(int detr_index, CrSystemInfo* system_info);
	CARERAY_API_PORT int CrGetApplicationMode(int detr_index, CrModeInfo* mode_array, int* valid_mode_num);
	// The row_index and column_index parameters are only used for panoramic mode, please assign 0 in other modes.
	CARERAY_API_PORT int CrRegisterApplicationMode(int detr_index, int app_mode_key, int mode_id, float *frame_rate, float *exposure_time, int trigger_type, int gain_level, int row_index, int column_index);
	CARERAY_API_PORT int CrSetFrameFilter(int detr_index, int app_mode_key, const char *frame_filter);
	CARERAY_API_PORT int CrSetRoi(int detr_index, int app_mode_key, int x, int y, int width, int height);
	CARERAY_API_PORT int CrGetModeInfoByAppModeKey(int detr_index, int app_mode_key, CrModeInfo *mode_info);
	CARERAY_API_PORT int CrGetModeInfoByModeId(int detr_index, int mode_id, CrModeInfo *mode_info);
	CARERAY_API_PORT int CrLoadReference(int detr_index, int app_mode_key);
	CARERAY_API_PORT int CrUnloadReference(int detr_index, int app_mode_key);
	CARERAY_API_PORT int CrStartAcquisition(int detr_index, int app_mode_key, int transfer_images_by_callback);
	CARERAY_API_PORT int CrStartAcquisitionWithCorrOpt(int detr_index, int app_mode_key, unsigned int correct_options, int transfer_images_by_callback);
	// Only for rad
	CARERAY_API_PORT int CrStartDarkAcquisition(int detr_index, int app_mode_key);
	// Only for rad
	CARERAY_API_PORT int CrStartDarkAcquisitionWithCorrOpt(int detr_index, int app_mode_key, unsigned int correct_options);
	CARERAY_API_PORT int CrStopAcquisition(int detr_index);
	CARERAY_API_PORT int CrGetAcquisitionStatInfo(int detr_index, CrAcquisitionStatInfo* status);
	CARERAY_API_PORT int CrQueryAcquisitionStatus(int detr_index, CrExpProgress* exposure_progress);
	CARERAY_API_PORT int CrQueryAedExposureProgress(int detr_index, CrAedExpProgress* exposure_progress);
	CARERAY_API_PORT int CrRequestExposure(int detr_index);
	CARERAY_API_PORT int CrPermitExposure(int detr_index);
	// wait_milliseconds: recommended value is 3000
	CARERAY_API_PORT int CrGetImage(int detr_index, char* image_buffer, int buffer_length, int wait_milliseconds);
	CARERAY_API_PORT int CrGetNoHeaderImage(int detr_index, char* image_buffer, int buffer_length, int wait_milliseconds);
	CARERAY_API_PORT int CrGetPreviewImage(int detr_index, char* image_buffer, int buffer_length, int wait_milliseconds);
	CARERAY_API_PORT int CrClearCachedImages(int detr_index);
	// is_traditional_type: recommended value is 1, is_update_defect: recommended value is 0
	CARERAY_API_PORT int CrStartDarkCalibration(int detr_index, int app_mode_key, int is_traditional_type, int is_update_defect);
	CARERAY_API_PORT int CrStartGainCalibration(int detr_index, int app_mode_key);
	CARERAY_API_PORT int CrStopCalibration(int detr_index);
	CARERAY_API_PORT int CrQueryCalibrationStatus(int detr_index, CrCalibrationInfo* calib_progress);
	CARERAY_API_PORT int CrQueryReferenceStatus(int detr_index, int app_mode_key, CrRefStatusInfo* reference_status);
	CARERAY_API_PORT int CrGetDetrStatus(int detr_index, CrDetrStatus* status);
	CARERAY_API_PORT int CrGetConnectionStatus(int detr_index, CrConnectionStatus *status);
	CARERAY_API_PORT int CrGetDaecActiveAreas(int detr_index, CrDaecAreaInfo *active_areas_array, int *active_areas_num, int *threshold, int app_mode_key);
	CARERAY_API_PORT int CrSetDaecActiveAreas(int detr_index, CrDaecAreaInfo *active_areas_array, int active_areas_num, int threshold, int app_mode_key);
	CARERAY_API_PORT int CrForwardMsgViaDetr(int detr_index, void *byte_array, int array_length);
	CARERAY_API_PORT int CrSetActiveDetr(int detr_index);
	CARERAY_API_PORT int CrGetDefectImage(int detr_index, int app_mode_key, int frame_position, unsigned char* image_buffer, int buffer_length);
	CARERAY_API_PORT int CrAddBadLine(int detr_index, int app_mode_key, int frame_position, int *bad_line_array, int total_line_num, int horizontal_flag);
	CARERAY_API_PORT int CrDeleteBadLine(int detr_index, int app_mode_key, int frame_position, int *bad_line_array, int total_line_num, int horizontal_flag);
	CARERAY_API_PORT int CrAddBadPixels(int detr_index, int app_mode_key, int frame_position, const CrPixel *bad_pixel_array, int bad_pixels_num);
	CARERAY_API_PORT int CrDeleteBadPixels(int detr_index, int app_mode_key, int frame_position, const CrPixel *bad_pixel_array, int bad_pixels_num);
	CARERAY_API_PORT int CrRepairImage(int detr_index, int app_mode_key, int frame_position, unsigned short *pixel_array);
	CARERAY_API_PORT int CrGetSuperResolutionImage(unsigned short *dest_pixels, int dest_size, int *dest_width, int *dest_height, unsigned short *src_pixels, int src_width, int src_height);

	// ********************* YosemiteView API ********************* //
	CARERAY_API_PORT int CrNudge(int detr_index);
	CARERAY_API_PORT int CrSetWorklist(int detr_index, CrExamination* examinations, int total_exam_num);
	CARERAY_API_PORT int CrGetWorklist(int detr_index, int* total_exam_num, CrExamination* examinations);
	CARERAY_API_PORT int CrClearWorklist(int detr_index);
	CARERAY_API_PORT int CrGetStoredImage(int detr_index, char* image_buffer, int buffer_length, const char* file_name, int wait_milliseconds);
	CARERAY_API_PORT int CrGetInnerStoredFileList(int detr_index, char** inner_file_name_list, int* inner_file_num);
	CARERAY_API_PORT int CrSetPowerMode(int detr_index, CrPowerModeId mode_id);
	CARERAY_API_PORT int CrGetBatteryInfos(int detr_index, CrBatteryInfo* battery_array, int* valid_battery_num);
	CARERAY_API_PORT int CrGetWirelessStatus(int detr_index, CrWirelessStatus* wireless_status);
	CARERAY_API_PORT int CrStopAcquisitionWithoutPowerSwitching(int detr_index);
	CARERAY_API_PORT int CrStartPairingDaecBox(int detr_index, unsigned int daec_box_id);
	CARERAY_API_PORT int CrStopPairingDaecBox(int detr_index);
	// progress: -1 in pairing; 0 paired successfully; 1 paired failed
	CARERAY_API_PORT int CrGetDaecBoxPairingProgress(int detr_index, int *progress);
	CARERAY_API_PORT int CrGetDaecBoxId(int detr_index, unsigned int *daec_box_id);
	// delay_mins: 0 immediately; > 0 delay time
	CARERAY_API_PORT int CrSetPoweroffDelay(int detr_index, int delay_mins);
	CARERAY_API_PORT int CrGetPoweroffDelay(int detr_index, int *delay_mins);
	// rotate flag: 0 do not rotate; 1 rorate 180°
	CARERAY_API_PORT int CrSetDaecRotateFlag(int detr_index, int rotate_flag);

	// ************************* JSON API ************************* //
	// New interface: simplify acquisition and calibration
	// Get all support application modes in JSON format
	CARERAY_API_PORT void CrGetApplicationModeInJson(int detr_index, char *dest_string_buffer);
	// Register custom modes from configuration file
	CARERAY_API_PORT void CrRegisterCustomModeFromConfigFile(int detr_index, char *result_string_buffer);
	// Generate sample configuration file for custom modes
	CARERAY_API_PORT void CrGenerateSampleCustomModeConfigFile(int detr_index);
	// Get acquisition progress in JSON format - rad mode only
	CARERAY_API_PORT void CrGetAcquisitionProgressInJson(int detr_index, char *dest_string_buffer);
	// Get calibration progress in JSON format
	CARERAY_API_PORT void CrGetCalibrationProgressInJson(int detr_index, char *dest_string_buffer);
	CARERAY_API_PORT void CrGetModeInfoByAppModeKeyInJson(int detr_index, int app_mode_key, char *result_string_buffer);
	CARERAY_API_PORT void CrGetRegedInfoByAppModeKeyInJson(int detr_index, int app_mode_key, char *result_string_buffer);

#ifdef __cplusplus
}
#endif

#endif
