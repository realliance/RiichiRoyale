import setuptools

with open("README.md", "r") as fh:
    long_description = fh.read()

setuptools.setup(
    name="riichiroyale",
    version="1.3.0",
    author="Christopher Miller",
    author_email="cmiller548@gmail.com",
    description="Singleplayer Riichi Mahjong Game",
    long_description=long_description,
    long_description_content_type="text/markdown",
    install_requires=[
        'pygame==2.0.0.dev14',
        'pygame-gui==0.5.7',
        'pybind11~=2.5',
        'numpy==1.19.3',
        'toml'
    ],
    url="https://github.com/HartleyAHartley/RiichiRoyale",
    packages=['libmahjong', 'riichiroyale', 'riichiroyale.resources', 'riichiroyale.storyfiles', 'riichiroyale.resources.fonts', 'riichiroyale.resources.images', 'riichiroyale.audio', 'riichiroyale.sprites', 'riichiroyale.sprites.elements', 'riichiroyale.utils', 'riichiroyale.test', 'riichiroyale.game', 'riichiroyale.tutorial', 'riichiroyale.views'],
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
