#include "settings.h"
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/filedialog.h>
#include <gtkmm/cssprovider.h>
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

    const auto css_provider = Gtk::CssProvider::create();

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
    SetCssTheme(css_provider);

    // Respective folder entries

    for (int i = 0; i < 2; ++i) {
        auto entry = folder_entries[i];

        auto* entry_box = Gtk::manage(new Gtk::Box(Gtk::Orientation::VERTICAL));
        std::string folder_key = entry.second;

        // Label
        auto* entry_label = Gtk::manage(new Gtk::Label(entry.first));
        entry_label->set_halign(Gtk::Align::START);
        entry_label->set_hexpand(true);
        entry_box->append(*entry_label);

        // Content box
        auto* entry_content = Gtk::manage(new Gtk::Box(Gtk::Orientation::HORIZONTAL));
        entry_content->set_halign(Gtk::Align::FILL);
        entry_content->set_hexpand(true);
        entry_box->append(*entry_content);

        // Info label
        auto* entry_info = Gtk::manage(new Gtk::Label(config.at(folder_key) == "" ? "Folder not selected" : config.at(folder_key)));
        entry_info->set_halign(Gtk::Align::START);
        entry_info->set_valign(Gtk::Align::CENTER);
        entry_info->set_hexpand(true);
        entry_content->append(*entry_info);

        // Select button
        auto* entry_button = Gtk::manage(new Gtk::Button("Select"));
        entry_button->set_halign(Gtk::Align::END);
        entry_button->set_valign(Gtk::Align::CENTER);
        entry_button->set_hexpand(false);
        entry_button->add_css_class("inline-button");
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

    m_reload.append(m_reload_custom);

    m_reload_custom_label.set_halign(Gtk::Align::START);
    m_reload_custom_label.set_hexpand(true);
    m_reload_custom_label.set_valign(Gtk::Align::CENTER);
    m_reload_custom.append(m_reload_custom_label);

    m_reload_custom_entry.set_text(config.at("WallpaperReloadCommand"));
    m_reload_custom_entry.set_valign(Gtk::Align::CENTER);
    m_reload_custom_entry.signal_changed().connect([this]() {
        std::string text = m_reload_custom_entry.get_text();
        config["WallpaperReloadCommand"] = text;
        SaveConfig();
    });
    m_reload_custom.append(m_reload_custom_entry);

    m_reload.append(m_reload_template);

    m_reload_template_label.set_halign(Gtk::Align::START);
    m_reload_template_label.set_hexpand(true);
    m_reload_template_label.set_valign(Gtk::Align::CENTER);
    m_reload_template.append(m_reload_template_label);

    for (int i = 0; i < 2; ++i) {
        std::pair<std::string, std::string> template_entry = reload_command_entries[i];

        auto* button = Gtk::manage(new Gtk::Button(template_entry.first));
        button->set_valign(Gtk::Align::CENTER);
        button->add_css_class("inline-button");
        button->add_css_class("left");
        button->signal_clicked().connect([this, template_entry]() {
            m_reload_custom_entry.set_text(template_entry.second);
            config["WallpaperReloadCommand"] = template_entry.second;
            SaveConfig();
        });
        m_reload_template.append(*button);
    }

    m_reload.add_css_class("entry");
    m_content.append(m_reload);

    // Theme
    m_theme_label.set_halign(Gtk::Align::START);;
    m_theme_label.add_css_class("header");
    m_content.append(m_theme_label);

    m_theme_template_label.set_halign(Gtk::Align::START);
    m_theme_template_label.set_hexpand(true);
    m_theme_template_label.set_valign(Gtk::Align::CENTER);
    m_theme_template.append(m_theme_template_label);

    m_theme_light.add_css_class("inline-button");
    m_theme_light.add_css_class("left");
    m_theme_light.signal_clicked().connect([this, css_provider]() {
        config["Theme"] = "light";
        SaveConfig();
        SetCssTheme(css_provider);
    });

    m_theme_dark.add_css_class("inline-button");
    m_theme_dark.add_css_class("left");
    m_theme_dark.signal_clicked().connect([this, css_provider]() {
        config["Theme"] = "dark";
        SaveConfig();
        SetCssTheme(css_provider);
    });

    m_theme_template.append(m_theme_light);
    m_theme_template.append(m_theme_dark);
    m_theme.append(m_theme_template);

    m_theme.add_css_class("entry");
    m_content.append(m_theme);

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
    return config.at("WallpapersFolder");
}

std::string Settings::GetResultFolderPath() const {
    return config.at("ResultFolder");
}

std::string Settings::GetWallpaperReloadCommand() const {
    return config.at("WallpaperReloadCommand");
}

void Settings::SetCssTheme(const Glib::RefPtr<Gtk::CssProvider>& css_provider) {
    std::string user_css_path = std::string(g_get_user_config_dir()) + "/theme-customizer/style-" + config.at("Theme") + ".css";
    std::string css_path = user_css_path;

    if (std::filesystem::exists(user_css_path)) {
        std::cout << "Loaded CSS from user directory." << "\n";
    } else {
        css_path = "css/style-" + config.at("Theme") + ".css";
        std::cout << "Loaded CSS from fallback directory." << "\n";
    }

    css_provider->load_from_path(css_path);

    Glib::RefPtr<Gtk::StyleProvider> style_provider = css_provider;
    Gtk::StyleProvider::add_provider_for_display(
        get_display(),
        style_provider,
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );
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
                config[folder_key] = path;
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
        key_file->set_string("Config", key, value);
    }

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

        for (const auto& [key, value] : config) {
            try {
                std::string path = key_file->get_string("Config", key);
                config[key] = path;
            } catch (const Glib::Error& err) {
                std::cerr << "Failed to load " << key << ": " << err.what() << std::endl;
            }
        }
    } catch (const Glib::Error& ex) {
        // File doesn't exist yet, which is fine on first launch
    }
}