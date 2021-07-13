/*
 * Tencent is pleased to support the open source community by making IoT Hub
 available.
 * Copyright (C) 2018-2020 THL A29 Limited, a Tencent company. All rights
 reserved.

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

#ifndef QCLOUD_IOT_EXPORT_HASIM_H_
#define QCLOUD_IOT_EXPORT_HASIM_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _IN_
#define _IN_
#endif

#ifndef _OUT_
#define _OUT_
#endif

typedef enum qcloud_hasim_operator_index {
    QCLOUD_HASIM_OPERATOR_INVALID_E       = -1,
    QCLOUD_HASIM_OPERATOR_CHINA_MOBILE_E  = 0,
    QCLOUD_HASIM_OPERATOR_CHINA_UNICOM_E  = 1,
    QCLOUD_HASIM_OPERATOR_CHINA_TELECOM_E = 2,
    QCLOUD_HASIM_OPERATOR_INDEX_MAX_E
} QCLOUD_HASIM_OPERATOR_INDEX_E;

typedef enum qcloud_hasim_net_mode {
    QCLOUD_HASIM_OPERATOR_NET_MODE_INVALID = -1,
    QCLOUD_HASIM_OPERATOR_NET_MODE_2G      = 2,
    QCLOUD_HASIM_OPERATOR_NET_MODE_3G      = 3,
    QCLOUD_HASIM_OPERATOR_NET_MODE_4G      = 4,
    QCLOUD_HASIM_OPERATOR_NET_MODE_MAX_E
} QCLOUD_HASIM_NET_MODE_E;

/* The returned value is the execution result of the function, success is >= 0 ,
 * other < 0 */
typedef struct {
    int (*get_imei_func)(_OUT_ char *imei_buf, _IN_ const int buf_len);
    int (*get_iccid_func)(_OUT_ char *iccid_buf, _IN_ const int buf_len);
    int (*get_imsi_func)(_OUT_ char *imsi_buf, _IN_ const int buf_len);

    int (*get_lac_cellid_func)(_OUT_ char *lac_buf, _IN_ const int lac_buf_len,
                               _OUT_ char *   cellid_buf,
                               _IN_ const int cellid_buf_len);

    int (*get_gps_lon_lat_func)(_OUT_ char *   lontitue_buf,
                                _IN_ const int lontitue_buf_len,
                                _OUT_ char *   latitude_buf,
                                _IN_ const int latitude_buf_len);

    int (*module_init_func)(void);
    int (*auto_dial_up_func)(void);
    int (*reset_module_func)(void);

    int (*get_dbm_func)(_OUT_ int *dbm);
    int (*get_network_vendor_func)(
        _OUT_ QCLOUD_HASIM_OPERATOR_INDEX_E *network_vendor);
    int (*get_network_mode_func)(_OUT_ QCLOUD_HASIM_NET_MODE_E *mode);
    int (*get_ps_status_func)(_OUT_ bool *attach_status);
    int (*check_network_status_func)(_OUT_ const char *host,
                                     _OUT_ bool *      net_status);

    int (*config_save_func)(_IN_ const char *config_data,
                            _IN_ const int   config_data_len);
    int (*config_read_func)(_OUT_ char *   config_data_buf,
                            _IN_ const int buf_len);

    int (*send_at_cmd_wait_resp_func)(_IN_ const char *       atcmd,
                                      _IN_ const int          atcmd_len,
                                      _IN_ const char *       expect_response,
                                      _IN_ const unsigned int timeout_ms);
} QCLOUD_HASIM_HAL_API_S;

/**
 * @brief qcloud hasim entry fun
 *
 * @param hasim_api  		hasim run need platform api
 * @return					QCLOUD_RET_SUCCESS for success
 *							otherwise, failure
 */
int QCLOUD_HASIM_RUN(const QCLOUD_HASIM_HAL_API_S *hasim_api);

/**
 * @brief qcloud hasim user select operator
 *
 * @param mobile			true is select china mobile
 * @param unicom			true is select china unicom
 * @param telecom			true is select china telecom
 * @return					void
 */
void QCLOUD_HASIM_API_select_operator(bool mobile, bool unicom, bool telecom);

/**
 * @brief user sets the max query cellular network state interval
 *
 * @param max_state_query_interval   	max query interval
 * @return					void
 */
void QCLOUD_HASIM_API_set_state_query_interval(int max_state_query_interval);

/**
 * @brief user sets the max monitor net unregister is no cellular network
 * service
 *
 * @param max_monitor_unregister_time      max unregister time
 * @return					void
 */
void QCLOUD_HASIM_API_set_unregister_monitor_time(
    int max_monitor_unregister_time);

/**
 * @brief The user sets the max monitor ping no reply time is no data service
 *
 * @param max_monitor_pingnoreply_time     max ping no reply time
 * @return                  void
 */
void QCLOUD_HASIM_API_set_ping_noreply_monitor_time(
    int max_monitor_pingnoreply_time);

/**
 * @brief The user sets the detection of whether the network is available as an
 * independent thread
 *
 * @param use_independent_thread   true is use independent thread or not use
 * @return                  void
 */
void QCLOUD_HASIM_API_set_net_check_indepenthread(bool use_independent_thread);

/**
 * @brief user disable HASIM SDK to switch between carriers
 *
 * @param disable    true is diable or is enable
 * @return           void
 */
void QCLOUD_HASIM_API_disable_sdk_switch_operator(bool disable);

/**
 * @brief are used to switch operators by user, please disable sdk switch first.
 *
 * @param target_operator_e   The target operator that the user wants to use
 * @return           void
 */
int QCLOUD_HASIM_API_switch_operator(
    QCLOUD_HASIM_OPERATOR_INDEX_E target_operator_e);

/**
 * @brief are used to notify sdk sim entiry is old card iccid is 89 not 99 start
 *
 * @param is_old_card   The target operator that the user wants to use
 * @return           void
 */
void QCLOUD_HASIM_API_set_oldcard(bool is_old_card);

/**
 * @brief Expect sdk idle to call interval sleep release cpu
 *
 * @param enable   true enable; false disable
 * @return         void
 */
void QCLOUD_HASIM_API_enable_sdk_idle_sleep(bool enable);

/**
 * @brief Set the interval at which heartbeats are sent to the cloud management
 * platform
 *
 * @param interval   the interval default 30s
 * @return           void
 */
void QCLOUD_HASIM_API_set_heartbeat_interval(int interval);

/**
 * @brief Set the weakest RSSI of the switching operator
 *
 * @param weak_rssi  default -1 don`t check weak signal; Take the average five
 * times;
 * @return           void
 */
void QCLOUD_HASIM_API_set_weaksignal_value(int weak_rssi);

/**
 * @brief Set the maximum delay for switching operators
 *
 * @param ping_interval  The interval of each time ping
 * @param ping_max_delay  The maximum delay ms; Take the average five times;
 * default -1ms
 * @return           void
 */
void QCLOUD_HASIM_API_set_ping_delay(int ping_interval, int ping_max_delay);

/**
 * @brief Power on to select the optimal signal operator
 *
 * @param enable  true enable select; false disable select; default is false
 * @return           void
 */
void QCLOUD_HASIM_API_select_best_signal(bool enable);

/**
 * @brief Enable SDK disconnection to switch operators
 *
 * @param enable  true enable local switch; false disable; default is true
 * @return           void
 */
void QCLOUD_HASIM_API_enable_localswitch(bool enable);

/**
 * @brief Enable move scene check
 *
 * @param enable  true enable check; false disable; default is true
 * @return           void
 */
void QCLOUD_HASIM_API_enable_movescene_check(bool enable);

/**
 * @brief power on priority operator
 *
 * @param net_operator_index  operator index; default -1 is last power off
 * operator
 * @return           void
 */
int QCLOUD_HASIM_API_precedence_operator(
    QCLOUD_HASIM_OPERATOR_INDEX_E net_operator_index);

/**
 * @brief The operator you do not want to choose when power on
 *
 * @param net_operator_index  operator index; default -1
 * @return QCLOUD_RET_SUCCESS set success; QCLOUD_ERR_FAILURE is set failed
 */
int QCLOUD_HASIM_API_final_operator(
    QCLOUD_HASIM_OPERATOR_INDEX_E net_operator_index);

/**
 * @brief A network mode that you don't want to use
 *
 * @param num  1 is 2G don't use; 2 3G don't use; 3 2G/3G don't use
 * @return           void
 */
void QCLOUD_HASIM_API_dont_usemode(int num);

#ifdef __cplusplus
}
#endif

#endif /* QCLOUD_IOT_EXPORT_YTCARD_H_ */
