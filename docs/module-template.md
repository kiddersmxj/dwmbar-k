# Bar Module Template

This document is the canonical reference for adding a new module to
`dwmbar-k`. It covers the file layout, the contract a module must obey,
the conventions used by the existing modules, and the exact set of
files you need to touch to get a new module running on the bar.

If you only want the short version, jump to
[Checklist: adding a new module](#checklist-adding-a-new-module).

---

## 1. How a module fits into the program

The runtime is a single process (`src/dwmbar.cpp`) that:

1. Loads `~/.dwmbar.conf` via `initializeConfig` (`src/config.cpp`).
2. Asks the singleton `ModuleRegistry` for a `Module` instance for every
   name in `[layout].layout`, deduplicating repeats.
3. Spawns one `std::thread` per instance, running `Module::run()`.
4. On every tick (`sleep.sleeptime` ms) it briefly waits on each
   module's condition variable, collects the latest published output,
   re-orders by `ModuleLayout`, inserts separators, hex-decodes Font
   Awesome glyphs, and writes the result to the X root window with
   `XStoreName` (the dwm "XSetRoot" status mechanism).

A module never touches X, never reads the config file directly, and
never coordinates with other modules. It only:

- runs an infinite loop on its own thread,
- fetches a value (sysfs / procfs / `k::ExecCmd`),
- formats it with the colour array + icon defined in `config.hpp`,
- calls `updateOutput(...)` to publish, or `updateOutput(NoOutputCode)`
  to suppress its slot for that tick,
- sleeps for `<Name>SleepTime` ms.

---

## 2. Required files

Every module lives in two files:

- `inc/<name>.hpp` — class declaration + `REGISTER_MODULE` macro.
- `src/<name>.cpp` — implementation of `run()` and any helpers.

`CMakeLists.txt` lists every source file explicitly under
`target_sources(dwmbarsrc ...)` — you **must** add `PRIVATE src/foo.cpp`
to that list, otherwise `claude.cpp`-style "undefined reference to
vtable" link errors will show up (the `REGISTER_MODULE` registrar is
linked in via the header, but the class implementation is not).

---

## 3. Header template

```cpp
// inc/foo.hpp
#pragma once

#include "moduleregistry.hpp"
#include "config.hpp"
#include "module.hpp"

#include <string>

class FooModule : public Module {
public:
    void run() override;

private:
    // Helpers — keep value-fetching separate from formatting.
    std::string getFooValue();

    // Per-instance state that needs to survive between ticks
    // (e.g. animation phase, cached poll result, cooldown counters).
    int Cooldown = 0;
};

REGISTER_MODULE("foo", FooModule)

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
```

Notes:

- The `REGISTER_MODULE("foo", FooModule)` macro **must** appear in the
  header, outside the class. It expands to a static registrar whose
  constructor inserts a factory into `ModuleRegistry::instance()` at
  process start. If it lives in the `.cpp` only, the linker is allowed
  to drop the static initializer and the module silently disappears.
- The string `"foo"` is the layout key — it is what users put in
  `[layout].layout` in `dwmbar.conf` to enable the module.
- Inherit publicly from `Module`. Override `run()`. Don't touch
  `output_mutex` / `output_cv` directly; only call `updateOutput()`.

---

## 4. Source template

```cpp
// src/foo.cpp
#include "../inc/foo.hpp"

#include <chrono>
#include <thread>

void FooModule::run() {
    while (true) {
        std::string value = getFooValue();

        if (value.empty()) {
            updateOutput(NoOutputCode);          // suppress slot this tick
        } else {
            std::string out = FCol[0] + IFoo + " "
                            + FCol[1] + value + "%";
            updateOutput(out);
        }

        std::this_thread::sleep_for(
            std::chrono::milliseconds(FooSleepTime));
    }
}

std::string FooModule::getFooValue() {
    std::string raw;
    k::ExecCmd(R"(some-shell-command)", raw);
    return k::StripTrailingNL(raw);
}

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
```

Conventions every module follows:

- `run()` is `while (true)` — no shutdown path is wired up. The threads
  are detached at process exit.
- One `updateOutput(...)` per tick, at the end of the loop body.
  Returning `NoOutputCode` ("NaN") tells the renderer to omit the slot
  for that tick; `ParseXSR` will collapse adjacent separators so the
  bar doesn't get a visual gap.
- Sleep at the **end** of the loop, using the module's own
  `<Name>SleepTime` constant. Don't sleep on the global `SleepTime`.
- Fetch and format are usually split into helpers. Keep `run()` as the
  high-level orchestration only.

### Fetching values

- Shell out via `k::ExecCmd("cmd", out_string)` from `std-k`. Strip the
  trailing newline with `k::StripTrailingNL(out)` before parsing.
- For files in `/proc` and `/sys`, prefer `std::ifstream` directly —
  see `cpu.cpp` and `brightness.cpp`.
- If a command can fail (player disconnects, no battery, no sink), wrap
  any `std::stoi` / `std::stof` in try/catch and return a sentinel —
  see `media.cpp::GetTimeFromStart` and `volume.cpp::GetVolumeLevel`.
- For range remapping (e.g. raw 0–MAX → 0–100 %) use `k::Map`.

### State that survives between ticks

Put it on the class as a private member with a default initialiser.
Examples in the tree:

- `BatteryModule::Charging`, `BIcon`, `CIcon` — animation state for the
  charging-icon cycle.
- `CPUModule` uses a function-local `static std::vector<long>
  prevCpuStats` for the previous `/proc/stat` snapshot.
- `TogglModule::Cooldown` and the cached `TogglStatus` so that a cheap
  per-tick render can run without re-shelling out every time.

### Decoupled polling vs. rendering

If your data source is slow or rate-limited (HTTP, external scripts),
do not call it every tick. Cache the last result and re-poll every N
ticks — see `TogglModule::run` driven by `TogglGetStatusWait`.

---

## 5. Output formatting

The bar is rendered as a single string written to the X root window.
Colour switches are encoded as `^Cn^` escapes (handled by the dwm
status patches), and icons are Font Awesome glyphs stored as raw
hex-escaped UTF-8 (`R"(\xef\x...)"`) and decoded just before
`XStoreName`.

### Colour escapes

Use the `Colour::*` namespace from `config.hpp`:

```cpp
Colour::Black, Red, Green, Yellow, Blue, Magenta, Cyan, White,
Grey, AltRed, AltGreen, AltYellow, AltBlue, AltMagenta, AltCyan,
AltWhite
```

Don't hard-code `^Cn^` strings; always go through the namespace so
re-themes are a one-liner.

### Per-module colour arrays

Each module gets its own colour array in `config.hpp` with one slot per
output segment, plus a comment line showing what each slot colours.
Example for a hypothetical `foo` module with three coloured segments:

```cpp
const int FooNumColours = 3;
const std::string FCol[FooNumColours] = {
    Colour::Magenta, Colour::Grey, Colour::Magenta
//        Icon              Value             Unit
};
```

Why:

- Single source of truth — colour changes don't touch the module's
  source.
- The comment row keeps the array self-documenting; copy this style
  exactly so future re-themes are mechanical.

### Icon constants

Add a new icon constant `IFoo` in `config.hpp`, alongside the existing
`IDate`, `IInternet`, `IVolMute`, etc. Use a raw string literal of
hex-escaped UTF-8:

```cpp
const std::string IFoo = R"(\xef\x80\x97)";
```

### Output assembly pattern

```cpp
std::string out = FCol[0] + IFoo + " "
                + FCol[1] + value
                + FCol[2] + unit;
```

For thresholded colour (e.g. CPU red/yellow/green) compute the colour
first, then assemble — see `cpu.cpp` for the canonical pattern.

---

## 6. Config wiring

`config.hpp` declares `inline` globals; `src/config.cpp` populates them
from `~/.dwmbar.conf` via the `KCONFIG_VAR*` macros from `std-k`.

For a new module `foo`, edits in `inc/config.hpp`:

```cpp
inline int FooSleepTime;                  // sleep period for run()
// + any thresholds, cooldowns, poll counts your module needs

const int FooNumColours = N;
const std::string FCol[FooNumColours] = { /* ... */ };

const std::string IFoo = R"(\xef\x...)";
```

Edit in `src/config.cpp`:

```cpp
KCONFIG_VAR(FooSleepTime, "sleep.foo", 500)
// + any KCONFIG_VAR for thresholds/cooldowns
```

Edit in `dwmbar.conf` (and `~/.dwmbar.conf` on the user's machine):

```ini
[sleep]
foo = 500

[layout]
layout = [
    ...,
    "foo",
    ...,
]
```

Use `KCONFIG_VAR_REQUIRED(...)` only for values the program cannot
sensibly default (currently just `SleepTime` and `ModuleLayout`).
Everything else gets a default so the bar still launches if the key is
missing.

The legacy `Modules[]` array + `ModulesLength` in `config.hpp` is
informational only — the `ModuleRegistry` is the actual source of
truth. Keep `Modules[]` in sync if you want the listing to be
accurate, but the program does not require it.

---

## 7. Wiring the module into main

Two edits, one of them being the layout entry already covered above.

### `inc/dwmbar.hpp`

Add the include:

```cpp
#include "foo.hpp"
```

This is the only edit to "main code" needed. The include exists purely
to pull the translation unit containing `REGISTER_MODULE`'s static
registrar into the link, so the factory actually self-registers.
Without this include, the linker is free to drop `foo.cpp`'s static
initializer and your module will never appear in the registry.

### `dwmbar.cpp`

No edits. The main loop is module-agnostic — it iterates the
`ModuleLayout` config array, looks each name up in the registry,
spawns a thread on `Module::run`, and renders whatever each module
publishes.

Behavioural rules enforced by `dwmbar.cpp`:

- A registered module **not** listed in `ModuleLayout` → never
  instantiated, no thread.
- A name in `ModuleLayout` that isn't registered → `std::cerr` warning
  and the slot is skipped.
- A name listed multiple times → instantiated **once**; the same
  cached output is reused at every layout slot.
- `";"` (the `Separator`) in the layout is not a module — it produces
  the visual divider between groups.

---

## 8. Checklist: adding a new module

1. `inc/foo.hpp` — declare `class FooModule : public Module`, override
   `run()`, end the file with `REGISTER_MODULE("foo", FooModule)`.
2. `src/foo.cpp` — implement `run()` as `while (true) { fetch; format;
   updateOutput; sleep_for(FooSleepTime); }`.
3. `inc/config.hpp` —
   - `inline int FooSleepTime;`
   - `const int FooNumColours = N;` + `FCol[]` array with the
     per-segment comment row
   - `const std::string IFoo = R"(\xef\x...)";`
   - (optional) append `"foo"` to `Modules[]` and bump `ModulesLength`
4. `src/config.cpp` — `KCONFIG_VAR(FooSleepTime, "sleep.foo", 500)`
   plus any other keys.
5. `inc/dwmbar.hpp` — `#include "foo.hpp"`.
6. `CMakeLists.txt` — add `PRIVATE src/foo.cpp` inside the
   `target_sources(dwmbarsrc ...)` block.
7. `dwmbar.conf` (and the user's `~/.dwmbar.conf`) —
   - `[sleep] foo = 500`
   - add `"foo"` (and any `";"` separator) inside `[layout].layout`.
8. Rebuild via `./install.sh` (runs CMake) and restart the bar (the
   `wrapper` binary does `pkill dwmbar` and relaunches).

If the module isn't appearing on the bar, in order:

- Did `REGISTER_MODULE` end up in the **header**? (not the .cpp)
- Is `inc/foo.hpp` included from `inc/dwmbar.hpp`?
- Is `"foo"` actually present in `[layout].layout`?
- Does `dwmbar` log `Warning: unknown module name in layout: foo`?
  → registry never saw it → likely the include or `REGISTER_MODULE`
  problem above.
- Does `run()` ever call `updateOutput(...)` with a non-empty,
  non-`NoOutputCode` value? Empty / `"NaN"` outputs are intentionally
  dropped by the renderer.

---

## 9. Conventions worth not breaking

- One module = one thread = one slot in the layout.
- Modules never block on each other; only on their own
  `std::this_thread::sleep_for`.
- All formatting goes through the colour array + icon constants in
  `config.hpp` — no hard-coded `^Cn^` strings, no inline UTF-8 byte
  sequences.
- Cadence is per-module via `<Name>SleepTime`, configured in
  `[sleep]`.
- Use `NoOutputCode` to mean "I have nothing useful right now"; do
  **not** publish empty strings or stale data.
- Long-running or rate-limited fetches must be decoupled from render
  cadence (cache + poll-every-N-ticks pattern).
- Catch parsing errors (`stoi`/`stof`) at the boundary with external
  tools — they will return junk eventually.
- Keep `run()` short; push fetching/parsing into private helpers on
  the class.
