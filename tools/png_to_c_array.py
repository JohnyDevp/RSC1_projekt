#!/usr/bin/env python3
"""
Convert a PNG image into C arrays for graphics_draw_image_u8.

Output arrays:
- <name>_rgb[]   : uint8_t RGB image data (channels = 3)
- <name>_gray[]  : uint8_t grayscale image data (channels = 1)

Usage:
  python tools/png_to_c_array.py input.png --name logo --max-width 128 --max-height 64 --mode rgb --out src/assets/logo.h
"""

from __future__ import annotations

import argparse
from pathlib import Path


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Convert PNG to C uint8_t array")
    parser.add_argument("input", type=Path, help="Path to input PNG")
    parser.add_argument("--name", default="img", help="C variable base name")
    parser.add_argument("--max-width", type=int, default=None, help="Optional maximum output width")
    parser.add_argument("--max-height", type=int, default=None, help="Optional maximum output height")
    parser.add_argument("--mode", choices=["rgb", "gray"], default="rgb", help="Output mode")
    parser.add_argument("--out", type=Path, default=Path("image_data.h"), help="Output header path")
    return parser.parse_args()


def format_c_array_u8(name: str, data: bytes, cols: int = 12) -> str:
    lines = []
    for i in range(0, len(data), cols):
        chunk = data[i : i + cols]
        lines.append("    " + ", ".join(str(v) for v in chunk) + ",")
    body = "\n".join(lines)
    return f"const unsigned char {name}[] = {{\n{body}\n}};\n"


def main() -> None:
    args = parse_args()

    try:
        from PIL import Image
    except Exception as exc:
        raise SystemExit(
            "Pillow is required. Install with: pip install pillow"
        ) from exc

    if not args.input.exists():
        raise SystemExit(f"Input file not found: {args.input}")

    img = Image.open(args.input)

    if args.max_width or args.max_height:
        max_w = args.max_width if args.max_width is not None else img.width
        max_h = args.max_height if args.max_height is not None else img.height
        img.thumbnail((max_w, max_h), Image.Resampling.LANCZOS)

    if args.mode == "rgb":
        img = img.convert("RGB")
        raw = img.tobytes()
        array_name = f"{args.name}_rgb"
        channels = 3
    else:
        img = img.convert("L")
        raw = img.tobytes()
        array_name = f"{args.name}_gray"
        channels = 1

    guard = f"{args.name.upper()}_IMAGE_DATA_H"

    content = []
    content.append(f"#ifndef {guard}")
    content.append(f"#define {guard}")
    content.append("")
    content.append("#include <stdint.h>")
    content.append("")
    content.append(f"#define {args.name.upper()}_WIDTH {img.width}")
    content.append(f"#define {args.name.upper()}_HEIGHT {img.height}")
    content.append(f"#define {args.name.upper()}_CHANNELS {channels}")
    content.append("")
    content.append(format_c_array_u8(array_name, raw))
    content.append("")
    content.append(f"#endif /* {guard} */")
    content.append("")

    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text("\n".join(content), encoding="ascii")

    print(f"Wrote {args.out}")
    print(f"Image size: {img.width}x{img.height}, channels={channels}")
    print(f"Use with graphics_draw_image_u8({array_name}, {img.width}, {img.height}, {channels}, dst_x, dst_y)")


if __name__ == "__main__":
    main()
