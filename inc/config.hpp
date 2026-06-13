#ifndef Kconfig
#define Kconfig

#include <std-k.hpp>

#include <cstring>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>

using std::chrono_literals::operator""ms;

const std::string ConfigFile = "/home/kidders/.dwmbar.conf";

// Define configuration variables with default values using inline
inline std::vector<std::string> ModuleLayout = {};

inline int SleepTime; // How long while loop waits before re-execution in ms
inline int BluetoothSleepTime;
inline int NetworkSleepTime;
inline int MediaSleepTime;
inline int VolumeSleepTime;
inline int WeatherSleepTime;
inline int BatterySleepTime;
inline int SignalSleepTime;
inline int CPUSleepTime;
inline int ThermalThrottleSleepTime;
inline int BrightnessSleepTime;
inline int WinConnectSleepTime;
inline int ChronologSleepTime;
inline int MemorySleepTime;
inline int StorageSleepTime;
inline int TogglSleepTime;
inline int ClaudeSleepTime;
inline int ArchAuditSleepTime;
inline int CcorralSleepTime;

// Cooldown for keeping timer on bar after stop
inline int ChronologCooldown;

// How many long toggle module waits between fetching new data
// equal to ToggleGetStatusWait x TogglSleepTime
inline int TogglGetStatusWait;

// Claude module: how many ticks between API polls
inline int ClaudeGetStatusWait;
// Claude module: 5-hour utilisation thresholds (% of quota) at which
// the icons shift colour to yellow / red
inline int ClaudeFiveHourYellow;
inline int ClaudeFiveHourRed;

// ArchAudit module: how many ticks between arch-audit polls
inline int ArchAuditGetStatusWait;
// ArchAudit module: vulnerable-package count thresholds at which the
// shield icon shifts colour to yellow / red
inline int ArchAuditYellow;
inline int ArchAuditRed;

// Ccorral module: how many ticks between `ccorral count attention` (X11) polls
inline int CcorralAttentionWait;

// Function to initialize global configuration variables
int initializeConfig(const std::string& filePath);

const int ModulesLength = 18;
const std::string Modules[ModulesLength] =  {
    "network",
    "time",
    "weather",
    "bluetooth",
    "media",
    "battery",
    "volume",
    "signal",
    "cpu",
    "throttling",
    "brightness",
    "winconnect",
    "memory",
    "storage",
    "toggl",
    "claude",
    "archaudit",
    "ccorral",
};

const std::string Separator = ";";

// const std::vector<std::string> ModuleLayout = {
//     "network",
//     "time",
//     Separator,
//     "cpu",
//     "throttling",
//     "brightness",
//     "bluetooth",
//     "signal",
//     "weather",
//     Separator,
//     "battery",
//     "volume",
//     "media",
// };

const std::string NoOutputCode = "NaN";

// Scale the volume based on individual system - 1 for off
const float VolScaler = 1.0;
// VolDisplayMax: MAX_VOL * 100 — must match MAX_VOL in media.sh.
// Maps wpctl's 0–MAX_VOL range onto 0–100 % for the bar display.
const int VolDisplayMax = 300;
const std::time_t t = std::time(0);
inline const std::tm* now = std::localtime(&t);
const std::string DT = getenv("logdatetime");
/* const std::string DT = std::to_string(now->tm_hour) + ':' + std::to_string(now->tm_min) \ */
/*                        + ':' + std::to_string(now->tm_sec) + '.' \ */
/*                        + std::to_string(now->tm_mday) + '-' \ */
/*                        + std::to_string(now->tm_mon + 1) + '-' \ */
/*                        + std::to_string(now->tm_year + 1900); */
const std::string TmpDir = "/tmp/dwmbar-k"; // Dir to store tmp files in
const std::string WDir = "/usr/local/bin/dwmbar-k"; // Working directory
const std::string TDir = TmpDir; // Full TmpDir path
const std::string Logfile = "/log/dwmbar-k/" + DT + ".dwmbar.log"; // Full DataDir path

const std::chrono::system_clock::duration Latency = 200ms; // How long while loop waits before re-execution in ms

// Volume levels as a percentage for corresponding icons
// Stored value is the bottom value for that icon
// Next value above is the top band for that icon
const int VHigh = 70; // 70-100
const int VMid = 40; // 40-69
const int VLow = 1; // 1-39
const int VMute = 0; // 0

// Battery levels as a percentage for corresponding icons
// Stored value is the bottom value for that icon
// Next value above is the top band for that icon
const int BFull = 90; // 90-100
const int BThreeQuart = 75; // 75-89
const int BHalf = 40; // 40 -74
const int BQuart = 20; // 20-39
const int BEmpty = 1; // Recomend non-zero value to catch broken battery

// CPU Thresholds for icon colour change
const int CPURed = 70;
const int CPUYellow = 50;

const std::string ModuleDelimeter = "|";
const std::string BarDelimeter = ";";

const std::string IInternet = R"(\xef\x82\xac)";

const std::string ICPU = R"(\xef\x8b\x9b)";

const std::string IArrowUp = R"(\xef\x85\xb6)";
const std::string IArrowDown = R"(\xef\x85\xb5)";

const std::string IDate = R"(\xef\x84\xb3)";

const std::string IVolMute = R"(\xef\x9a\xa9)";
const std::string IVolLow = R"(\xef\x80\xa6)";
const std::string IVolMid = R"(\xef\x80\xa7)";
const std::string IVolHigh = R"(\xef\x80\xa8)";

const std::string IBatteryFull = R"(\xef\x89\x80)";
const std::string IBatteryThreeQuart = R"(\xef\x89\x81)";
const std::string IBatteryHalf = R"(\xef\x89\x82)";
const std::string IBatteryQuart = R"(\xef\x89\x83)";
const std::string IBatteryEmpty = R"(\xef\x89\x84)";
const std::string ICharging = R"(\xef\x83\xa7)";

const std::string IPlay = R"(\xe2\x96\xb6)";
const std::string IPause = R"(\xe2\x8f\xb8)";

const std::string IBluetooth = R"(\xef\x8a\x94)";

const std::string IThermometer = R"(\xef\x8b\x87)";

const std::string ILight = R"(\xef\x83\xab)";

const std::string IMemory = R"(\xef\x94\xb8)";

const std::string IHdd = R"(\xef\x82\xa0)";

const std::string IToggl = R"(\xef\x80\x97)";

const std::string IReset  = R"(\xef\x8b\xb9)"; // U+F2F9 rotate-right

// Colours
namespace Colour {
    const std::string Black = "^C0^";
    const std::string Red = "^C1^";
    const std::string Green = "^C2^";
    const std::string Yellow = "^C3^";
    const std::string Blue = "^C4^";
    const std::string Magenta = "^C5^";
    const std::string Cyan = "^C6^";
    const std::string White = "^C7^";
    const std::string Grey = "^C8^";
    const std::string AltRed = "^C9^";
    const std::string AltGreen = "^C10^";
    const std::string AltYellow = "^C11^";
    const std::string AltBlue = "^C12^";
    const std::string AltMagenta = "^C13^";
    const std::string AltCyan = "^C14^";
    const std::string AltWhite = "^C15^";
};

const std::string BDCol = Colour::Grey;

const int SignalNumColours = 6;
const std::string SCol[SignalNumColours] = {
    Colour::Magenta, Colour::Grey, Colour::Grey, Colour::Magenta, Colour::Grey, Colour::Grey
//              Icon                Number              Unit                Icon                Number              Unit
};
const int NetworkNumColours = 4;
const std::string NCol[NetworkNumColours] = {
    Colour::Magenta, Colour::Magenta, Colour::Grey, Colour::Magenta
//          Icon              AP Name         Private IP       Public IP
};
const int TimeNumColours = 4;
const std::string TCol[TimeNumColours] = {
    Colour::Green, Colour::Green, Colour::Grey, Colour::Green
//          Icon              Day               Date              Time
};
const int WeatherNumColours = 1;
const std::string WCol[WeatherNumColours] = {
    Colour::Yellow
//          Icon
};
const int VolumeNumColours = 2;
const std::string VCol[VolumeNumColours] = {
    Colour::Red, Colour::Grey
//          Icon            Percentage
};
const int MediaNumColours = 6;
const std::string MCol[MediaNumColours] = {
    Colour::Blue, Colour::Blue, Colour::Grey, Colour::Blue, Colour::Grey, Colour::Grey
//          Icon              Artist               Dash             Title           Brackets           Duration
};
const int BatteryNumColours = 7;
const std::string BCol[BatteryNumColours] = {
    Colour::Green, Colour::Green, Colour::Yellow, Colour::Yellow, Colour::Red, Colour::Grey, Colour::Grey
//        Icon(4/4)          Icon(3/4)          Icon(2/4)           Icon(1/4)          Icon(0/4)        Extra Icon     Percentage
};
const int CPUNumColours = 4;
const std::string CPUCol[CPUNumColours] = {
    Colour::Grey, Colour::Green, Colour::Yellow, Colour::Red
//        Number            Icon 1              Icon2               Icon3
};
const int BrightnessNumColours = 2;
const std::string BrCol[BrightnessNumColours] = {
    Colour::Yellow, Colour::Grey
//        Icon                 Name
};
const int BluetoothNumColours = 2;
const std::string BlCol[BluetoothNumColours] = {
    Colour::Blue, Colour::Grey
//        Icon                 Name
};
const int ThrottlingNumColours = 1;
const std::string ThCol[ThrottlingNumColours] = {
    Colour::Red
//        Icon
};

const int WinConnectNumColours = 1;
const std::string WCCol[WinConnectNumColours] = {
    Colour::Blue
//        Icon
};

const int ChronologNumColours = 2;
const std::string CLCol[ChronologNumColours] = {
    Colour::Red,   Colour::Grey
//        Name              Time
};
const int MemoryNumColours = 4;
const std::string MemCol[MemoryNumColours] = {
    Colour::Blue, Colour::Grey
//        Icon                 Memory
};
const int StorageNumColours = 4;
const std::string StrCol[StorageNumColours] = {
    Colour::Red, Colour::Grey
//        Icon                 Memory
};

const int TogglNumColours = 4;
const std::string TglCol[StorageNumColours] = {
    Colour::Magenta, Colour::Grey, Colour::Magenta
//        Icon                 Tags             Description
};

const int ClaudeNumColours = 4;
const std::string ClCol[ClaudeNumColours] = {
    Colour::Green, Colour::Grey, Colour::Green, Colour::Grey
//        Gauge          5h%/7d%        IReset            Time
};
const int ClaudeNumTiers = 3;
const std::string ClTier[ClaudeNumTiers] = {
    Colour::Green, Colour::Yellow, Colour::Red
//        <yellow         <red          ≥red
};
// Gauge icon per tier: simple (low + mid), simple-high (high) — keeps the
// "simple" gauge aesthetic and gives a double-cue (icon + colour) at red.
const std::string ClGauge[ClaudeNumTiers] = {
    R"(\xef\x98\xa9)", // gauge-simple        U+F629  (low)
    R"(\xef\x98\xa9)", // gauge-simple        U+F629  (mid)
    R"(\xef\x98\xaa)", // gauge-simple-high   U+F62A  (high)
};

const int ArchAuditNumColours = 2;
const std::string ArCol[ArchAuditNumColours] = {
    Colour::Green, Colour::Grey
//        Icon            Count
};
const int ArchAuditNumTiers = 3;
const std::string ArTier[ArchAuditNumTiers] = {
    Colour::Green, Colour::Yellow, Colour::Red
//        0 vulns        <red          ≥red
};
// Shield icon — severity is conveyed by the tier colour (green/yellow/
// red), same single-icon-plus-tier-colour ethos as the cpu module.
// md-security (outline shield). If it doesn't render, fall back to
// md-shield_half U+F0780 (\xf3\xb0\x9e\x80) or fa-shield_halved
// U+F3ED (\xef\x8f\xad).
const std::string IArchAudit = R"(\xf3\xb0\x92\x83)"; // md-security U+F0483

const int CcorralNumColours = 2;
const std::string CcCol[CcorralNumColours] = {
    Colour::Green, Colour::Grey
//        Bell           Counts
};
// Ccorral module: a single bell icon whose glyph reflects mute state.
// Both render green; the open/slashed bell carries the meaning.
const std::string ICcorralBell  = R"(\xef\x83\xb3)"; // fa-bell        U+F0F3
const std::string ICcorralMuted = R"(\xef\x87\xb6)"; // fa-bell_slash  U+F1F6

#endif
