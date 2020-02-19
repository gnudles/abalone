#include <webkit2/webkit-web-extension.h>


static void
sayHello( gpointer usr_data)
{
	printf("hello\n");
}
static void
web_page_document_loaded (WebKitWebPage *web_page,
               gpointer       user_data)
{
WebKitFrame * web_frame = webkit_web_page_get_main_frame (web_page);
	JSCContext * jscContext = 	webkit_frame_get_js_context (web_frame);


	g_autoptr(JSCValue) function = jsc_value_new_function (jscContext,
	                        "sayHello",
	                        G_CALLBACK(sayHello),
	                        0,
	                        NULL,
	                        G_TYPE_NONE,
	                        0
	                        );
	    jsc_context_set_value(jscContext, "sayHello", function);
}
static  void
web_page_created_callback (WebKitWebExtension *extension,
                           WebKitWebPage      *web_page,
                           gpointer            user_data)
{
    g_print ("Page %ld created for %s\n", 
             webkit_web_page_get_id (web_page),
             webkit_web_page_get_uri (web_page));
    g_signal_connect (web_page, "document-loaded",
                      G_CALLBACK (web_page_document_loaded), 
                      NULL);


	
}

extern "C" G_MODULE_EXPORT  void
webkit_web_extension_initialize (WebKitWebExtension *extension)
{
    g_print ("in webkit_web_extension_initialize...\n");
	g_signal_connect (extension, "page-created",
                      G_CALLBACK (web_page_created_callback), 
                      NULL);
}
