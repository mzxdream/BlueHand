#include "BhClientApp.h"
#include <iostream>
#include <gtkmm/main.h>

int main(int argc, char *argv[])
{
    Gtk::Main kit(argc, argv);

    if (!BhClientApp::Instance().Init())
    {
	return 0;
    }
    if (!BhClientApp::Instance().Run())
    {
	return 0;
    }
    std::cin.get();
    BhClientApp::Instance().Clear();
    
    return 0;
}
