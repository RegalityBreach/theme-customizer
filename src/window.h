#pragma once

#include "main_paned.h"
#include <gtkmm/window.h>
#include <gtkmm/box.h>

class MainWindow : public Gtk::Window
{
public:
	MainWindow();
	~MainWindow() override = default;
private:
	MainPaned m_paned;
};