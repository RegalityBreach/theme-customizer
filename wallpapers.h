#pragma once

#include <gtkmm/box.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/label.h>
#include <glibmm/main.h>
#include <string>

class Wallpapers : public Gtk::Box
{
public:
	Wallpapers();
	~Wallpapers() override = default;

	void PopulateContainer();
	bool IsLoading() const;
	void SetLoading(bool toggle);
private:
	Gtk::Label m_label;
	Gtk::ScrolledWindow m_container;
	Gtk::Box m_content;
	std::string wallpaper_folder = "/home/opsec-warrior/.config/wallpapers";
	std::string result_folder = "/home/opsec-warrior/.config";
	bool is_loading = false;
	struct WallpaperItem {
		std::string name;
		Glib::RefPtr<Gdk::Pixbuf> pixbuf;
	};
};