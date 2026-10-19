# Frame Window

Geometry Dash / Geode timing-practice utility.

## v0.2.2 — input recorder
- Records each real `handleButton` DOWN and UP event received by PlayLayer.
- Writes elapsed attempt time, game update-frame counter, event type, button identifier and player identifier to the Geode log.
- Displays input down/up counts and the most recent input in the optional in-level overlay.
- Shows configured visual timing window, target FPS, frame count and attempt progress.
- Does not synthesize, delay, correct, or rewrite inputs, and does not modify collisions or physics.

## Compatibility and build status
The manifest requests Geometry Dash 2.2081 on Android and Windows. The GitHub Actions workflow attempts Android32 and Android64 builds, then combines the packages into an artifact. This repository update is not itself proof that compilation succeeded; check the Actions run and download its artifact if the workflow completes successfully.

## Local build
With a compatible Geode SDK and CLI installed:

```sh
geode build -p android64
# or for 32-bit Android:
geode build -p android32
```
