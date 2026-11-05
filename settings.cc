#include "settings.h"
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <iostream>

Settings::Settings()
	: Gtk::Box{Gtk::Orientation::VERTICAL},
		m_label{"Settings"}
{
	set_name("settings");

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
	m_content.set_name("settings-content");
	m_container.set_name("settings-container");
	m_label.set_name("settings-label");
    m_label.add_css_class("header");

	append(m_label);
	append(m_container);
}