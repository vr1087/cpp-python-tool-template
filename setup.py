# setup.py
from skbuild import setup

setup(
    name="aligncount_demo",
    version="0.2.0",
    description="Aligncount wrapper",
    packages=["cli"],
    entry_points={"console_scripts": ["aligncount=cli.entrypoint:main"]},
)