# dwmbar-k
[![CMake](https://img.shields.io/github/actions/workflow/status/kiddersmxj/dwmbar-k/cmake.yml?style=for-the-badge)](https://github.com/kiddersmxj/dwmbar-k/actions/workflows/cmake.yml)

A multi-threaded status-bar backend for [dwm](https://dwm.suckless.org/),
written in C++. Each module runs on its own thread, publishes its
output through a thread-safe `Module` base class, and the main loop
collects, orders and writes the combined string to the X root window
via `XStoreName` (the standard "XSetRoot" status mechanism). Designed
to be used with the dwm `statuscolors` / multi-bar patches so coloured
icons and per-module separators render correctly.

## Modules

| Module        | Shows                                                |
|---------------|------------------------------------------------------|
| `network`     | Active interface / SSID + private + public IP        |
| `time`        | Day, date, second-aligned clock                      |
| `weather`     | Local weather                                        |
| `bluetooth`   | Connected device name                                |
| `media`       | Player title / artist + position via `playerctl`     |
| `battery`     | Charge %, charge-state icon + animation              |
| `volume`      | PipeWire sink volume (via `wpctl`, supports >100%)   |
| `signal`      | Wi-Fi signal strength + up/down throughput           |
| `cpu`         | Aggregate CPU usage, tier-coloured (green/yellow/red)|
| `throttling`  | Thermal-throttle indicator                           |
| `brightness`  | Backlight % (sysfs, fallback `brightnessctl`)        |
| `winconnect`  | Indicator when a Windows machine is reachable        |
| `chronolog`   | Active timer from chronolog with cooldown            |
| `memory`      | RAM and swap utilisation                             |
| `storage`     | Root filesystem utilisation                          |
| `toggl`       | Current Toggl timer (description, tags, duration)    |
| `claude`      | Claude.ai OAuth quota: 5h/7d %, reset time, gauge    |

Every module is independent — none of them are required to be
present, and the runtime composition is driven entirely by the
`[layout].layout` array in your config.

## Installation

```sh
git clone https://github.com/kiddersmxj/dwmbar-k
cd dwmbar-k
./install.sh
```

`install.sh` runs CMake (which fetches [`std-k`](https://github.com/kiddersmxj/std-k)
automatically), builds, and `sudo`-installs:

- `/usr/local/bin/dwmbarsrc/dwmbarsrc` — the long-running daemon
- `/usr/local/bin/dwmbar` — small wrapper that does
  `pkill dwmbar; dwmbarsrc &` (use to restart the bar)

It also wipes `~/.dwmbar.conf` before installing so the in-repo
`dwmbar.conf` is re-copied on every install — back up your config if
you have local changes you want to preserve.

For the latest tagged release see
<https://github.com/kiddersmxj/dwmbar-k/releases>.

### Build dependencies

- CMake ≥ 3.11, a C++17 compiler
- ALSA development headers (still linked by the build, even though
  the volume module now uses `wpctl`)
- X11 development headers
- `git` (CMake `FetchContent` pulls `std-k`)

### Runtime dependencies (only needed for the modules you enable)

| Module(s)                  | Requires                                           |
|----------------------------|----------------------------------------------------|
| `network`                  | `iproute2`, `nmcli`, `curl`                        |
| `volume`                   | `wpctl` (PipeWire / WirePlumber)                   |
| `media`                    | `playerctl`                                        |
| `battery`                  | `acpi`                                             |
| `brightness`               | `brightnessctl` (used as fallback)                 |
| `bluetooth`                | `bluetoothctl`                                     |
| `claude`                   | `jq`, `curl`, `date`, `~/.claude/.credentials.json`|
| `toggl`                    | external `/usr/bin/toggl.sh`                       |
| All                        | a Font Awesome (or compatible Nerd) font for icons |

## Usage

Add the wrapper to your `~/.xinitrc` (or equivalent):

```sh
dwmbar &
```

To restart the bar at any time, run `dwmbar` again — the wrapper
`pkill`s the existing daemon and relaunches it.

## Configuration

User config lives at `~/.dwmbar.conf` (copied from `dwmbar.conf` in
the repo on first install). Key sections:

```ini
[sleep]
sleeptime = 100        # main-loop tick (ms)
network   = 1000       # per-module poll cadence (ms)
volume    = 500
# ... one entry per module

[claude]
poll            = 120  # API re-poll every N ticks
fivehour.yellow = 60
fivehour.red    = 85

[layout]
layout = [
    "network",
    "time",
    ";",               # ; is a visual separator, not a module
    "battery",
    "volume",
    "media",
    ";",
    "cpu",
    "claude",
    # ... etc
]
```

`[layout].layout` is the source of truth for which modules actually
run — if a module isn't listed, no thread is spawned for it.
Listing the same name twice instantiates it once and renders the same
output in both slots. Unknown names are skipped with a warning at
startup.

Colours, icons, thresholds (CPU red %, battery levels, Claude tiers,
etc.) live in `inc/config.hpp` and require a rebuild to change. The
runtime cadence and layout are config-only and require only a bar
restart.

## Adding a new module

See [`docs/module-template.md`](docs/module-template.md) for the full
template, conventions, and a step-by-step checklist. The short
version:

1. Inherit from `Module`, override `run()` as a `while (true)` loop
   that publishes via `updateOutput(...)`.
2. End the header with `REGISTER_MODULE("yourname", YourModule)`.
3. Add `#include "yourname.hpp"` to `inc/dwmbar.hpp` and
   `PRIVATE src/yourname.cpp` to `CMakeLists.txt`.
4. Add a sleep key + colour array + icon constant in `inc/config.hpp`
   / `src/config.cpp` / `dwmbar.conf`, and append the name to
   `[layout].layout`.

The main loop is module-agnostic — you do not edit `dwmbar.cpp`.
