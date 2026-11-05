#include "window.h"
#include <iostream>
#include <string>
#include <glib.h>
#include <gtkmm/cssprovider.h>
#include <filesystem>

MainWindow::MainWindow() {
	set_child(m_paned);
	set_default_size(700, 500);

	m_paned.set_hexpand(true);

	auto css_provider = Gtk::CssProvider::create();

	std::string user_css_path = std::string(g_get_user_config_dir()) + "/sway-theme-customizer/style.css";
    
	if (std::filesystem::exists(user_css_path)) {
		css_provider->load_from_path(user_css_path);
    	std::cout << "Loaded CSS from user directory." << "\n";
	} else {
		css_provider->load_from_path("style.css");
    	std::cout << "Loaded CSS from fallback directory." << "\n";
	}

    Gtk::StyleProvider::add_provider_for_display(
        get_display(),
        css_provider,
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );
}