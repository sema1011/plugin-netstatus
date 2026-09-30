# NetManager Plugin for LXQt Panel

A panel plugin that displays network connection status and speed using NetworkManager-Qt.

## Features

- Real-time network connection status display
- Upload/download speed monitoring
- Tooltips with connection details
- Configurable update interval
- Internationalization support (English, Russian)

## Requirements

- LXQt 1.0+
- Qt 6.x
- NetworkManager-Qt
- CMake 3.16+

## Building

```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$(lxqt-config --prefix)
make
sudo make install
```

## Installation

After building, install the plugin:

```bash
sudo make install
```

Then restart LXQt Panel or load the plugin through Panel Settings.

## Configuration

Right-click the plugin icon and select "Settings..." to configure:
- Show/hide speed indicator
- Show/hide tooltip
- Update interval (100-10000 ms)

## Translation

To update translation files:

```bash
lupdate -ts translations/template.ts src/*.h src/*.cpp
```

To compile translation files:

```bash
lrelease translations/template.ts translations/ru.ts
```

## License

This plugin is licensed under the GNU Lesser General Public License version 2.1 or later.

## See Also

- [LXQt Website](https://lxqt.org/)
- [NetworkManager-Qt](https://github.com/KDE/NetworkManagerQt)
