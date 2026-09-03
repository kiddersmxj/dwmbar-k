# dwmbar-k

A modular status bar for dwm. Each module is a self-registering C++ class that
runs in its own thread and writes a coloured segment to the bar.

## Building / installing

**`./install.sh` is the only supported route to build *and* install.** It is
not enough to `cmake --build build` — a change is not live until installed,
because the running bar uses the installed binary at
`/usr/local/bin/dwmbarsrc/dwmbarsrc`.

```sh
./install.sh          # build + install
/usr/local/bin/dwmbar # restart the bar so the new binary is actually running
```

What it does: removes `~/.dwmbar.conf` (so the in-repo `dwmbar.conf` is
re-copied on install), then runs `cmake -B build && cmake --build build &&
sudo cmake --install build`. It needs sudo for the install step.

**Installing does not restart the bar.** The running `dwmbarsrc` is orphaned to
init with nothing supervising it, so it keeps serving the old binary — and the
old `~/.dwmbar.conf`, which is only read at startup — until it is replaced. The
restart lives in the wrapper `/usr/local/bin/dwmbar` (`pkill dwmbar;
dwmbarsrc &`), which `install.sh` deliberately does not call; run it yourself.

To check which binary is actually live, compare the process start time against
the installed binary's mtime:

```sh
ps -o lstart= -p "$(pgrep -x dwmbarsrc)"
ls -l --time-style=full-iso /usr/local/bin/dwmbarsrc/dwmbarsrc
```

`cmake --build build` on its own is fine for a quick compile-check, but always
finish with `./install.sh` *and* the restart to make the change take effect.

## Adding a module

A new module touches the same set of files every time — mirror an existing
module (e.g. `archaudit` or `ccorral`) exactly:

1. `inc/<name>.hpp` — class deriving from `Module`, ending in
   `REGISTER_MODULE("<name>", <Class>)`.
2. `src/<name>.cpp` — implement `run()`. Build the output string from the
   module's own colour array, **`<Xx>Col[0] + Icon + BDCol`** (the trailing
   `BDCol` resets the colour). Do **not** use `Colour::Green` etc. inline —
   every module renders via its own `<Xx>Col[]` array defined in `config.hpp`.
   Use `updateOutput(NoOutputCode)` when there is nothing to show.
3. `inc/config.hpp` —
   - add a `<Name>SleepTime` (and any `<Name>GetStatusWait`) inline int;
   - bump `ModulesLength` and add `"<name>"` to the `Modules[]` array;
   - add `<Name>NumColours` + the `<Xx>Col[]` colour array and any icon
     `std::string` constants.
4. `src/config.cpp` — `KCONFIG_VAR(...)` lines for the new config keys.
5. `inc/dwmbar.hpp` — `#include "<name>.hpp"`.
6. `CMakeLists.txt` — add `PRIVATE src/<name>.cpp`.
7. `dwmbar.conf` — add the `[sleep]` entry, any `[<name>]` section, and place
   `"<name>"` in the `[layout]` array.

Then run `./install.sh`.
