//
// Created by dingjing on 25-6-16.
//

#ifndef hook_func_HOOK_FUNC_H
#define hook_func_HOOK_FUNC_H
#include "macros/macros.h"

C_BEGIN_EXTERN_C

#define HOOK_FUNC_ERROR_INTERNAL_ERROR                 -1
#define HOOK_FUNC_ERROR_SUCCESS                         0
#define HOOK_FUNC_ERROR_OUT_OF_MEMORY                   1
#define HOOK_FUNC_ERROR_ALREADY_INSTALLED               2
#define HOOK_FUNC_ERROR_DISASSEMBLY                     3
#define HOOK_FUNC_ERROR_IP_RELATIVE_OFFSET              4
#define HOOK_FUNC_ERROR_CANNOT_FIX_IP_RELATIVE          5
#define HOOK_FUNC_ERROR_FOUND_BACK_JUMP                 6
#define HOOK_FUNC_ERROR_TOO_SHORT_INSTRUCTIONS          7
#define HOOK_FUNC_ERROR_MEMORY_ALLOCATION               8 /* memory allocation error */
#define HOOK_FUNC_ERROR_MEMORY_FUNCTION                 9 /* other memory function errors */
#define HOOK_FUNC_ERROR_NOT_INSTALLED                   10
#define HOOK_FUNC_ERROR_NO_AVAILABLE_REGISTERS          11
#define HOOK_FUNC_ERROR_NO_SPACE_NEAR_TARGET_ADDR       12

typedef struct _HookFunc                                HookFunc;
typedef struct _HookFuncInfo                            HookFuncInfo;
typedef struct _HookFuncParams                          HookFuncParams;
typedef struct _HookFuncArgHandle                       HookFuncArgHandle;

typedef void (*HookFuncHook) (HookFuncInfo*);

struct _HookFuncInfo
{
    void*                       origTargetFunc;
    void*                       targetFunc;
    void*                       trampolineFunc;
    void*                       hookFunc;
    void*                       userData;
    HookFuncArgHandle*          argHandles;
};

struct _HookFuncParams
{
    void*                       hookFunc;
    HookFuncHook                preHook;
    void*                       userData;
    unsigned int                flags;
};

/**
 * @brief 创建一个HookFunc句柄，用于管理多个函数的Hook
 * @return 失败则返回NULL
 */
C_SYMBOL_EXPORT HookFunc*       hook_func_create                (void);

/**
 * @brief 准备Hook函数，但不执行安装。该函数分析目标函数，设置Hook
 * @param handle
 * @param targetFunc 要Hook的函数
 * @param hookFunc Hook 函数
 * @return 成功返回0,失败返回错误码
 */
C_SYMBOL_EXPORT int             hook_func_prepare               (HookFunc* handle, void** targetFunc, void* hookFunc);

/**
 * @brief hook_func_prepare() 的扩展版本，允许通过 HookFuncParams 结构设置额外参数
 * @param handle
 * @param targetFunc
 * @param params
 * @return 成功返回0,失败返回错误码
 */
C_SYMBOL_EXPORT int             hook_func_prepare_with_params   (HookFunc* handle, void** targetFunc, const HookFuncParams* params);

/**
 * @brief 安装使用给定的 HookFunc 句柄准备的所有钩子。该函数会修改目标函数的代码，并将其执行重定向到钩子函数
 * @param handle
 * @param flags
 * @return 成功返回0, 失败返回错误码
 */
C_SYMBOL_EXPORT int             hook_func_install               (HookFunc* handle, int flags);

/**
 * @brief 卸载与给定 HookFunc 句柄相关的所有钩子。此函数恢复目标函数的原始代码
 * @param handle
 * @param flags
 * @return 成功返回0, 失败返回错误码
 */
C_SYMBOL_EXPORT int             hook_func_uninstall             (HookFunc* handle, int flags);

/**
 * @brief 销毁 HookFunc 句柄并释放所有相关资源。调用此函数前必须卸载钩子
 * @param handle
 * @return 成功则返回0, 否则返回错误号
 */
C_SYMBOL_EXPORT int             hook_func_destroy               (HookFunc** handle);

/**
 * @brief 获取错误信息
 * @param handle
 * @return
 */
C_SYMBOL_EXPORT const char*     hook_func_error_message         (const HookFunc* handle);

C_SYMBOL_EXPORT void*           hook_func_arg_get_int_reg_addr  (const HookFuncArgHandle* argHandle, int pos);
C_SYMBOL_EXPORT void*           hook_func_arg_get_flt_reg_addr  (const HookFuncArgHandle* argHandle, int pos);
C_SYMBOL_EXPORT void*           hook_func_arg_get_stack_addr    (const HookFuncArgHandle* argHandle, int pos);

C_END_EXTERN_C


#endif // hook_func_HOOK_FUNC_H
