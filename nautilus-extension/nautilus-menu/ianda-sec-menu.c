#include <glib-object.h>
#include <libnautilus-extension/nautilus-file-info.h>
#include <libnautilus-extension/nautilus-info-provider.h>
#include <libnautilus-extension/nautilus-menu-provider.h>
#include <libnautilus-extension/nautilus-extension-types.h>
#include <libnautilus-extension/nautilus-property-page-provider.h>

#define IANDA_SEC_TYPE_MENU_PROVIDER 		(ianda_sec_menu_provider_get_type())

typedef struct _IandaSecMenuProvider 		IandaSecMenuProvider;
typedef struct _IandaSecMenuProviderClass 	IandaSecMenuProviderClass;


struct _IandaSecMenuProviderClass
{
	GObjectClass parent_class;
};

struct _IandaSecMenuProvider
{
	GObject parent_instance;
};

static GType  gsProviderTypes[1];
static GType  gsIandaSecType;

GType ianda_sec_get_type(void);

static void 	ianda_sec_register_type		(GTypeModule* module);
//static void 	ianda_sec_menu_provider_init 	(NautilusMenuProviderIface* iface);
//void 		ianda_sec_module_list_types	(const GType** types, int* numTypes);


void nautilus_module_initialize(GTypeModule* module)
{
	ianda_sec_register_type(module);
	gsProviderTypes[0] = ianda_sec_get_type();
	printf ("nautilus module initialize\n");
}

void nautilus_module_shutdown(void)
{
	printf ("nautilus module shutdown\n");
}

void nautilus_module_list_types(const GType** types, int* numTypes)
{
	printf ("nautilus module list types\n");
	*types     = gsProviderTypes;
	*numTypes  = G_N_ELEMENTS(gsProviderTypes);
}

GType ianda_sec_get_type( void )
{
	return gsIandaSecType;
}

static void ianda_menu_test1(NautilusMenuItem* item, gpointer udata)
{
	printf ("activated 1!");
}

static void ianda_menu_test2(NautilusMenuItem* item, gpointer udata)
{
	printf ("activated 2!");
}

GList* ianda_menu (NautilusMenuProvider *provider, GtkWidget* window, GList* files)
{
	GList* l = NULL;
	for (l = files; l; l = g_list_next(l)) {
		NautilusFileInfo* file = (NautilusFileInfo*) (l->data);
		GFile* f = nautilus_file_info_get_location(file);
		if (f) {
			char* loc = g_file_get_path(f);
			if (loc) {
				printf ("file: %s\n", loc);
				g_free (loc);
			}
			g_object_unref(f);
		}
	}

	if (files) {
		GList* mItemList = NULL;
		NautilusMenuItem* item = nautilus_menu_item_new("ianda -- test menu(name)", "ianda(label)", "ianda(tip)", NULL/*icon*/);
		mItemList = g_list_append(mItemList, item);
		g_signal_connect(item, "activate", G_CALLBACK(ianda_menu_test2), files);
		return mItemList;
	}

	return NULL;
}


GList* ianda_menu_background (NautilusMenuProvider* provider, GtkWidget* window, NautilusFileInfo* folderInfo)
{
	GFile* location = nautilus_file_info_get_location(folderInfo);
	char* loc = g_file_get_path(location);
	printf ("background path: %s\n", loc);
	GList* mItemList = NULL;
	NautilusMenuItem* item = nautilus_menu_item_new("ianda -- test menu(name)", "ianda(label)", "ianda(tip)", NULL/*icon*/);
	mItemList = g_list_append(mItemList, item);
	g_signal_connect(item, "activate", G_CALLBACK(ianda_menu_test1), loc);

	return mItemList;
}


static void ianda_menu_provider_iface_init(NautilusMenuProviderIface *iface)
{
	iface->get_file_items = ianda_menu;
	iface->get_background_items = ianda_menu_background;
}


static void ianda_sec_instance_init(IandaSecMenuProvider* object)
{
	printf ("instance init\n");
}

static void ianda_sec_class_init(IandaSecMenuProviderClass* klass)
{
	printf ("class init\n");
}


static void ianda_sec_register_type(GTypeModule* module)
{
	static const GTypeInfo info = {
		sizeof(IandaSecMenuProviderClass),
		(GBaseInitFunc) NULL,
		(GBaseFinalizeFunc) NULL,
		(GClassInitFunc) ianda_sec_class_init,
		NULL,
		NULL,
		sizeof(IandaSecMenuProvider),
		0,
		(GInstanceInitFunc)ianda_sec_instance_init,
	};

	gsIandaSecType = g_type_module_register_type(module, G_TYPE_OBJECT, "IandaSecMenuProvider", &info, 0);

	const GInterfaceInfo menuProviderIfaceInfo = {(GInterfaceInitFunc) ianda_menu_provider_iface_init, NULL, NULL};

	g_type_module_add_interface(module, gsIandaSecType, NAUTILUS_TYPE_MENU_PROVIDER, &menuProviderIfaceInfo);
}

