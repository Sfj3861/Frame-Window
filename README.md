# Frame Window

Geometry Dash / Geode timing-practice utility.

## Android CI

The workflow at [`.github/workflows/build-android.yml`](.github/workflows/build-android.yml) handles validation, builds, packaging, and releases:

1. **Validate** required project files and the `mod.json` manifest.
2. **Build** Android32 and Android64 independently so one failure does not hide the other.
3. **Combine** the platform builds into a single `.geode` package and fail if no package was produced.
4. **Upload** the installable package as the `Frame-Window-Android` artifact for 14 days.
5. **Publish a GitHub release** when a version tag such as `v0.2.2` is pushed. The tag must exactly match `mod.json`'s version.

The workflow runs on pushes to `main`, pull requests, manual dispatches, and version tags (`v*`). Pull requests run validation and builds but do not publish releases.

### Downloading a development build

1. Open the repository's **Actions** tab.
2. Open the latest successful **Frame Window CI** run.
3. Download the `Frame-Window-Android` artifact.
4. Extract the ZIP and install the contained `.geode` file using a compatible Geode setup.

A green workflow means the configured build succeeded; the mod should still be tested in the target Geometry Dash/Geode version before being considered fully verified.

## v0.2.2 — input recorder

- Records real `handleButton` DOWN and UP events received by `PlayLayer`.
- Writes elapsed attempt time, update-frame counter, event type, button identifier, and player identifier to the Geode log.
- Displays input counts and the most recent input in the optional in-level overlay.
- Shows the configured visual timing window, target FPS, frame count, and attempt progress.
- Does not synthesize, delay, correct, or rewrite inputs, and does not modify collisions or physics.

## Compatibility

The manifest currently declares Geometry Dash 2.2081 for Android and Windows. The workflow builds Android32 and Android64 packages; it does not build a Windows package.

## Local build

With a compatible Geode SDK and CLI installed:

```sh
geode build -p android64
# Or, for 32-bit Android:
geode build -p android32
```
