#pragma once

// Logical design space for layout and UI (16:9 — standard for modern displays).
// Background art: export master plates at 1920x1080 (or 3840x2160 @2x, scaled in-engine).
// Keep interactive UI inside the central ~90% safe zone; edges may be cropped on ultrawide.
struct DisplayConfig {
    unsigned designWidth = 1920;
    unsigned designHeight = 1080;
    unsigned windowWidth = 1600;
    unsigned windowHeight = 900;
    bool startFullscreen = false;
    bool vsync = true;
};
