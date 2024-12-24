#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <glib/gi18n.h>

#include <nautilus/nautilus-extension.h>

#if 1
#include "/data/code/anda/sec_linux/src/client/lib/ipc.h"
#else
#include "../client/lib/ipc.h"
#endif


#define EFS_TYPE_MENU_PROVIDER                  (efs_menu_provider_get_type())
#define EFS_TYPE_INFO_PROVIDER                  (efs_info_provider_get_type())

typedef struct _EfsMenuProvider                 EfsMenuProvider;
typedef struct _EfsMenuProviderClass            EfsMenuProviderClass;

typedef struct _EfsInfoProvider                 EfsInfoProvider;
typedef struct _EfsInfoProviderClass            EfsInfoProviderClass;

GType                gProviderTypes[2] = {0};
GType                gsEfsInfoType;
GType                gsEfsMenuType;

static GObjectClass* parent_class;

typedef struct
{
    const char*      src;
    const char*      dest;
} AndsecLanguage;

struct _MsgReq
{
    int              msgType;
    int              msgLen;
    char             msg[];
};

typedef struct 
{
    GObject          parent_slot;
} Efs;

typedef struct 
{
    GObjectClass parent_slot;
} EfsClass;

struct _EfsMenuProviderClass
{
    GObjectClass parent_class;
};

struct _EfsInfoProviderClass
{
    GObjectClass parent_class;
};

struct _EfsInfoProvider
{
    GObject parent_instance;
};

struct _EfsMenuProvider
{
    GObject parent_instance;
};

extern int MonIsEncryptFile                     (const char* pFileName);

void nautilus_module_shutdown                   (void);
void nautilus_module_initialize                 (GTypeModule* module);
void nautilus_module_list_types                 (const GType**types, int *numTypes);

GList* efs_menu                                 (NautilusMenuProvider *provider, GList* files);

static void efs_register_type                   (GTypeModule* module);
static void efs_menu_dec_files                  (NautilusMenuItem* item, gpointer udata);
static void efs_menu_enc_files                  (NautilusMenuItem* item, gpointer udata);

static gboolean andsec_language_is_chinese      (void);
static gboolean andsec_language_is_english      (void);
static const char* andsec_translate             (const char* str);
static void andsec_files_execute_cmd            (GList* files, gboolean enc);
static void send_data_to_daemon                 (IpcServerType type, short isCN, char* files, unsigned int len);

static NautilusOperationResult efs_emblem_update_file_info(NautilusInfoProvider* provider, NautilusFileInfo* file, GClosure* updateComplete, NautilusOperationHandle** handle);

static const AndsecLanguage gsChinese[] = {
    { "Andsec", "安得卫士" },
    { "Manual encryption",      "手动加密"},
    { "Privileged decryption",  "特权解密"},
    { "Decryption success",     "解密成功"},
    { "Decryption failed",      "解密失败"},
    { "Encryption success",     "加密成功"},
    { "Encryption failed",      "加密失败"},
    { "Total file",             "文件总数"},
    {NULL, NULL},
};

const char* gEmblemName = "andsec_lock";


static void efs_menu_test1(NautilusMenuItem* item, gpointer udata)
{
    printf ("activated 1!\n");
}

static void efs_menu_dec_files(NautilusMenuItem* item, gpointer udata)
{
    g_return_if_fail(udata);

    GList* files = (GList*) udata;

    andsec_files_execute_cmd(files, FALSE);
}

static void efs_menu_enc_files(NautilusMenuItem* item, gpointer udata)
{
    g_return_if_fail(udata);

    GList* files = (GList*) udata;

    andsec_files_execute_cmd(files, TRUE);
}


GList* efs_menu (NautilusMenuProvider *provider, GList* files)
{
    printf("efs_menu!\n");

    GList* l = NULL;
    GList* myFiles = NULL;
    for (l = files; l; l = g_list_next(l)) {
        NautilusFileInfo* file = (NautilusFileInfo*) (l->data);
        GFile* f = nautilus_file_info_get_location(file);
        if (f) {
            char* loc = g_file_get_path(f);
            if (loc) {
                myFiles = g_list_append(myFiles, g_strdup(loc));
                g_free (loc);
            }
        }
    }

    if (myFiles) {
        GList* mItemList = NULL;
        NautilusMenu* menuT = nautilus_menu_new();

        // 

        NautilusMenuItem* item = NULL;

        item = nautilus_menu_item_new(andsec_translate("Manual encryption"), andsec_translate("Manual encryption"), andsec_translate("Manual encryption"), NULL/*icon*/);
        g_signal_connect(item, "activate", G_CALLBACK(efs_menu_enc_files), myFiles);
        nautilus_menu_append_item(menuT, item);

        item = nautilus_menu_item_new(andsec_translate("Privileged decryption"), andsec_translate("Privileged decryption"), andsec_translate("Privileged decryption"), NULL/*icon*/);
        g_signal_connect(item, "activate", G_CALLBACK(efs_menu_dec_files), myFiles);
        nautilus_menu_append_item(menuT, item);

        NautilusMenuItem* efsMenu = nautilus_menu_item_new(andsec_translate("Andsec"), andsec_translate("Andsec"), andsec_translate("Andsec"), NULL/*icon*/);
	    nautilus_menu_item_set_submenu(efsMenu, menuT);
        mItemList = g_list_append(mItemList, efsMenu);

        return mItemList;
    }

    return NULL;
}


static void efs_menu_provider_iface_init(NautilusMenuProviderInterface* iface)
{
    iface->get_file_items = efs_menu;

    printf("efs_menu_provider_iface_init\n");
}

static void efs_info_provider_iface_init(NautilusInfoProviderInterface* iface)
{
    iface->update_file_info = efs_emblem_update_file_info;

    printf("efs_info_iface_init\n");
}

static void efs_menu_instance_init(EfsMenuProvider* object)
{
    printf ("instance init\n");
}

static void efs_info_instance_init(EfsInfoProvider* object)
{
    printf ("instance init\n");
}

static void efs_menu_class_init(EfsMenuProviderClass* klass)
{
    printf ("class init\n");
}

static void efs_info_class_init(EfsInfoProviderClass* klass)
{
    printf ("class init\n");
}

void nautilus_module_initialize (GTypeModule* module)
{
    printf("efs nautilus module initialize...\n");

    efs_register_type(module);
    gProviderTypes[0] = gsEfsMenuType;
    gProviderTypes[1] = gsEfsInfoType;
}


void nautilus_module_shutdown(void)
{
    /* Any module-specific shutdown */
}


void nautilus_module_list_types (const GType **types, int* numTypes)
{
    *types     = gProviderTypes;
    *numTypes = G_N_ELEMENTS(gProviderTypes);

    printf("num types: %d\n", *numTypes);
}



static void efs_register_type (GTypeModule* module)
{
    static const GTypeInfo menuInfo = {sizeof(EfsMenuProviderClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) efs_menu_class_init, NULL, NULL, sizeof(EfsMenuProvider), 0, (GInstanceInitFunc)efs_menu_instance_init,};
    gsEfsMenuType = g_type_module_register_type(module, G_TYPE_OBJECT, "AndsecMenuProvider", &menuInfo, 0);
    const GInterfaceInfo menuProviderIfaceInfo = {(GInterfaceInitFunc) efs_menu_provider_iface_init, NULL, NULL};
    g_type_module_add_interface(module, gsEfsMenuType, NAUTILUS_TYPE_MENU_PROVIDER, &menuProviderIfaceInfo);

    static const GTypeInfo info = {sizeof(EfsInfoProviderClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) efs_info_class_init, NULL, NULL, sizeof(EfsInfoProvider), 0, (GInstanceInitFunc) efs_info_instance_init,};
    gsEfsInfoType = g_type_module_register_type(module, G_TYPE_OBJECT, "AndsecInfoProvider", &info, 0);
    const GInterfaceInfo iconInfo = {(GInterfaceInitFunc) efs_info_provider_iface_init, NULL, NULL};
    g_type_module_add_interface(module, gsEfsInfoType, NAUTILUS_TYPE_INFO_PROVIDER, &iconInfo);
}


static NautilusOperationResult efs_emblem_update_file_info(NautilusInfoProvider* provider, NautilusFileInfo* file, GClosure* updateComplete, NautilusOperationHandle** handle)
{
    printf ("emblem\n");
    if (!nautilus_file_info_is_directory(file)) {
        char* uri      = nautilus_file_info_get_uri(file);
        char* filename = g_filename_from_uri(uri, NULL, NULL);

        int enc = MonIsEncryptFile(filename);
        printf("filename[%s], enc[%d]\n", filename, enc);
        if (enc) {
            nautilus_file_info_add_emblem(file, gEmblemName);
        }

        if (filename) {
            g_free(filename);
        }

        if (uri) {
            g_free(uri);
        }
    }

    return  NAUTILUS_OPERATION_COMPLETE;
}


static const char* andsec_translate (const char* str)
{
    if (andsec_language_is_chinese()) {
        int i = 0;
        for (i = 0; NULL != gsChinese[i].src; ++i) {
            if (0 == strcmp(str, gsChinese[i].src)) {
                return gsChinese[i].dest;
            }
        }
    }

    return str;
}

static gboolean andsec_language_is_english (void)
{
    char* ls = getenv("LANG");

    return (!ls || (0 == strncmp(ls, "en_US", 5)));
}

static gboolean andsec_language_is_chinese (void)
{
    char* ls = getenv("LANG");

    return (ls && (0 == strncmp(ls, "zh_CN", 5)));
}

static void andsec_files_execute_cmd(GList* files, gboolean enc)
{
    g_return_if_fail(files);

    GList* l = NULL;
    char* cmdStr = NULL;
    char* cmdStrT = NULL;

    for (l = files; l; l = g_list_next(l)) {
        char* file = (char*) (l->data);
        if (file) {
            printf ("%s: %s\n", enc ? "[ENC]" : "[DEC]", file);
            cmdStrT = cmdStr;
            cmdStr = g_strdup_printf("%s|%s", cmdStrT ? cmdStrT : "", file);
            if (!cmdStr) {
                printf("[IMPOSSIBLE] malloc error\n");
                exit(1);
            }

            if (cmdStrT) {
                g_free(cmdStrT);
                cmdStrT = NULL;
            }
        }
    }

    g_return_if_fail(cmdStr);

    // 发送文件
    if (cmdStr) {
        printf("[CHK] %s\n", cmdStr);
        send_data_to_daemon((enc ? IPC_TYPE_PRIVILEGED_ENCRYPT_FILES_SYNC : IPC_TYPE_PRIVILEGED_DECRYPT_FILES_SYNC), andsec_language_is_chinese(), cmdStr, strlen(cmdStr));
        g_free(cmdStr);
    }

    return;
}

void send_data_to_daemon(IpcServerType type, short isCN, char* files, unsigned int dataLen)
{
    int val = 1;
    int sockFd = 0;
    int timeout = 2000;
    int revtimeout = 31 * 1000;
    struct sockaddr_un address;

    if ((sockFd = socket(PF_UNIX, SOCK_STREAM, 0)) < 0) {
        printf("socket() failed\n");
        return;
    }

    setsockopt(sockFd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
    setsockopt(sockFd, SOL_SOCKET, SO_RCVTIMEO, &revtimeout, sizeof(revtimeout));
    setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    memset(&address, 0, sizeof(struct sockaddr_un));
    address.sun_family = AF_UNIX;
    snprintf(address.sun_path, 108, IPC_SERVER_SOCKET_PATH);
    int len = sizeof(address);

    if (0 != connect(sockFd, (struct sockaddr*)&address, len)) {
        printf("connect error\n");
        return;
    }

    struct IpcMessage msgBuf;
    memset(&msgBuf, 0, sizeof(msgBuf));

    struct PrivilegedDecAndEncData data;
    memset(&data, 0, sizeof(data));

    data.isCN = isCN;
    data.isPierec = 1;
    data.filesLen = dataLen;

    msgBuf.type = type;
    msgBuf.dataLen = sizeof(data) + dataLen + 1;

    int allLen = dataLen + sizeof(data) + sizeof(msgBuf);
    char* sendBuf = (char*) malloc (allLen);
    if (sendBuf) {
        memset(sendBuf, 0, allLen);
        memcpy(sendBuf, &msgBuf, sizeof(msgBuf));
        memcpy(sendBuf + sizeof(msgBuf), &data, sizeof(data));
        memcpy(sendBuf + sizeof(msgBuf) + sizeof(data), files, dataLen);
        write(sockFd, sendBuf, allLen);
        free(sendBuf);
    }

    close(sockFd);

    return;
}

