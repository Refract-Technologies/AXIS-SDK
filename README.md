# Table of Contents

1. [How to Use](#how-to-Use)
2. [Example](#example)
3. [Documentation](#documentation)
4. [Changelog](#changelog)
5. [KnownIssues](#known-issues)

# How to Use #

- Include axis_sdk.h
    - For windows, link to axis_sdk.lib and make sure the dll is in the same directory as your compiled executable
    - For linux, link to libaxis_sdk.so

# Example #

- Example can be found [here](https://github.com/Refract-Technologies/AXIS-SDK/example/example.cpp)

# Documentation #

- [Documentation](https://refract-technologies.github.io/AXIS-SDK/group___functions.html)

# Changelog #

### v2.4.1 ###

- Convert AxisSkeleton from OpenVR coordinate system to Unity Coordinate system before sending out via OSC
- Convert to hipRooted skeleton before sending out via OSC

### v2.4.0 ###

- First release

# Known Issues #

- Linear accel in devpacket is not working for node firmware 2.4.5
- Number of nodes on affects the rate of data transfer from the dongle in dongle firmware 2.4.5
- Number of nodes on affects the rate of data transmission between the nodes and the dongle
    - This is due to packetloss when all the nodes are trying to talk to the dongle at the same time
    - To reduce packetloss, it is recommended to lower the transmission rate