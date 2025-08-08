import asyncio
from enum import Enum
from pathlib import Path
import sys
from typing import Final


class TermColor(Enum):
    RED = "\033[91m"
    GREEN = "\033[92m"
    RESET = "\033[0m"


def add_color(message: str, color: TermColor) -> str:
    return f"{color.value}{message}{TermColor.RESET.value}"


MSG_OK: Final[str] = add_color("OK", TermColor.GREEN)
MSG_FAILED: Final[str] = add_color("FAILED", TermColor.RED)


def get_tests(tests_dir: Path) -> list[str]:
    return list(
        sorted(map(lambda p: p.stem, tests_dir.glob("./*.in")))
    )


async def run_all_tests(tests_dir: Path, calculator: Path) -> int:
    tests: list[str] = get_tests(tests_dir)
    failed_tests = 0
    for test in tests:
        notation = Path(tests_dir, f"{test}.notation").read_text().strip()
        in_expression = Path(tests_dir, f"{test}.in").read_text()
        rc_expected = Path(tests_dir, f"{test}.rc").read_text().strip()
        out_expected = Path(tests_dir, f"{test}.out").read_text().strip()
        rc_actual, out_actual, err_actual = await run_test(calculator, notation, in_expression)
        if not validate_test(
            test, int(rc_expected), rc_actual, out_expected, out_actual, err_actual
        ):
            failed_tests += 1
    return failed_tests


async def run_test(
    calculator: Path,
    notation: str,
    in_expression: str,
) -> tuple[int, str, str]:
    proc = await asyncio.create_subprocess_exec(
        calculator, notation,
        stdin=asyncio.subprocess.PIPE,
        stdout=asyncio.subprocess.PIPE,
        stderr=asyncio.subprocess.PIPE,
    )

    stdout, stderr = await proc.communicate(input=bytes(in_expression, "ascii"))
    rc = await proc.wait()
    out_text = stdout.decode("ascii", "replace").strip()
    err_text = stderr.decode("ascii", "replace").strip()
    return rc, out_text, err_text


def validate_test(
    test_name: str,
    rc_expected: int,
    rc_actual: int,
    out_expected: str,
    out_actual: str,
    err_actual: str,
) -> bool:
    if rc_expected != rc_actual:
        print(f"{MSG_FAILED:10} {test_name}: return code {rc_expected} != {rc_actual}")
        return False
    if rc_expected != 0 and not err_actual:
        print(f"{MSG_FAILED:10} {test_name}: stderr is empty in incorrect test")
        return False
    if rc_expected == 0 and err_actual:
        print(f"{MSG_FAILED:10} {test_name}: stderr is not empty in correct test")
        return False
    if rc_expected == 0 and int(out_expected) != int(out_actual):
        print(f"{MSG_FAILED:10} {test_name}: result {out_expected} != {out_actual}")
        return False
    print(f"{MSG_OK:10} {test_name}")
    return True


def main() -> int:
    comparer = Path(sys.argv[1])
    tests_dir = Path(sys.argv[2])
    failed_tests: int = asyncio.run(run_all_tests(tests_dir, comparer))
    if failed_tests > 0:
        print(add_color(f"{failed_tests} tests FAILED", TermColor.RED))
        return 1
    print(add_color("All tests PASSED", TermColor.GREEN))
    return 0


if __name__ == "__main__":
    rc = main()
    sys.exit(rc)
