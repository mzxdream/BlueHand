#include "BlueHandApp.h"
#include <iostream>
#include <gtkmm/main.h>

int main(int argc, char *argv[])
{
    Gtk::Main kit(argc, argv);
    do
    {
	if (!BlueHandApp::Instance().Init(argc, argv))
	{
	    std::cerr << "app init failed" << std::endl;
	    break;
	}
	if (!BlueHandApp::Instance().Run())
	{
   	    std::cerr << "app run failed" << std::endl;
	    break;
	}
    } while(false);
    BlueHandApp::Instance().Clear();
    
    return 0;
}
