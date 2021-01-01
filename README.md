# Projet ELE4205 {#mainpage}

This app can mainly do 3 things depending on how its started. It can record a video or stream a video when a camera is available and it can watch a video stream when a GUI is available on the host machine. When in watch mode, the client can also decode QR codes and and the odroid will play the decoded strings in morse code.

[Read the docs](https://ele4205-projet-aut2020-29.netlify.app)

## Required packages

You will need to install the following libraries to run the app in server mode or client mode:

- OpenCV v2.4
- Zbar
- Odroid SDK if you plan on running this on an odroid
- Rest of the list coming soon...

## Setup
- Clone the repo (Replace USERNAME with your bitbucket username)
```sh
git clone https://USERNAME@bitbucket.org/rgourdeau/ele4205_projet_aut2020_29.git
cd ele4205_projet_aut2020_29
```
- Create the x86 build system (Replace `Debug` with `Release` to generate a release build system)
```sh
mkdir build_x86 && cd build_x86
cmake -DCMAKE_BUILD_TYPE=Debug ..
cd ..
```
- Create the oc2 build system (Replace `Debug` with `Release` to generate a release build system)
```sh
mkdir build_oc2 && cd build_oc2
source /opt/poky/2.1.3/environment-setup-aarch64-poky-linux 
cmake -DCMAKE_BUILD_TYPE=Debug ..
cd ..
```

## Build the app
Builds can be found in `build_dir/bin/debug/Projet` or `build_dir/bin/release/Projet`. Replace `build_dir` with `build_x86` or `build_oc2`.

- x86
```sh
cd build_x86
make
```

- oc2
```sh
cd build_oc2
make
```

## Transfer the oc2 build to the oc2
Copy `build_oc2/bin/debug/Projet` or `build_oc2/bin/release/Projet` to your oc2 to run it there with something like scp, sftp, etc.

## Usage

```
Projet ELE4205
Usage: ./Projet [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  -m,--mode TEXT              App mode. One of [record, stream, watch].
  -x,--xRes UINT              Video X resolution. Default: 800.
  -y,--yRes UINT              Video Y resolution. Default: 600.
  -l,--length FLOAT           Video length in seconds. Only in 'record' mode. Default: 5.000000.
  -p,--port UINT              Streaming port. Only in 'stream' and 'watch' mode. Default: 3382.
  -u,--uri TEXT               Streaming endpoint i.e IP. Only in 'watch' mode. Default: 127.0.0.1.
```