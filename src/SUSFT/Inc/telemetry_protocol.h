/**
 * @brief  Frame Header
 * @note 
 * @retval None  
 */

typedef struct header_s
{
    unsigned char   start_byte;
    enum            frame_type;
    unsigned int    epoch;
} header_t;

/**
 * @brief  Application Core PDU (ACPDU)
 * @note
 * @retval None   
 */

typedef struct ACPDU_s
{
    header_t  header;
    short   rpm;
    short   water_temp_c;
    short   tps_perc;
    short   battery_mv;
    short   external_5v_mv;
    short   fuel_flow;
    short   lambda;
    short   speed_kph;
} ACPDU_t;

/**
 * @brief  Application Aero PDU (AAPDU)
 * @note   
 * @retval None
 */

typedef struct AAPDU_s
{
    header_t  header;
    short   evo_scanner1;
    short   evo_scanner2;
    short   evo_scanner3;
    short   evo_scanner4;
    short   evo_scanner5;
    short   evo_scanner6;
    short   evo_scanner7;
} AAPDU_t;

/**
 * @brief  Application Diagnostic PDU (ADPDU)
 * @note   
 * @retval None
 */

typedef struct ADPDU_s
{
    header_t  header;
    enum    ECU_status;
    enum    engine_status;
    enum    battery_status;
    enum    car_logging_status;
} ADPDU_t;

/**
 * @brief  Application Power-Train PDU (APPDU)
 * @note   
 * @retval None
 */

typedef struct APPDU_s
{
    header_t  header;
    short   injection_time;
    short   injection_duty_cycle;
    short   lambda_pid_adjust;
    short   lambda_pid_target;
    short   advance;
} APPDU_t;

/**
 * @brief  Application Suspension PDU (ASPDU)
 * @note   
 * @retval None
 */

typedef struct ASPDU_s
{
    header_t  header;
    short   ride_height_fl_cm;
    short   ride_height_fr_cm;
    short   ride_height_flw_cm;
    short   ride_height_rear_cm;
} ASPDU_t;

/**
 * @brief  Application Misc PDU (AMPDU)
 * @note   
 * @retval None
 */

typedef struct AMPDU_s
{
    header_t  header;
    int     lap_timer_s;
    short   accel_fl_x_mg; 
    short   accel_fl_y_mg; 
    short   accel_fl_z_mg; 
} AMPDU_t;

