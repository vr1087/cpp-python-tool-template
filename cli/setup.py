# cli/setup.py  (pure-Python installer, uses setuptools + metadata from setup_common.cfg)

import os
import configparser
from setuptools import setup, find_packages

# 1) Load the same shared INI file from the parent directory
cfg = configparser.ConfigParser()
cfg.read(os.path.join(os.path.dirname(__file__), "..", "setup.cfg"))

# 2) Extract metadata
metadata = cfg["metadata"]
options = cfg["options"]
entry_points = {
    "console_scripts": [
        line.strip()
        for line in cfg["options.entry_points"]["console_scripts"].splitlines()
        if line.strip()
    ]
}

# 3) Call setuptools.setup() with that metadata
setup(
    name=metadata["name"],
    version=metadata["version"],
    description=metadata["description"],
    python_requires=options["python_requires"],
    packages=["."],
    entry_points=entry_points,
)