/**
 * \file axis_sdk.h
 * API for axis-sdk.
 * \defgroup Functions
 * \defgroup Enums
 * \defgroup Packets
 * \defgroup Commands
 * \defgroup Callbacks
 * \defgroup Structs
 */
#pragma once
#ifndef _AXIS_SDK_API_H
#define _AXIS_SDK_API_H

#include <cstdint>
#include <vector>

#if defined(_WIN32)
    // Microsoft
    #ifdef AXIS_EXPORTING
        #define AXIS_API extern "C" __declspec( dllexport )
    #else
        #define AXIS_API extern "C" __declspec( dllimport )
    #endif
#elif defined(__GNUC__) || defined(COMPILER_GCC) || defined(__APPLE__)
    //  GCC
    #ifdef AXIS_EXPORTING
        #define AXIS_API extern "C" __attribute__((visibility("default")))
    #else
        #define AXIS_API extern "C"
    #endif
#else
    #error What platform is this?
#endif

#define AXIS_MAX_NODE_COUNT 17

namespace Refract { namespace AXIS
{
    #define AXIS_RUNTIME_ERRORS_ENUM(DO) \
    DO(OK) /**< No error */\
    DO(PacketCallbackNotSet) /**< No packet callback was set in the config */ \
    DO(InvalidDongleCommand) /**< Dongle command was invalid. Please check parameters */ \
    DO(HubTimeout) /**< Command to Axis Hub timed out */ \
    DO(InvalidIPAddress) /**< IP address is invalid */ \
    DO(InvalidParameter) /**< Parameter passed into function was invalid */ \
    DO(NoLANAdapter) /**< Could not find a LAN adapter to try and search for Axis Hubs */ \
    DO(HTTPError) /**< HTTP Request error code. Check the errorCode parameter */ \
    DO(HTTPException) /**< Exception occurred while making a HTTP Request. Check the errorString parameter */ \
    DO(SocketError) /**< Error occurred while trying to reading from a socket. Check the errorCode parameter */ \
    DO(SocketException) /**< Exception occurred while trying to open a socket. Check the errorString parameter */ \
    DO(PreviousOpInProgress) /**< Previous operation still in progress. Wait for it to complete first */ \
    DO(DongleNotConnected) /**< Tried to send a dongle command when the dongle wasn't connected */ \
    DO(SerialPortError) /**< A serial port error has occurred. Error codes are platform dependant */ \
    DO(InternalError) /**< Something went wrong within the SDK. Shouldn't happen */ \
    DO(SDKNotStarted) /**< SDK has not been started. */ \
    DO(NoSubnet) /**< Ip address given was not in the same subnet as any LAN adapters */\
    DO(RUNTIME_ERROR_COUNT)

    #define AXIS_PACKET_TYPES_ENUM(DO) \
    DO(IMUData) /**< Stream of orientation and linear acceleration data from the nodes. Refer to Refract::AXIS::IMUData_t */\
    DO(VisibleNodes) /**< Array of nodes' MAC addresses visible to the dongle. Refer to Refract::AXIS::NodesInfo_t */\
    DO(PairedNodes) /**< Array of nodes' MAC addresses paired with the dongle. Refer to Refract::AXIS::NodesInfo_t */\
    DO(SetWifiResponse) /**< Response from the dongle after sending a SetWifi command. Refer to Refract::AXIS::WifiRes_t */\
    DO(ReportWifi) /**< Current wifi channel used by the dongle and nodes. Refer to Refract::AXIS::WifiRes_t */\
    DO(ReportBatt) /**< Battery info of paired nodes. Refer to Refract::AXIS::BattInfo_t */\
    DO(DoubleClick) /**< MAC address of the node where user pressed the button twice. Refer to Refract::AXIS::NodesInfo_t */\
    DO(Versions) /**< Versions of the dongle and paired nodes. Refer to Refract::AXIS::VersionsInfo_t */\
    DO(SuitInfo) /**< Info for the entire suit. Refer to Refract::AXIS::SuitInfo_t */\
    DO(NodeAssignAck) /**< Array of nodes' MAC addresses and their positions. Refer to Refract::AXIS::NodesInfo_t */\
    DO(DevPacket) /**< Stream of DevPackets from the dongle. Refer to Refract::AXIS::DevPacket_t */\
    DO(NodeConfigResponse) /**< Node configuration information. Refer to Refract::AXIS::NodeConfigResponse_t */\
    DO(SkeletonData) /**< Stream of orientation and position data of joints. Refer to Refract::AXIS::SkeletonData_t */\
    DO(PACKET_TYPES_COUNT)

    #define AXIS_DONGLE_COMMAND_TYPES_ENUM(DO) \
    DO(Shutdown) /**< Turn off all paired nodes */\
    DO(GetWifi)  /**< Get the current wifi channel being used by the dongle */\
    DO(GetBatt)  /**< Get the battery info of all paired nodes */\
    DO(GetVersions)  /**< Get the version info of the dongle and all paired nodes */\
    DO(Suitinfo)  /**< Get info for entire suit */\
    DO(Reboot)  /**< Reboot paired nodes. */\
    DO(SetWifi)  /**< Set Wifi channel to be used by the dongle. */\
    DO(NodeAssign) /**< Assign nodes to specific body positions. */\
    DO(NodeInfo) /**< Get node info for paired / visible nodes. */\
    DO(NodeAssignSingle) /**< Assign a node to a specific body position */\
    DO(NodeConfigSet) /**< Set a specific node configuration. From firmware version 2.4.x onwards */\
    DO(Calibration) /**< Perform calibration */\
    DO(NodeOperationMode) /**< Set nodes to a specific operation mode */\
    DO(Buzz) /**< Vibrate a specific node by body position */\
    DO(LED) /**< Change the color and/or brightness of a specific node by body position */\
    DO(BuzzMAC) /**< Vibrate a specific node by MAC address */\
    DO(LEDMAC) /**< Change the color and/or brightness of a specific node by MAC address */\
    DO(NodeConfigGet) /**< Get node configuration. From firmware version 2.4.x onwards */\
    DO(DONGLE_COMMAND_TYPES_COUNT)

    #define AXIS_NODE_POSITIONS_ENUM(DO) \
    DO(RightThigh) /**< Right thigh */\
    DO(RightCalf) /**< Right shin */\
    DO(LeftThigh) /**< Left thigh */\
    DO(LeftCalf) /**< Left shin */\
    DO(RightUpperArm) /**< Right upper Arm */\
    DO(RightForearm) /**< Right forearm */\
    DO(LeftUpperArm) /**< Left upper Arm */\
    DO(LeftForearm) /**< Left forearm */\
    DO(Spine) /**< Spine */\
    DO(RightFoot) /**< Right foot */\
    DO(LeftFoot) /**< Left foot */\
    DO(RightHand) /**< Right hand */\
    DO(LeftHand) /**< Left hand */\
    DO(RightShoulder) /**< Right shoulder */\
    DO(LeftShoulder) /**< Left shoulder */\
    DO(Head) /**< Head */\
    DO(Hip) /**< Hip / Waist */\
    DO(Unassigned) /**< Unpaired nodes will have this as their node index */\
    DO(NODE_INDEX_COUNT)

    #define AXIS_NODE_OPERATION_MODES_ENUM(DO) \
    DO(Unsupported) /**< A node with unsupported operation mode is probably a node with old firmware and needs to be updated */\
    DO(Fusion) /**< Sensor fusion using magnetometer. This is the default */\
    DO(FusionNoMag) /**< Alternative sensor fusion algorithm without magnetometer */\
    DO(OPERATION_MODE_COUNT)

    #define AXIS_HUB_LOCATIONS_ENUM(DO) \
    DO(HubLocationBack) /**< Axis hub is located on user's back */\
    DO(HubLocationFront) /**< Axis hub is located on user's front */\
    DO(AXIS_HUB_LOCATION_COUNT)

    #define AXIS_HIP_PROVIDER_MODES_ENUM(DO) \
    DO(HipProviderNone) /**< No Hip provider */\
    DO(HipProviderNode) /**< Using Axis node for hip */\
    DO(HipProviderHub) /**< UUsing Axis Hub for hip */\
    DO(AXIS_HIP_PROVIDER_MODES_COUNT)

    #define AXIS_NODE_CONFIG_CATEGORIES_ENUM(DO) \
    DO(DEV) /**< Dev category */\
    DO(Power) /**< Power configuration category */\
    DO(AXIS_NODE_CONFIG_CATEGORIES_COUNT)

    #define AXIS_NODE_CONFIG_DEV_PARAMS_ENUM(DO) \
    DO(DevPacketBitflags) /**< Flags for Refract::AXIS::DevPacket_t. See Refract::AXIS::DevPacket_t::DevPacketComponents */\
    DO(AXIS_NODE_CONFIG_DEV_PARAMETERS_COUNT)

    #define AXIS_NODE_CONFIG_POWER_PARAMS_ENUM(DO) \
    DO(TransmissionPower) /**< Transmission Power (0-20dbm). Lower values save more power but decrease range */\
    DO(TransmissionFrequency) /**< Transmission Frequency. Lower value saves more power but decreases tracking smoothness */\
    DO(AXIS_NODE_CONFIG_POWER_PARAMETERS_COUNT)

    #define AXIS_VMC_BONE_CONFIG_BONES_ENUM(DO) \
    DO(VMCHips) /**< Hips */\
    DO(VMCLeftUpperLeg) /**< Left upper leg */\
    DO(VMCRightUpperLeg) /**< Right upper leg */\
    DO(VMCLeftLowerLeg) /**< Left knee */\
    DO(VMCRightLowerLeg) /**< Right knee */\
    DO(VMCLeftFoot) /**< Left ankle */\
    DO(VMCRightFoot) /**< Right ankle */\
    DO(VMCSpine) /**< Spine */\
    DO(VMCChest) /**< Chest */\
    DO(VMCUpperChest) /**< Upper chest */\
    DO(VMCNeck) /**< Neck */\
    DO(VMCHead) /**< Head */\
    DO(VMCLeftShoulder) /**< Left shoulder */\
    DO(VMCRightShoulder) /**< Right shoulder */\
    DO(VMCLeftUpperArm) /**< Left upper arm */\
    DO(VMCRightUpperArm) /**< Right upper arm */\
    DO(VMCLeftLowerArm) /**< Left elbow */\
    DO(VMCRightLowerArm) /**< Right elbow */\
    DO(VMCLeftHand) /**< Left wrist */\
    DO(VMCRightHand) /**< Right wrist */\
    DO(VMC_CONFIG_COUNT) /**< Maximum number of VMC bones */

    #define AXIS_SKELETON_JOINTS_ENUM(DO) \
    DO(SKHMD) /**< Head mounted display */\
    DO(SKHead) /**< Head */\
    DO(SKNeck) /**< Neck */\
    DO(SKUpperSpine) /**< Upper spine */\
    DO(SKMiddleSpine) /**< Middle spine */\
    DO(SKLowerSpine) /**< Lower spine */\
    DO(SKHip) /**< Hip */\
    DO(SKRightHip) /**< Right hip */\
    DO(SKRightKnee) /**< Right knee */\
    DO(SKRightAnkle) /**< Right ankle */\
    DO(SKRightFoot) /**< Right foot */\
    DO(SKLeftHip) /**< Left hip */\
    DO(SKLeftKnee) /**< Left knee */\
    DO(SKLeftAnkle) /**< Left ankle */\
    DO(SKLeftFoot) /**< Left foot */\
    DO(SKRightScapula) /**< Right shoulder (scapula) */\
    DO(SKRightShoulder) /**< Right shoulder (glenohumeral) */\
    DO(SKRightElbow) /**< Right elbow */\
    DO(SKRightWrist) /**< Right wrist */\
    DO(SKRightHand) /**< Right hand */\
    DO(SKLeftScapula) /**< Left shoulder (scapula) */\
    DO(SKLeftShoulder) /**< Left shoulder (glenohumeral) */\
    DO(SKLeftElbow) /**< Left elbow */\
    DO(SKLeftWrist) /**< Left wrist */\
    DO(SKLeftHand) /**< Left hand */\
    DO(SK_JOINT_COUNT) /**< Maximum number of VMC bones */

    #define MAKE_ENUM(VAR) VAR,
    
    /// \ingroup Enums
    /// @brief Runtime error codes
    enum AxisRuntimeErrors : uint16_t{
        AXIS_RUNTIME_ERRORS_ENUM(MAKE_ENUM)
    };

    /// \ingroup Enums
    /// @brief Packet types
    enum AxisPacketTypes : uint16_t{
        AXIS_PACKET_TYPES_ENUM(MAKE_ENUM)
    };

    /// \ingroup Enums
    /// @brief NodeIndex and what position on the body they represent
    enum AxisNodePositions : uint16_t{
        AXIS_NODE_POSITIONS_ENUM(MAKE_ENUM)
    };

    /// \ingroup Enums
    /// @brief Operating modes used by AXIS nodes
    enum AxisNodeOperationModes : uint16_t{
        AXIS_NODE_OPERATION_MODES_ENUM(MAKE_ENUM)
    };

    /// \ingroup Enums
    /// @brief Command types
    enum AxisDongleCommandTypes : uint16_t{
        AXIS_DONGLE_COMMAND_TYPES_ENUM(MAKE_ENUM)
    };

    /// \ingroup Enums
    /// @brief Hip Provider modes
    enum AxisHipProviderModes : uint16_t{
        AXIS_HIP_PROVIDER_MODES_ENUM(MAKE_ENUM)
    };

    /// \ingroup Enums
    /// @brief Axis Hub locations
    enum AxisHubLocations : uint16_t{
        AXIS_HUB_LOCATIONS_ENUM(MAKE_ENUM)
    };

    /// \ingroup Enums
    /// @brief Categories for node configuration. See Refract::AXIS::NodeConfig_t.
    enum AxisNodeConfigCategories : uint16_t{
        AXIS_NODE_CONFIG_CATEGORIES_ENUM(MAKE_ENUM)
    };

    /// \ingroup Enums
    /// @brief Parameters for node configuration in Refract::AXIS::AxisNodeConfigCategories::DEV category
    enum AxisNodeConfigDevParams : uint16_t{
        AXIS_NODE_CONFIG_DEV_PARAMS_ENUM(MAKE_ENUM)
    };

    /// \ingroup Enums
    /// @brief Parameters for node configuration in Refract::AXIS::AxisNodeConfigCategories::Power category
    enum AxisNodeConfigPowerParams : uint16_t{
        AXIS_NODE_CONFIG_POWER_PARAMS_ENUM(MAKE_ENUM)
    };

    /// \ingroup Enums
    /// @brief Bones used for VMC
    enum AxisVmcConfigBones : uint16_t{
        AXIS_VMC_BONE_CONFIG_BONES_ENUM(MAKE_ENUM)
    };

    /// \ingroup Enums
    /// @brief Joints used for Refract::AXIS::SkeletonData_t
    enum AxisSkeletonJoints : uint16_t{
        AXIS_SKELETON_JOINTS_ENUM(MAKE_ENUM)
    };
    #undef MAKE_ENUM

#pragma pack(push, 8)
    /// \ingroup Structs
    /// @brief Quaternion
    struct Quat_t
    {
        float x; ///< x / i
        float y; ///< y / j
        float z; ///< z / k
        float w; ///< w / r
    };

    /// \ingroup Structs
    /// @brief Vector of 3 floats
    struct Vector3_t
    {
        float x; ///< x
        float y; ///< y
        float z; ///< z
    };

    /// \ingroup Structs
    /// @brief Position and Orientation data
    struct PosQuat_t
    {
        Quat_t quat; ///< Orientation
        Vector3_t pos; ///< Position
    };

    /// \ingroup Structs
    /// @brief IMUData
    struct NodeIMUData_t
    {
        bool on; ///< Whether the node is on or not
        Quat_t quat; ///< Orientation data
        Vector3_t linAccel; ///< Linear acceleration (relative to node) in m/s2
    };

    /// \ingroup Structs
    /// @brief Node Info
    struct NodeInfo_t
    {
        AxisNodePositions nodeIndex; ///< Position of the node.
        bool on; ///< Whether the node is on or not. Only used for Refract::AXIS::AxisPacketTypes::PairedNodes
        uint8_t mac[6]; ///< MAC address of the node
    };

    /// \ingroup Packets
    /// @brief GetWifi Response
    struct WifiRes_t
    {
        uint8_t wifiChannel; ///< Wifi channel currently used
    };

    /// \ingroup Packets
    /// @brief SetWifi Response
    struct SetWifiRes_t
    {
        uint8_t nodeCount; ///< Number of nodes currently paired
        bool success; ///< Whether SetWifi was successful or not
    };

    /// \ingroup Packets
    /// @brief IMUData
    struct IMUData_t
    {
        NodeIMUData_t nodeImuData[AXIS_MAX_NODE_COUNT]; ///< Array of individual node data. Refer to Refract::AXIS::AxisNodePositions for body positions
        PosQuat_t hubData; ///< Orientation and position data from Axis Hub. Will be default value if Axis Hub is not on
    };

    /// \ingroup Packets
    /// @brief Node info
    struct NodesInfo_t
    {
        uint8_t nodeCount; ///< Number of nodes with data in the nodeInfo array
        NodeInfo_t nodeInfo[AXIS_MAX_NODE_COUNT]; ///< Array of NodeInfo. Be sure to check nodeCount
    };

    /// \ingroup Structs
    /// @brief Node battery info
    struct NodeBattInfo_t
    {
        AxisNodePositions nodeIndex; ///< Position of the node
        uint8_t batteryLevel; ///< Battery level as a percentage. 0 is node is off
        float voltageLevel; ///< Voltage of the battery. 0 is node is off
    };

    /// \ingroup Packets
    /// @brief Battery info
    struct BattInfo_t
    {
        uint8_t nodeCount; ///< Number of nodes with data in the nodeBattInfo array
        NodeBattInfo_t nodeBattInfo[AXIS_MAX_NODE_COUNT]; ///< Array of NodeInfo. Be sure to check nodeCount
    };

    /// \ingroup Structs
    /// @brief Version info
    struct Version_t
    {
        uint8_t major; ///< Major revision number
        uint8_t minor; ///< Minor revision number
        uint8_t patch; ///< Patch revision number
    };

    /// \ingroup Structs
    /// @brief Version info for the dongle
    struct DongleVersion_t
    {
        Version_t hardware; ///< Hardware version
        Version_t firmware; ///< Software version
    };

    /// \ingroup Structs
    /// @brief Version info for nodes
    struct NodeVersion_t
    {
        AxisNodePositions nodeIndex; ///< Position of the node
        Version_t hardware; ///< Hardware version
        Version_t firmware; ///< Software version
    };

    /// \ingroup Packets
    /// @brief Version information in response to a GetVersions command
    struct VersionsInfo_t
    {
        DongleVersion_t dongleVersion; ///< Version of the dongle
        uint8_t nodeCount; ///< Number of nodes with data in the nodeVersions array
        NodeVersion_t nodeVersions[AXIS_MAX_NODE_COUNT]; ///< Array of NodeVersion_t. Be sure to check nodeCount
    };

    /// \ingroup Structs
    /// @brief Suit info for nodes
    struct NodeSuitInfo_t
    {
        AxisNodePositions nodeIndex; ///< Position of the node
        uint8_t mac[6]; ///< MAC address of the node
        Version_t firmware; ///< Firmware version of the node
        AxisNodeOperationModes operationMode; ///< Operation mode of the node
        uint8_t batteryPercent; ///< Battery level of the node in percent
        bool on; ///< Whether the node is on or not
    };

    /// \ingroup Packets
    /// @brief Suit info for nodes and receiver
    struct SuitInfo_t
    {
        uint8_t nodeCount; ///< Number of nodes with data in the nodesSuitInfo array
        NodeSuitInfo_t nodesSuitInfo[AXIS_MAX_NODE_COUNT]; ///< Array of NodeSuitInfo_t. Be sure to check nodeCount
        uint8_t dongleMAC[6]; ///< MAC of the dongle
        DongleVersion_t dongleVersion; ///< Version of the dongle
        uint8_t homeWifiChannel; ///< Wifi channel the dongle should switch to after exiting pairing mode
        uint8_t currentWifiChannel; ///< Wifi channel the dongle is currently on
    };

    /// \ingroup Structs
    /// @brief Node configuration float payload
    struct NodeConfigPayloadFloat_t
    {
        float value; ///< float value
    };

    /// \ingroup Structs
    /// @brief Node configuration uint32_t payload
    struct NodeConfigPayloadUInt32_t
    {
        uint32_t value; ///< uint32_t payload
    };

    /// \ingroup Structs
    /// @brief Sensor Data for DevPacket
    struct DevPacketSensorData_t
    {
        /// @brief Sensor data
        Vector3_t data;
        /// @brief Sensor accuracy
        /// * 0 = Unreliable
        /// * 1 = Low
        /// * 2 = Medium
        /// * 3 = High 
        uint8_t acc; 
    };

    /// \ingroup Structs
    /// @brief State of the sensor fusion algorithm
    struct DevPacketFusionState_t
    {
        float accelError; ///< Accelerometer error value in degress
        float accelRecovTrigger; ///< Recovery trigger for accelerometer
        float magError; ///< Magnetometer error value in degrees
        float magRecovTrigger; ///< Recovery trigger for magnetometer
        bool accelIgnored; ///< Whether accelerometer is being ignored
        bool magIgnored; ///< Whether magnetometer is being ignored
        bool initializing; ///< Whether fusion algorithm is in initializing state
        bool angularRateRecov; ///< Whether gyroscope is in recovery state
        bool accelRecov; ///< Whether accelerometer is in recovery 
        bool magRecov; ///< Whether magnetometer is in recovery
    };

    /// \ingroup Structs
    /// @brief Tracker profile for SteamVR
    struct SteamVRTrackerProfile_t
    {
        bool rightCalf = false; ///< Whether right calf tracker is on or off
        bool leftCalf = false; ///< Whether left calf tracker is on or off
        bool rightUpperArm = false; ///< Whether right upper arm tracker is on or off
        bool rightForearm = false; ///< Whether right forearm tracker is on or off
        bool leftUpperArm = false; ///< Whether left upper arm tracker is on or off
        bool leftForearm = false; ///< Whether left forearm tracker is on or off
        bool chest = false; ///< Whether chest tracker is on or off
        bool rightFoot = false; ///< Whether right foot tracker is on or off
        bool leftFoot = false; ///< Whether left foot tracker is on or off
        bool rightWrist = false; ///< Whether right wrist tracker is on or off
        bool leftWrist = false; ///< Whether left wrist tracker is on or off
        bool rightShoulder = false; ///< Whether right shoulder tracker is on or off
        bool leftShoulder = false; ///< Whether left shoulder tracker is on or off
        bool hip = false; ///< Whether hip tracker is on or off
    };

    struct SteamVRLimbLengths_t
    {
        float height;
        float unit;
        float hmdOffsetZ;
        float hmdOffsetY;
        float neck;
        float fakeNeck;
        float shoulder;
        float torso;
        float spine3;
        float spine2;
        float spine1;
        float hipUp;
        float hipDown;
        float hipWidth;
        float hipWidthHalf;
        float thigh;
        float shin;
        float foot;
        float shoulderRootX;
        float shoulderRootY;
        float shoulderX;
        float shoulderY;
        float upperArm;
        float forearm;
        float hand;        
    };

    /// \ingroup Packets
    /// @brief A packet with raw sensor data and sensor fusion state useful for development.
    /// Only available from firmware 2.4.x onwards
    struct DevPacket_t
    {
        /// @brief Components in DevPacket
        enum DevPacketComponents : uint16_t
        {
            Accelerometer, ///< Accelerometer
            Gyroscope, ///< Gyroscope
            Magnetometer, ///< Magnetometer
            FusionStates, ///< State of fusion algorithm
            LinearAccelerometer, ///< Accelerometer with gravity removed
            DEV_PACKET_COMPONENT_COUNT,
        };
        bool flags[DEV_PACKET_COMPONENT_COUNT]; ///< Whether or not the payload has data
        /// Check flags[DevPacket_t::DevPacketComponents::Accelerometer] to see if this has data.
        /// * values are in m/s^2
        DevPacketSensorData_t accelerometer;
        /// Check flags[DevPacket_t::DevPacketComponents::Gyroscope] to see if this has data.
        /// * values are in rads/s
        DevPacketSensorData_t gyroscope; 
        /// Check flags[DevPacket_t::DevPacketComponents::Magnetometer] to see if this has data.
        /// * values are in uT
        DevPacketSensorData_t magnetometer;
        /// Check flags[DevPacket_t::DevPacketComponents::FusionStates] to see if this has data.
        DevPacketFusionState_t fusionState;
        /// Position of the node
        AxisNodePositions nodeIndex;
        /// Ticks since the node started
        uint32_t timeStamp;
        /// Check flags[DevPacket_t::DevPacketComponents::LinearAccelerometer] to see if this has data.
        /// * values are in m/s^2
        DevPacketSensorData_t linearAccelerometer;
    };

    /// \ingroup Structs
    /// @brief Payload for Node configuration packet
    typedef union
    {
        /// Flags for Refract::AXIS::DevPacket_t. See Refract::AXIS::DevPacket_t::DevPacketComponents
        /// * Used for Refract::AXIS::AxisNodeConfigCategories::DEV + Refract::AXIS::AxisNodeConfigDevParameters::DevPacketBitflags
        bool devPacketBitflags[DevPacket_t::DEV_PACKET_COMPONENT_COUNT];
        /// Generic unsigned integer value.
        /// * Used for parameters in Refract::AXIS::AxisNodeConfigCategories::Power
        /// * For Refract::AXIS::AxisNodeConfigPowerParameters::TransmissionPower, value must be between 0-20.
        /// * For Refract::AXIS::AxisNodeConfigPowerParameters::TransmissionFrequency, value must be between 62-125.
        uint32_t value;
    } NodeConfigPayload_t;

    /// \ingroup Commands
    /// \ingroup Packets
    /// @brief Node configuration packet. Only available from firmware version 2.4.x onwards
    struct NodeConfig_t
    {
        /// Category of the configuration
        /// * For Refract::AXIS::AxisDongleCommandTypes::NodeConfigGet, only Refract::AXIS::AxisNodeConfigCategories::Power is supported currently
        AxisNodeConfigCategories category;
        /// Parameter depending on the category
        /// * Refract::AXIS::AxisNodeConfigDevParams if category is Refract::AXIS::AxisNodeConfigCategories::DEV
        /// * Refract::AXIS::AxisNodeConfigPowerParams if category is Refract::AXIS::AxisNodeConfigCategories::Power
        uint8_t parameter;
        NodeConfigPayload_t payload; ///< Payload according to category and parameter
    };

    /// \ingroup Structs
    /// @brief Payload for NodeConfigResponse_t for Refract::AXIS::AxisNodeConfigCategories::Power
    struct NodeConfigResponsePowerPayload_t
    {
        /// @brief Transmission power in dBm
        uint8_t power;
        /// @brief Transmission frequency in Hz
        uint8_t frequency;
    };

    /// \ingroup Structs
    /// @brief Payload for Node configuration response packet
    typedef union
    {
        /// @brief Payload Refract::AXIS::AxisNodeConfigCategories::Power
        NodeConfigResponsePowerPayload_t power;
    } NodeConfigResponsePayload_t;

    /// \ingroup Packets
    /// @brief Node configuration response packet. Only from firmware version 2.4.x onwards
    struct NodeConfigResponse_t
    {
        /// Category of the configuration response
        /// * Only Refract::AXIS::AxisNodeConfigCategories::Power is supported currently
        AxisNodeConfigCategories category;
        NodeConfigResponsePayload_t payload; ///< Payload according to category
    };

    /// \ingroup Packets
    /// @brief Orientation and position of joints. X is right, Y is up, -Z is forward
    struct SkeletonData_t
    {
        /// Array of joint orientation and position. Use Refract::AXIS::AxisSkeletonJoints
        PosQuat_t joints[AxisSkeletonJoints::SK_JOINT_COUNT]; 
    };

    /// \ingroup Packets
    /// @brief AxisPacket. Check the packetType and access the respective member.
    typedef union 
    {
        /// * Refract::AXIS::AxisPacketTypes::IMUData
        IMUData_t imuData;
        /// * Refract::AXIS::AxisPacketTypes::VisibleNodes
        /// * Refract::AXIS::AxisPacketTypes::PairedNodes
        /// * Refract::AXIS::AxisPacketTypes::DoubleClick
        /// * Refract::AXIS::AxisPacketTypes::NodeAssignAck
        NodesInfo_t nodesInfo;
        /// * Refract::AXIS::AxisPacketTypes::SetWifiResponse
        SetWifiRes_t setWifiRes;
        /// * Refract::AXIS::AxisPacketTypes::ReportWifi
        WifiRes_t wifiRes;
        /// * Refract::AXIS::AxisPacketTypes::ReportBatt
        BattInfo_t battInfo;
        /// * Refract::AXIS::AxisPacketTypes::Versions
        VersionsInfo_t versions;
        /// * Refract::AXIS::AxisPacketTypes::SuitInfo
        SuitInfo_t suitInfo;
        /// * Refract::AXIS::AxisPacketTypes::DevPacket
        DevPacket_t devPacket;
        /// * Refract::AXIS::AxisPacketTypes::NodeConfigResponse
        NodeConfigResponse_t nodeConfigRes;
        /// * Refract::AXIS::AxisPacketTypes::SkeletonData
        SkeletonData_t skeletonData;
    } AxisPacket_t;

    /// \ingroup Commands
    /// @brief Payload for Refract::AXIS::AxisDongleCommandTypes::Reboot
    struct RebootCommandPayload_t
    {
        /// @brief Modes for RebootCommandPayload_t
        enum RebootCommandModes : uint16_t
        {
            /// Reboot all paired nodes
            AllPaired,
            /// Reboot only paired nodes on legs
            /// * Refract::AXIS::AxisNodePositions::RightThigh
            /// * Refract::AXIS::AxisNodePositions::RightCalf
            /// * Refract::AXIS::AxisNodePositions::LeftThigh
            /// * Refract::AXIS::AxisNodePositions::LeftCalf
            Legs,
            /// Reboot only paired nodes on upper body
            /// * Refract::AXIS::AxisNodePositions::RightUpperArm
            /// * Refract::AXIS::AxisNodePositions::RightForearm
            /// * Refract::AXIS::AxisNodePositions::LeftUpperArm
            /// * Refract::AXIS::AxisNodePositions::LeftForearm
            /// * Refract::AXIS::AxisNodePositions::Spine
            UpperBody,
        };
        /// Reboot mode
        RebootCommandModes mode;
    };

    /// \ingroup Commands
    /// @brief Payload for Refract::AXIS::AxisDongleCommandTypes::SetWifi
    struct WifiCommandPayload_t
    {
        /// Channel to change to.
        /// * 1 - 11 = Normal operating channels
        /// * 13 = Pairing mode
        uint8_t channel;
    };

    /// \ingroup Commands
    /// @brief Payload for Refract::AXIS::AxisDongleCommandTypes::NodeInfo
    struct NodeInfoCommandPayload_t
    {
        /// Modes for NodeInfoCommandPayload_t
        enum NodeInfoCommandModes : uint16_t
        {
            /// Get info for visible nodes (non-paired)
            Visible,
            /// Get info for paired nodes
            Paired
        };
        /// Node info mode
        NodeInfoCommandModes mode;
    };

    /// \ingroup Commands
    /// @brief Payload for Refract::AXIS::AxisDongleCommandTypes::Calibration
    struct NodeCalibrationCommandPayload_t
    {
        /// Types of calibration
        enum CalibrationCommandTypes : uint16_t
        {
            /// Do both calibrate frame and zero
            All,
            /// Only calibrate the reference frame
            CalibrateFrame,
            /// Only zero
            Zero,
        };
        /// Pose for calibration
        enum CalibrationCommandPoses : uint16_t
        {
            /// A more accurate type of calibration that requires 2 steps
            /// * Put nodes flat on the table/floor, facing forward.
            /// * Send this command with CalibrationCommandTypes::CalibrateFrame type
            /// * Put nodes on the body, ask user to do a T-Pose and face forward
            /// * Send this command with CalibrationCommandTypes::Zero type
            TableFloor,
            /// A one step calibration type
            /// * Put nodes on the body, ask user to do a T-Pose and face forward
            /// * Send this command with CalibrationCommandTypes::All
            ///
            /// If tracking goes off after some time due to the nodes slipping
            /// * Ask user to do a T-Pose and face forward
            /// * Send this command with CalibrationCommandTypes::Zero
            Body,
        };
        /// Calibration type
        CalibrationCommandTypes type;
        /// Calibration pose
        CalibrationCommandPoses pose;
    };

    /// \ingroup Commands
    /// @brief Payload for Refract::AXIS::AxisDongleCommandTypes::NodeOperationMode
    struct NodeOperationModeCommandPayload_t
    {
        /// Operation mode for the nodes
        /// * Refract::AXIS::AxisNodeOperationModes::Fusion only available on nodes 2.2.3 and above
        /// * Refract::AXIS::AxisNodeOperationModes::FusionNoMag only available on nodes 2.3.0 and above
        AxisNodeOperationModes mode;
    };

    /// @brief Payload for Refract::AXIS::AxisDongleCommandTypes::Buzz
    struct BuzzCommandPayload_t
    {
        /// Position of the node
        AxisNodePositions nodeIndex;
        /// Vibration intensity (1-255)
        uint8_t intensity;
        /// Vibration duration (value * 100 ms)
        uint8_t duration;
    };

    /// \ingroup Commands
    /// @brief Payload for Refract::AXIS::AxisDongleCommandTypes::LED
    struct LEDCommandPayload_t
    {
        /// Position of the node
        AxisNodePositions nodeIndex;
        /// Red value (0-255)
        /// * Value for this when the node starts up is 0
        uint8_t red;
        /// Green value (0-255)
        /// * Value for this when the node starts up is 183
        uint8_t green;
        /// Blue value (0-255)
        /// * Value for this when the node starts up is 178
        uint8_t blue;
        /// Brightness value (0-255)
        /// * Value for this when the node starts up is 16
        uint8_t brightness;
    };

    /// \ingroup Commands
    /// @brief Payload for Refract::AXIS::AxisDongleCommandTypes::BuzzMAC
    struct BuzzMACCommandPayload_t
    {
        /// MAC address of the node to vibrate
        uint8_t mac[6];
        /// Vibration intensity (1-255)
        uint8_t intensity;
        /// Vibration duration (value * 100 ms)
        uint8_t duration;
    };

    /// \ingroup Commands
    /// @brief Payload for Refract::AXIS::AxisDongleCommandTypes::LEDMAC
    struct LEDMACCommandPayload_t
    {
        /// MAC address of the node whose LED color is to be changed
        uint8_t mac[6];
        /// Red value (0-255)
        /// * Value for this when the node starts up is 0
        uint8_t red;
        /// Green value (0-255)
        /// * Value for this when the node starts up is 183
        uint8_t green;
        /// Blue value (0-255)
        /// * Value for this when the node starts up is 178
        uint8_t blue;
        /// Brightness value (0-255)
        /// * Value for this when the node starts up is 16
        uint8_t brightness;
    };
    
    /// \ingroup Commands
    /// @brief Payload of dongle command
    typedef union
    {
        /// Payload for Refract::AXIS::AxisDongleCommandTypes::Reboot
        RebootCommandPayload_t reboot;
        /// Payload for Refract::AXIS::AxisDongleCommandTypes::SetWifi
        WifiCommandPayload_t wifi;
        /// Payload for Refract::AXIS::AxisDongleCommandTypes::NodeAssign
        NodesInfo_t nodeAssign;
        /// Payload for Refract::AXIS::AxisDongleCommandTypes::NodeInfo
        NodeInfoCommandPayload_t nodeInfo;
        /// Payload for Refract::AXIS::AxisDongleCommandTypes::NodeAssignSingle
        NodeInfo_t nodeAssignSingle;
        /// Payload for Refract::AXIS::AxisDongleCommandTypes::NodeConfigSet & Refract::AXIS::AxisDongleCommandTypes::NodeConfigGet
        NodeConfig_t nodeConfig;
        /// Payload for Refract::AXIS::AxisDongleCommandTypes::Calibration
        NodeCalibrationCommandPayload_t calibration;
        /// Payload for Refract::AXIS::AxisDongleCommandTypes::NodeOperationMode
        NodeOperationModeCommandPayload_t nodeOperationMode;
        /// Payload for Refract::AXIS::AxisDongleCommandTypes::Buzz
        BuzzCommandPayload_t buzz;
        /// Payload for Refract::AXIS::AxisDongleCommandTypes::LED
        LEDCommandPayload_t LED;
        /// Payload for Refract::AXIS::AxisDongleCommandTypes::BuzzMAC
        BuzzMACCommandPayload_t buzzMAC;
        /// Payload for Refract::AXIS::AxisDongleCommandTypes::LEDMAC
        LEDMACCommandPayload_t LEDMAC;
    } DongleCommandPayload_t;
    
    /// \ingroup Commands
    /// @brief Dongle command
    struct AxisDongleCommand_t
    {
        /// The type of command
        AxisDongleCommandTypes type;
        /// Payload of the command. The following commands do NOT have payloads:
        /// * Refract::AXIS::AxisDongleCommandTypes::Shutdown
        /// * Refract::AXIS::AxisDongleCommandTypes::GetWifi
        /// * Refract::AXIS::AxisDongleCommandTypes::GetBatt
        /// * Refract::AXIS::AxisDongleCommandTypes::GetVersions
        /// * Refract::AXIS::AxisDongleCommandTypes::Suitinfo
        DongleCommandPayload_t payload;
    };

    /// \ingroup Structs
    /// @brief Infomation for a device with Axis Hub
    struct AxisHubInfo_t
    {
        /// Name of the device with Axis Hub
        const char* deviceName = "";
        /// IP address of the device with Axis Hub
        const char* deviceIp = "";
        /// Port of the device with Axis Hub
        int devicePort;
        /// MAC address of the device with Axis Hub
        const char* deviceMac = "";
    };

    /// \ingroup Structs
    /// @brief Array with hub information 
    struct AxisHubsInfo_t
    {
        /// Status of the SearchForAxisHub request
        /// * Refract::AXIS::AxisRuntimeErrors::OK if got the status successfully
        /// * Refract::AXIS::AxisRuntimeErrors::SocketException if there was an error opening a socket. Check errorString for the reason.
        AxisRuntimeErrors error;
        /// String describing the error if any.
        const char* errorString = "";
        /// Total number of hubs in the hubsInfo array
        uint8_t hubsDetected;
        /// Current page number. Starts from 0
        /// * hubsInfo only stores 10 so if the total is 21 there would be 3 pages total (pg 0: 1-10, pg 1: 11-20, pg 2: 21-21)
        uint8_t pageNumber;
        /// Array of information for devices with Axis Hub
        /// * Only returns first 10 detected
        AxisHubInfo_t hubsInfo[10];
    };

    /// \ingroup Structs
    /// @brief Information regarding Axis Hub status
    struct AxisHubStatus_t
    {
        /// Status of the GetHubStatus request
        /// * Refract::AXIS::AxisRuntimeErrors::OK if got the status successfully
        /// * Refract::AXIS::AxisRuntimeErrors::HubTimeout if the request timeout
        /// * Refract::AXIS::AxisRuntimeErrors::HTTPError if a HTTP error occured. Check errorCode for the reason.
        /// * Refract::AXIS::AxisRuntimeErrors::HTTPException if an exception occurred. Check errorString for the reason.
        AxisRuntimeErrors error;
        /// Error code of the error if any.
        int errorCode;
        /// String describing the error if any.
        const char* errorString = "";
        /// Battery level
        float battery;
        /// Hub device name
        const char* deviceName = "";
        /// Hub MAC address
        const char* deviceMac = "";
    };

    /// \ingroup Callbacks
    /// @brief Callback for runtime errors
    /// @param runtimeError Refract::AXIS::AxisRuntimeErrors
    typedef void (*AxisRuntimeErrorCallback)(AxisRuntimeErrors runtimeError, int errorCode, const char* errorString);
    /// \ingroup Callbacks
    /// @brief Callback for getting packets from the dongle
    /// @param packetType Refract::AXIS::AxisPacketTypes
    /// @param incomingPacket Refract::AXIS::AxisPacket_t
    typedef void (*AxisPacketCallback)(AxisPacketTypes packetType, const AxisPacket_t* incomingPacket);
    /// \ingroup Callbacks
    /// @brief Callback for getting Axis Hub info
    /// @param hubInfo Refract::AXIS::AxisHubsInfo_t
    typedef void (*AxisHubInfoCallback)(const AxisHubsInfo_t* hubInfo);
    /// \ingroup Callbacks
    /// @brief Callback for getting Axis Hub status
    /// @param hubStatus Refract::AXIS::AxisHubStatus_t
    typedef void (*AxisHubStatusCallback)(const AxisHubStatus_t* hubStatus);
    /// \ingroup Callbacks
    /// @brief Callback for when dongle is connected or disconnected
    /// @param connected
    typedef void (*AxisDongleConnectionCallback)(bool connected);

    /// \ingroup Structs
    /// @brief Configuration struct for SteamVR integration
    struct AxisSteamVRConfig_t
    {
        float userHeight = 170.0f; ///< Height of user in cm. Used to guesstimate limb lengths
        SteamVRTrackerProfile_t steamVRTrackers; ///< Which trackers for SteamVR are enabled/disabled
    };

    /// \ingroup Structs
    /// @brief Configuration struct for Axis SDK
    struct AxisSDKConfig_t
    {
        AxisRuntimeErrorCallback runtimeErrorCallback = nullptr; ///< Will be called when a runtime error occurs outside of api functions
        AxisPacketCallback packetCallback = nullptr; ///< Set this to get packets from the dongle. Must be set or SetConfig will return AxisRuntimeErrors::PacketCallbackNotSet
        AxisDongleConnectionCallback dongleConnectionCallback = nullptr; ///< Set this to get a callback when the dongle is connected / disconnected.
        bool steamVR = false; ///< Enable/disable steamVR driver integration. Default off
        AxisSteamVRConfig_t steamVRConfig; ///< Configuration for SteamVR
        AxisHipProviderModes hipProvider = AxisHipProviderModes::HipProviderNone; ///< Where hip information is being derived from
        AxisHubLocations hubLocation = AxisHubLocations::HubLocationBack; ///< Where the Axis hub is located on the user's body
    };

    /// \ingroup Structs
    /// @brief Configuration for which bones to output to VMC
    struct AxisVMCBoneConfig_t
    {
        /// Array of booleans. Use Refract::AXIS::AxisVmcConfigBones
        bool config[AxisVmcConfigBones::VMC_CONFIG_COUNT]; 
    };
#pragma pack(pop)

    /// \ingroup Functions
    /// @brief SetConfig before calling Start
    /// @param config AxisSDKConfig_t
    /// @return Refract::AXIS::AxisRuntimeErrors::OK if no errors
    /// @return Refract::AXIS::AxisRuntimeErrors::PacketCallbackNotSet if no packetCallback was set in the config
    AXIS_API AxisRuntimeErrors SetConfig(const AxisSDKConfig_t* config);
    /// \ingroup Functions
    /// @brief Start the SDK. Call SetConfig first
    /// @return Refract::AXIS::AxisRuntimeErrors::OK if no errors
    /// @return Refract::AXIS::AxisRuntimeErrors::PacketCallbackNotSet if SetConfig has not been called yet
    AXIS_API AxisRuntimeErrors Start();
    /// \ingroup Functions
    /// @brief Sends a command to the dongle
    /// @return Refract::AXIS::AxisRuntimeErrors::OK if no errors
    /// @return Refract::AXIS::AxisRuntimeErrors::InvalidDongleCommand if command was not configured properly
    /// @return Refract::AXIS::AxisRuntimeErrors::SDKNotStarted if Start has not been called yet
    AXIS_API AxisRuntimeErrors SendDongleCommand(const AxisDongleCommand_t* command);
    /// \ingroup Functions
    /// @brief Check whether the dongle is connected or not
    /// @return true if dongle is connected
    /// @return false if dongle is not connected
    AXIS_API bool IsDongleConnected();
    /// \ingroup Functions
    /// @brief Search for Axis Hubs
    /// @param callback
    /// @return Refract::AXIS::AxisRuntimeErrors::OK if no errors.
    /// @return Refract::AXIS::AxisRuntimeErrors::InvalidParameter if callback is null
    /// @return Refract::AXIS::AxisRuntimeErrors::NoLANAdapter if there was no LAN adapter that could be used to search for Axis Hubs
    /// @return Refract::AXIS::AxisRuntimeErrors::SDKNotStarted if Start has not been called yet
    /// @return Refract::AXIS::AxisRuntimeErrors::PreviousOpInProgress if Stop() was called and there are still requests pending
    /// @return Refract::AXIS::AxisRuntimeErrors::SocketException if there was an error creating the sockets needed connecting with Axis Hubs
    AXIS_API AxisRuntimeErrors SearchForAxisHub(AxisHubInfoCallback callback);
    /// \ingroup Functions
    /// @brief Returns specific page of Refract::AXIS::AxisHubsInfo_t
    /// @param pageNumber
    /// @param callback
    /// @return Refract::AXIS::AxisRuntimeErrors::OK if no errors.
    /// @return Refract::AXIS::AxisRuntimeErrors::InvalidParameter if pageNumber is out of range or callback is null
    /// @return Refract::AXIS::AxisRuntimeErrors::SDKNotStarted if Start has not been called yet
    AXIS_API AxisRuntimeErrors GetHubInfoPageNumber(uint8_t pageNumber, AxisHubInfoCallback callback);
    /// \ingroup Functions
    /// @brief Check whether the hub is connected or not
    /// @param callback Refract::AXIS::AxisRuntimeErrors::OK, Refract::AXIS::AxisRuntimeErrors::HTTPError or Refract::AXIS::AxisRuntimeErrors::HTTPException
    /// @return Refract::AXIS::AxisRuntimeErrors::OK if no errors. Callback might be different result
    /// @return Refract::AXIS::AxisRuntimeErrors::InvalidParameter if callback is null
    /// @return Refract::AXIS::AxisRuntimeErrors::InvalidIPAddress if hub IP has not been set
    /// @return Refract::AXIS::AxisRuntimeErrors::SDKNotStarted if Start has not been called yet
    /// @return Refract::AXIS::AxisRuntimeErrors::PreviousOpInProgress if Stop() was called and there are still requests pending
    /// @return Refract::AXIS::AxisRuntimeErrors::SocketException if there was an error creating the sockets needed connecting with Axis Hubs
    AXIS_API AxisRuntimeErrors IsAxisHubConnected(AxisRuntimeErrorCallback callback);
    /// \ingroup Functions
    /// @brief Pair with Hub
    /// @param ipAddress Pointer to null terminated string standard text format.
    /// @param port
    /// @param callback callback with any errors. Refract::AXIS::AxisRuntimeErrors::OK if paired successfully.
    /// @return Refract::AXIS::AxisRuntimeErrors::OK if no errors
    /// @return Refract::AXIS::AxisRuntimeErrors::InvalidIPAddress if ipAddress is invalid
    /// @return Refract::AXIS::AxisRuntimeErrors::InvalidParameter if port is invalid or callback is null
    /// @return Refract::AXIS::AxisRuntimeErrors::SDKNotStarted if Start has not been called yet
    /// @return Refract::AXIS::AxisRuntimeErrors::PreviousOpInProgress if Stop() was called and there are still requests pending
    /// @return Refract::AXIS::AxisRuntimeErrors::SocketException if there was an error creating the sockets needed connecting with Axis Hubs
    /// @return Refract::AXIS::AxisRuntimeErrors::NoSubnet if there was no lan adapter matching the ipAddress given
    AXIS_API AxisRuntimeErrors PairWithHub(const char* ipAddress, int port, AxisRuntimeErrorCallback callback);
    /// \ingroup Functions
    /// @brief Get Hub Status
    /// @param callback
    /// @return Refract::AXIS::AxisRuntimeErrors::OK if no errors
    /// @return Refract::AXIS::AxisRuntimeErrors::InvalidIPAddress if no paired hub
    /// @return Refract::AXIS::AxisRuntimeErrors::InvalidParameter if callback is null
    /// @return Refract::AXIS::AxisRuntimeErrors::SDKNotStarted if Start has not been called yet
    /// @return Refract::AXIS::AxisRuntimeErrors::PreviousOpInProgress if Stop() was called and there are still requests pending
    /// @return Refract::AXIS::AxisRuntimeErrors::SocketException if there was an error creating the sockets needed connecting with Axis Hubs
    AXIS_API AxisRuntimeErrors GetHubStatus(AxisHubStatusCallback callback);
    /// \ingroup Functions
    /// @brief Tells connected Axis Hub to start streaming
    /// @param callback Refract::AXIS::AxisRuntimeErrors::OK, Refract::AXIS::AxisRuntimeErrors::HTTPError or Refract::AXIS::AxisRuntimeErrors::HTTPException
    /// @return Refract::AXIS::AxisRuntimeErrors::OK if no errors
    /// @return Refract::AXIS::AxisRuntimeErrors::InvalidParameter if callback is null
    /// @return Refract::AXIS::AxisRuntimeErrors::InvalidIPAddress if hub IP has not been set
    /// @return Refract::AXIS::AxisRuntimeErrors::SDKNotStarted if Start has not been called yet
    /// @return Refract::AXIS::AxisRuntimeErrors::PreviousOpInProgress if Stop() was called and there are still requests pending
    /// @return Refract::AXIS::AxisRuntimeErrors::SocketException if there was an error creating the sockets needed connecting with Axis Hubs
    AXIS_API AxisRuntimeErrors StartHubStream(AxisRuntimeErrorCallback callback);
    /// \ingroup Functions
    /// @brief Tells connected Axis Hub to stop streaming
    /// @param callback Refract::AXIS::AxisRuntimeErrors::OK, Refract::AXIS::AxisRuntimeErrors::HTTPError or Refract::AXIS::AxisRuntimeErrors::HTTPException
    /// @return Refract::AXIS::AxisRuntimeErrors::OK if no errors
    /// @return Refract::AXIS::AxisRuntimeErrors::InvalidParameter if callback is null
    /// @return Refract::AXIS::AxisRuntimeErrors::InvalidIPAddress if hub IP has not been set
    /// @return Refract::AXIS::AxisRuntimeErrors::SDKNotStarted if Start has not been called yet
    /// @return Refract::AXIS::AxisRuntimeErrors::PreviousOpInProgress if Stop() was called and there are still requests pending
    /// @return Refract::AXIS::AxisRuntimeErrors::SocketException if there was an error creating the sockets needed connecting with Axis Hubs
    AXIS_API AxisRuntimeErrors StopHubStream(AxisRuntimeErrorCallback callback);
    /// \ingroup Functions
    /// @brief Tells connected Axis Hub to reset position tracking
    /// @param callback
    /// @return Refract::AXIS::AxisRuntimeErrors::OK if no errors
    /// @return Refract::AXIS::AxisRuntimeErrors::InvalidParameter if callback is null
    /// @return Refract::AXIS::AxisRuntimeErrors::InvalidIPAddress if hub IP has not been set
    /// @return Refract::AXIS::AxisRuntimeErrors::SDKNotStarted if Start has not been called yet
    /// @return Refract::AXIS::AxisRuntimeErrors::PreviousOpInProgress if Stop() was called and there are still requests pending
    /// @return Refract::AXIS::AxisRuntimeErrors::SocketException if there was an error creating the sockets needed connecting with Axis Hubs
    AXIS_API AxisRuntimeErrors ResetHubStream(AxisRuntimeErrorCallback callback);
    /// \ingroup Functions
    /// @brief Set the location of the Axis Hub (whether mounted on user's back or front)
    /// @param location
    /// @return Refract::AXIS::AxisRuntimeErrors::OK if no errors
    /// @return Refract::AXIS::AxisRuntimeErrors::InvalidParameter if location is not valid
    AXIS_API AxisRuntimeErrors SetHubLocation(AxisHubLocations location);
    /// \ingroup Functions
    /// @brief Set where the sdk should get the hip information from
    /// @param mode
    /// @return Refract::AXIS::AxisRuntimeErrors::OK if no errors
    /// @return Refract::AXIS::AxisRuntimeErrors::InvalidParameter if mode is not valid
    AXIS_API AxisRuntimeErrors SetHipProviderMode(AxisHipProviderModes mode);
    /// \ingroup Functions
    /// @brief Set which trackers for SteamVR are enabled/disabled
    /// @param trackers
    /// @return Refract::AXIS::AxisRuntimeErrors::OK if no errors
    /// @return Refract::AXIS::AxisRuntimeErrors::InvalidParameter if trackers is null
    AXIS_API AxisRuntimeErrors SetSteamVRTrackerProfile(const SteamVRTrackerProfile_t* trackers);
    /// \ingroup Functions
    /// @brief Set height for user which is used to guesstimate limb lengths
    /// @param height Unit is centimeters.
    /// @return Refract::AXIS::AxisRuntimeErrors::OK if no errors
    /// @return Refract::AXIS::AxisRuntimeErrors::InvalidParameter if height is negative / NaN
    AXIS_API AxisRuntimeErrors SetSteamVRUserHeight(float height);
    /// \ingroup Functions
    /// @brief Enables SteamVR integration
    /// @param config
    /// @return Refract::AXIS::AxisRuntimeErrors::OK if no errors
    /// @return Refract::AXIS::AxisRuntimeErrors::InvalidParameter if config is null
    /// @return Refract::AXIS::AxisRuntimeErrors::SDKNotStarted if Start has not been called yet
    AXIS_API AxisRuntimeErrors EnableSteamVR(const AxisSteamVRConfig_t* config);
    /// \ingroup Functions
    /// @brief Disables SteamVR integration
    /// @return Refract::AXIS::AxisRuntimeErrors::OK if no errors
    /// @return Refract::AXIS::AxisRuntimeErrors::SDKNotStarted if Start has not been called yet
    AXIS_API AxisRuntimeErrors DisableSteamVR();
    /// \ingroup Functions
    /// @brief Stop the SDK.
    AXIS_API void Stop();
    /// @brief Enables output to VMC output via OSC
    /// @param ipAddress Pointer to null terminated string standard text format. If null, will default to 127.0.0.1
    /// @param port Defaults to 39539
    /// @return Refract::AXIS::AxisRuntimeErrors::OK if no errors
    /// @return Refract::AXIS::AxisRuntimeErrors::SDKNotStarted if Start has not been called yet
    /// @return Refract::AXIS::AxisRuntimeErrors::InvalidIPAddress if ipAddress is invalid
    /// @return Refract::AXIS::AxisRuntimeErrors::InvalidParameter if port is invalid
    AXIS_API AxisRuntimeErrors EnableVMCOSCOutput(const char* ipAddress, int port = 39539);
    /// \ingroup Functions
    /// @brief Enables output to VR Chat via OSC
    /// @return Refract::AXIS::AxisRuntimeErrors::OK if no errors
    /// @return Refract::AXIS::AxisRuntimeErrors::SDKNotStarted if Start has not been called yet
    AXIS_API AxisRuntimeErrors EnableVRChatOSCOutput();
    /// \ingroup Functions
    /// @brief Disables OSC output 
    AXIS_API void DisableOSC();
    /// \ingroup Functions
    /// @brief Sets which bones to output to OSC. Default is all on.
    /// @param config Setting null here will set the config to default.
    AXIS_API void SetVMCBoneConfig(AxisVMCBoneConfig_t* config);
    /// \ingroup Functions
    /// @brief Set the IP of the Axis Hub
    /// @param ipAddress Pointer to null terminated string standard text format.
    /// @param port
    /// @return Refract::AXIS::AxisRuntimeErrors::OK if no errors
    /// @return Refract::AXIS::AxisRuntimeErrors::Invalid IP if IP is not valid
    AXIS_API AxisRuntimeErrors SetHubIP(const char* ipAddress, int port);

    AXIS_API const char* GetAxisRuntimeErrorName(AxisRuntimeErrors error);
    AXIS_API const char* GetAxisPacketTypeName(AxisPacketTypes type);
    AXIS_API const char* GetAxisNodePositionName(AxisNodePositions index);
    AXIS_API const char* GetAxisNodeOperationModeName(AxisNodeOperationModes mode);
    AXIS_API const char* GetAxisDongleCommandTypeName(AxisDongleCommandTypes command);
    AXIS_API const char* GetAxisHubLocationName(AxisHubLocations location);
    AXIS_API const char* GetAxisHipProviderModeName(AxisHipProviderModes mode);
    AXIS_API const char* GetAxisNodeConfigCategoryName(AxisNodeConfigCategories category);
    AXIS_API const char* GetAxisNodeConfigDevParamName(AxisNodeConfigDevParams parameter);
    AXIS_API const char* GetAxisNodeConfigPowerParamName(AxisNodeConfigPowerParams parameter);
    AXIS_API const char* GetAxisVMCBoneConfigBoneName(AxisVmcConfigBones bone);
    AXIS_API const char* GetAxisSkeletonJointName(AxisSkeletonJoints joint);

    AXIS_API AxisRuntimeErrors SetSteamVRLimbLengths(const SteamVRLimbLengths_t* lengths);
    AXIS_API void SetSteamVRManualOrientationOffset(float offset);

    // These functions are here because it is easier to do this than to try and data marshall a union from C# -> C++
    AXIS_API AxisRuntimeErrors SendNoPayloadCommand(AxisDongleCommandTypes type);
    AXIS_API AxisRuntimeErrors SendCalibrationCommand(NodeCalibrationCommandPayload_t::CalibrationCommandTypes type, NodeCalibrationCommandPayload_t::CalibrationCommandPoses pose);
    AXIS_API AxisRuntimeErrors SendRebootCommand(RebootCommandPayload_t::RebootCommandModes mode);
    AXIS_API AxisRuntimeErrors SendSetWifiCommand(uint8_t channel);
    AXIS_API AxisRuntimeErrors SendNodeAssignCommand(const NodesInfo_t* nodesInfo);
    AXIS_API AxisRuntimeErrors SendNodeInfoCommand(NodeInfoCommandPayload_t::NodeInfoCommandModes mode);
    AXIS_API AxisRuntimeErrors SendNodeAssignSingleCommand(const NodeInfo_t* nodeInfo);
    AXIS_API AxisRuntimeErrors SendSetDevpacketBitflags(bool accel, bool gyro, bool mag, bool fusion, bool linAccel);
    AXIS_API AxisRuntimeErrors SendSetPowerSettings(AxisNodeConfigPowerParams param, uint32_t value);
    AXIS_API AxisRuntimeErrors SendGetPowerSettings();
    AXIS_API AxisRuntimeErrors SendSetOperationMode(AxisNodeOperationModes mode);
    AXIS_API AxisRuntimeErrors SendLEDCommand(const LEDCommandPayload_t* command);
    AXIS_API AxisRuntimeErrors SendBuzzCommand(const BuzzCommandPayload_t* command);
    AXIS_API AxisRuntimeErrors SendLEDMACCommand(const LEDMACCommandPayload_t* command);
    AXIS_API AxisRuntimeErrors SendBuzzMACCommand(const BuzzMACCommandPayload_t* command);

    // These functions are for unit testing C++ -> C# data marshalling
    AXIS_API void TriggerTestAxisPacket(AxisPacketTypes packetType);
    AXIS_API void TriggerTestAxisHubInfo(AxisHubInfoCallback callback);
    AXIS_API void TriggerTestAxisHubStatus(AxisHubStatusCallback callback);
    AXIS_API void TriggerTestDongleConnection(AxisDongleConnectionCallback callback);
    
} 
}
#endif // _AXIS_SDK_API_H
