# Theme Customizer

Theme Customizer is a GTK 4 desktop application for browsing wallpaper images, converting a selected image to PNG, and running a wallpaper reload command. It is designed for Linux desktop setups, with built-in reload command templates for Sway and Hyprpaper.

## Requirements

- A C++17 compiler
- Meson and Ninja
- GTKmm 4 development files (`gtkmm-4.0`)
- FFmpeg (`ffmpeg`) for wallpaper conversion
- `xdg-user-dir` is optional and is used by the build script to find the Desktop folder

Install the packages using your distribution's package manager. For example, on Debian or Ubuntu the main development dependencies are `g++`, `meson`, `ninja-build`, and `libgtkmm-4.0-dev`; install `ffmpeg` as well to apply wallpapers.

## Build and Run

To build the application and create desktop and application-menu launchers, run:

```sh
./build.sh
```

The executable is written to `build/theme-customizer`. You can also build directly with Meson:

```sh
meson setup build
meson compile -C build
./build/theme-customizer
```

The launcher script places `theme-customizer.desktop` on the Desktop (or `$HOME/Desktop` if `xdg-user-dir` is unavailable) and in `$XDG_DATA_HOME/applications` or `~/.local/share/applications`.

## Use

1. Open **Settings** and select a folder containing wallpaper images and a result folder.
2. Set a wallpaper reload command, or choose the Sway or Hyprpaper template.
3. Open the wallpaper list and select **Apply** next to an image.

The selected image is converted to `wallpaper.png` in the result folder. After a successful conversion, the configured reload command is started. Choose a reload command appropriate for your desktop environment; the command runs with your user permissions.

The light theme is selected by default. Choose **Light** or **Dark** in Settings to switch the interface theme.

## Configuration

Settings are saved to:

```text
~/.config/theme-customizer/theme-customizer.ini
```

The application also looks for custom theme stylesheets at `~/.config/theme-customizer/style-light.css` and `~/.config/theme-customizer/style-dark.css`. If a custom stylesheet is not present, the corresponding stylesheet in the repository's `css/` directory is used.
