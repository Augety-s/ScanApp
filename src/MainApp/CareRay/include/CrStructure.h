#ifndef __CR_STRUCTURE_H__	
#define __CR_STRUCTURE_H__

#ifdef __cplusplus
extern "C" {
#endif

#define CR_DIR_TO_SAVE_CAL_FILES    "DirToSaveCalFiles"

//*****************************************************************************************
// enum starts here.
//*****************************************************************************************

enum CrTrigType
{
	CR_RAD_EXT_SYNC        = 0x01,
	CR_RAD_SOFT_SYNC       = 0x02,
	CR_RAD_AUTO_SYNC       = 0x03,
	CR_RAD_MANUAL_SYNC     = 0x04,
	CR_RAD_AED_SYNC        = 0x05,
	CR_RAD_DAEC_SYNC       = 0x06,
	CR_RAD_DAEC_FLIP_SYNC  = 0x07,
	CR_FLUORO_EXT_SYNC     = 0x08,
	CR_FLUORO_INT_SYNC     = 0x09,
};

enum CrGainLevel
{
	CR_G0 = 0,
	CR_G1 = 1,
	CR_G2 = 2,
	CR_G3 = 3,
	CR_G4 = 4,
	CR_G5 = 5,
	CR_G6 = 6,
	CR_G7 = 7,
};

enum CrPowerModeId
{
	CR_PWR_STANDBY       = 0, //set front-end into nap mode
	CR_PWR_FULL_RUNNING  = 1, //set front-end into normal mode.
	CR_PWR_SMART_RUNNING = 2, //set front-end into nap mode at the integration phase.
	CR_PWR_DOWN_FE       = 4, //set front-end into power-down mode.               
	CR_PWR_SLEEPING      = 5,
	CR_PWR_DEEP_SLEEPING = 6,
	CR_PWR_SUSPEND       = 7, // power down
};

enum CrEventId
{
	CR_EVT_DETR_DISCONNECTED       = 1,
	CR_EVT_TEMPERATURE_INFO        = 3,
	CR_EVT_BATTERY_INFO            = 4,
	CR_EVT_WIRELESS_INFO           = 5,
	CR_EVT_NEW_FRAME               = 6,
	CR_EVT_CALIBRATION_IN_PROGRESS = 7,
	CR_EVT_CALIBRATION_FINISHED    = 8,
	CR_EVT_ACQ_STAT_INFO           = 10,
	CR_EVT_RAD_ACQ_IN_PROGRESS     = 11,
	CR_EVT_DETR_RECONNECTED        = 13,
	CR_EVT_DISCARD_FRAME           = 15,
	CR_EVT_DAEC_BOX_INFO           = 18,
};

enum CrExpStatus
{
	CR_EXP_ERROR = -1,
	CR_EXP_INIT,
	CR_EXP_READY,
	CR_EXP_WAIT_PERMISSION,
	CR_EXP_PERMITTED,
	CR_EXP_EXPOSE,
	CR_EXP_COMPLETE,
};

enum CrReturnCodeType
{
	CR_INFOR = 1,
	CR_WARN,
	CR_ERROR,
	CR_FATALERROR
};

enum CrProcChainOpt
{
	CR_PROCCHAIN_SANITYCHECK    = 0x01,
	CR_PROCCHAIN_DARKCORR       = 0x02,
	CR_PROCCHAIN_GAINCORR       = 0x04,
	CR_PROCCHAIN_DEFECTCORR     = 0x08,
	CR_PROCCHAIN_LAGCORR        = 0x10,
	CR_PROCCHAIN_IMGCROPPING    = 0x20,
	CR_PROCCHAIN_RTPIXELCORR    = 0x40,
	CR_PROCCHAIN_DENOISING      = 0x80,
	CR_PROCCHAIN_ROTATE_IAMGE   = 0x200,
	CR_PROCCHAIN_FLIP_IMAGE     = 0x80000,
};

//*****************************************************************************************
// struct starts here.
//*****************************************************************************************

typedef struct CrSystemInfo
{
	unsigned int raw_img_width;
	unsigned int raw_img_height;
	unsigned int frame_header_len;
	char hardware_version[64];
	char serial_number[64];
	char software_version[64];
	char firmware_version[64];
	char detr_machine_id[64];
	char detr_desc[128];
	char reserved[512];
}CrSystemInfo;

typedef struct CrDefectInfo
{
	unsigned int bad_pixel_num;
	unsigned int bad_raw_num;
	unsigned int bad_col_num;
	unsigned short *bad_pixel;
	unsigned short *bad_row;
	unsigned short *bad_col;
}CrDefectInfo;

typedef struct CrConnectionStatus
{
	int server_connection_status;
	int detr_connection_status;
}CrConnectionStatus;

typedef struct CrTemperature
{
	float min;
	float cpu;
	float wired_network;
	float reserved;
	float current;
	float max;
	float avg;
	int overheat;
}CrTemperature;

typedef struct CrDetrStatus
{
	CrTemperature detr_temperature;
}CrDetrStatus;

typedef struct CrDetrIdxAndIPAddr
{
	int index;
	char ip_addr[64];
	int device_type;
	char reserved[124];
}CrDetrIdxAndIPAddr;

typedef struct CrRegAttrInfo
{
	int	         is_writable;
	int          is_float_type;
	unsigned int int_value;
	float        float_value;
	char         property_name[32];
}CrRegAttrInfo;

typedef struct CrRegModeInfo
{
};

typedef struct CrModeInfo
{
	int mode_id;
	int image_width;         // ROI dimension along x direction after binning
	int image_height;        // ROI dimension along y direction after binning
	int cutoff_x;            // number of cut-off lines (along x direction) from left after binning
	int cutoff_y;            // number of cut-off lines (along y direction) from top after binning
	int bin_x;               // binning scheme along x direction
	int bin_y;               // binning scheme along y direction
	float max_frame_rate;    // in Hz
	float max_exposure_time; // in ms
	int pixel_depth;         // bit depth of each pixel sending out from detector
	int trigger_types[16];
	int valid_trigger_type_num;
	int gain_levels[16];
	int valid_gain_num;
	int default_trigger_type;
	int default_gain_level;
	int roi_x;               // x coordinate of top left point after binning
	int roi_y;               // y coordinate of top left point after binning
	int mode_conf;
	char desc[256];          // description of this mode
}CrModeInfo;

typedef struct CrExtendModeInfo
{
	float readout_time;
	int	block_length;
	int bin_mode;
	int roi_type;
	int acquire_type;
	unsigned char cut_low_bits[16];
	unsigned char cut_high_bits[16];
}CrExtendModeInfo;

typedef struct CrCalibrationInfo
{
	int total_frame_num;
	int current_frame_num;
	int current_frame_mean;
	char error_msg[256];
	int result;
	int exposure_status;
	int target_gray_value;
}CrCalibrationInfo;

typedef struct CrEvent
{
	int event_id;
	int detr_index;
	int width;
	int height;
	int pixel_depth;
	int header_len;
	void* data;
}CrEvent;

typedef struct CrAcquisitionStatInfo
{
	int total_frame_num;
	int lost_frame_num;
	float frame_rate;
	float transmission_speed;
	long long duration_milliseconds;
	float fpga_frame_rate;
}CrAcquisitionStatInfo;

typedef struct CrExpProgress
{
	int exposure_status;
	int is_fetchable;
	int result;
}CrExpProgress;

typedef struct CrAedExpProgress
{
	CrExpProgress progress;
	float aed_elapsed_time_since_last_readout;
	float aed_refresh_interval;
}CrAedExpProgress;

//Reference status and expiration duration (unit: minute) of special application mode
typedef struct CrRefStatusInfo
{
	int dark_status;
	int gain_status;
	int defect_status;
	int dark_left_minutes;
	int gain_left_minutes;
	int defect_left_minutes;
}CrRefStatusInfo;

typedef struct CrDaecAreaInfo
{
	int x;
	int y;
	int width;
	int height;
	float weight;
}CrDaecAreaInfo;

typedef struct CrCheckInfo
{
	char id[128];
	char body_part_id[32];
	char series_time[32];
}CrCheckInfo;

typedef struct CrBodyPartInfo
{
	char id[32];
	char part_name[32];
}CrBodyPartInfo;

typedef struct CrExamination
{
	char id[16];
	char checklist_number[64];
	char patient_id[64];
	char patient_name[64];
	char patient_sex[8];
	char patient_age[4];
	int body_part_num;
	int check_info_num;
	CrBodyPartInfo body_parts[16];
	CrCheckInfo check_infos[32];
}CrExamination;

typedef struct CrBatteryInfo
{
	unsigned char  id;                      //battery id
	unsigned char  accessible;              //is battery accessible
	unsigned short remaining_capacity;      //unit: mAh
	unsigned short full_capacity;           //unit: mAh
	unsigned short voltage;                 //unit: mV
	short          current;                 //unit: mA
	unsigned short charging_current;        //unit: mA
	unsigned short run_time_to_empty;       //unit: min
	unsigned short avg_time_to_empty;       //unit: min
	unsigned short avg_time_to_full;        //unit: min
	unsigned short cycle_count;             //unit: cycles
	unsigned short temperature;             //unit: 0.1K
	unsigned short relative_state_of_charge;//relative capacity, unit: %
	unsigned short absolute_state_of_charge;//relative capacity, unit: %
	char           is_power_supply;         //1: powered by battery; 0: powered by DC; -1: unknown
	char           is_low_battery;          //1: low; 0: normal; -1: unknown
	unsigned char  reserved[12];
}CrBatteryInfo;

typedef struct CrWirelessStatus
{
    char essid[32];
    char passwd[32];
    char protocol[16];
    char desc[32];
    int mode;
    int channel;
    double frequency;
    int bit_rate;
    int status;
    int quality;
    int level;
    int noise;
    unsigned int ipv4;
    int factory_flag;
}CrWirelessStatus;

struct CrPixel
{
	int row_index;
	int col_index;
};

typedef void (* CrCallbackFunPtr)(int, CrEvent *);

#ifdef __cplusplus
}
#endif

#endif