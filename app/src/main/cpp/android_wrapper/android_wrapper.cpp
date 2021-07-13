#include <jni.h>
#include <string>
#include <android/log.h>

#include <pthread.h>
#include <unistd.h>

#include "qcloud_iot_export.h"
#include "qcloud_iot_import.h"
#include "cJSON.h"
#include "android_wrapper.h"

static JavaVM *local_vm = NULL;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    local_vm = vm;
    __android_log_print(ANDROID_LOG_DEBUG, "iotsdk", "%s_%d", __func__, __LINE__);
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        __android_log_print(ANDROID_LOG_DEBUG, "iotsdk", "%s_%d", __func__, __LINE__);
        return -1;
    }
    __android_log_print(ANDROID_LOG_DEBUG, "iotsdk", "%s_%d", __func__, __LINE__);
    return JNI_VERSION_1_6;
}


#ifdef __cplusplus
extern "C" {
#endif
jobject class_obj;
static char class_name[128] = "";
enum {
    METHOD_GET_IMEI,    // 0
    METHOD_GET_DBM,
    METHOD_GET_ICCID,
    METHOD_GET_IMSI,
    METHOD_GET_CELLID,
    METHOD_GET_OPERATOR,    // 5
    METHOD_GET_NETWORKMODE,
    METHOD_GET_PSSTATUS,
    METHOD_GET_LOCATION,
    METHOD_SEND_ATCMD,
    METHOD_AUTO_DIALUP,     // 10
    METHOD_RESET_MODEM,
    METHOD_GET_NETWORKDELAY,
    METHOD_SAVE_CONFIG,
    METHOD_READ_CONFIG,
    METHOD_SAVE_STRATEGY,   // 15
    METHOD_READ_STRATEGY,
    METHOD_CHECK_NETWORK,
    METHOD_DEVICE_INIT,
    METHOD_GET_NETWORKVENDOR,

    METHOD_SIZE,
};

#define METHOD_SIG_TYPE1    ("()Ljava/lang/String;")
#define METHOD_SIG_TYPE2    ("()I")
#define METHOD_SIG_TYPE3    ("(Ljava/lang/String;I)Ljava/lang/String;")
#define METHOD_SIG_TYPE4    ("(Ljava/lang/String;)I")

static const char *method_name[][2] = {
        {"getImei", METHOD_SIG_TYPE1},  // 0
        {"getDbm", METHOD_SIG_TYPE2},
        {"getICCID", METHOD_SIG_TYPE1},
        {"getIMSI", METHOD_SIG_TYPE1},
        {"getCellid", METHOD_SIG_TYPE1},
        {"getOperator", METHOD_SIG_TYPE2},  // 5
        {"getNetworkMode", METHOD_SIG_TYPE2},
        {"getPsStatus", METHOD_SIG_TYPE2},
        {"getLocation", METHOD_SIG_TYPE1},
        {"sendATCmd", METHOD_SIG_TYPE3},
        {"autoDialUp", METHOD_SIG_TYPE2},   // 10
        {"resetModem", METHOD_SIG_TYPE2},
        {"getNetworkDelay", METHOD_SIG_TYPE2},
        {"saveConfig", METHOD_SIG_TYPE4},
        {"readConfig", METHOD_SIG_TYPE1},
        {"saveStrategy",METHOD_SIG_TYPE4},  // 15
        {"readStrategy",METHOD_SIG_TYPE1},
        {"checkNetwork",METHOD_SIG_TYPE4},
        {"deviceInit", METHOD_SIG_TYPE2},
        {"getNetworkVendor", METHOD_SIG_TYPE2}
};

#define CHECK_NULL_WITH_RET(p, r)   do { if ((p) == NULL) { Log_d("NULL pointer"); return (r);} } while(0)

static jmethodID _get_method(int method_id, JNIEnv **pp_env, jclass *p_cls) {
    *p_cls = (jclass)class_obj;
    CHECK_NULL_WITH_RET(*p_cls, NULL);
    int getEnvStat = local_vm->GetEnv((void **)pp_env, NULL);
    if (getEnvStat == JNI_EDETACHED) {
        jint ret = local_vm->AttachCurrentThread(pp_env, NULL);

        CHECK_NULL_WITH_RET(*pp_env, NULL);
    }

    Log_d("local_vm %p, class_name %s class_obj %p method_id %d", local_vm, class_name, *p_cls, method_id);

    return (*pp_env)->GetStaticMethodID(*p_cls, method_name[method_id][0], method_name[method_id][1]);
}

int get_buffer_method_type1(char *buffer, int len, int id) {
    int ret = QCLOUD_RET_SUCCESS;
    JNIEnv *env = NULL;
    jclass cls_id;
    jmethodID method_id = _get_method(id, &env, &cls_id);
    Log_d("call method is %d buffer %s", id, buffer);

    CHECK_NULL_WITH_RET(method_id, QCLOUD_ERR_FAILURE);
    jstring sret = (jstring)(env->CallStaticObjectMethod(cls_id, method_id));
    const char *p_str = env->GetStringUTFChars(sret, 0);
    Log_d("call method is %d get return %s", id, p_str);
    size_t str_len = strlen(p_str);
    if (str_len >= len) {
        Log_e("buffer not enough");
        local_vm->DetachCurrentThread();

        return QCLOUD_ERR_BUF_TOO_SHORT;
    }
    strncpy(buffer, p_str, len);

    local_vm->DetachCurrentThread();
    return str_len;
}

int get_int_method_type2(int id) {
    JNIEnv *env = NULL;
    jclass cls_id;
    jmethodID method_id = _get_method(id, &env, &cls_id);
    Log_d("call method %d env %p method_id %p cls_id %p", id, env, method_id, cls_id);

    CHECK_NULL_WITH_RET(method_id, QCLOUD_ERR_FAILURE);
    jint iret = env->CallStaticIntMethod(cls_id, method_id);
    Log_d("call method return %d", iret);

    local_vm->DetachCurrentThread();
    return iret;
}

int get_buffer_method_type3(char *out_buf, const char *in_buf, int i_arg, int id) {
    JNIEnv *env = NULL;
    jclass cls_id;
    jmethodID method_id = _get_method(id, &env, &cls_id);
    jstring s_arg = env->NewStringUTF(in_buf);
    jint ji_arg = (jint)i_arg;
    Log_d("call method %d in_buf %s", id, in_buf);

    CHECK_NULL_WITH_RET(method_id, QCLOUD_ERR_FAILURE);
    jstring sret = (jstring)(env->CallStaticObjectMethod(cls_id, method_id, s_arg, ji_arg));
    const char *str_ret = env->GetStringUTFChars(sret, 0);
    Log_d("call method return %s", str_ret);
    strcpy(out_buf, str_ret);

    local_vm->DetachCurrentThread();
    return QCLOUD_RET_SUCCESS;
}

int get_int_method_type4(const char *in_buf, int id) {
    JNIEnv *env = NULL;
    jclass cls_id;
    jmethodID method_id = _get_method(id, &env, &cls_id);
    jstring s_arg = env->NewStringUTF(in_buf);
    Log_d("call method %d in_buf %s", id, in_buf);

    CHECK_NULL_WITH_RET(method_id, QCLOUD_ERR_FAILURE);
    jint iret = env->CallStaticIntMethod(cls_id, method_id, s_arg);

    local_vm->DetachCurrentThread();
    return iret;
}

int platform_ytcard_get_imei(char *imei, int len) {
    return get_buffer_method_type1(imei, len, METHOD_GET_IMEI);
}

int platform_ytcard_get_dbm(int *rssi) {
    int ret = get_int_method_type2(METHOD_GET_DBM);
    if (ret <= -1000) return QCLOUD_ERR_FAILURE;
    *rssi = ret;

    return 0;
}

int platform_ytcard_get_iccid(char *iccid, int len) {
    (void)get_buffer_method_type1(iccid, len, METHOD_GET_ICCID);
    return 0;
}

int platform_ytcard_get_imsi(char *imsi, int len) {
    return get_buffer_method_type1(imsi, len, METHOD_GET_IMSI);
}

int platform_ytcard_get_lac_cellid(char *lac, int laclen, char *cellid,
                                   int cellid_len) {
    char cell_info[256];
    int ret = get_buffer_method_type1(cell_info, sizeof(cell_info), METHOD_GET_CELLID);
    if (ret != 0) return ret;
    cJSON *root = cJSON_Parse(cell_info);
    CHECK_NULL_WITH_RET(root, QCLOUD_ERR_FAILURE);
    cJSON *lac_json = cJSON_GetObjectItem(root, "lat");
    cJSON *cell_json = cJSON_GetObjectItem(root, "cell");
    if (!lac_json || !cell_json ||
        strlen(cJSON_GetStringValue(lac_json)) >= laclen ||
        strlen(cJSON_GetStringValue(cell_json)) >= cellid_len) {
        Log_e("Not enough buffer");
        cJSON_Delete(root);
        return QCLOUD_ERR_BUF_TOO_SHORT;
    }
    strncpy(lac, cJSON_GetStringValue(lac_json), laclen);
    strncpy(cellid, cJSON_GetStringValue(cell_json), cellid_len);

    return QCLOUD_RET_SUCCESS;
}

int platform_ytcard_get_operator(int *operator_) {
    int ret;
    ret = get_int_method_type2(METHOD_GET_OPERATOR);
    if (ret < 0) return ret;
    *operator_ = ret;

    return 0;
}

int platform_ytcard_get_network_mode(_OUT_ QCLOUD_HASIM_NET_MODE_E *mode_) {
    int ret = get_int_method_type2(METHOD_GET_NETWORKMODE);
    if (ret < 0) return ret;
    *mode_ = (QCLOUD_HASIM_NET_MODE_E)ret;

    return 0;
}

int platform_ytcard_get_network_vendor(_OUT_ QCLOUD_HASIM_OPERATOR_INDEX_E *vendor_) {
    int ret = get_int_method_type2(METHOD_GET_NETWORKVENDOR);
    if (ret < 0) return ret;
    *vendor_ = (QCLOUD_HASIM_OPERATOR_INDEX_E)ret;

    return 0;
}

int platform_ytcard_get_ps_status(_OUT_ bool *ps_) {
   int ret =  get_int_method_type2(METHOD_GET_PSSTATUS);
   if (ret < 0) return ret;
   *ps_ = (bool)ret;

   return 0;
}

int platform_ytcard_get_lon_lat(char *lon, int lonlen, char *lat, int latlen) {
    char loc_info[256];
    int ret = get_buffer_method_type1(loc_info, sizeof(loc_info), METHOD_GET_CELLID);
    if (ret != 0) return ret;
    cJSON *root = cJSON_Parse(loc_info);
    CHECK_NULL_WITH_RET(root, QCLOUD_ERR_FAILURE);
    cJSON *lon_json = cJSON_GetObjectItem(root, "lontitude");
    cJSON *lat_json = cJSON_GetObjectItem(root, "latitude");
    if (!lon_json || !lat_json ||
        strlen(cJSON_GetStringValue(lon_json)) >= lonlen ||
        strlen(cJSON_GetStringValue(lat_json)) >= latlen) {
        Log_e("Not enough buffer");
        cJSON_Delete(root);
        return QCLOUD_ERR_BUF_TOO_SHORT;
    }
    strncpy(lon, cJSON_GetStringValue(lon_json), lonlen);
    strncpy(lat, cJSON_GetStringValue(lat_json), latlen);

    return QCLOUD_RET_SUCCESS;
}

int platform_ytcard_get_networkdelay(int *delay_ms) {
    int ret = get_int_method_type2(METHOD_GET_NETWORKDELAY);
    if (ret < 0) return ret;

    *delay_ms = ret;
    return QCLOUD_RET_SUCCESS;
}

int platform_ytcard_send_at_cmd(const char *atCmd, const int atcmd_len, const char *atRespone,
                                unsigned int timeOut) {
    char at_reply[256]  = {0};
    int ret = get_buffer_method_type3(at_reply, atCmd, timeOut, METHOD_SEND_ATCMD);
    if (ret) return ret;
    if (strcmp(atRespone, at_reply)) return QCLOUD_ERR_FAILURE;
    return 0;
}

int platform_ytcard_config_save(const char *configdata, int datalen) {
    return get_int_method_type4(configdata, METHOD_SAVE_CONFIG);
}

int platform_ytcard_config_read(char *configdata, int datalen) {

    return get_buffer_method_type1(configdata, datalen, METHOD_READ_CONFIG);
}

int platform_ytcard_strategy_save(const char *configdata, int datalen) {
    return get_int_method_type4(configdata, METHOD_SAVE_STRATEGY);
}

int platform_ytcard_strategy_read(char *configdata, int datalen) {
    return get_buffer_method_type1(configdata, datalen, METHOD_READ_STRATEGY);
}

int platform_ytcard_auto_dial_up(void) {
    return get_int_method_type2(METHOD_AUTO_DIALUP);
}

int platform_ytcard_reset_modem(void) {
    return get_int_method_type2(METHOD_RESET_MODEM);
}

int platform_ytcard_check_network_status(_OUT_ const char *host, _OUT_ bool *stat) {
   int ret = get_int_method_type4(host, METHOD_CHECK_NETWORK);
   if (ret < 0) return ret;
   *stat = (bool)ret;

   return QCLOUD_RET_SUCCESS;
}

int platform_ytcard_device_init(void) {
    return get_int_method_type2(METHOD_DEVICE_INIT);
}

static const QCLOUD_HASIM_HAL_API_S sg_ytcard_api = {
        .get_imei_func             = platform_ytcard_get_imei,
        .get_iccid_func            = platform_ytcard_get_iccid,
        .get_imsi_func             = platform_ytcard_get_imsi,
        .get_lac_cellid_func       = platform_ytcard_get_lac_cellid,
        .get_gps_lon_lat_func      = platform_ytcard_get_lon_lat,
        .module_init_func          = platform_ytcard_device_init,
        .auto_dial_up_func         = platform_ytcard_auto_dial_up,
        .reset_module_func          = platform_ytcard_reset_modem,
        .get_dbm_func              = platform_ytcard_get_dbm,
        //.get_operator_func         = platform_ytcard_get_operator,
        .get_network_vendor_func    = platform_ytcard_get_network_vendor,
        .get_network_mode_func     = platform_ytcard_get_network_mode,
        .get_ps_status_func        = platform_ytcard_get_ps_status,
        //.get_network_delay_func    = platform_ytcard_get_networkdelay,
        .check_network_status_func = platform_ytcard_check_network_status,
        .config_save_func          = platform_ytcard_config_save,
        .config_read_func          = platform_ytcard_config_read,
        //.strategy_save_func        = platform_ytcard_strategy_save,
        //.strategy_read_func        = platform_ytcard_strategy_read,
        .send_at_cmd_wait_resp_func          = platform_ytcard_send_at_cmd
};

void thd_fn(void *arg) {
    // init log level
    IOT_Log_Set_Level(eLOG_DEBUG);

    Log_d("qcloud iot ytcard demo app.");
    if (local_vm == NULL)  {
        Log_e("local_vm is null");
        return;
    }
    QCLOUD_HASIM_RUN(&sg_ytcard_api);
//    Log_d("Get result %d", get_int_method_type4("abcdefg", 17));
    JNIEnv **pp_env = NULL;
    int getEnvStat = local_vm->GetEnv((void **)pp_env, NULL);
    if (getEnvStat == JNI_EDETACHED) {
        jint ret = local_vm->AttachCurrentThread(pp_env, NULL);

        return;
    }
    (*pp_env)->DeleteGlobalRef(class_obj);
}

#ifdef __cplusplus
}
#endif

int YuntuStart(JNIEnv *env, const char* cls_name) {
    jclass cls = env->FindClass(cls_name);
    Log_d("%s_%d %p", __func__, __LINE__, cls);
    if (cls == NULL) {
        Log_e("Failed to find class");
        return -1;
    }
    Log_d("cls is %p", cls);

    class_obj = env->NewGlobalRef(cls);

    static ThreadParams thread_params = {0};
    thread_params.thread_func = thd_fn;
    thread_params.thread_name = "test";
    thread_params.user_arg = NULL;
    HAL_ThreadCreate(&thread_params);

    return 0;
}
