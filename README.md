# Frame Window

Frame Window is a Geometry Dash / Geode practice utility.

## Features in v0.2.0
- Adds a **Frame Window** button to the main menu.
- Displays an in-level overlay with the configured timing window.
- Shows the target frame rate derived from the game's animation interval.
- Optional extra stats: attempt progress and overlay update count.
- Configurable timing window from 0 to 30 frames.
- Visual-only: does not modify input timing, hitboxes, collisions, player physics, or level completion.

## Compatibility
The manifest requests Geometry Dash 2.2081 for Android and Windows. A successful build and in-game test are still required to confirm compatibility with the installed Geode SDK and game build.

## Build
Use a matching Geode SDK / CLI and run:

```sh
geode build
```

The GitHub Actions workflow under `.github/workflows/build-android.yml` attempts Android32 and Android64 builds and uploads the combined `.geode` as an artifact.
