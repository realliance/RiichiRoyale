import setuptools

with open("README.md", "r") as fh:
    long_description = fh.read()

setuptools.setup(
    name="riichiroyale",
    version="0.1.14",
    author="Christopher Miller",
    author_email="cmiller548@gmail.com",
    description="Singleplayer Riichi Mahjong Game",
    long_description=long_description,
    long_description_content_type="text/markdown",
    install_requires=[
        'pygame==2.0.0.dev14',
        'pygame-gui==0.5.7'
    ],
    url="https://github.com/HartleyAHartley/RiichiRoyale",
    packages=['libmahjong', 'riichiroyale', 'riichiroyale.resources', 'riichiroyale.resources.fonts', 'riichiroyale.audio', 'riichiroyale.sprites', 'riichiroyale.sprites.elements', 'riichiroyale.utils', 'riichiroyale.test', 'riichiroyale.game', 'riichiroyale.tutorial', 'riichiroyale.views'],
    classifiers=[
        "Programming Language :: Python :: 3",
        "Operating System :: Microsoft :: Windows",
        "Operating System :: Unix",
    ],
    entry_points={
        'console_scripts':['riichiroyale=riichiroyale.__main__:main']
    },
    include_package_data=True,
    python_requires='>=3.6',
)
