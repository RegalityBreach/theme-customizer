#pragma once

#include "settings.h"
#include <gtkmm/box.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/label.h>
#include <glibmm/main.h>
#include <string>

class Wallpapers : public Gtk::Box
{
public:
	Wallpapers(Settings& settings);
	~Wallpapers() override = default;

	void PopulateContainer();
	bool IsLoading() const;
	void SetLoading(bool toggle);
private:
	Gtk::Label m_label;
	Gtk::ScrolledWindow m_container;
	Gtk::Box m_content;
	bool is_loading = false;
	struct WallpaperItem {
		std::string name;
		std::string path;
		Glib::RefPtr<Gdk::Pixbuf> pixbuf;
	};
	void ConvertToPng(std::string filename, std::string output_path) const;
	Settings& m_settings;
};