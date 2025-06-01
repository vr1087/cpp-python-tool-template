# tests/python/test_wrapper.py

import pytest
import cli.entrypoint as entrypoint

@pytest.mark.parametrize("cmd, raw, expected", [
    ("count-mapped", b"5\n", "5"),
    ("count-unmapped", b"7\n", "UNMAPPED: 7"),
    ("count-mapped", b"123\n", "123"),
    ("count-unmapped", b"0\n", "UNMAPPED: 0"),
    # Unknown subcommand: fall back to raw
    ("foo", b"42\n", "42"),
])
def test_format_output(cmd, raw, expected):
    """
    Ensure format_output() returns exactly the expected string for each case.
    """
    result = entrypoint.format_output(cmd, raw)
    assert result == expected