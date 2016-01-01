#include "mdiCtrl.hpp"
#include <axLib/axOSFileSystem.h>

namespace mdi {
Ctrl::Ctrl()
{
}

/*
 * Control factory.
 */
std::unique_ptr<CtrlFactory> CtrlFactory::_instance = nullptr;

CtrlFactory* CtrlFactory::GetInstance()
{
	if (!_instance) {
		_instance = std::unique_ptr<CtrlFactory>(new CtrlFactory());
	}
	return _instance.get();
}

CtrlFactory::CtrlFactory()
{
	LoadAllPlugins();
}

void CtrlFactory::LoadAllPlugins()
{
	ax::os::Directory dir;
	dir.Goto("plugin");
	
	ax::StringVector folders_name;
	for(auto& n : dir.GetContent()) {
		ax::Print("Folder :", n.name);
		
		if(n.type == ax::os::File::FOLDER) {
			ax::os::Directory plugin_folder;
			plugin_folder.Goto("plugin/" + n.name);
			for(auto& plugin_file : plugin_folder.GetContent()) {
				if(plugin_file.ext == "ctrl") {
					const std::string plugin_path("plugin/" + n.name + "/" + plugin_file.name);
					
					ax::Print("PLUGIN PATH :", plugin_path);
					
					_builder_map.insert(std::pair<std::string, PluginPtr>(n.name,
						PluginPtr(new Plugin(plugin_path, "Create"))));
				}
			}
		}
	}
}

Ctrl::Ptr CtrlFactory::Create(
	const ax::Point& pos, const std::string& ctrl_name, const ax::StringPairVector& attributes)
{
	auto it = _builder_map.find(ctrl_name);
	
	if(it != _builder_map.end()) {
		ax::Print("Found knob");
		return Ctrl::Ptr(it->second->GetFunctionHandle()(pos, attributes));
	}
	
	return nullptr;
}
}