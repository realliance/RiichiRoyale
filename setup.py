import setuptools

with open("README.md", "r") as fh:
    long_description = fh.read()

setuptools.setup(
    name="riichiroyale-cmiller548",
    version="0.0.6",
    author="Christopher Miller",
    author_email="cmiller548@gmail.com",
    description="Singleplayer Riichi Mahjong Game",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/HartleyAHartley/RiichiRoyale",
    packages=setuptools.find_packages(),
    classifiers=[
        "Programming Language :: Python :: 3",
        "Operating System :: Microsoft :: Windows",
        "Operating System :: Unix",
    ],
    package_data={
      'library': ['libmahjong.so']
    },
    include_package_data=True,
    python_requires='>=3.6',
)
