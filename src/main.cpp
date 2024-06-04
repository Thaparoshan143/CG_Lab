#include"./DDALineAlgo.cpp"

int main()
{
	#ifdef EXPORT_ENABLED
	Application *global_app = Extra::export_app_instance();
	#else
	Application *global_app = nullptr;
	#endif

	if(global_app == nullptr)
	{
		std::cout << "Application instance not attached from outside";
	}
	else
	{
		global_app->Initialize();
		global_app->Loop();
	}

	return 0;
}