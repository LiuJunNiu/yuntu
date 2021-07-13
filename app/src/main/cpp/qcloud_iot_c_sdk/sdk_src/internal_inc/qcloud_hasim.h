/*
 * Copyright (c) 2021 Tencent Cloud. All rights reserved.

 * Licensed under the MIT License (the "License"); you may not use this file
 except in
 * compliance with the License. You may obtain a copy of the License at
 * http://opensource.org/licenses/MIT

 * Unless required by applicable law or agreed to in writing, software
 distributed under the License is
 * distributed on an "AS IS" basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 KIND,
 * either express or implied. See the License for the specific language
 governing permissions and
 * limitations under the License.
 *
 */

#ifndef _QCLOUD_HASIM_H_
#define _QCLOUD_HASIM_H_

#ifdef __cplusplus
extern "C" {
#endif

#define QCLOUD_HASIM_OPERATOR_SIGNAL_CHECK_COUNT 8
#define QCLOUD_HASIM_OPERATOR_MOBILE_CHECK_COUNT 10

#define QCLOUD_HASIM_OPERATOR_PING_DELAY_CHECK_COUNT 8

#define QCLOUD_HASIM_LAC_LEN    4
#define QCLOUD_HASIM_CELLID_LEN 8
#define QCLOUD_HASIM_GPS_X_LEN  10
#define QCLOUD_HASIM_GPS_Y_LEN  9

#define QCLOUD_HASIM_IMEI_LEN  17
#define QCLOUD_HASIM_IMSI_LEN  15
#define QCLOUD_HASIM_ICCID_LEN 20

#define QCLOUD_HASIM_CLOUD_CONFIG_DEFAULT_HEARTBEAT_INTERVAL 30
#define QCLOUD_HASIM_CLOUD_CONFIG_DEFAULT_WEAK_SIGNAL        -1
#define QCLOUD_HASIM_CLOUD_CONFIG_DEFAULT_PING_DELAYVALUE    -1
#define QCLOUD_HASIM_CLOUD_CONFIG_DEFAULT_PING_INTERVAL_TIME 10
#define QCLOUD_HASIM_CLOUD_CONFIG_DEFAULT_PING_LOSTNUM       -1
#define QCLOUD_HASIM_CLOUD_CONFIG_DEFAULT_FIRST_OPERATOR     -1
#define QCLOUD_HASIM_CLOUD_CONFIG_DEFAULT_LAST_OPERATOR      -1
#define QCLOUD_HASIM_CLOUD_CONFIG_DEFAULT_DONTUSE_NET_MODE   -1
#define QCLOUD_HASIM_CLOUD_CONFIG_DEFAULT_MOVESCEN_CHECK     true
#define QCLOUD_HASIM_CLOUD_CONFIG_DEFAULT_AUTOSWITCH_CHECK   true
#define QCLOUD_HASIM_CLOUD_CONFIG_DEFAULT_BEST_SIGNAL_CHECK  false
#define QCLOUD_HASIM_CLOUD_CONFIG_DEFAULT_BEST_MODE_CHECK    false

// struct net manage
typedef struct qcloud_hasim_net_manage {
    char imsi[QCLOUD_HASIM_IMSI_LEN + 2];
    char iccid[QCLOUD_HASIM_ICCID_LEN + 2];
    int  rssi;
    int  ping_delay_time;
    int  ping_continuous_noreply_curr_quantity;
    char lac[QCLOUD_HASIM_LAC_LEN + 2];
    char cellid[QCLOUD_HASIM_CELLID_LEN + 2];
    char gps_x[QCLOUD_HASIM_GPS_X_LEN + 2];
    char gps_y[QCLOUD_HASIM_GPS_Y_LEN + 2];

    int signal_statistics[QCLOUD_HASIM_OPERATOR_INDEX_MAX_E];
    int signal_statistics_curr_quantity[QCLOUD_HASIM_OPERATOR_INDEX_MAX_E];

    char lac_statistics[QCLOUD_HASIM_OPERATOR_MOBILE_CHECK_COUNT]
                       [QCLOUD_HASIM_LAC_LEN + 2];
    char cellid_statistics[QCLOUD_HASIM_OPERATOR_MOBILE_CHECK_COUNT]
                          [QCLOUD_HASIM_CELLID_LEN + 12];
    int lac_cellid_statistics_curr_quantity;

    char gps_x_statistics[QCLOUD_HASIM_OPERATOR_MOBILE_CHECK_COUNT]
                         [QCLOUD_HASIM_GPS_X_LEN + 2];
    char gps_y_statistics[QCLOUD_HASIM_OPERATOR_MOBILE_CHECK_COUNT]
                         [QCLOUD_HASIM_GPS_Y_LEN + 2];
    int gps_xy_statistics_curr_quantity;

    int ping_delay_statistics[QCLOUD_HASIM_OPERATOR_INDEX_MAX_E];
    int ping_delay_statistics_curr_quantity[QCLOUD_HASIM_OPERATOR_INDEX_MAX_E];
    int cloud_heartbeat_time_left;
    QCLOUD_HASIM_OPERATOR_INDEX_E net_operator_index;
    QCLOUD_HASIM_NET_MODE_E       net_mode;
    int                           net_mode_get_quantity;
    bool                          net_unregister_start_timing;
    bool net_registered[QCLOUD_HASIM_OPERATOR_INDEX_MAX_E];
    bool net_ping_fail_start_timing;
    bool net_ping_reply[QCLOUD_HASIM_OPERATOR_INDEX_MAX_E];
    bool net_mode_available[QCLOUD_HASIM_OPERATOR_INDEX_MAX_E];
    bool net_ping_delay_low[QCLOUD_HASIM_OPERATOR_INDEX_MAX_E];
    bool net_weak_signal[QCLOUD_HASIM_OPERATOR_INDEX_MAX_E];
    bool net_move_scene;
    bool net_all_check_success;
    bool net_dialup;
} QCLOUD_HASIM_NET_MANAGE_T;

// struct card manage
typedef struct qcloud_hasim_card_manage {
    void *mqtt_client;
    char  imei[QCLOUD_HASIM_IMEI_LEN + 2];
    Timer timer_net_registered_check;
    int   switch_all_operator_unregister_times;
    bool  switched_operator[QCLOUD_HASIM_OPERATOR_INDEX_MAX_E];
    QCLOUD_HASIM_OPERATOR_INDEX_E curr_switch_operator_index;
    QCLOUD_HASIM_OPERATOR_INDEX_E remote_switch_operator_index;
    bool                          need_switch;
    bool                          remote_switch;
    bool                          remote_switch_reply;
    bool                          net_check_independent_thread;
    bool                          net_available;
    bool                          disable_optimization_strategy;
} QCLOUD_HASIM_CARD_MANAGE_T;

// struct user config
typedef struct qcloud_hasim_user_config {
    int net_register_state_query_interval; /* rssi cgreg iccid imsi net mode
                                              position data */
    int  net_unregister_max_monitor_time;
    int  net_ping_noreply_max_monitor_time;
    char net_operator_select_mask;
    bool disable_sdk_switch;
    bool net_check_use_independent_thread;
    bool is_old_card;
    bool enable_idle_sleep;
} QCLOUD_HASIM_USER_CONFIG_T;

// struct cloud config
#define QCLOUD_HASIM_CLOUD_CONFIG_MAX_HOST_LEN   32
#define QCLOUD_HASIM_CLOUD_CONFIG_VERSION_NUM    1
#define QCLOUD_HASIM_CLOUD_CONFIG_DEFAULT_CONFID 1

typedef struct qcloud_hasim_cloud_config {
    char sha1[20];
    int  version_num;
    int  config_id;
    int  cloud_heartbeat_interval;
    int  weak_signal_value;
    int  net_ping_delay_value;
    int  net_ping_interval;
    int  ping_continuous_noreply_totalnum;
    char net_ping_host[2][QCLOUD_HASIM_CLOUD_CONFIG_MAX_HOST_LEN + 2];
    QCLOUD_HASIM_OPERATOR_INDEX_E
                                  switch_operator_order[QCLOUD_HASIM_OPERATOR_INDEX_MAX_E];
    QCLOUD_HASIM_OPERATOR_INDEX_E poweron_love_net_operator;
    QCLOUD_HASIM_OPERATOR_INDEX_E poweron_hate_net_operator;
    char                          net_mode_dont_use;
    bool                          poweron_best_signal_enable;
    bool                          move_scene;
    bool                          local_auto_switch_enable;
} QCLOUD_HASIM_CLOUD_CONFIG_T;

// struct hasim timer
typedef struct qcloud_hasim_timer {
    Timer timer_cloud_heartbeat;
    Timer timer_net_ping_montior;
    Timer timer_net_ping_interval;
    Timer timer_net_resigter_query_interval;
    Timer timer_net_resigter_montior;
} QCLOUD_HASIM_TIMER_T;

extern QCLOUD_HASIM_NET_MANAGE_T g_qcloud_hasim_net_manage;

extern QCLOUD_HASIM_CARD_MANAGE_T g_qcloud_hasim_card_manage;

extern QCLOUD_HASIM_CLOUD_CONFIG_T g_qcloud_hasim_cloud_config;

extern QCLOUD_HASIM_USER_CONFIG_T g_qcloud_hasim_user_config;

extern QCLOUD_HASIM_TIMER_T g_qcloud_hasim_timer;

extern char *g_qcloud_hasim_set_poweron_cmd[];

int QCLOUD_HASIM_cloud_init(void);

int QCLOUD_HASIM_cloud_recv(void);

void QCLOUD_HASIM_cloud_fini(void);

void QCLOUD_HASIM_report_weak_signal_status(bool weak_signal);

void QCLOUD_HASIM_create_operator_order(void);

void QCLOUD_HASIM_Config_calc_sha1(char *output);

void QCLOUD_HASIM_cloud_config_init();

int qcloud_hasim_send_cmd_to_module_wait_resp(char *cmd, int cmd_len,
                                              char *expect_resp,
                                              int   timeout_ms);
int qcloud_hasim_get_imei_from_module(char *imei_buf, int imei_buf_len);
int qcloud_hasim_get_imsi_from_module(char *imsi_buf, int imsi_buf_len);
int qcloud_hasim_get_iccid_from_module(char *iccid_buf, int buf_len);
int qcloud_hasim_get_lac_cellid_from_module(char *lac_buf, int lac_buf_len,
                                            char *cellid_buf,
                                            int   cellid_buf_len);
int qcloud_hasim_get_gps_lon_lat_from_module(char *gps_lon_buf, int lon_buf_len,
                                             char *gps_lat_buf,
                                             int   lat_buf_len);
int qcloud_hasim_init_module(void);
int qcloud_hasim_auto_dial_up_to_module(void);
int qcloud_hasim_reset_module(void);
int qcloud_hasim_get_dbm_from_module(int *dbm);
int qcloud_hasim_get_network_mode_from_module(QCLOUD_HASIM_NET_MODE_E *mode);
int qcloud_hasim_get_network_vendor_from_module(
    QCLOUD_HASIM_OPERATOR_INDEX_E *network_vendor);
int qcloud_hasim_get_ps_status_from_module(bool *attach_status);
int qcloud_hasim_get_network_delay_from_module(int *delay);
int qcloud_hasim_check_network_status_from_module(char *host, bool *net_status);
int qcloud_hasim_config_save_to_module(char *config_data, int data_len);
int qcloud_hasim_config_read_from_module(char *config_data, int data_len);
int QCLOUD_HASIM_Set_HAL_API(const QCLOUD_HASIM_HAL_API_S *hasim_api);

#ifdef __cplusplus
}
#endif

#endif  // _HASIM_CLIENT_CONFIG_H_
