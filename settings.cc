#include "settings.h"
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/filedialog.h>
#include <glibmm.h>
#include <iostream>
#include <filesystem>

Settings::Settings()
	: Gtk::Box{Gtk::Orientation::VERTICAL},
		m_label{"Settings"},
        m_config_file(Glib::build_filename(Glib::get_user_config_dir(), "theme-customizer", "theme-customizer.ini"))
{
    std::filesystem::create_directories(
        std::filesystem::path(m_config_file).parent_path()
    );

	set_name("settings");

    set_halign(Gtk::Align::FILL);

	m_content.set_orientation(Gtk::Orientation::VERTICAL);
	m_content.set_vexpand(true);
    m_content.set_hexpand(true);
    m_content.set_halign(Gtk::Align::FILL);

    m_folders_label.set_halign(Gtk::Align::START);
    m_folders_label.set_name("folders-label");
    m_folders_label.add_css_class("header");
    m_content.append(m_folders_label);

    m_folders.add_css_class("entry");
    m_content.append(m_folders);

    LoadConfig();

    // Respective folder entries

    for (int i = 0; i < 2; ++i) {
        auto entry = folder_entries[i];

        auto* entry_box = Gtk::manage(new Gtk::Box(Gtk::Orientation::VERTICAL));
        std::string folder_key = entry.at(1);

        // Label
        auto* entry_label = Gtk::manage(new Gtk::Label(entry[0]));
        entry_label->set_halign(Gtk::Align::START);
        entry_label->set_hexpand(true);
        entry_box->append(*entry_label);

        // Content box
        auto* entry_content = Gtk::manage(new Gtk::Box(Gtk::Orientation::HORIZONTAL));
        entry_content->set_halign(Gtk::Align::FILL);
        entry_content->set_hexpand(true);
        entry_box->append(*entry_content);

        // Info label
        auto* entry_info = Gtk::manage(new Gtk::Label(folders[folder_key] == "" ? "Folder not selected" : folders[folder_key]));
        entry_info->set_halign(Gtk::Align::START);
        entry_info->set_valign(Gtk::Align::CENTER);
        entry_info->set_hexpand(true);
        entry_content->append(*entry_info);

        // Select button
        auto* entry_button = Gtk::manage(new Gtk::Button("Select"));
        entry_button->set_halign(Gtk::Align::END);
        entry_button->set_valign(Gtk::Align::CENTER);
        entry_button->set_hexpand(false);
        entry_content->append(*entry_button);

        entry_button->signal_clicked().connect([this, folder_key, entry_info, entry_button] {
            Select(folder_key, *entry_info, *entry_button);
        });

        // CSS
        entry_label->add_css_class("header");
        entry_box->add_css_class("folder-entry");

        m_folders.append(*entry_box);
    }

    // Reload
    m_reload_label.set_halign(Gtk::Align::START);
    m_reload_label.set_name("reload-label");
    m_reload_label.add_css_class("header");
    m_content.append(m_reload_label);

    m_reload_custom_label.set_halign(Gtk::Align::START);
    m_reload_custom_label.set_hexpand(true);
    m_reload.append(m_reload_custom_label);

    m_reload.add_css_class("entry");
    m_content.append(m_reload);

	m_container.set_child(m_content);
    m_container.set_hexpand(true);
    m_container.set_halign(Gtk::Align::FILL);
    m_container.set_policy(
        Gtk::PolicyType::NEVER,
        Gtk::PolicyType::AUTOMATIC
    );

    m_label.set_hexpand(true);

    // CSS styling for the container and label
	m_content.set_name("settings-content");
	m_container.set_name("settings-container");
	m_label.set_name("settings-label");
    m_label.add_css_class("header");

	append(m_label);
	append(m_container);
}

std::string Settings::GetWallpapersFolderPath() const {
    return folders.at("wallpapers_folder");
}

std::string Settings::GetResultFolderPath() const {
    return folders.at("result_folder");
}

void Settings::Select(const std::string& folder_key, Gtk::Label& entry_info, Gtk::Button& entry_button) {
    entry_button.set_sensitive(false);

    auto dialog = Gtk::FileDialog::create();
    dialog->set_title("Select Folder");

    dialog->select_folder([this, dialog, &entry_button, folder_key, &entry_info](const auto& result) {
        std::string path;
        try {
            auto file = dialog->select_folder_finish(result);
            if (file) {
                path = file->get_path();

                std::cout << "Selected folder for " << folder_key << ": " << path << std::endl;
                folders[folder_key] = path;
                entry_info.set_text(path);
                SaveConfig();
            }
        } catch (const Glib::Error& err) {
            std::cerr << "Cancelled or error: " << err.what() << std::endl;
        }

        entry_button.set_sensitive(true);
    });
}

void Settings::SaveConfig() const {
    auto key_file = Glib::KeyFile::create();

    for (const auto& [key, value] : config) {
        key_file->set_string("Paths", key, value);
    }

    key_file->set_string("Config", "WallpaperReloadCommand", m_wallpaper_reload_command);

    try {
        key_file->save_to_file(m_config_file);
    } catch (const Glib::Error& err) {
        std::cerr << "Failed to save config: " << err.what() << std::endl;
    }
}

void Settings::LoadConfig() {
    auto key_file = Glib::KeyFile::create();
    try {
        key_file->load_from_file(m_config_file);

        for (const auto& [key, folder_key] : config) {
            try {
                std::string path = key_file->get_string("Paths", key);
                folders[folder_key] = path;
            } catch (const Glib::Error& err) {
                std::cerr << "Failed to load " << key << ": " << err.what() << std::endl;
            }
        }

        try {
            m_wallpaper_reload_command = key_file->get_string("Config", "WallpaperReloadCommand");
        } catch (const Glib::Error& err) {
            std::cerr << "Failed to load WallpaperReloadCommand: " << err.what() << std::endl;
        }

    } catch (const Glib::Error& ex) {
        // File doesn't exist yet, which is fine on first launch
    }
}