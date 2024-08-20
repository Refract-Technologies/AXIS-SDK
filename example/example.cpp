#include <atomic>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "axis_sdk.h"

#ifndef TEST
#define TEST(result, expectedResult, message) if (result == expectedResult) { std::cout << message << std::endl; } else { std::cout << std::to_string(result) << "!=" << std::to_string(expectedResult) << std::endl; return 1; }
#endif
#ifndef WAIT
#define WAIT(boolean, message) if (!boolean) { std::cout << message << std::endl; while (!boolean) { std::this_thread::sleep_for(std::chrono::seconds(1)); }}
#endif
#ifndef EXPECT
#define EXPECT(expectedType) s_gotExpectedPacket = false; s_expectedPacketType = expectedType;
#endif

static std::atomic_bool s_dongleConnected = false;
static std::atomic_bool s_gotIMU = false;
static std::atomic_bool s_gotExpectedPacket = false;
static Refract::AXIS::NodesInfo_t s_nodesInfo;
static Refract::AXIS::WifiRes_t s_wifiRes;
static Refract::AXIS::SuitInfo_t s_suitInfo;
static Refract::AXIS::AxisPacketTypes s_expectedPacketType = Refract::AXIS::AxisPacketTypes::IMUData;
static Refract::AXIS::PosQuat_t s_hubData;
static std::atomic_uint32_t s_imuCount;
static std::atomic_bool s_countingIMU = false;

bool CheckVersion(const Refract::AXIS::Version_t& version, uint8_t major, uint8_t minor, uint8_t patch)
{
    uint32_t versionA = (version.major << 16) + (version.minor << 8) + version.patch;
    uint32_t versionB = (major << 16) + (minor << 8) + patch;
    return versionA >= versionB;
}

void HandlePacket(Refract::AXIS::AxisPacketTypes packetType, const Refract::AXIS::AxisPacket_t* incomingPacket)
{
    using namespace Refract::AXIS;
    switch (packetType)
    {
        case AxisPacketTypes::IMUData:
            if (!s_gotIMU)
            {
                std::cout << "Got " << Refract::AXIS::GetAxisPacketTypeName(packetType) << std::endl;
                std::cout << "Right Thigh X: " << std::to_string(incomingPacket->imuData.nodeImuData[Refract::AXIS::AxisNodePositions::RightThigh].quat.x) 
                    << " Y: " << std::to_string(incomingPacket->imuData.nodeImuData[Refract::AXIS::AxisNodePositions::RightThigh].quat.y)
                    << " Z: " << std::to_string(incomingPacket->imuData.nodeImuData[Refract::AXIS::AxisNodePositions::RightThigh].quat.z)
                    << " W: " << std::to_string(incomingPacket->imuData.nodeImuData[Refract::AXIS::AxisNodePositions::RightThigh].quat.w)
                    << std::endl;
                s_hubData = incomingPacket->imuData.hubData;
                s_gotIMU = true;
            }
            else if (s_countingIMU)
            {
                s_imuCount++;
            }
            break;
        case AxisPacketTypes::Versions:
            if (s_expectedPacketType == packetType)
            {
                std::cout << "Got " << Refract::AXIS::GetAxisPacketTypeName(packetType) << std::endl;
                std::cout << "Dongle FW Version: " << std::to_string(incomingPacket->versions.dongleVersion.firmware.major) 
                    << "." << std::to_string(incomingPacket->versions.dongleVersion.firmware.minor)
                    << "." << std::to_string(incomingPacket->versions.dongleVersion.firmware.patch)
                    << std::endl;
                s_gotExpectedPacket = true;
            }
            break;
        case AxisPacketTypes::PairedNodes:
            if (s_expectedPacketType == packetType)
            {
                s_nodesInfo = incomingPacket->nodesInfo;
                std::cout << "Got " << Refract::AXIS::GetAxisPacketTypeName(packetType) << std::endl;
                std::cout << "Paired nodes: " << std::to_string(incomingPacket->nodesInfo.nodeCount) << std::endl;;
                s_gotExpectedPacket = true;
            }
            break;
        case AxisPacketTypes::VisibleNodes:
            if (s_expectedPacketType == packetType)
            {
                s_nodesInfo = incomingPacket->nodesInfo;
                std::cout << "Got " << Refract::AXIS::GetAxisPacketTypeName(packetType) << std::endl;
                std::cout << "Visible nodes: " << std::to_string(incomingPacket->nodesInfo.nodeCount) << std::endl;;
                s_gotExpectedPacket = true;
            }
            break;
        case AxisPacketTypes::SetWifiResponse:
            if (s_expectedPacketType == packetType)
            {
                std::cout << "Got " << Refract::AXIS::GetAxisPacketTypeName(packetType) << std::endl;
                std::cout << "SetWifi success: " << (incomingPacket->setWifiRes.success ? "true" : "false") << std::endl;;
                s_gotExpectedPacket = true;
            }
            break;
        case AxisPacketTypes::ReportWifi:
            if (s_expectedPacketType == packetType)
            {
                std::cout << "Got " << Refract::AXIS::GetAxisPacketTypeName(packetType) << std::endl;
                std::cout << "Wifi Channel: " << std::to_string(incomingPacket->wifiRes.wifiChannel) << std::endl;;
                s_wifiRes = incomingPacket->wifiRes;
                s_gotExpectedPacket = true;
            }
            break;
        case AxisPacketTypes::NodeAssignAck:
            if (s_expectedPacketType == packetType)
            {
                std::cout << "Got " << Refract::AXIS::GetAxisPacketTypeName(packetType) << std::endl;
                s_gotExpectedPacket = true;
            }
            break;
        case AxisPacketTypes::SuitInfo:
            if (s_expectedPacketType == packetType)
            {
                std::cout << "Got " << Refract::AXIS::GetAxisPacketTypeName(packetType) << std::endl;
                s_suitInfo = incomingPacket->suitInfo;
                s_gotExpectedPacket = true;
            }
            break;
        case AxisPacketTypes::DevPacket:
            if (s_expectedPacketType == packetType)
            {
                std::cout << "Got " << Refract::AXIS::GetAxisPacketTypeName(packetType) << std::endl;
                if (incomingPacket->devPacket.flags[Refract::AXIS::DevPacket_t::DevPacketComponents::Accelerometer])
                {
                    std::cout << "Accel {" << std::to_string(incomingPacket->devPacket.accelerometer.data.x)
                    << "," << std::to_string(incomingPacket->devPacket.accelerometer.data.y)
                    << "," << std::to_string(incomingPacket->devPacket.accelerometer.data.z)
                    << "}[" << std::to_string(incomingPacket->devPacket.accelerometer.acc)
                    << "]" << std::endl;
                }
                if (incomingPacket->devPacket.flags[Refract::AXIS::DevPacket_t::DevPacketComponents::Gyroscope])
                {
                    std::cout << "Gyro {" << std::to_string(incomingPacket->devPacket.gyroscope.data.x)
                    << "," << std::to_string(incomingPacket->devPacket.gyroscope.data.y)
                    << "," << std::to_string(incomingPacket->devPacket.gyroscope.data.z)
                    << "}[" << std::to_string(incomingPacket->devPacket.gyroscope.acc)
                    << "]" << std::endl;
                }
                if (incomingPacket->devPacket.flags[Refract::AXIS::DevPacket_t::DevPacketComponents::Magnetometer])
                {
                    std::cout << "Mag {" << std::to_string(incomingPacket->devPacket.magnetometer.data.x)
                    << "," << std::to_string(incomingPacket->devPacket.magnetometer.data.y)
                    << "," << std::to_string(incomingPacket->devPacket.magnetometer.data.z)
                    << "}[" << std::to_string(incomingPacket->devPacket.magnetometer.acc)
                    << "]" << std::endl;
                }
                if (incomingPacket->devPacket.flags[Refract::AXIS::DevPacket_t::DevPacketComponents::FusionStates])
                {
                    std::cout << "Fusion States { AccelError:" << std::to_string(incomingPacket->devPacket.fusionState.accelError)
                    << ", AccelIgnored: " << std::to_string(incomingPacket->devPacket.fusionState.accelIgnored)
                    << ", AccelRecov: " << std::to_string(incomingPacket->devPacket.fusionState.accelRecov)
                    << ", AccelRecovTrigger: " << std::to_string(incomingPacket->devPacket.fusionState.accelRecovTrigger)
                    << ", AngularRateRecov: " << std::to_string(incomingPacket->devPacket.fusionState.angularRateRecov)
                    << ", Initializing: " << std::to_string(incomingPacket->devPacket.fusionState.initializing)
                    << ", MagError: " << std::to_string(incomingPacket->devPacket.fusionState.magError)
                    << ", MagIgnored: " << std::to_string(incomingPacket->devPacket.fusionState.magIgnored)
                    << ", MagRecov: " << std::to_string(incomingPacket->devPacket.fusionState.magRecov)
                    << ", MagRecovTrigger: " << std::to_string(incomingPacket->devPacket.fusionState.magRecovTrigger)
                    << " }" << std::endl;
                }
                s_gotExpectedPacket = true;
            }
            break;
         case AxisPacketTypes::SkeletonData:
            if (s_expectedPacketType == packetType)
            {
                std::cout << "Got Skeleton Data " << std::to_string(incomingPacket->skeletonData.joints[0].pos.x) << "," 
                    << std::to_string(incomingPacket->skeletonData.joints[0].pos.y) << "," 
                    << std::to_string(incomingPacket->skeletonData.joints[0].pos.z)
                    << std::endl;
                s_gotExpectedPacket = true;
                s_expectedPacketType = Refract::AXIS::AxisPacketTypes::IMUData;
            }
            break;
        default:
            break;
    }
}

void OnDongleConnection(bool connected)
{
    s_dongleConnected = connected;
}

void RuntimeErrorCallback(Refract::AXIS::AxisRuntimeErrors runtimeError, int errorCode, const char* errorString)
{
    if (runtimeError != Refract::AXIS::AxisRuntimeErrors::OK)
    {
        std::cout << "Error!: " << Refract::AXIS::GetAxisRuntimeErrorName(runtimeError) << " code: " << std::to_string(errorCode);
        if (errorString) { std::cout << " message: " << errorString; }
        std::cout << std::endl;
    }
}

int DoIMUCount()
{
    Refract::AXIS::AxisSteamVRConfig_t steamConf;
    steamConf.userHeight = 170.0f;
    steamConf.steamVRTrackers.chest = false;
    steamConf.steamVRTrackers.hip = false;
    steamConf.steamVRTrackers.leftCalf = false;
    steamConf.steamVRTrackers.leftFoot = false;
    steamConf.steamVRTrackers.leftForearm = false;
    steamConf.steamVRTrackers.leftShoulder = false;
    steamConf.steamVRTrackers.leftUpperArm = false;
    steamConf.steamVRTrackers.leftWrist = false;
    steamConf.steamVRTrackers.rightCalf = true;
    steamConf.steamVRTrackers.rightFoot = false;
    steamConf.steamVRTrackers.rightForearm = false;
    steamConf.steamVRTrackers.rightShoulder = false;
    steamConf.steamVRTrackers.rightUpperArm = false;
    steamConf.steamVRTrackers.rightWrist = false;
    auto res = Refract::AXIS::EnableSteamVR(&steamConf);
    TEST(res, Refract::AXIS::AxisRuntimeErrors::OK, "Enabled SteamVR")
    Refract::AXIS::SetVMCBoneConfig(nullptr);
    res = Refract::AXIS::EnableVMCOSCOutput(nullptr);
    TEST(res, Refract::AXIS::AxisRuntimeErrors::OK, "Enabled VMC OSC Output")
    std::chrono::time_point start = std::chrono::system_clock::now();
    std::cout << "Calculating IMUdata frequency, please wait 10 seconds" << std::endl;
    s_imuCount = 0;
    s_countingIMU = true;
    std::this_thread::sleep_for(std::chrono::seconds(10));
    s_countingIMU = false;
    std::chrono::time_point now = std::chrono::system_clock::now();
    std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
    uint32_t count = s_imuCount.load();
    float pps = count / (duration.count() / 1000.0f);
    std::cout << "Got " + std::to_string(s_imuCount.load()) << " IMU packets in " 
        << std::to_string(duration.count()) << " ms (" << std::to_string(pps) << "Hz)" << std::endl;
    res = Refract::AXIS::DisableSteamVR();
    TEST(res, Refract::AXIS::AxisRuntimeErrors::OK, "Disabled SteamVR")
    return 0;
}

int DoSerialExample()
{
    std::cout << "Dongle connected, getting version" << std::endl;
    EXPECT(Refract::AXIS::AxisPacketTypes::Versions)
    Refract::AXIS::AxisDongleCommand_t command;
    command.type = Refract::AXIS::AxisDongleCommandTypes::GetVersions;
    auto res = Refract::AXIS::SendDongleCommand(&command);
    TEST(res, Refract::AXIS::AxisRuntimeErrors::OK, "Send GetVersions OK")
    WAIT(s_gotExpectedPacket, "Waiting for version")

    // Get paired nodes
    EXPECT(Refract::AXIS::AxisPacketTypes::PairedNodes);
    command.type = Refract::AXIS::AxisDongleCommandTypes::NodeInfo;
    command.payload.nodeInfo.mode = Refract::AXIS::NodeInfoCommandPayload_t::NodeInfoCommandModes::Paired;
    res = Refract::AXIS::SendDongleCommand(&command);
    TEST(res, Refract::AXIS::AxisRuntimeErrors::OK, "Send Node Info Paired OK")
    WAIT(s_gotExpectedPacket, "Waiting for paired nodes")

    if (s_nodesInfo.nodeCount > 0)
    {
        EXPECT(Refract::AXIS::AxisPacketTypes::NodeAssignAck);
        std::cout << std::to_string(s_nodesInfo.nodeCount) << " nodes paired. Unpairing all" << std::endl;
        command.type = Refract::AXIS::AxisDongleCommandTypes::NodeAssign;
        // in order to unassign nodes using this command, just don't have them in the list
        command.payload.nodeAssign.nodeCount = 0;
        res = Refract::AXIS::SendDongleCommand(&command);
        TEST(res, Refract::AXIS::AxisRuntimeErrors::OK, "Send NodeAssign OK")
        WAIT(s_gotExpectedPacket, "Waiting for NodeAssignAck")
        EXPECT(Refract::AXIS::AxisPacketTypes::PairedNodes);
        command.type = Refract::AXIS::AxisDongleCommandTypes::NodeInfo;
        command.payload.nodeInfo.mode = Refract::AXIS::NodeInfoCommandPayload_t::NodeInfoCommandModes::Paired;
        res = Refract::AXIS::SendDongleCommand(&command);
        WAIT(s_gotExpectedPacket, "Waiting for PairedNodes")
        TEST((s_nodesInfo.nodeCount == 0), true, "All nodes unpaired")
    }

    EXPECT(Refract::AXIS::AxisPacketTypes::ReportWifi);
    command.type = Refract::AXIS::AxisDongleCommandTypes::GetWifi;
    res = Refract::AXIS::SendDongleCommand(&command);
    TEST(res, Refract::AXIS::AxisRuntimeErrors::OK, "Send GetWifi OK")
    WAIT(s_gotExpectedPacket, "Waiting for NodeAssignAck")
    if (s_wifiRes.wifiChannel == 13)
    {
        std::cout << "Wifi channel shouldn't be 13 at this point" << std::endl;
        return 1;
    }

    // Change to Wifi channel 13 for pairing mode
    EXPECT(Refract::AXIS::AxisPacketTypes::SetWifiResponse);
    command.type = Refract::AXIS::AxisDongleCommandTypes::SetWifi;
    command.payload.wifi.channel = 12;
    res = Refract::AXIS::SendDongleCommand(&command);
    TEST(res, Refract::AXIS::AxisRuntimeErrors::InvalidDongleCommand, "SetWifi can only accept channel 1-11 and channel 13 for pairing mode")
    command.payload.wifi.channel = 13;
    res = Refract::AXIS::SendDongleCommand(&command);
    TEST(res, Refract::AXIS::AxisRuntimeErrors::OK, "SetWifi to 13 sent")
    WAIT(s_gotExpectedPacket, "Waiting for SetWifiResponse")

    // Get visible nodes
    s_nodesInfo.nodeCount = 0;
    while (s_nodesInfo.nodeCount == 0)
    {
        EXPECT(Refract::AXIS::AxisPacketTypes::VisibleNodes);
        command.type = Refract::AXIS::AxisDongleCommandTypes::NodeInfo;
        command.payload.nodeInfo.mode = Refract::AXIS::NodeInfoCommandPayload_t::NodeInfoCommandModes::Visible;
        res = Refract::AXIS::SendDongleCommand(&command);
        TEST(res, Refract::AXIS::AxisRuntimeErrors::OK, "Looking for visible nodes")
        WAIT(s_gotExpectedPacket, "Waiting for VisibleNodes. Turn on a node by holding down the power button for 3 seconds to make it start in pairing mode.")
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    while(s_nodesInfo.nodeCount > 1)
    {
        std::cout << "More than 1 node found. Please turn off the other nodes" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
    if (s_nodesInfo.nodeCount == 1)
    {
        std::cout << "Got a visible node. Changing LED color to green" << std::endl;
        // Node is still unpaired so you need to use LEDMAC or BUZZMAC commands to buzz or change the LED color
        command.type = Refract::AXIS::AxisDongleCommandTypes::LEDMAC;
        command.payload.LEDMAC.green = 200;
        command.payload.LEDMAC.blue = 0;
        command.payload.LEDMAC.red = 0;
        command.payload.LEDMAC.brightness = 16;
        for (uint8_t i = 0; i < 6; i++)
        {
            command.payload.LEDMAC.mac[i]= s_nodesInfo.nodeInfo[0].mac[i];
        }
        res = Refract::AXIS::SendDongleCommand(&command);
        TEST(res, Refract::AXIS::AxisRuntimeErrors::OK, "Sent LEDMAC command")
        EXPECT(Refract::AXIS::AxisPacketTypes::NodeAssignAck)
        command.type = Refract::AXIS::AxisDongleCommandTypes::NodeAssignSingle;
        for (uint8_t i = 0; i < 6; i++)
        {
            command.payload.nodeAssignSingle.mac[i]= s_nodesInfo.nodeInfo[0].mac[i];
        }
        std::this_thread::sleep_for(std::chrono::seconds(5));
        // Assigning node to Right Thigh
        command.payload.nodeAssignSingle.nodeIndex = Refract::AXIS::AxisNodePositions::RightThigh;
        res = Refract::AXIS::SendDongleCommand(&command);
        TEST(res, Refract::AXIS::AxisRuntimeErrors::OK, "Sent NodeAssignSingle")
        WAIT(s_gotExpectedPacket, "Waiting for NodeAssignAck")
    }

    // Get SuitInfo to find out what the home wifi channel is
    EXPECT(Refract::AXIS::AxisPacketTypes::SuitInfo)
    command.type = Refract::AXIS::AxisDongleCommandTypes::Suitinfo;
    res = Refract::AXIS::SendDongleCommand(&command);
    TEST(res, Refract::AXIS::AxisRuntimeErrors::OK, "Sent SuitInfo")
    WAIT(s_gotExpectedPacket, "Waiting for SuitInfo")

    // Change to Wifi channel home channel
    EXPECT(Refract::AXIS::AxisPacketTypes::SetWifiResponse);
    command.type = Refract::AXIS::AxisDongleCommandTypes::SetWifi;
    command.payload.wifi.channel = s_suitInfo.homeWifiChannel;
    res = Refract::AXIS::SendDongleCommand(&command);
    TEST(res, Refract::AXIS::AxisRuntimeErrors::OK, "SetWifi to home channel sent")
    WAIT(s_gotExpectedPacket, "Waiting for SetWifiResponse")
    
    // Now that node has been assigned you can buzz or change its LED using nodeIndex
    command.type = Refract::AXIS::AxisDongleCommandTypes::LED;
    command.payload.LED.blue = 178;
    command.payload.LED.green = 183;
    command.payload.LED.red = 0;
    command.payload.LED.brightness = 16;
    command.payload.LED.nodeIndex = Refract::AXIS::AxisNodePositions::RightThigh;
    res = Refract::AXIS::SendDongleCommand(&command);
    TEST(res, Refract::AXIS::AxisRuntimeErrors::OK, "Setting LED color back to what it was")
    std::this_thread::sleep_for(std::chrono::seconds(2));
    command.type = Refract::AXIS::AxisDongleCommandTypes::Buzz;
    command.payload.buzz.duration = 5;
    command.payload.buzz.intensity = 10;
    command.payload.buzz.nodeIndex = Refract::AXIS::AxisNodePositions::RightThigh;
    res = Refract::AXIS::SendDongleCommand(&command);
    TEST(res, Refract::AXIS::AxisRuntimeErrors::OK, "Buzzing the node")
    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << "Node Version " << std::to_string(s_suitInfo.nodesSuitInfo[Refract::AXIS::AxisNodePositions::RightThigh].firmware.major)
        << "." << std::to_string(s_suitInfo.nodesSuitInfo[Refract::AXIS::AxisNodePositions::RightThigh].firmware.minor)
        << "." << std::to_string(s_suitInfo.nodesSuitInfo[Refract::AXIS::AxisNodePositions::RightThigh].firmware.patch) << std::endl;

    // both dongle and node must be 2.4.0 or above
    bool devPacketOK = CheckVersion(s_suitInfo.dongleVersion.firmware, 2, 4, 0)
        && CheckVersion(s_suitInfo.nodesSuitInfo[Refract::AXIS::AxisNodePositions::RightThigh].firmware, 2, 4, 0);

    command.type = Refract::AXIS::AxisDongleCommandTypes::NodeConfigSet;
    command.payload.nodeConfig.category = Refract::AXIS::AxisNodeConfigCategories::DEV;
    command.payload.nodeConfig.parameter = Refract::AXIS::AxisNodeConfigDevParams::DevPacketBitflags;
    command.payload.nodeConfig.payload.devPacketBitflags[Refract::AXIS::DevPacket_t::Accelerometer] = true;
    command.payload.nodeConfig.payload.devPacketBitflags[Refract::AXIS::DevPacket_t::Gyroscope] = true;
    command.payload.nodeConfig.payload.devPacketBitflags[Refract::AXIS::DevPacket_t::Magnetometer] = true;
    command.payload.nodeConfig.payload.devPacketBitflags[Refract::AXIS::DevPacket_t::FusionStates] = true;
    EXPECT(Refract::AXIS::AxisPacketTypes::DevPacket);
    res = Refract::AXIS::SendDongleCommand(&command);
    TEST(res, Refract::AXIS::AxisRuntimeErrors::OK, "Sending NodeConfig for DevPacket")
    // Nothing will happen if sending commands that are not supported by version
    if (devPacketOK) { WAIT(s_gotExpectedPacket, "Waiting for DevPacket") }

    command.payload.nodeConfig.category = Refract::AXIS::AxisNodeConfigCategories::DEV;
    command.payload.nodeConfig.parameter = Refract::AXIS::AxisNodeConfigDevParams::DevPacketBitflags;
    command.payload.nodeConfig.payload.devPacketBitflags[Refract::AXIS::DevPacket_t::Accelerometer] = false;
    command.payload.nodeConfig.payload.devPacketBitflags[Refract::AXIS::DevPacket_t::Gyroscope] = false;
    command.payload.nodeConfig.payload.devPacketBitflags[Refract::AXIS::DevPacket_t::Magnetometer] = false;
    command.payload.nodeConfig.payload.devPacketBitflags[Refract::AXIS::DevPacket_t::FusionStates] = false;
    res = Refract::AXIS::SendDongleCommand(&command);
    TEST(res, Refract::AXIS::AxisRuntimeErrors::OK, "Turning off DevPacket")

    return 0;
}

static std::atomic_bool s_gotHubsInfo;
static Refract::AXIS::AxisHubsInfo_t s_hubsInfo;
static std::atomic_bool s_gotHubCallback;
static Refract::AXIS::AxisRuntimeErrors s_hubCallbackStatus;
static Refract::AXIS::AxisHubStatus_t s_hubStatus;

void AxisHubInfoCallback(const Refract::AXIS::AxisHubsInfo_t* hubInfo)
{
    s_hubsInfo = *hubInfo;
    std::cout << "Got HubInfo" << std::endl;
    s_gotHubsInfo = true;
}

void AxisHubErrorCallback(Refract::AXIS::AxisRuntimeErrors runtimeError, int errorCode, const char* errorString)
{
    s_gotHubCallback = true;
    s_hubCallbackStatus = runtimeError;
    if (runtimeError != Refract::AXIS::AxisRuntimeErrors::OK)
    {
        std::cout << "Got AxisHub callback error " << Refract::AXIS::GetAxisRuntimeErrorName(runtimeError) 
            << " " << std::to_string(errorCode) << " " << errorString << std::endl;
    }
    else
    {
        std::cout << "Got AxisHub callback" << std::endl;
    }
}

void AxisHubStatusCallback(const Refract::AXIS::AxisHubStatus_t* hubStatus)
{
    s_gotHubCallback = true;
    s_hubStatus = *hubStatus;
    if (s_hubStatus.error != Refract::AXIS::AxisRuntimeErrors::OK)
    {
        std::cout << "Got HubStatus callback error " << Refract::AXIS::GetAxisRuntimeErrorName(s_hubStatus.error) 
            << " " << std::to_string(s_hubStatus.errorCode) << " " << s_hubStatus.errorString << std::endl;
    }
}

int DoAxisHubExample()
{   
    while (s_hubsInfo.hubsDetected == 0)
    {
        s_gotHubsInfo = false;
        auto res = Refract::AXIS::SearchForAxisHub(&AxisHubInfoCallback);
        if (res != Refract::AXIS::AxisRuntimeErrors::OK)
        {
            std::cout << Refract::AXIS::GetAxisRuntimeErrorName(res) << std::endl;
            return 1;
        }
        WAIT(s_gotHubsInfo, "Waiting for hubsInfo. Press Start pairing on your Axis Hub app.");
        if (s_hubsInfo.error != Refract::AXIS::AxisRuntimeErrors::OK)
        {
            std::cout << Refract::AXIS::GetAxisRuntimeErrorName(s_hubsInfo.error) << " " << std::string(s_hubsInfo.errorString) << std::endl;
            return 1;
        }
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    
    std::cout << std::to_string(s_hubsInfo.hubsDetected) << " hubs detected." << std::endl;
    std::cout << "Name: " << s_hubsInfo.hubsInfo[0].deviceName << ". IP: " << s_hubsInfo.hubsInfo[0].deviceIp 
        << ". Mac: " << s_hubsInfo.hubsInfo[0].deviceMac << ". Port: " << std::to_string(s_hubsInfo.hubsInfo[0].devicePort) << std::endl;
    s_gotHubCallback = false;
    auto res = Refract::AXIS::PairWithHub(s_hubsInfo.hubsInfo[0].deviceIp, s_hubsInfo.hubsInfo[0].devicePort, &AxisHubErrorCallback);
    TEST(res, Refract::AXIS::AxisRuntimeErrors::OK, "PairWithHub OK")
    WAIT(s_gotHubCallback, "Waiting for PairWithHub callback")
    TEST(s_hubCallbackStatus, Refract::AXIS::AxisRuntimeErrors::OK, "Paired With Hub")

    s_gotHubCallback = false;
    res = Refract::AXIS::StartHubStream(&AxisHubErrorCallback);
    TEST(res, Refract::AXIS::AxisRuntimeErrors::OK, "StartHubStream OK")
    WAIT(s_gotHubCallback, "Waiting for StartHubStream callback")
    TEST(s_hubCallbackStatus, Refract::AXIS::AxisRuntimeErrors::OK, "Started Hub Stream")

    s_gotIMU = false;
    WAIT(s_gotIMU, "Waiting for IMUData")
    std::cout << "Hub quat {" << std::to_string(s_hubData.quat.x) << "," << std::to_string(s_hubData.quat.y)
        << "," << std::to_string(s_hubData.quat.z) << "," << std::to_string(s_hubData.quat.w) << "}" << std::endl;

    s_gotHubCallback = false;
    res = Refract::AXIS::StopHubStream(&AxisHubErrorCallback);
    TEST(res, Refract::AXIS::AxisRuntimeErrors::OK, "StopHubStream OK")
    WAIT(s_gotHubCallback, "Waiting for StopHubStream callback")
    TEST(s_hubCallbackStatus, Refract::AXIS::AxisRuntimeErrors::OK, "Stopped Hub Stream")

    s_gotHubCallback = false;
    res = Refract::AXIS::GetHubStatus(&AxisHubStatusCallback);
    TEST(res, Refract::AXIS::AxisRuntimeErrors::OK, "GetHubStatus OK")
    WAIT(s_gotHubCallback, "Waiting for GetHubStatus callback")
    TEST(s_hubStatus.error, Refract::AXIS::AxisRuntimeErrors::OK, "Got Hub Status")
    
    std::cout << "Name: " << s_hubStatus.deviceName << ". Mac: " << s_hubStatus.deviceMac << ". Hub Batt: " << std::to_string(s_hubStatus.battery) << std::endl;

    return 0;
}

int DoSteamVRExample()
{
    Refract::AXIS::AxisSteamVRConfig_t steamConf;
    steamConf.userHeight = 170.0f;
    steamConf.steamVRTrackers.chest = false;
    steamConf.steamVRTrackers.hip = false;
    steamConf.steamVRTrackers.leftCalf = false;
    steamConf.steamVRTrackers.leftFoot = false;
    steamConf.steamVRTrackers.leftForearm = false;
    steamConf.steamVRTrackers.leftShoulder = false;
    steamConf.steamVRTrackers.leftUpperArm = false;
    steamConf.steamVRTrackers.leftWrist = false;
    steamConf.steamVRTrackers.rightCalf = true;
    steamConf.steamVRTrackers.rightFoot = false;
    steamConf.steamVRTrackers.rightForearm = false;
    steamConf.steamVRTrackers.rightShoulder = false;
    steamConf.steamVRTrackers.rightUpperArm = false;
    steamConf.steamVRTrackers.rightWrist = false;
    auto res = Refract::AXIS::EnableSteamVR(&steamConf);
    TEST(res, Refract::AXIS::AxisRuntimeErrors::OK, "Enabled SteamVR")

    std::cout << "You can now test SteamVR. Input anything and press Enter to end steamVR example" << std::endl;
    char input;
    std::cin >> input;

    res = Refract::AXIS::DisableSteamVR();
    TEST(res, Refract::AXIS::AxisRuntimeErrors::OK, "Disabled SteamVR")

    return 0;
}

int main()
{
    Refract::AXIS::AxisDongleCommand_t command;
    int result = 0;
    Refract::AXIS::AxisSDKConfig_t config;
    Refract::AXIS::AxisRuntimeErrors res;
    char input;

    res = Refract::AXIS::SetConfig(nullptr);
    TEST(res, Refract::AXIS::AxisRuntimeErrors::InvalidParameter, "Can't pass in null config")
    res = Refract::AXIS::Start();
    TEST(res, Refract::AXIS::AxisRuntimeErrors::PacketCallbackNotSet, "Can't call Start before calling SetConfig")
    command.type = Refract::AXIS::AxisDongleCommandTypes::GetVersions;
    res = Refract::AXIS::SendDongleCommand(&command);
    TEST(res, Refract::AXIS::AxisRuntimeErrors::SDKNotStarted, "Need to start SDK first before calling other functions")
    res = Refract::AXIS::SetConfig(&config);
    TEST(res, Refract::AXIS::AxisRuntimeErrors::PacketCallbackNotSet, "Need to pass in a packetcallback")
    config.packetCallback = &HandlePacket;
    res = Refract::AXIS::SetConfig(&config);
    TEST(res, Refract::AXIS::AxisRuntimeErrors::OK, "Config OK")
    
    // set other callbacks
    config.dongleConnectionCallback = &OnDongleConnection;
    config.runtimeErrorCallback = &RuntimeErrorCallback;
    Refract::AXIS::SetConfig(&config);
    
    res = Refract::AXIS::Start();
    TEST(res, Refract::AXIS::AxisRuntimeErrors::OK, "Start OK")

    res = Refract::AXIS::AxisRuntimeErrors::OK;
    EXPECT(Refract::AXIS::AxisPacketTypes::SkeletonData);
    s_dongleConnected = Refract::AXIS::IsDongleConnected();
    if (!s_dongleConnected)
    {
        WAIT(s_dongleConnected, "Dongle not connected. Please connect the dongle")
    }

    result = DoAxisHubExample();
    TEST(result, 0, "Axis Hub Example Done");

#ifdef _WIN32
    result = DoSteamVRExample();
    TEST(result, 0, "SteamVR Example Done");
#endif

    // Unassign the node
    EXPECT(Refract::AXIS::AxisPacketTypes::NodeAssignAck)
    command.type = Refract::AXIS::AxisDongleCommandTypes::NodeAssignSingle;
    command.payload.nodeAssignSingle.nodeIndex = Refract::AXIS::AxisNodePositions::RightThigh;
    // to unassign, set the mac to all 0 for the nodeIndex you want to unassign
    for (uint8_t i = 0; i < 6; i++)
    {
        command.payload.nodeAssignSingle.mac[i] = 0;
    }
    res = Refract::AXIS::SendDongleCommand(&command);
    TEST(res, Refract::AXIS::AxisRuntimeErrors::OK, "Sent NodeAssignSingle to unassign node")
    WAIT(s_gotExpectedPacket, "Waiting for NodeAssignAck")

    // Get paired nodes
    EXPECT(Refract::AXIS::AxisPacketTypes::PairedNodes);
    command.type = Refract::AXIS::AxisDongleCommandTypes::NodeInfo;
    command.payload.nodeInfo.mode = Refract::AXIS::NodeInfoCommandPayload_t::NodeInfoCommandModes::Paired;
    res = Refract::AXIS::SendDongleCommand(&command);
    TEST(res, Refract::AXIS::AxisRuntimeErrors::OK, "Send Node Info Paired OK")
    WAIT(s_gotExpectedPacket, "Waiting for paired nodes")

    if (s_nodesInfo.nodeCount == 0)
    {
        std::cout << "No more nodes paired" << std::endl;
    }
    else
    {
        std::cout << "Node still paired" << std::endl;
        return 1;
    }

    std::cout << "Input anything and press Enter to end" << std::endl;
    std::cin >> input;
    return 0;
}