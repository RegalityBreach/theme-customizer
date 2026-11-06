#pragma once

#include "wallpapers.h"
#include "settings.h"
#include <gtkmm/paned.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/stack.h>
#include <gtkmm/image.h>

class MainPaned : public Gtk::Paned
{
public:
	MainPaned();
	~MainPaned() override = default;
private:
	Gtk::Box m_sidebar{Gtk::Orientation::VERTICAL};
	Gtk::Stack m_stack;

	// Sidebar buttons
	Gtk::Label m_header_label{"Theme Customizer"};
	Gtk::Button m_button_wallpapers;

	// settings
	Gtk::Box m_options_container{Gtk::Orientation::VERTICAL};
	Gtk::Button m_options;
	Gtk::Image m_gear_icon;

	// Stack windows
	Settings m_settings;
	Wallpapers m_wallpapers{m_settings};
};