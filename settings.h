#pragma once

#include <gtkmm/box.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/label.h>
#include <glibmm/main.h>
#include <gtkmm/button.h>
#include <string>
#include <map>

class Settings : public Gtk::Box
{
public:
	Settings();
	~Settings() override = default;

	// Selecting files.
	void Select(const std::string& folder_key, Gtk::Label& entry_info, Gtk::Button& entry_button);
	void SaveConfig() const;
	void LoadConfig();
	std::string GetWallpapersFolderPath() const;
	std::string GetResultFolderPath() const;
	std::string GetWallpaperReloadCommand() const { return m_wallpaper_reload_command; }
private:
	Gtk::Label m_label;
	Gtk::ScrolledWindow m_container;
	Gtk::Box m_content;

	// FOLDERS
	Gtk::Label m_folders_label{"Default Folders"};
	Gtk::Box m_folders{Gtk::Orientation::VERTICAL};
	// Respective folder entries:
	std::array<std::string, 2> folder_entries[3] = {
		{"Default Wallpapers Folder", "wallpapers_folder"},
		{"Default Result Folder", "result_folder"}
	};
	std::map<std::string, std::string> folders = {
		{"wallpapers_folder", ""},
		{"result_folder", ""}
	};
	std::map<std::string, std::string> config = {
        {"WallpapersFolder", "wallpapers_folder"},
        {"ResultFolder", "result_folder"},
	};

	// RELOAD COMMAND
	Gtk::Label m_reload_label{"Wallpaper Reload Command"};
	Gtk::Box m_reload;
	Gtk::Label m_reload_custom_label{"Enter Reload Command:"};
	std::string m_wallpaper_reload_command = "";
	const std::string m_config_file;
};