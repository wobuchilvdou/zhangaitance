import subprocess
import sys
from pathlib import Path


def main() -> int:
    root = Path(__file__).resolve().parent.parent / "simulator"
    result = subprocess.run(
        [sys.executable, "-m", "pytest", str(root), "-v", "--tb=short"],
        cwd=root.parent,
    )
    return result.returncode


if __name__ == "__main__":
    raise SystemExit(main())
