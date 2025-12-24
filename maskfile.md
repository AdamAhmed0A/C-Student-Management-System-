# Tasks for SIS system

## build (preset)

> Builds the app with specified config (linux-debug, linux-release, windows-debug, windows-release, default linux-debug)

```bash
root="$(git rev-parse --show-toplevel)"

cd "${root}"

if [[ "${preset}" == "linux-debug" ]]; then
  presetName="linux-ninja-debug"
  build_preset="linux-debug"
elif [[ "${preset}" == "linux-release" ]]; then
  presetName="linux-ninja-release"
  build_preset="linux-release"
elif [[ "${preset}" == "windows-debug" ]]; then
  presetName="windows"
  build_preset="windows-debug"
elif [[ "${preset}" == "windows-release" ]]; then
  presetName="windows"
  build_preset="windows-release"
else
  presetName="linux-ninja-debug"
  build_preset="linux-debug"
fi

cmake --preset "${presetName}"
cmake --build --preset "${build_preset}" --parallel 3
exitCode=$?

cd "${root}"

exit "${exitCode}"
```

## run (mode) (preset)

> Runs the app with specified mode (cli or gui) and preset (linux-debug, linux-release, windows-debug, windows-release, default linux-debug)

```bash
root="$(git rev-parse --show-toplevel)"

cd "${root}"

mask build "${preset}" && clear

if [[ "${preset}" == "windows-debug" || "${preset}" == "windows-release" ]]; then
  build_dir="build-vs"
elif [[ "${preset}" == "linux-debug" ]]; then
  build_dir="build-debug"
elif [[ "${preset}" == "linux-release" ]]; then
  build_dir="build-release"
fi

if [[ "${mode}" == "gui" ]]; then
  "./${build_dir}/UniManage/unimanage_gui"
else
  "./${build_dir}/UniManageCli/unimanage_cli"
fi
```
