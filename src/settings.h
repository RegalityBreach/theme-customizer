#pragma once

#include <gtkmm/box.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/label.h>
#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/cssprovider.h>
#include <glibmm/main.h>
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
	void SetCssTheme(const Glib::RefPtr<Gtk::CssProvider>& css_provider);
	std::string GetWallpapersFolderPath() const;
	std::string GetResultFolderPath() const;
	std::string GetWallpaperReloadCommand() const;
private:
	Gtk::Label m_label;
	Gtk::ScrolledWindow m_container;
	Gtk::Box m_content;

	// FOLDERS
	Gtk::Label m_folders_label{"Default Folders"};
	Gtk::Box m_folders{Gtk::Orientation::VERTICAL};
	// Respective folder entries:
	std::pair<std::string, std::string> folder_entries[2] = {
		{"Default Wallpapers Folder", "WallpapersFolder"},
		{"Default Result Folder", "ResultFolder"}
	};
	std::map<std::string, std::string> config = {
        {"WallpapersFolder", ""},
        {"ResultFolder", ""},
		{"WallpaperReloadCommand", ""},
		{"Theme", "light"}
	};
	std::pair<std::string, std::string> reload_command_entries[2] = {
		{"Swaybg", "swaymsg reload"},
		{"Hyprpaper", "pkill hyprpaper; hyprpaper"}
	};

	// RELOAD COMMAND
	Gtk::Label m_reload_label{"Wallpaper Reload Command"};
	Gtk::Box m_reload{Gtk::Orientation::VERTICAL};
	Gtk::Box m_reload_custom{Gtk::Orientation::HORIZONTAL};
	Gtk::Label m_reload_custom_label{"Enter Reload Command:"};
	Gtk::Entry m_reload_custom_entry;

	Gtk::Box m_reload_template{Gtk::Orientation::HORIZONTAL};
	Gtk::Label m_reload_template_label{"Use Template:"};

	// THEME
	Gtk::Label m_theme_label{"Theme"};
	Gtk::Box m_theme{Gtk::Orientation::VERTICAL};
	Gtk::Box m_theme_template{Gtk::Orientation::HORIZONTAL};
	Gtk::Label m_theme_template_label{"Use Theme:"};
	Gtk::Button m_theme_light{"Light"};
	Gtk::Button m_theme_dark{"Dark"};
	const std::string m_config_file;
};