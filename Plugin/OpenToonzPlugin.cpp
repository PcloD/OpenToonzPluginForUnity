#include "pch.h"
#include "OpenToonzPlugin.h"
#include "fcFoundation.h"
#include "otPlugin.h"


std::map<std::string, otModule*> g_plugins;


otCLinkage otExport otImage* otImageCreate(int width, int height)
{
    return new ImageRGBAu8(width, height);
}

otCLinkage otExport otImage* otImageCreateIntrusive(void *data, int width, int height)
{
    return new ImageIntrusiveRGBAu8((RGBAu8*)data, width, height);
}

otCLinkage otExport void otImageDestroy(otImage *img)
{
    delete img;
}

otCLinkage otExport void otImageGetData(otImage *img, otImageData *data)
{
    data->width = img->getWidth();
    data->height = img->getHeight();
    data->data = img->getData();
}


fcCLinkage fcExport otModule* otLoad(const char *path)
{
    auto i = g_plugins.find(path);
    if (i != g_plugins.end()) {
        return i->second;
    }

    otModule *plugin = new otModule();
    if (!plugin->load(path)) {
        delete plugin;
        return nullptr;
    }
    g_plugins[path] = plugin;
    return plugin;
}

fcCLinkage fcExport void otUnload(otModule *plugin)
{
    // do nothing
}


fcCLinkage fcExport int otGetNumPlugins(otModule *mod)
{
    if (!mod) { return 0; }
    return mod->getNumPlugins();
}

fcCLinkage fcExport otPlugin* otGetPlugin(otModule *mod, int i)
{
    if (!mod) { return nullptr; }
    return mod->getPlugin(i);
}


otCLinkage otExport void otGetPluginInfo(otPlugin *plugin, otPluginInfo *dst)
{
    if (!plugin || !dst) { return; }
    *dst = plugin->getPluginInfo();
}

fcCLinkage fcExport int otGetNumParams(otPlugin *plugin)
{
    if (!plugin) { return 0; }
    return plugin->getNumParams();
}

otCLinkage otExport void otGetParamInfo(otPlugin *plugin, int i, otParamInfo *pinfo)
{
    if (!plugin || !pinfo) { return; }
    *pinfo = plugin->getParam(i)->getRawInfo();
}

otCLinkage otExport int otGetParamLength(otPlugin *plugin, int i)
{
    if (!plugin) { return 0; }
    return plugin->getParam(i)->getLength();
}

otCLinkage otExport void otGetParamValue(otPlugin *plugin, int i, void *dst)
{
    if (!plugin) { return; }
    plugin->getParam(i)->getValue(dst);
}

otCLinkage otExport void otSetParamValue(otPlugin *plugin, int i, const void *src)
{
    if (!plugin) { return; }
    plugin->getParam(i)->setValue(src);
}

fcCLinkage fcExport otImage* otApplyFx(otPlugin *plugin, otImage *src, double frame)
{
    if (!plugin) { return nullptr; }
    return plugin->applyFx(src, frame);
}