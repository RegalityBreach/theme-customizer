#!/usr/bin/env bash
set -euo pipefail

project_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
build_dir="$project_dir/build"
app_path="$build_dir/theme-customizer"

meson setup "$build_dir"
meson compile -C "$build_dir"

desktop_dir="$(xdg-user-dir DESKTOP 2>/dev/null || true)"
if [[ -z "$desktop_dir" ]]; then
	desktop_dir="$HOME/Desktop"
fi
applications_dir="${XDG_DATA_HOME:-$HOME/.local/share}/applications"
mkdir -p "$desktop_dir" "$applications_dir"

escape_exec_arg() {
	local value="$1"
	value="${value//\\/\\\\}"
	value="${value//\"/\\\"}"
	printf '%s' "$value"
}

exec_path="$(escape_exec_arg "$app_path")"
write_launcher() {
	local desktop_file="$1"
	cat > "$desktop_file" <<EOF
[Desktop Entry]
Version=1.0
Type=Application
Name=Theme Customizer
Comment=Customize wallpaper themes
Exec="$exec_path"
Path=$project_dir
Icon=preferences-system
Terminal=false
Categories=Settings;DesktopSettings;
EOF

	chmod +x "$desktop_file"
	printf 'Created launcher: %s\n' "$desktop_file"
}

write_launcher "$desktop_dir/theme-customizer.desktop"
write_launcher "$applications_dir/theme-customizer.desktop"

if command -v update-desktop-database >/dev/null 2>&1; then
	update-desktop-database "$applications_dir"
fi