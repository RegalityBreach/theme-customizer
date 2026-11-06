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
}