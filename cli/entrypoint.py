import argparse, subprocess, sys
from pathlib import Path

def format_output(cmd: str, raw_output: bytes) -> str:
    """
    Given the subcommand (either 'count-mapped' or 'count-unmapped')
    and the raw bytes output from `linecount`, return the final string
    to print.
    """
    count = raw_output.decode().strip()
    if cmd == "count-mapped":
        return count
    elif cmd == "count-unmapped":
        return f"UNMAPPED: {count}"
    else:
        # In case we ever add more subcommands, fall back to raw
        return count

def main():
    parser = argparse.ArgumentParser(prog="aligncount")
    sub = parser.add_subparsers(dest="cmd", required=True)

    c1 = sub.add_parser("count-mapped")
    c1.add_argument("-a", "--alignments", required=True)

    c2 = sub.add_parser("count-unmapped")
    c2.add_argument("-a", "--alignments", required=True)

    args = parser.parse_args()
    sam_path = Path(args.alignments)
    if not sam_path.exists():
        print(f"Error: input file '{args.alignments}' does not exist.", file=sys.stderr)
        sys.exit(1)

    try:
        raw = subprocess.check_output(["aligncount", "-a", args.alignments])
    except FileNotFoundError:
        print("Error: C++ binary 'aligncount' not found in PATH.", file=sys.stderr)
        sys.exit(1)
    except subprocess.CalledProcessError as e:
        print(f"Error: 'aligncount' failed (exit {e.returncode}).", file=sys.stderr)
        sys.exit(e.returncode)

    # Use our new helper to produce the final string to print
    out_str = format_output(args.cmd, raw)
    print(out_str)