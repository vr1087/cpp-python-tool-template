# setup.py  (root, uses scikit-build + metadata from setup_common.cfg)

import os
import configparser
from skbuild import setup

# 1) Load the shared INI file
cfg = configparser.ConfigParser()
cfg.read(os.path.join(os.path.dirname(__file__), "setup.cfg"))

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

# 3) Call scikit-build’s setup(), passing exactly our shared metadata
setup(
    name=metadata["name"],
    version=metadata["version"],
    description=metadata["description"],
    python_requires=options["python_requires"],
    packages=["cli"],
    entry_points=entry_points,
)