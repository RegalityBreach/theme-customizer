#pragma once

#include <gtkmm/box.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/label.h>
#include <glibmm/main.h>
#include <string>

class Settings : public Gtk::Box
{
public:
	Settings();
	~Settings() override = default;
private:
	Gtk::Label m_label;
	Gtk::ScrolledWindow m_container;
	Gtk::Box m_content;
};