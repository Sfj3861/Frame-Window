# Frame Window

A Geode starter project for a Geometry Dash timing-practice utility.

## Current scope
- Adds a **Frame Window** button to the main menu.
- Reads persistent settings from Geode's mod settings.
- Shows the configured practice window and overlay/feedback toggles.
- Does **not** modify player inputs, collisions, physics, or level completion.

## Compatibility
The metadata currently declares Geometry Dash 2.2081 as the requested target. This is not proof that a compatible Geode SDK/loader build exists for that exact Android build. Confirm the installed Geode SDK version and supported GD bindings before compiling.

## Build
Install a matching Geode SDK and Geode CLI, then follow the official documentation:
https://docs.geode-sdk.org/

Typical command:
```sh
geode build
```

If your local SDK uses a different Geode version, update the `geode` field in `mod.json` to the exact version you build against.
