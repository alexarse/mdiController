#ifndef __MDI_CTRL_H__
#define __MDI_CTRL_H__

#include <axLib/axLib.h>
#include <axLib/axOSDynamic.h>
#include <map>

namespace mdi {

//template <typename R> class PluginLib {
//public:
//	typedef std::function<R()> EntryFunction;
//
//	PluginLib(const std::string& path, const std::string& fct_name)
//	{
//		_binHandle = dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL);
//
//		if (_binHandle != nullptr) {
//			_fctHandle = reinterpret_cast<R (*)()>(dlsym(_binHandle, fct_name.c_str()));
//		}
//		else {
//			std::cerr << "failed to load library" << std::endl;
//		}
//	}
//
//	PluginLib()
//	{
//		dlclose(_binHandle);
//	}
//
//	EntryFunction GetFunctionHandle()
//	{
//		return _fctHandle;
//	}
//
//private:
//	void* _binHandle;
//	EntryFunction _fctHandle;
//};

class Midi {
public:
	enum MessageType { CHANNEL_VOICE, CHANNEL_MODE, SYSTEM_COMMON, SYSTEM_REAL_TIME, SYSTEM_EXCLUSIVE };

	Midi()
	{
	}

private:
	unsigned char _channel;
	MessageType _msg_type;
};

//	class Abstract {
//	public:
//		virtual void do() = 0;
//	};
//
//	class Factory {
//	public:
//		Abstract *create();
//	};
//
//	Factory f;
//	Abstract *a = f.create();
//	a->do();

class Ctrl {
public:
	typedef std::shared_ptr<Ctrl> Ptr;
	enum ControlType { CTRL_BUTTON, CTRL_TOGGLE, CTRL_SLIDER, CTRL_KNOB, CTRL_XY, CTRL_CUSTOM };

	//`Ctrl();
	
	virtual ~Ctrl()
	{
		
	}
	

	virtual std::shared_ptr<ax::Window::Backbone> GetBackbone() = 0;

private:
	ControlType _type;
	Midi _midi;
};

class CtrlFactory {
public:
	typedef ax::os::DynamicLib<Ctrl*, const ax::Point&, const ax::StringPairVector&> Plugin;
	typedef std::unique_ptr<Plugin> PluginPtr;
	
	static CtrlFactory* GetInstance();

	Ctrl::Ptr Create(
		const ax::Point& pos, const std::string& ctrl_name, const ax::StringPairVector& attributes);

private:
	CtrlFactory();
	static std::unique_ptr<CtrlFactory> _instance;

	std::map<std::string, PluginPtr> _builder_map;

	void LoadAllPlugins();

	

	//	std::vector<PluginPtr> _builder_plugins;
};
}

#endif // __MDI_CTRL_H__
