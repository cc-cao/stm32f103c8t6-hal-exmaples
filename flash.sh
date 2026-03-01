#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<'USAGE'
Usage:
  ./flash.sh <example_name>

Example:
  ./flash.sh can
USAGE
}

if [[ $# -ne 1 ]]; then
  usage
  exit 1
fi

example_name="$1"
chip="STM32F103C8"
out_dir="build/target"

if ! command -v probe-rs >/dev/null 2>&1; then
  echo "Error: probe-rs not found in PATH." >&2
  exit 1
fi

candidates=(
  "$out_dir/$example_name"
  "$out_dir/$example_name.elf"
  "$out_dir/$example_name.bin"
)

image=""
for f in "${candidates[@]}"; do
  if [[ -f "$f" ]]; then
    image="$f"
    break
  fi
done

if [[ -z "$image" ]]; then
  echo "Error: image not found for '$example_name'." >&2
  echo "Expected one of:" >&2
  printf '  - %s\n' "${candidates[@]}" >&2
  echo "Build first, e.g.: cmake --build --preset release --target $example_name" >&2
  exit 1
fi

echo "Flashing: $image"
probe-rs download --chip "$chip" "$image"
probe-rs reset --chip "$chip"
