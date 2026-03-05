

Dependencies for > Wayland and X11

On Debian, Ubuntu and other derivatives:

```sudo apt install libwayland-dev libxkbcommon-dev xorg-dev```

On Fedora derivatives:

```sudo dnf install wayland-devel libxkbcommon-devel libXcursor-devel libXi-devel libXinerama-devel libXrandr-devel```

On FreeBSD:

```pkg install wayland libxkbcommon evdev-proto xorgproto```

If errors persists, disable wayland and X11 support with cmake options:

```-D GLFW_BUILD_X11=0 -D GLFW_BUID_WAYLAND=0```
