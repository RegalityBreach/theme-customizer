#include "wallpapers.h"
#include "settings.h"
#include <pangomm.h>
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/picture.h>
#include <gtkmm/button.h>
#include <gdkmm/pixbuf.h>
#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <glibmm.h>
#include <thread>
#include <vector>
#include <utility>

Wallpapers::Wallpapers(Settings& settings)
	: Gtk::Box{Gtk::Orientation::VERTICAL},
		m_label{"Select a wallpaper."},
		m_settings{settings}
{
	set_name("wallpapers");

    set_halign(Gtk::Align::FILL);

	m_content.set_orientation(Gtk::Orientation::VERTICAL);
	m_content.set_vexpand(true);
    m_content.set_hexpand(true);
    m_content.set_halign(Gtk::Align::FILL);

	m_container.set_child(m_content);
    m_container.set_hexpand(true);
    m_container.set_halign(Gtk::Align::FILL);
    m_container.set_policy(
        Gtk::PolicyType::NEVER,
        Gtk::PolicyType::AUTOMATIC
    );

    m_label.set_hexpand(true);

    // CSS styling for the container and label
	m_content.set_name("wallpapers-content");
	m_container.set_name("wallpapers-container");
	m_label.set_name("wallpapers-label");
    m_label.add_css_class("header");

	append(m_label);
	append(m_container);
}

void Wallpapers::ConvertToPng(std::string filename, std::string output_path) const {
    std::string command = "ffmpeg -y -i \"" + filename + "\" \"" + output_path + "/wallpaper.png\"";
    if (std::system(command.c_str()) == 0) {
        std::system(m_settings.GetWallpaperReloadCommand().c_str());
    }
}

void Wallpapers::PopulateContainer()
{
    std::string wallpaper_folder = m_settings.GetWallpapersFolderPath();
    std::string result_folder = m_settings.GetResultFolderPath();

    if (IsLoading()) {
        return; // Prevent multiple simultaneous loads
    }

    if (wallpaper_folder == "") {
        auto* loading_label = Gtk::manage(new Gtk::Label("Please select a valid wallpapers folder in the settings."));
        loading_label->set_name("loading-label");
        m_content.append(*loading_label);
        return;
    }

    if (result_folder == "") {
        auto* loading_label = Gtk::manage(new Gtk::Label("Please select a valid result folder in the settings."));
        loading_label->set_name("loading-label");
        m_content.append(*loading_label);
        return;
    }

    SetLoading(true);

    for (auto const& old_entry : m_content.get_children()) {
        old_entry->unparent();
    }

    auto* loading_label = Gtk::manage(new Gtk::Label("Loading wallpapers... Please wait"));
    loading_label->set_name("loading-label");
    m_content.append(*loading_label);

    std::thread([this, loading_label, wallpaper_folder, result_folder]() {
        try {
            std::vector<WallpaperItem> items;

            if (std::filesystem::exists(wallpaper_folder)) {
                for (auto const& entry : std::filesystem::directory_iterator(wallpaper_folder)) {
                    try {
                        if (entry.is_regular_file()) {
                            auto pixbuf = Gdk::Pixbuf::create_from_file(entry.path().string(), 100, 100, true);
                            items.push_back({entry.path().filename().string(), entry.path().string(), pixbuf});
                        }
                    } catch (...) {
                        // Skip unreadable files safely
                    }
                }
            }

            Glib::signal_idle().connect_once([this, items = std::move(items), loading_label, result_folder]() {
                for (auto const& item : items) {
                    auto* entry_box = Gtk::manage(new Gtk::Box(Gtk::Orientation::HORIZONTAL));
                    entry_box->add_css_class("wallpaper-entry");
                    entry_box->add_css_class("entry");
                    entry_box->set_hexpand(true);
                    entry_box->set_halign(Gtk::Align::FILL);

                    auto* name_label = Gtk::manage(new Gtk::Label(item.name));
                    name_label->set_hexpand(true);
                    name_label->set_halign(Gtk::Align::FILL);
                    name_label->set_valign(Gtk::Align::CENTER);

                    name_label->set_ellipsize(Pango::EllipsizeMode::MIDDLE);
                    name_label->set_single_line_mode(true);

                    auto* picture = Gtk::manage(new Gtk::Picture(item.pixbuf));
                    picture->set_content_fit(Gtk::ContentFit::CONTAIN);
                    picture->set_size_request(100, 75);
                    picture->set_can_shrink(true);

                    auto* button = Gtk::manage(new Gtk::Button("Apply"));
                    button->set_halign(Gtk::Align::END);
                    button->set_valign(Gtk::Align::CENTER);
                    button->set_hexpand(false);
                    button->signal_clicked().connect([this, item, result_folder]() {
                        ConvertToPng(item.path, result_folder);
                    });

                    entry_box->append(*picture);
                    entry_box->append(*name_label);
                    entry_box->append(*button);
                    m_content.append(*entry_box);
                }

                loading_label->unparent();
                SetLoading(false);
            });

        } catch (const std::exception& e) {
            std::cerr << "Error loading wallpapers: " << e.what() << std::endl;
            
            Glib::signal_idle().connect_once([this, loading_label]() {
                loading_label->unparent();
                SetLoading(false);
            });
        }
    }).detach();
}

bool Wallpapers::IsLoading() const {
	return is_loading;
}

void Wallpapers::SetLoading(bool toggle){
	is_loading = toggle;
}