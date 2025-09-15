#include "RunParameters.h"
#include "napi/native_api.h"
#include "napi_utils.h"
#include "SoundStretch.h"

static RunParameters * readRunParameters(napi_env env, napi_value optionsNapiValue) {
    RunParameters *params = new RunParameters();
    if (!IsNValueUndefined(env, optionsNapiValue)) {
        // tempo
        napi_value tempoDeltaNapiValue;
        napi_get_named_property(env, optionsNapiValue, "tempo", &tempoDeltaNapiValue);
        if (!IsNValueUndefined(env, tempoDeltaNapiValue)) {
            int tempoDelta = NValueToInt32(env, tempoDeltaNapiValue);
            params->tempoDelta = tempoDelta;
        }
        
        // pitch
        napi_value pitchDeltaNapiValue;
        napi_get_named_property(env, optionsNapiValue, "pitch", &pitchDeltaNapiValue);
        if (!IsNValueUndefined(env, pitchDeltaNapiValue)) {
            int pitchDelta = NValueToInt32(env, pitchDeltaNapiValue);
            params->pitchDelta = pitchDelta;
        }
        
        // rate
        napi_value rateDeltaNapiValue;
        napi_get_named_property(env, optionsNapiValue, "rate", &rateDeltaNapiValue);
        if (!IsNValueUndefined(env, rateDeltaNapiValue)) {
            int rateDelta = NValueToInt32(env, rateDeltaNapiValue);
            params->rateDelta = rateDelta;
        }
        
        // quick
        napi_value quickNapiValue;
        napi_get_named_property(env, optionsNapiValue, "quick", &quickNapiValue);
        if (!IsNValueUndefined(env, quickNapiValue)) {
            int quick = NValueToInt32(env, quickNapiValue);
            params->quick = quick;
        }
        
        // noAntiAlias
        napi_value noAntiAliasNapiValue;
        napi_get_named_property(env, optionsNapiValue, "noAntiAlias", &noAntiAliasNapiValue);
        if (!IsNValueUndefined(env, noAntiAliasNapiValue)) {
            int noAntiAlias = NValueToInt32(env, noAntiAliasNapiValue);
            params->noAntiAlias = noAntiAlias;
        }
        
        // detectBPM
        napi_value detectBPNNapiValue;
        napi_get_named_property(env, optionsNapiValue, "detectBPM", &detectBPNNapiValue);
        bool detectBPM = false;
        if (!IsNValueUndefined(env, detectBPNNapiValue)) {
            detectBPM = NValueToBool(env, detectBPNNapiValue);
            params->detectBPM = detectBPM;
        }
        
        // goalBPM
        if (detectBPM) {
            napi_value goalBPNNapiValue;
            napi_get_named_property(env, optionsNapiValue, "goalBPM", &goalBPNNapiValue);
            if (!IsNValueUndefined(env, goalBPNNapiValue)) {
                float goalBPM = NValueToDouble(env, goalBPNNapiValue);
                params->goalBPM = goalBPM;
            }
        }
        
        // speech
        napi_value speechNapiValue;
        napi_get_named_property(env, optionsNapiValue, "speech", &speechNapiValue);
        if (!IsNValueUndefined(env, speechNapiValue)) {
            bool speech = NValueToBool(env, speechNapiValue);
            params->speech = speech;
        }
    }
    params->checkLimits();
    return params;
}

static napi_value ProcessSync(napi_env env, napi_callback_info info)
{
    size_t argc = 3;
    napi_value args[3] = {nullptr};
    NAPI_CALL(napi_get_cb_info(env, info, &argc, args, nullptr, nullptr));
    
    std::string inputPath = NValueToString(env, args[0]);
    std::string outputPath = NValueToString(env, args[1]);
    RunParameters *params = readRunParameters(env,  args[2]);
    
    int res = run(inputPath.c_str(), outputPath.c_str(), params);
    return Int32ToNValue(env,res);
}

static napi_value ProcessAsync(napi_env env, napi_callback_info info) {
    size_t argc = 3;
    napi_value args[3] = {nullptr};
    NAPI_CALL(napi_get_cb_info(env, info, &argc, args, nullptr, nullptr));

    std::string inputPath = NValueToString(env, args[0]);
    std::string outputPath = NValueToString(env, args[1]);
    RunParameters *params = readRunParameters(env,  args[2]);
    
    return NAPIUndefined(env);
}

EXTERN_C_START
static napi_value Init(napi_env env, napi_value exports)
{
    napi_property_descriptor desc[] = {
        { "processSync", nullptr, ProcessSync, nullptr, nullptr, nullptr, napi_default, nullptr },
        { "processAsync", nullptr, ProcessAsync, nullptr, nullptr, nullptr, napi_default, nullptr }
    };
    napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc);
    return exports;
}
EXTERN_C_END

static napi_module demoModule = {
    .nm_version = 1,
    .nm_flags = 0,
    .nm_filename = nullptr,
    .nm_register_func = Init,
    .nm_modname = "soundtouch",
    .nm_priv = ((void*)0),
    .reserved = { 0 },
};

extern "C" __attribute__((constructor)) void RegisterSoundtouchModule(void)
{
    napi_module_register(&demoModule);
}
