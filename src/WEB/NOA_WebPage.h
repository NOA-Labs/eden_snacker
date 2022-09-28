/*
  NOA_WebPage.h - Library for Web Page.
  Copyright 2012 NOA Labs
  Copyright 2021 Mike mao
  Released under an MIT license. See LICENSE file. 
*/

#ifndef NOA_WEB_PAGE_H
#define NOA_WEB_PAGE_H

#ifdef __cplusplus
extern "C" {
#endif

// static Html page
// create a favion: https://www.favicon.cc/
// convert to hex: http://tomeko.net/online_tools/file_to_hex.php?lang=en or https://www.onlinehexeditor.com/
// Please note that if PROGMEM variables are not globally defined, 
// you have to define them locally with static keyword, in order to work with PROGMEM.
/* const static char tblFavicon[] PROGMEM = {0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x10, 0x10, 0x10, 0x00, 0x01, 0x00, 0x04, 0x00, 0x28, 0x01, 
                                          0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x20, 0x00, 
                                          0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                          0x00, 0x00, 0x82, 0x7E, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x11, 0x11, 0x10, 0x00, 0x00, 0x00, 0x00, 
                                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                          0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                          0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                          0x00, 0x11, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x01, 0x00, 0x01, 0x00, 0x10, 0x00, 0x10, 0x10, 
                                          0x01, 0x00, 0x01, 0x00, 0x10, 0x00, 0x11, 0x10, 0x01, 0x00, 0x01, 0x00, 0x11, 0x10, 0x10, 0x10, 
                                          0x01, 0x00, 0x01, 0x00, 0x10, 0x10, 0x10, 0x10, 0x11, 0x10, 0x11, 0x10, 0x11, 0x10, 0xFF, 0xFF, 
                                          0x00, 0x00, 0xF0, 0x1F, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFE, 0xFF, 0x00, 0x00, 0xFE, 0xFF, 
                                          0x00, 0x00, 0xFE, 0xFF, 0x00, 0x00, 0xFE, 0xFF, 0x00, 0x00, 0xFA, 0xBF, 0x00, 0x00, 0xFC, 0x7F, 
                                          0x00, 0x00, 0xFE, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x5B, 0xB7, 0x00, 0x00, 0x5B, 0xB7, 
                                          0x00, 0x00, 0x1B, 0xB1, 0x00, 0x00, 0x5B, 0xB5, 0x00, 0x00, 0x51, 0x11, 0x00, 0x00 
                                         }; */
// 0066cc
// 378BE4
// /cjson/wlan_ap_list.json
static char wlan_ap_list_file[4096] = { "\
[{\"name\":\"ssid1\",\"value\":\"01:02:03:04:05:06:07:08_49%_1_WPAWPA2\"},\
{\"name\":\"ssid 1 23\",\"value\":\"01:02:03:04:05:06:07:08_60%_2_WPAWPA2\"}\
]\
"};

// /cjson/system_para.json
static char system_para_page_file[4096] = { "\
{\"string\":\
{\"0\":\"0\", /* Reserved */\
\"1\":\"0\",  /* Type     */\
\"2\":\"0\",  /* SN       */\
\"3\":\"0\",  /* Factory Name */\
\"4\":\"0\",  /* Reboot Flag */\
\"5\":\"0\",  /* Soft Ver */\
\"6\":\"0\",  /* Hard Ver */\
\"7\":\"0\",  /* Factory Time */\
\"8\":\"0\",  /* Reserved */\
\"9\":\"0\",  /* Reserved */\
\"10\":\"0\",  /* Reserved */\
\"11\":\"0\",  /* AP Load Number */\
\"12\":\"0\",  /* Reserved */\
\"13\":\"0\",  /* Reserved */\
\"14\":\"0\",  /* Reserved */\
\"15\":\"0\",  /* STA IP Mode */\
\"16\":\"0\",  /* STA IP Addr */\
\"17\":\"0\",  /* STA IP SubMask */\
\"18\":\"0\",  /* STA IP Gateway */\
\"19\":\"0\",  /* STA IP DNS1 */\
\"20\":\"0\",  /* STA Local Port */\
\"21\":\"0\",  /* STA Remote Sever IP */\
\"22\":\"0\",  /* STA IP DNS2 */\
\"23\":\"0\",  /* Reserved */\
\"24\":\"0\",  /* STA Remote Port */\
\"25\":\"0\",  /* TimeZone */\
\"26\":\"0\",  /* LocalTime */\
\"27\":\"0\",  /* Network Load(iperf bitrate) */\
\"28\":\"0\",  /* PD SNK Voltage */\
\"29\":\"0\",  /* PD SNK Current */\
\"30\":\"0\",  /* PD SNK Capabilities */\
\"31\":\"0\",  /* PD SRC Voltage */\
\"32\":\"0\",  /* PD SRC Current */\
\"33\":\"0\",  /* PD SRC Capabilities */\
\"34\":\"0\",  /* Wireless Charge Voltage*/\
\"35\":\"0\",  /* Wireless Charge Temperature */\
\"36\":\"0\",  /* OTA status1 */\
\"37\":\"0\",  /* OTA status2 */\
\"38\":\"0\",  /* OTA file size */\
\"39\":\"0\",  /* Reserved */\
\"40\":\"0\",  /* Reserved */\
\"41\":\"0\",  /* STA Router SSID */\
\"42\":\"0\",  /* STA Router Encryption Mode */\
\"43\":\"0\",  /* STA Router Key */\
\"44\":\"0\",  /* MAC Of STA */\
\"45\":\"0\",  /* RSSI Of STA */\
\"46\":\"0\",  /* AP SSID */\
\"47\":\"0\",  /* AP SSID Passwd */\
\"48\":\"0,0\",/* Net Status */\
\"49\":\"0\",  /* Active STA SSID */\
\"50\":\"0\",  /* AP Scan */\
\"51\":\"1\",  /* FW Release Date */\
\"52\":\"2\",  /* FW Release Time */\
\"53\":\"3\",  /* Web admin */\
\"54\":\"4\",  /* Web admin passwd */\
\"55\":\"5\",  /* RSSI Of AP */\
\"56\":\"6\",  /* MAC Of AP */\
\"57\":\"7\",  /* AP IP Addr */\
\"58\":\"8\",  /* Reserved */\
\"59\":\"9\",  /* Reserved */\
\"60\":\"10\", /* NFC UUID */\
\"61\":\"11\", /* NFC Sector0 Addr0 */\
\"62\":\"12\", /* NFC Sector0 Addr1 */\
\"63\":\"13\", /* NFC Sector0 Addr2 */\
\"64\":\"14\", /* NFC Sector0 Addr3 */\
\"65\":\"15\", /* Reserved */\
\"66\":\"16\", /* Reserved */\
\"67\":\"17\", /* Reserved */\
\"68\":\"18\", /* Reserved */\
\"69\":\"19\", /* Reserved */\
\"70\":\"20\", /* Reserved */\
\"71\":\"21\", /* AP Remote Sever IP */\
\"72\":\"22\", /* Reserved */\
\"73\":\"23\", /* Reserved */\
\"74\":\"24\", /* AP Remote Port */\
\"75\":\"25\", /* Reserved */\
\"76\":\"26\", /* Reserved */\
\"77\":\"27\", /* Reserved */\
\"78\":\"28\", /* Reserved */\
\"79\":\"29\", /* Reserved */\
\"80\":\"30\", /* Reserved */\
\"81\":\"31\", /* Reserved */\
\"82\":\"32\", /* Reserved */\
\"83\":\"33\", /* Reserved */\
\"84\":\"34\", /* Reserved */\
\"85\":\"35\", /* Reserved */\
\"86\":\"36\", /* Reserved */\
\"87\":\"37\", /* Reserved */\
\"88\":\"38\", /* Reserved */\
\"89\":\"39\", /* Reserved */\
\"90\":\"30\", /* Reserved */\
\"91\":\"41\", /* Reserved */\
\"92\":\"42\", /* Reserved */\
\"93\":\"43\", /* Reserved */\
\"94\":\"44\", /* Reserved */\
\"95\":\"45\", /* Reserved */\
\"96\":\"46\", /* Reserved */\
\"97\":\"47\", /* Reserved */\
\"98\":\"48\", /* Reserved */\
\"99\":\"49\"  /* Web Set Status */\
}}\
"};

// static const char cn_page_file[] = {"{\"string\":{\"wifi\":\"NOA SNacker设置\",\"Chinese\":\"中文\"}}"};
static const char cn_page_file[] = {"\
{\"string\":\
{\"wifi\":\"NOA SNacker设置\",\
\"Chinese\":\"中文\",\
\"English\":\"English\",\
\"SystemInfo\":\"系统信息\",\
\"wifiAPSet\":\"无线接入点设置\",\
\"STASettings\":\"无线终端设置\",\
\"moduleManagement\":\"模块管理\",\
\"networkSet\":\"串口及网络端口设置\",\
\"firmware\":\"固件升级\",\
\"SystemInfoIntroduction\":\"查看系统的参数信息及运行状态\",\
\"DevicePN\":\"设备PN\",\
\"softwareVersion\":\"软件版本号\",\
\"hardwareVersion\":\"硬件版本号\",\
\"MAC\":\"MAC地址\",\
\"wirelessStatus\":\"无线连接状态\",\
\"networkStatus\":\"路由信号\",\
\"SSID\":\"路由器SSID\",\
\"IP\":\"IP地址\",\
\"deviceStatus\":\"SNacker AP连接状态\",\
\"deviceOnline\":\"设备上线数\",\
\"Load\":\"网络负载\",\
\"wifiAP\":\"无线接入点设置，包括AP广播的参数（SSID，加密）及接入模式（DHCP，静态连接）等。\",\
\"wifiAPParamSet\":\"无线接入点参数设置\",\
\"modeSSID\":\"终端接入的网络名称（SSID）\",\
\"search\":\"搜索\",\
\"EncryptionMode\":\"加密模式\",\
\"EncryptionAlgorithm\":\"加密算法\",\
\"pwd\":\"密码\",\
\"confirm\":\"确定\",\
\"cancel\":\"取消\",\
\"DHCP\":\"DHCP 模式\",\
\"IPSet\":\"IP地址获取方式\",\
\"CPU\":\"主机名（可选）\",\
\"STASettingsIntroduction\":\"无线终端接口的设置，包括SSID，加密等。\",\
\"NetworkMode\":\"网络模式\",\
\"NetworkName\":\"网络名称（SSID）\",\
\"modeMAC\":\"MAC地址\",\
\"NoChannelSelection\":\"无线信道选择\",\
\"WPA\":\"WPA加密\",\
\"Wi-Fi_AP\":\"AP 密码\",\
\"DeviceMonitor\":\"设备监控\",\
\"DeviceMonitorIntroduction\":\"监控设备PD端口、无线充电、NFC状态，重启设备。\",\
\"Restart\":\"重启设备\",\
\"RestartNow\":\"立即重启\",\
\"ProtocolCode\":\"协议代码\",\
\"LANParameterSet\":\"局域网参数设置\",\
\"DHCPGateway\":\"IP地址（DHCP网关设置）\",\
\"SubnetMask\":\"子网掩码\",\
\"DHCPtype\":\"DHCP类型\",\
\"networkSetIntroduction\":\"设置模块应用程序的串口参数及网络端口参数。\",\
\"SerialPortSet\":\"串口参数设置\",\
\"bote\":\"波特率\",\
\"DataBit\":\"数据位\",\
\"CheckBit\":\"检验位\",\
\"StopBit\":\"停止位\",\
\"CISRTS\":\"硬件流控（CISRTS）\",\
\"UpgradeSet\":\"固件升级设置\",\
\"account\":\"账号\",\
\"password\":\"口令\",\
\"reset\":\"恢复出厂设置\",\
\"firmwareIntroduction\":\"请选择需要升级的文件，点击升级按钮进行升级\",\
\"FirmwareFile\":\"固件文件\",\
\"selFile\":\"选择文件\",\
\"upgrade\":\"升级\",\
\"Gateway\":\"网关设置\",\
\"server\":\"域名服务器\",\
\"wifiSetSTA\":\"无线终端参数设置\",\
\"clickHere\":\"点击此处\",\
\"serverName\":\"服务器\",\
\"stop\":\"停用\",\
\"dynamic\":\"动态（自动获取）\",\
\"static\":\"静态（固定IP）\",\
\"PDInfoSNK\":\"USB-PD快充Sink端口状态\",\
\"PD_SNKVoltage\":\"SNK电压(mv)\",\
\"PD_SNKCurrent\":\"SNK电流(ma)\",\
\"PD_SNKCapabilities\":\"SNK上游协议(mv:ma)\",\
\"PDInfoSRC\":\"USB-PD快充Source端口状态\",\
\"PD_SRCVoltage\":\"SRC电压(mv)\",\
\"PD_SRCCurrent\":\"SRC电流(ma)\",\
\"PD_SRCCapabilities\":\"SRC下游协议(mv:ma)\",\
\"WirelessChargeStatus\":\"无线充电状态\",\
\"WirelessCharge_Voltage\":\"电压(mv)\",\
\"WirelessCharge_Temperature\":\"温度(℃)\",\
\"NFCStatus\":\"NFC状态\",\
\"NFC_UUID\":\"设备UUID\",\
\"NFC_Sector0_Addr0\":\"扇区0地址0数据\",\
\"NFC_Sector0_Addr1\":\"扇区0地址1数据\",\
\"NFC_Sector0_Addr2\":\"扇区0地址2数据\",\
\"NFC_Sector0_Addr3\":\"扇区0地址3数据\",\
\"FWReleaseTime\":\"发布时间\",\
\"APMAC\":\"AP MAC地址\",\
\"APIP\":\"AP IP地址\",\
\"STADNS1\":\"域名解析服务器1\",\
\"STADNS2\":\"域名解析服务器2\",\
\"WorkingTime\":\"设备运行时间\",\
\"sitesurvey\":\"无线网络列表\"\
}}\
"};

static const char en_page_file[] = { "\
{\"string\":\
{\"wifi\":\"NOA SNacker Setting\",\
\"Chinese\":\"Chinese\",\
\"English\":\"English\",\
\"SystemInfo\":\"System Information\",\
\"wifiAPSet\":\"Wi-Fi AP Settings\",\
\"STASettings\":\"Wi-Fi STA Settings\",\
\"moduleManagement\":\"Module management\",\
\"networkSet\":\"Serial and network port settings\",\
\"firmware\":\"Firmware Upgrade\",\
\"SystemInfoIntroduction\":\"View system parameter information and operating status\",\
\"DevicePN\":\"Device PN\",\
\"softwareVersion\":\"Software Version\",\
\"hardwareVersion\":\"Hardware Version\",\
\"MAC\":\"MAC\",\
\"wirelessStatus\":\"Wi-Fi Connection Status\",\
\"networkStatus\":\"Router RSSI\",\
\"SSID\":\"Router SSID\",\
\"IP\":\"IP Address\",\
\"deviceStatus\":\"SNacker AP Connection Status\",\
\"deviceOnline\":\"Number of devices online\",\
\"Load\":\"Network Load\",\
\"wifiAP\":\"Wireless AP Settings, including AP parameters (SSID, encryption) and access mode (DHCP, static connection) to be connected.\",\
\"wifiAPParamSet\":\"Wireless AP Parameter Setting\",\
\"modeSSID\":\"SSID\",\
\"search\":\"search\",\
\"EncryptionMode\":\"Encryption Mode\",\
\"EncryptionAlgorithm\":\"Encryption Algorithm\",\
\"pwd\":\"password\",\
\"confirm\":\"confirm\",\
\"cancel\":\"cancel\",\
\"DHCP\":\"DHCP mode\",\
\"IPSet\":\"How to obtain IP address\",\
\"CPU\":\"Hostname (optional)\",\
\"STASettingsIntroduction\":\"Wireless STA interface settings, including SSID, encryption, etc.\",\
\"NetworkMode\":\"Network Mode\",\
\"NetworkName\":\"Network Name (SSID)\",\
\"modeMAC\":\"MAC Address\",\
\"NoChannelSelection\":\"Wireless Channel Selection\",\
\"WPA\":\"WPA Encryption\",\
\"Wi-Fi_AP\":\"AP Password\",\
\"DeviceMonitor\":\"Device Monitor\",\
\"DeviceMonitorIntroduction\":\"Monitor device USB-PD/Wireless charge/NFC status, restart device.\",\
\"Restart\":\"Restart the device\",\
\"RestartNow\":\"Restart now\",\
\"ProtocolCode\":\"Protocol code\",\
\"LANParameterSet\":\"LAN parameter setting\",\
\"DHCPGateway\":\"IP address (DHCP gateway setting)\",\
\"SubnetMask\":\"Subnet Mask\",\
\"DHCPtype\":\"DHCP type\",\
\"networkSetIntroduction\":\"Set the serial port parameters and network port parameters of the module application.\",\
\"SerialPortSet\":\"Serial port parameter setting\",\
\"bote\":\"bate\",\
\"DataBit\":\"Data bit\",\
\"CheckBit\":\"Check bit\",\
\"StopBit\":\"Stop bit\",\
\"CISRTS\":\"Hardware flow control (CISRTS)\",\
\"UpgradeSet\":\"Firmware Upgrade Settings\",\
\"account\":\"account\",\
\"password\":\"password\",\
\"reset\":\"reset\",\
\"firmwareIntroduction\":\"Please select the file to be upgraded, click the upgrade button to upgrade\",\
\"FirmwareFile\":\"Firmware file\",\
\"selFile\":\"Select file\",\
\"upgrade\":\"update\",\
\"Gateway\":\"Gateway Settings\",\
\"server\":\"Domain name server\",\
\"wifiSetSTA\":\"Wireless STA parameter settings\",\
\"clickHere\":\"click here\",\
\"serverName\":\"Server\",\
\"stop\":\"Deactivate\",\
\"dynamic\":\"Dynamic (obtained automatically)\",\
\"static\":\"Static (fixed IP)\",\
\"PDInfoSNK\":\"USB Power Delivery Sink Port Status\",\
\"PD_SNKVoltage\":\"SNK Voltage(mv)\",\
\"PD_SNKCurrent\":\"SNK Current(ma)\",\
\"PD_SNKCapabilities\":\"SNK Capabilities (mv:ma)\",\
\"PDInfoSRC\":\"USB Power Delivery Source Port Status\",\
\"PD_SRCVoltage\":\"SRC Voltage(mv)\",\
\"PD_SRCCurrent\":\"SRC Current(ma)\",\
\"PD_SRCCapabilities\":\"SRC Capabilities (mv:ma)\",\
\"WirelessChargeStatus\":\"Wireless Charge Status\",\
\"WirelessCharge_Voltage\":\"Voltage(mv)\",\
\"WirelessCharge_Temperature\":\"Temperature(°C)\",\
\"NFCStatus\":\"NFC Status\",\
\"NFC_UUID\":\"Device UUID\",\
\"NFC_Sector0_Addr0\":\"Sector 0 Address 0\",\
\"NFC_Sector0_Addr1\":\"Sector 0 Address 1\",\
\"NFC_Sector0_Addr2\":\"Sector 0 Address 2\",\
\"NFC_Sector0_Addr3\":\"Sector 0 Address 3\",\
\"FWReleaseTime\":\"Release Time\",\
\"APMAC\":\"AP MAC\",\
\"APIP\":\"AP IP Address\",\
\"STADNS1\":\"Domain Name Server1\",\
\"STADNS2\":\"Domain Name Server2\",\
\"WorkingTime\":\"Device Working Time\",\
\"sitesurvey\":\"Wi-Fi List\"\
}}\
"};

static const char ServerIndex[] PROGMEM =
R"(<!DOCTYPE html>
    <head>
      <meta charset='UTF-8'>
      <meta name='viewport' content='width=device-width, initial-scale=1.0'>
      <title>NOA SNacker</title>
      <style>body,div,ul,li,h1,h2,h3,h4,h5,h6,form,input,button,p,th,
        td {margin: 0px;padding: 0px;}
        *:focus {outline: none;webkit-tap-highlight-color: transparent;}
        li {list-style: none;}
        a {text-decoration: none;color: #999999}
        i {font-style: normal;}
        button {background: none;border: none;}
        textarea {resize: none;}
        select {margin: 0;outline-offset: 0;outline-style: none;outline-width: 0; -webkit-font-smoothing: inherit;background-image: none;}
        input,select {border: 1px solid #e4e4e4;background: none;border-radius: 3px;}
        body {font-family: -apple-system-font, Helvetica Neue, Helvetica, sans-serif;color: #404040;font-size: 14px;}
        .clearfix::after {content: '.';clear: both;display: block;height: 0;visibility: hidden;}
        .main {width: 70%;max-width: 1300px;padding-top: 40px;margin: 0 auto;display: flex;min-width: 1100px;}
        .fl {float: left;}
        .fl_bold {float: left;font-weight:bold;}
        .fr {float: right;}
        .title {font-size: 24px;}
        .title_bold {font-size: 24px;font-weight:bold;}
        .Introduction {padding: 8px 0;border-bottom: 1px solid #e4e4e4;}
        .card {width: 100%;border: 1px solid #f2f2f2;margin-top: 30px;box-sizing: border-box;}
        .card .cTop {padding: 8px 5px 8px 20px;background-color: #378BE4;color: #fff;}
        .card .cItem {width: 100%;padding: 8px 0;}
        .card .cItem span {box-sizing: border-box;padding-left: 20px;}
        .itemLeft {width: 33%;border-right: 1px solid #f2f2f2;float: left;}
        .pluralItem {background-color: #f2f2f2;}
        .formBox {padding: 30px 0 25px;text-align: center;}
        .formItem {margin-bottom: 15px;position: relative;display: flex;align-items: center;}
        .formItem .tips {padding: 20px 0;font-size: 24px;color: #378BE4;}
        .formItem .restartBtn {padding-bottom: 20px;font-size: 14px;}
        .formItem .restartBtn a {color: #378BE4;text-decoration: underline;}
        .formItem>label {left: 0;width: 30%;margin-right: 8px;text-align: right;}
        .formItem .search {margin-left: 10px;}
        .formItem>input,
        .formItem>select {width: 35%;height: 32px;padding-left: 8px;box-sizing: border-box;}
        .formItem .buttonBox {width: 35%;height: 32px;margin: 0 auto;text-align: left;}
        .radioBox {width: 35%;height: 32px;line-height: 32px;text-align: left;}
        .radioBox label {margin-right: 5px;vertical-align: middle}
        .radioBox input {vertical-align: middle}
        button {height: 32px;line-height: 32px;padding: 0 20px;background-color: #378BE4;color: #fff;border-radius: 5px;cursor: pointer;}
        button.cancel {background-color: #f2f2f2;color: #000;margin-left: 5px;}
        button:hover {opacity: 0.8;}
        header {width: 100%;height: 60px;background-color: #378BE4;color: #fff;font-size: 24px;min-width: 1300px;}
        header p {width: 70%;max-width: 1300px;min-width: 1100px;height: 60px;margin: 0 auto;line-height: 60px;}
        header span.active {font-weight: 700;}
        .mainList {width: 300px;height: 700px;background-color: #f2f2f2;}
        .mainList li {width: 100%;background-image: linear-gradient(rgb(242, 242, 242) 0%, rgb(243, 243, 243) 0%, rgb(255, 255, 255) 99%, rgb(255, 255, 255) 100%);box-sizing: border-box;padding: 15px 5px 15px 20px;cursor: pointer;}
        .mainList li:hover,.mainList li.active {color: #378BE4;}
        .mainList li span:first-child {color: #c9c9c9;margin-right: 10px;}
        .mainList li:hover span,
        .mainList li.active span {color: #378BE4;}
        .showPage {flex: 1;box-sizing: border-box;padding-left: 100px;}
        #hiddenBtn {position: fixed;top: 0;left: 0;width: 50px;height: 20px;}
        .hiddenItem {display: none;}
        .tipsBox .formItem,.tipsBox1 .formItem,.tipsBox2 .formItem {flex-direction: column;}
        @media screen and (max-width: 1000px) 
            {header {min-width: 300px;font-size: 18px;}
             header p {min-width: 100px;width: 95%;}
             .main {width: 100%;min-width: 100px;margin: 0;padding-top: 0;}
             .mainList {min-width: 100px;width: 25%;position: relative;top: 0;bottom: 0;}
             .mainList li {width: 100%;padding-left: 10px;}
             .mainList li span:first-child {margin-right: 5px;}
             .showPage {padding-left: 10px;}
              #hiddenBtn {left: 50%;margin-left: -25px;}
              .title {font-size: 18px;margin-top: 5px;}
              .Introduction {font-size: 12px;}
              .card {width: 98%;margin-top: 15px;}
              .card .cTop {padding-left: 10px;}
              .card .cItem span {padding-left: 10px;}
              .itemLeft {float: none;display: inline-block;}
              .card .cItem>span:last-child {width: 60%;display: inline-block;vertical-align: top;}
              .formItem {margin-bottom: 20px;padding-right: 10px;}
              .formItem .tips {padding: 15px 0;font-size: 20px;}
              .formItem>label {line-height: normal;}
              .formItem .buttonBox {width: 75%;text-align: center;}
              button {padding: 0 10px;}
             .formItem .search {margin-left: 5px;}
             .formItem>input,
             .formItem>select {flex: 1;}
            }
      </style>
    </head>
    <body>
      <header>
        <div id='hiddenBtn'></div>
        <p class='clearfix'>
          <span lang='wifi' class='fl_bold'></span>
          <span style='font-size: 14px;' class='fr'>
            <span id='cn' lang='Chinese' style='cursor: pointer;' onclick="changeLang('cn');"></span>
            <span>/</span>
            <span id='en' lang='English' style='cursor: pointer;' onclick="changeLang('en');"></span>
          </span>
        </p>
      </header>
      <div class='main' id='clientPage' style='display: flex;'>
        <ul class='mainList'>
          <li data-value='SystemInfo'><span>●</span><span lang='SystemInfo'></span></li>
          <li data-value='wifiAPSet'><span>●</span><span lang='wifiAPSet'></span></li>
          <li data-value='STASettings'><span>●</span><span lang='STASettings'></span></li>
          <li data-value='DeviceMonitor'><span>●</span><span lang='DeviceMonitor'></span></li>
          <li data-value='firmware'><span>●</span><span lang='firmware'></span></li>
        </ul>
        <div class='showPage' id='indexPage'></div>
      </div>
      <div class='main' id='debugPage' style='display: none;'>
        <ul class='mainList'>
          <li data-value='SystemInfo'><span>●</span><span lang='SystemInfo'></span></li>
          <li data-value='wifiAPSet'><span>●</span><span lang='wifiAPSet'></span></li>
          <li data-value='STASettings'><span>●</span><span lang='STASettings'></span></li>
          <li data-value='DeviceMonitor'><span>●</span><span lang='DeviceMonitor'></span></li>
          <li data-value='firmware'><span>●</span><span lang='firmware'></span></li>
        </ul>
        <div class='showPage' id='indexPageDebug'></div>
      </div>
      <script type='text/javascript'>
        checkUserCurLang();
        var shopCilck = false;
        localStorage.setItem('UpgradeStatus', '0');
        var regDetectJs = /<script(.|\n)*?>(.|\n|\r\n)*?<\/script>/ig;
        loadXMLDoc('SystemInfo', false);
        let lang = localStorage.getItem('lang');
        let cn = document.getElementById('cn');
        let en = document.getElementById('en');
        if (lang == 'cn') {
          cn.className = 'active';
        } else {
          en.className = 'active';
        };
        let liArr = document.querySelectorAll('.mainList>li');
        document.getElementById('hiddenBtn').onclick = () => {
          if (shopCilck) {
            if (lang == 'cn') {
              alert('请在页面加载完成后点击菜单切换')
            } else {
              alert('Please click the menu switch after the page has loaded')
            };
            return;
          }
          let UpgradeStatus = localStorage.getItem('UpgradeStatus');
          if (UpgradeStatus == 1) {
            var test_value;
            if (lang == 'cn') {
              test_value = confirm('要切换页面需要取消升级，请问是否需要取消升级?');
            } else {
              test_value = confirm('You need to cancel the upgrade if you want to switch pages. Do you need to cancel the upgrade?');
            };
            if (test_value) {
              exitUpgrade();
            } else {
              return;
            };
          };
          let debugPage = document.getElementById('debugPage');
          let clientPage = document.getElementById('clientPage');
          if (debugPage.style.display == 'none') {
            debugPage.style.display = 'flex';
            clientPage.style.display = 'none';
            liArr.forEach((element, index) => {
              if (index == 4) {
                element.className = 'active';
                loadXMLDoc(element.getAttribute('data-value'), true);
              } else {
                element.className = '';
              };
              element.onclick = function () {
                if (shopCilck) {
                  if (lang == 'cn') {
                    alert('请在页面加载完成后点击菜单切换')
                  } else {
                    alert('Please click the menu switch after the page has loaded')
                  };
                  return;
                };
                UpgradeStatus = localStorage.getItem('UpgradeStatus');
                if (UpgradeStatus == 1) {
                  if (lang == 'cn') {
                    test_value = confirm('要切换页面需要取消升级，请问是否需要取消升级?');
                  } else {
                    test_value = confirm('You need to cancel the upgrade if you want to switch pages. Do you need to cancel the upgrade?');
                  };
                  if (test_value) {
                    exitUpgrade();
                  } else {
                    return;
                  };
                };
                loadXMLDoc(element.getAttribute('data-value'), true);
                for (let i = 0; i < liArr.length; i++) {
                  liArr[i].className = '';
                };
                element.className = 'active';
              };
            });
          } else {
            clientPage.style.display = 'flex';
            debugPage.style.display = 'none';
            liArr.forEach((element, index) => {
              if (index == 0) {
                element.className = 'active';
                loadXMLDoc(element.getAttribute('data-value'), false);
              } else {
                element.className = '';
              };
              element.onclick = function () {
                if (shopCilck) {
                  if (lang == 'cn') {
                    alert('请在页面加载完成后点击菜单切换')
                  } else {
                    alert('Please click the menu switch after the page has loaded')
                  };
                  return;
                };
                loadXMLDoc(element.getAttribute('data-value'), false);
                for (let i = 0; i < liArr.length; i++) {
                  liArr[i].className = '';
                };
                element.className = 'active';
              };
            });
          };
        };
        liArr.forEach((element, index) => {
          if (index == 0) {
            element.className = 'active';
          };
          element.onclick = function () {
            if (shopCilck) {
              if (lang == 'cn') {
                alert('请在页面加载完成后点击菜单切换')
              } else {
                alert('Please click the menu switch after the page has loaded')
              };
              return;
            };
            loadXMLDoc(element.getAttribute('data-value'), false);
            for (let i = 0; i < liArr.length; i++) {
              liArr[i].className = '';
            };
            element.className = 'active';
          };
        });
        function exitUpgrade() {
          localStorage.setItem('UpgradeStatus', '2');
          var xmlfile;
          if (window.XMLHttpRequest) {
            xmlfile = new XMLHttpRequest();
          } else {
            xmlfile = new ActiveXObject('Microsoft.XMLHTTP');
          };
          xmlfile.timeout = 3000;
          xmlfile.onreadystatechange = function () {
            if (xmlfile.readyState == 4 && xmlfile.status == 200) {
              if (xmlfile.responseText == '0') {
                localStorage.setItem('UpgradeStatus', '0');
                console.log('退出成功');
              } else {
                localStorage.setItem('UpgradeStatus', '0');
                console.log('退出失败');
              }
            };
          };
          xmlfile.ontimeout = function () {
            exitUpgrade();
          };
          xmlfile.open('POST', '/cjson/system_para.json', true);
          xmlfile.send(`{'string':{'36':'0'}}`);
        };
        var intSys;
        var intDev;
        function StopintSys(){
          if(document.getElementById('DevicePN') == null) {
            clearInterval(intSys);
          }
        };
        function StopintDev(){
          if(document.getElementById('pdsnkvoltage') == null) {
            clearInterval(intDev);
          }
        };
        function changeLang(lang) {
          localStorage.setItem('lang', lang);
          location.reload();
        };
        function checkUserCurLang() {
          StopintDev();
          StopintSys();
          let lang = localStorage.getItem('lang');
          if (!lang) {
            localStorage.setItem('lang', 'cn');
            changeLanguage('cn');
          } else {
            changeLanguage(lang);
          };
        };
        function changeLanguage(lang) {
          var xhr = new XMLHttpRequest();
          xhr.timeout = 3000;
          xhr.overrideMimeType('application/json');
          xhr.open('get', './lang/' + lang + '.json', true);
          xhr.onload = function () {
            let languageList = JSON.parse(xhr.response);
            for (const key in languageList.string) {
              if (document.querySelectorAll('[lang=' + key + ']').length > 0) {
                for (let i = 0; i < document.querySelectorAll('[lang=' + key + ']').length; i++) {
                  document.querySelectorAll('[lang=' + key + ']')[i].innerText = languageList.string[key];
                };
              };
            };
          };
          xhr.ontimeout = function () {
            changeLanguage(lang);
          };
          xhr.send();
        };
        function loadXMLDoc(htmlName, isDebug) {
          shopCilck = true;
          var xmlhttp;
          if (window.XMLHttpRequest) {
            xmlhttp = new XMLHttpRequest();
          } else {
            xmlhttp = new ActiveXObject('Microsoft.XMLHTTP');
          };
          xmlhttp.timeout = 3000;
          xmlhttp.onreadystatechange = function () {
            if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
              if (isDebug == false) {
                document.getElementById('indexPage').innerHTML = xmlhttp.responseText;
              } else {
                document.getElementById('indexPageDebug').innerHTML = xmlhttp.responseText;
              }
              var jsContained = xmlhttp.responseText.match(regDetectJs);
              if (jsContained) {
                var regGetJS = /<script(.|\n)*?>((.|\n|\r\n)*)?<\/script>/im;
                var jsNums = jsContained.length;
                for (var i = 0; i < jsNums; i++) {
                  var jsSection = jsContained[i].match(regGetJS);
                  if (jsSection[2]) {
                    if (window.execScript) {
                      window.execScript(jsSection[2]);
                    } else {
                      window.eval(jsSection[2]);
                    };
                  };
                };
              };
              shopCilck = false;
            };
          };
          xmlhttp.ontimeout = function () {
            loadXMLDoc(htmlName, isDebug);
          };
          xmlhttp.open('GET', `${htmlName}.html`, true);
          xmlhttp.send();
        };
        function regIP(IP) {
          return /^((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)$/.test(IP)
        };
      </script>
    </body>
    </html>)";

static const char SystemInfo_page_file[] PROGMEM =
R"(<!DOCTYPE html>
    <head>
        <meta charset='UTF-8'>
    </head>
    <body>
        <p class='title' lang='SystemInfo'></p>
        <p class='Introduction' lang='SystemInfoIntroduction'></p>
        <div class='card'>
            <div class='cTop' lang='SystemInfo'></div>
            <div class='cItem'>
                <span class='itemLeft' lang='DevicePN'></span>
                <span id='DevicePN'></span>
            </div>
            <div class='cItem pluralItem'>
                <span class='itemLeft' lang='softwareVersion'></span>
                <span id='softwareVersion'></span>
            </div>
            <div class='cItem'>
                <span class='itemLeft' lang='hardwareVersion'></span>
                <span id='hardwareVersion'></span>
            </div>
            <div class='cItem pluralItem'>
                <span class='itemLeft' lang='FWReleaseTime'></span>
                <span id='fwreleasetime'></span>
            </div>
            <div class='cItem'>
                <span class='itemLeft' lang='WorkingTime'></span>
                <span id='workingtime'></span>
            </div>
        </div>
        <div class='card'>
            <div class='cTop' lang='wirelessStatus'></div>
            <div class='cItem'>
                <span class='itemLeft' lang='networkStatus'></span>
                <span id='networkStatus'></span>
            </div>
            <div class='cItem pluralItem'>
                <span class='itemLeft' lang='SSID'></span>
                <span id='SSID'></span>
            </div>
            <div class='cItem'>
                <span class='itemLeft' lang='IP'></span>
                <span id='ip'></span>
            </div>
            <div class='cItem pluralItem'>
                <span class='itemLeft' lang='SubnetMask'></span>
                <span id='subnetmask'></span>
            </div>
            <div class='cItem'>
                <span class='itemLeft' lang='Gateway'></span>
                <span id='gateway'></span>
            </div>
            <div class='cItem pluralItem'>
                <span class='itemLeft' lang='STADNS1'></span>
                <span id='stadns1'></span>
            </div>
            <div class='cItem'>
                <span class='itemLeft' lang='STADNS2'></span>
                <span id='stadns2'></span>
            </div>
            <div class='cItem pluralItem'>
                <span class='itemLeft' lang='MAC'></span>
                <span id='mac'></span>
            </div>
        </div>
        <div class='card'>
            <div class='cTop' lang='deviceStatus'></div>
            <div class='cItem'>
                <span class='itemLeft' lang='APIP'></span>
                <span id='apip'></span>
            </div>
            <div class='cItem pluralItem'>
                <span class='itemLeft' lang='deviceOnline'></span>
                <span id='deviceOnline'></span>
            </div>
            <div class='cItem'>
                <span class='itemLeft' lang='Load'></span>
                <span id='Load'></span>
            </div>
        </div>
    <script>
        checkUserCurLang();
        getSystemInfo();
        function getSystemInfo(){
            var xmlfile;
            if (window.XMLHttpRequest) {
                xmlfile = new XMLHttpRequest();
            } else {
                xmlfile = new ActiveXObject('Microsoft.XMLHTTP');
            };
            xmlfile.open('GET', '/cjson/system_para.json', true);
            xmlfile.send();
            xmlfile.timeout = 3000;
            xmlfile.onreadystatechange = function () {
                if (xmlfile.readyState == 4 && xmlfile.status == 200) {
                    let paramsList = JSON.parse(xmlfile.response);
                    if (document.getElementById('DevicePN') == null) {
                    } else {
                      document.getElementById('DevicePN').innerHTML = paramsList.string['2'];
                      document.getElementById('softwareVersion').innerHTML = paramsList.string['5'];
                      document.getElementById('hardwareVersion').innerHTML = paramsList.string['6'];
                      document.getElementById('mac').innerHTML = paramsList.string['44'];
                      document.getElementById('networkStatus').innerHTML = paramsList.string['48'];
                      document.getElementById('SSID').innerHTML = paramsList.string['49'];
                      document.getElementById('ip').innerHTML = paramsList.string['16'];
                      document.getElementById('deviceOnline').innerHTML = paramsList.string['11'];
                      document.getElementById('Load').innerHTML = paramsList.string['27'];
                      document.getElementById('fwreleasetime').innerHTML = paramsList.string['51'] + paramsList.string['52'];
                      document.getElementById('apip').innerHTML = paramsList.string['57'];
                      document.getElementById('subnetmask').innerHTML = paramsList.string['17'];
                      document.getElementById('gateway').innerHTML = paramsList.string['18'];
                      document.getElementById('stadns1').innerHTML = paramsList.string['19'];
                      document.getElementById('stadns2').innerHTML = paramsList.string['22'];
                      document.getElementById('workingtime').innerHTML = paramsList.string['26'];
                    };
                };
            };
            xmlfile.ontimeout = function () {
                getSystemInfo();
            };
        };
        function ReflashSys(){
          getSystemInfo();
        };
        clearInterval(intSys);
        intSys = setInterval(ReflashSys, 1000);
    </script>
    </body>
    </html>)";

static const char wifiAPSet_page_file[] PROGMEM =
R"(<!DOCTYPE html>
    <head>
        <meta charset='UTF-8'>
    </head>
    <body>
        <p class='title' lang='wifiAPSet'></p>
        <p class='Introduction' lang='wifiAP'></p>
        <div class='card'>
            <div class='cTop' lang='wifiAPParamSet'></div>
            <div class='formBox mainBox1'>
                <div class='formItem'>
                    <label for='mode' lang='NetworkMode'></label>
                    <select id='mode'>
                        <option value='1'>11b/g/n mixed mode</option>
                    </select>
                </div>
                <div class='formItem'>
                    <label for='SSID' lang='NetworkName'></label>
                    <input type='text' style='background-color: #f2f2f2;' disabled id='SSID' />
                </div>
                <div class='formItem'>
                    <label for='MAC' lang='modeMAC'></label>
                    <input type='text' style='background-color: #f2f2f2;' disabled id='MAC' />
                </div>
                <div class='formItem'>
                    <label for='channel' lang='NoChannelSelection'></label>
                    <select id='channel'>
                        <option value='0'>auto</option>
                        <option value='1'>2412MHz（channel 1）</option>
                        <option value='2'>2417MHz（channel 2）</option>
                        <option value='3'>2422MHz（channel 3）</option>
                        <option value='4'>2427MHz（channel 4）</option>
                        <option value='5'>2432MHz（channel 5）</option>
                        <option value='6'>2437MHz（channel 6）</option>
                        <option value='7'>2442MHz（channel 7）</option>
                        <option value='8'>2447MHz（channel 8）</option>
                        <option value='9'>2452MHz（channel 9）</option>
                        <option value='10'>2457MHz（channel 10）</option>
                        <option value='11'>2462MHz（channel 11）</option>
                    </select>
                </div>
                <div class='formItem'>
                    <div class='buttonBox'>
                        <button lang='confirm' onclick='setWifi()'></button>
                        <button class='cancel' lang='cancel'></button>
                    </div>
                </div>
            </div>
            <div class='formBox tipsBox1' style='display: none;'>
                <div class='formItem'>
                    <p class='tips tips1'></p>
                    <p class='restartBtn'>
                        <span lang='clickHere'></span>
                        <a lang='RestartNow' href='javascript:void(0);' onclick='restartNow()'></a>
                    </p>
                    <button onclick='backMain()' lang='confirm'></button>
                </div>
            </div>
        </div>
        <div class='card'>
            <div class='cTop' lang='WPA'></div>
            <div class='formBox'>
                <div class='formItem'>
                    <label for='modeID' lang='EncryptionAlgorithm'></label>
                    <div class='radioBox'>
                        <input type='radio' name='algorithm' checked id='AES' />
                        <label for='AES'>AES</label>
                    </div>
                </div>
                <div class='formItem'>
                    <label for='password' lang='Wi-Fi_AP'></label>
                    <input type='text' style='background-color: #f2f2f2;' disabled id='password' />
                </div>
            </div>
        </div>
        <script>
            checkUserCurLang();
            getSystemAPInfo();
            function getSystemAPInfo() {
                var xmlfile;
                if (window.XMLHttpRequest) {
                    xmlfile = new XMLHttpRequest();
                } else {
                    xmlfile = new ActiveXObject('Microsoft.XMLHTTP');
                };
                xmlfile.timeout = 3000;
                xmlfile.onreadystatechange = function () {
                    if (xmlfile.readyState == 4 && xmlfile.status == 200) {
                        let paramsList = JSON.parse(xmlfile.response);
                        document.getElementById('SSID').value = paramsList.string['46'];
                        document.getElementById('MAC').value = paramsList.string['56'];
                        document.getElementById('password').value = paramsList.string['47'];
                    };
                };
                xmlfile.ontimeout = function () {
                    getSystemAPInfo();
                };
                xmlfile.open('GET', '/cjson/system_para.json', true);
                xmlfile.send();
            };
            function setWifi() {
                var channel = document.getElementById('channel');
                var index = channel.selectedIndex;
                var xmlfile;
                if (window.XMLHttpRequest) {
                    xmlfile = new XMLHttpRequest();
                } else {
                    xmlfile = new ActiveXObject('Microsoft.XMLHTTP');
                };
                xmlfile.timeout = 3000;
                xmlfile.onreadystatechange = function () {
                    if (xmlfile.readyState == 4 && xmlfile.status == 200) {
                        if (xmlfile.responseText == '0') {
                            document.getElementsByClassName('restartBtn')[0].style.display = '';
                            if (lang == 'cn')
                                document.getElementsByClassName('tips1')[0].innerHTML = '设置成功，重启后使用新设置。';
                            else
                                document.getElementsByClassName('tips1')[0].innerHTML = 'The setting is successful, and the new setting will be used after restart.';
                        } else {
                            document.getElementsByClassName('restartBtn')[0].style.display = 'none';
                            if (lang == 'cn')
                                document.getElementsByClassName('tips1')[0].innerHTML = '设置失败,请重试';
                            else
                                document.getElementsByClassName('tips1')[0].innerHTML = 'Setting failed, please try again';
                        }
                        document.getElementsByClassName('tipsBox1')[0].style.display = '';
                        document.getElementsByClassName('mainBox1')[0].style.display = 'none';
                    };
                };
                xmlfile.ontimeout = function () {
                    setWifi();
                };
                xmlfile.open('POST', '/cjson/system_para.json', true);
                xmlfile.send(`{\"string\":{\"14\":\"${channel.options[index].value}\"}}`);
            };
            function backMain() {
                document.getElementsByClassName('tipsBox1')[0].style.display = 'none';
                document.getElementsByClassName('mainBox1')[0].style.display = '';
            };
            function restartNow() {
                var xmlfile;
                if (window.XMLHttpRequest) {
                    xmlfile = new XMLHttpRequest();
                } else {
                    xmlfile = new ActiveXObject('Microsoft.XMLHTTP');
                };
                xmlfile.timeout = 3000;
                xmlfile.onreadystatechange = function () {
                    if (xmlfile.readyState == 4 && xmlfile.status == 200) {
                        if (xmlfile.responseText == '0') {
                            if (lang == 'cn')
                                alert('重启成功');
                            else
                                alert('Successful restart');
                        } else {
                            if (lang == 'cn')
                                alert('重启失败');
                            else
                                alert('Restart error');
                        };
                        document.getElementsByClassName('tipsBox1')[0].style.display = 'none';
                        document.getElementsByClassName('mainBox1')[0].style.display = '';
                    };
                };
                xmlfile.ontimeout = function () {
                    restartNow();
                };
                xmlfile.open('POST', '/cjson/system_para.json', true);
                xmlfile.send(`{\"string\":{\"4\":\"1\"}}`);
            };
        </script>
    </body>
</html>)";

static const char STASettings_page_file[] PROGMEM =
R"(<!DOCTYPE html>
    <head>
        <meta charset='UTF-8'>
    </head>
    <head>
        <style>
            .searchTitle {font-size: 24px;}
            table {border: none;}
            table tr {height: 30px;}
            #tableSearch tr:nth-child(odd) {background-color: #f2f2f2;}
            #tableSearch tr td {text-align: center;padding: 0 10px;}
            button {padding: 0 10px;height: 30px;border-radius: 5px;border: none;}
            #searchTips{color: #f00;}
        </style>
    </head>
    <body>
        <div id='set'>
            <p class='title' lang='STASettings'></p>
            <p class='Introduction' lang='STASettingsIntroduction'></p>
            <div class='card'>
                <div class='cTop' lang='wifiSetSTA'></div>
                <div class='formBox mainBox1'>
                    <div class='formItem'>
                        <label for='SSID' lang='modeSSID'></label>
                        <input type='text' id='SSID' />
                        <button class='search' lang='search' onclick='go2search()'></button>
                    </div>
                    <div class='formItem'>
                        <label for='mode' lang='EncryptionMode'></label>
                        <select id='mode' onchange='changMode(value)'>
                            <option value='WPA/WPA2'>WPA/WPA2</option>
                            <option value='WPA'>WPA</option>
                            <option value='WPA2'>WPA2</option>
                            <option value='WEP'>WEP</option>
                            <option value='NONE'>NONE</option>
                        </select>
                    </div>
                    <div class='formItem' id='pwdIten'>
                        <label for='password' lang='pwd'></label>
                        <input type='password' id='password' />
                    </div>
                    <div class='formItem'>
                        <div class='buttonBox'>
                            <button lang='confirm' onclick='setPoint()'></button>
                            <button class='cancel' lang='cancel'></button>
                        </div>
                    </div>
                </div>
                <div class='formBox tipsBox1' style='display: none;'>
                    <div class='formItem'>
                        <p class='tips tips1'></p>
                        <p class='restartBtn'>
                            <span lang='clickHere'></span>
                            <a lang='RestartNow' href='javascript:void(0);' onclick='restartNow()'></a>
                        </p>
                        <button onclick='backMain1()' lang='confirm'></button>
                    </div>
                </div>
            </div>
            <div class='card'>
                <div class='cTop' lang='DHCP'></div>
                <div class='formBox mainBox2'>
                    <div class='formItem'>
                        <label for='modeID' lang='IPSet'></label>
                        <select id='modeID' onchange='changModeId(value)'>
                            <option value='0' lang='dynamic'>动态（自动获取）</option>
                            <option value='1' lang='static'>静态（固定IP）</option>
                        </select>
                    </div>
                    <div class='formItem hiddenItem' id='IPIten'>
                        <label for='IP' lang='IP'></label>
                        <input type='text' value='0.0.0.0' id='IP' />
                    </div>
                    <div class='formItem hiddenItem' id='SubnetMaskIten'>
                        <label for='SubnetMask' lang='SubnetMask'></label>
                        <input type='text' value='0.0.0.0' id='SubnetMask' />
                    </div>
                    <div class='formItem hiddenItem' id='GatewayIten'>
                        <label for='Gateway' lang='Gateway'></label>
                        <input type='text' value='0.0.0.0' id='Gateway' />
                    </div>
                    <div class='formItem hiddenItem' id='serverIten'>
                        <label for='server' lang='server'></label>
                        <input type='text' id='server' />
                    </div>
                    <div class='formItem'>
                        <div class='buttonBox'>
                            <button lang='confirm' onclick='setDHCP()'></button>
                            <button class='cancel' lang='cancel'></button>
                        </div>
                    </div>
                </div>
                <div class='formBox tipsBox2' style='display: none;'>
                    <div class='formItem'>
                        <p class='tips tips2'></p>
                        <p class='restartBtn'>
                            <span lang='clickHere'></span>
                            <a lang='RestartNow' href='javascript:void(0);' onclick='restartNow2()'></a>
                        </p>
                        <button onclick='backMain2()' lang='confirm'></button>
                    </div>
                </div>
            </div>
        </div>
        <div id='search'>
            <p class='searchTitle' lang='sitesurvey'></p>
            <table class='body'>
                <tr>
                    <td>
                        <form method=post name='sta_site_survey'>
                            <table id='tableSearch' width='540' cellpadding='0' cellspacing='0'>
                                <tr style='color:#fff;background-color: #378BE4;'>
                                    <td id='scanSelect'>&nbsp;</td>
                                    <td id='scanSSID'>SSID</td>
                                    <td id='scanBSSID'>BSSID</td>
                                    <td id='scanRSSI'>RSSI</td>
                                    <td id='scanChannel'>Channel</td>
                                    <td id='scanAuth'>Authentication</td>
                                </tr>
                                <tbody id='tableSearchBody'></tbody>
                            </table>
                            <table style='margin-top: 10px;' width='540'>
                                <tr>
                                    <td>
                                        <button style='background-color: #378BE4;color:#fff;' id='button_apply' name='button_apply'>Apply</button>&nbsp;
                                        <button id='button_refresh'>Refresh</button>
                                    </td>
                                </tr>
                            </table>
                            <span id='searchTips'></span>
                        </form>
                    </td>
                </tr>
            </table>
        </div>
        <script>
            checkUserCurLang();
            getSystemSTAInfo();
            function getSystemSTAInfo() {
                var xmlfile;
                if (window.XMLHttpRequest) {
                    xmlfile = new XMLHttpRequest();
                } else {
                    xmlfile = new ActiveXObject('Microsoft.XMLHTTP');
                };
                xmlfile.timeout = 3000;
                xmlfile.onreadystatechange = function () {
                    if (xmlfile.readyState == 4 && xmlfile.status == 200) {
                        let paramsList = JSON.parse(xmlfile.response);
                        document.getElementById('SSID').value = paramsList.string['41'];
                        document.getElementById('mode').value = paramsList.string['42'];
                        document.getElementById('password').value = paramsList.string['43'];
                    };
                };
                xmlfile.ontimeout = function () {
                    getSystemSTAInfo();
                };
                xmlfile.open('GET', '/cjson/system_para.json', true);
                xmlfile.send();
            };
            document.getElementById('search').style.display = 'none';
            var gg_ssid = '';
            var gg_mode = '';
            var aa = document.getElementById('mode');
            function changModeId(value) {
                if (value == 1) {
                    document.getElementById('IPIten').className = 'formItem'
                    document.getElementById('SubnetMaskIten').className = 'formItem'
                    document.getElementById('GatewayIten').className = 'formItem'
                    document.getElementById('serverIten').className = 'formItem'
                } else {
                    document.getElementById('IPIten').className = 'formItem hiddenItem'
                    document.getElementById('SubnetMaskIten').className = 'formItem hiddenItem'
                    document.getElementById('GatewayIten').className = 'formItem hiddenItem'
                    document.getElementById('serverIten').className = 'formItem hiddenItem'
                };
            };
            function changMode(value) {
                if (value == 'NONE') {
                    document.getElementById('pwdIten').className = 'formItem hiddenItem'
                } else {
                    document.getElementById('pwdIten').className = 'formItem'
                };
            };
            function go2search() {
                document.getElementById('set').style.display = 'none';
                document.getElementById('search').style.display = '';
                document.getElementById('searchTips').innerHTML = '';
                getWifiList();
            };
            document.getElementById('button_refresh').onclick = ((e) => {
                e.preventDefault();
                getWifiList();
            });
            document.getElementById('button_apply').onclick = ((e) => {
                e.preventDefault();
                document.getElementById('set').style.display = '';
                document.getElementById('search').style.display = 'none';
                document.getElementById('SSID').value = gg_ssid;
                var mode = document.getElementById('mode')
                if(gg_mode=='NONE'){
                    mode[2].selected=true;
                    document.getElementById('pwdIten').className = 'formItem hiddenItem'
                } else if(gg_mode=='WEP') {
                    mode[1].selected=true;
                    document.getElementById('pwdIten').className = 'formItem'
                } else {
                    mode[0].selected=true;
                    document.getElementById('pwdIten').className = 'formItem'
                };
            });
            function getWifiList() {
                var xmlfile;
                var tips = document.getElementById('searchTips');
                tips.innerHTML = ''
                if (window.XMLHttpRequest) {
                    xmlfile = new XMLHttpRequest();
                } else {
                    xmlfile = new ActiveXObject('Microsoft.XMLHTTP');
                };
                xmlfile.timeout = 10000;
                xmlfile.onreadystatechange = function () {
                    if (xmlfile.readyState == 4 && xmlfile.status == 200) {
                        document.querySelector('#tableSearchBody').innerHTML = '';
                        let paramsList = JSON.parse(xmlfile.response);
                        for (const p in paramsList) {
                            var key = paramsList[p].name;
                            var str = paramsList[p].value.split('_');
                            var tr = document.createElement('tr');
                            tr.innerHTML = '<td><input type=radio name=selectedSSID  value=' + key + ' onclick=selectedSSIDChange("' + escape(key) + '","' + str[3] + '")></td> <td>' + key + '</td> <td>' + str[0] + '</td> <td>' + str[1] + '</td> <td>' + str[2] + '</td> <td>' + str[3] + '</td>';
                            document.querySelector('#tableSearchBody').appendChild(tr);
                        };
                    };
                };
                xmlfile.ontimeout = function () {
                    if (lang == 'cn')
                        tips.innerHTML = '超时,请刷新';
                    else
                        tips.innerHTML = 'Timeout, please refresh';
                };
                xmlfile.open('GET', '/cjson/wlan_ap_list.json', true);
                xmlfile.send();
            };
            function selectedSSIDChange(ssid,mode) {
                gg_ssid = unescape(ssid);
                gg_mode = mode;
            };
            function setPoint() {
                var mode = document.getElementById('mode');
                var index = mode.selectedIndex;
                var SSID = document.getElementById('SSID').value;
                var password = document.getElementById('password').value;
                var data;
                if(mode.options[index].value == 'NONE'){
                    if (!SSID) {
                        if (lang == 'cn') alert('SSID不能为空');
                        else alert('SSID cannot be empty');
                        return;
                    }
                    data = `{\"string\":{\"41\":\"${SSID}\",\"42\":\"${mode.options[index].value}\"}}`
                } else {
                    if (!SSID || !password) {
                        if (lang == 'cn') alert('SSID和密码不能为空');
                        else alert('SSID and password cannot be empty');
                        return;
                    };
                    data = `{\"string\":{\"41\":\"${SSID}\",\"43\":\"${password}\",\"42\":\"${mode.options[index].value}\"}}`
                };
                var xmlfile;
                if (window.XMLHttpRequest) {
                    xmlfile = new XMLHttpRequest();
                } else {
                    xmlfile = new ActiveXObject('Microsoft.XMLHTTP');
                };
                xmlfile.timeout = 3000;
                xmlfile.onreadystatechange = function () {
                    if (xmlfile.readyState == 4 && xmlfile.status == 200) {
                        var tips1 = document.getElementsByClassName('tips1')[0];
                        let paramsList = JSON.parse(xmlfile.response);
                        if (paramsList.string['99'] == 1) {
                            document.getElementsByClassName('restartBtn')[0].style.display = 'none';
                            if (lang == 'cn') tips1.innerHTML = '设置成功，5秒后使用新设置。';
                            else tips1.innerHTML ='The setting is successful, and the new setting will be used after 5 Sec.';
                        } else {
                            document.getElementsByClassName('restartBtn')[0].style.display = 'none';
                            if (lang == 'cn') tips1.innerHTML = '设置失败,请重试';
                            else tips1.innerHTML ='Setting failed, please try again';
                        };
                        document.getElementsByClassName('tipsBox1')[0].style.display = '';
                        document.getElementsByClassName('mainBox1')[0].style.display = 'none';
                    };
                };
                xmlfile.ontimeout = function () {
                    setPoint();
                };
                xmlfile.open('POST', '/cjson/system_para.json', true);
                xmlfile.send(data);
            };
            function setDHCP() {
                var modeID = document.getElementById('modeID');
                var index = modeID.selectedIndex;
                var IP = document.getElementById('IP').value;
                var SubnetMask = document.getElementById('SubnetMask').value;
                var Gateway = document.getElementById('Gateway').value;
                var server = document.getElementById('server').value;
                var data;
                if (modeID.options[index].value == '0') {
                    data = `{\"string\":{\"20\":\"0\"}}`
                } else {
                    if (!IP || !SubnetMask || !Gateway) {
                        if (lang == 'cn') {
                            alert('IP地址、子网掩码、网关设置不能为空');
                            return;
                        } else {
                            alert('IP address, subnet mask, and gateway settings cannot be empty');
                            return;
                        };
                    };
                    if (!regIP(IP)) {
                        if (lang == 'cn') {
                            alert('IP地址不合法');
                            return;
                        } else {
                            alert('IP address is invalid');
                            return;
                        };
                    } else if (!regIP(SubnetMask)) {
                        if (lang == 'cn') {
                            alert('子网掩码不合法');
                            return;
                        } else {
                            alert('Invalid subnet mask');
                            return;
                        };
                    } else if (!regIP(Gateway)) {
                        if (lang == 'cn') {
                            alert('网关设置地址不合法');
                            return;
                        } else {
                            alert('Gateway is invalid');
                            return;
                        };
                    };
                    if (!server) {
                        data = `{\"string\":{\"15\":\"1\",\"16\":\"${IP}\",\"17\":\"${SubnetMask}\",\"18\":\"${Gateway}\"}}`
                    } else {
                        data = `{\"string\":{\"15\":\"1\",\"16\":\"${IP}\",\"17\":\"${SubnetMask}\",\"18\":\"${Gateway}\",\"19\":\"${server}\"}}`
                    };
                };
                var xmlfile;
                if (window.XMLHttpRequest) {
                    xmlfile = new XMLHttpRequest();
                } else {
                    xmlfile = new ActiveXObject('Microsoft.XMLHTTP');
                };
                xmlfile.timeout = 3000;
                xmlfile.onreadystatechange = function () {
                    if (xmlfile.readyState == 4 && xmlfile.status == 200) {
                        var tips2 = document.getElementsByClassName('tips2')[0];
                        if (xmlfile.responseText == '0') {
                            document.getElementsByClassName('restartBtn')[1].style.display = '';
                            if (lang == 'cn') tips2.innerHTML = '设置成功，重启后使用新设置。';
                            else tips2.innerHTML = 'The setting is successful, and the new setting will be used after restart.';
                        } else {
                            document.getElementsByClassName('restartBtn')[1].style.display = 'none';
                            if (lang == 'cn') tips2.innerHTML = '设置失败,请重试';
                            else tips2.innerHTML = 'Setting failed, please try again';
                        }
                        document.getElementsByClassName('tipsBox2')[0].style.display = '';
                        document.getElementsByClassName('mainBox2')[0].style.display = 'none';
                    };
                };
                xmlfile.ontimeout = function () {
                    setDHCP();
                };
                xmlfile.open('POST', '/cjson/system_para.json', true);
                xmlfile.send(data);
            };
            function backMain1() {
                document.getElementsByClassName('tipsBox1')[0].style.display = 'none';
                document.getElementsByClassName('mainBox1')[0].style.display = '';
            };
            function restartNow() {
                var xmlfile;
                if (window.XMLHttpRequest) {
                    xmlfile = new XMLHttpRequest();
                } else {
                    xmlfile = new ActiveXObject('Microsoft.XMLHTTP');
                };
                xmlfile.timeout = 3000;
                xmlfile.onreadystatechange = function () {
                    if (xmlfile.readyState == 4 && xmlfile.status == 200) {
                        if (xmlfile.responseText == '0') {
                            if (lang == 'cn') alert('重启成功');
                            else alert('Successful restart');
                        } else {
                            if (lang == 'cn') alert('重启失败');
                            else alert('Restart error');
                        };
                        document.getElementsByClassName('tipsBox1')[0].style.display = 'none';
                        document.getElementsByClassName('mainBox1')[0].style.display = '';
                    };
                };
                xmlfile.ontimeout = function () {
                    restartNow();
                };
                xmlfile.open('POST', '/cjson/system_para.json', true);
                xmlfile.send(`{\"string\":{\"4\":\"1\"}}`);
            };
            function backMain2() {
                document.getElementsByClassName('tipsBox2')[0].style.display = 'none';
                document.getElementsByClassName('mainBox2')[0].style.display = '';
            };
            function restartNow2() {
                var xmlfile;
                if (window.XMLHttpRequest) {
                    xmlfile = new XMLHttpRequest();
                } else {
                    xmlfile = new ActiveXObject('Microsoft.XMLHTTP');
                };
                xmlfile.timeout = 3000;
                xmlfile.onreadystatechange = function () {
                    if (xmlfile.readyState == 4 && xmlfile.status == 200) {
                        if (xmlfile.responseText == '0') {
                            if (lang == 'cn') alert('重启成功');
                            else alert('Successful restart');
                        } else {
                            if (lang == 'cn') alert('重启失败');
                            else alert('Restart error');
                        };
                        document.getElementsByClassName('tipsBox2')[0].style.display = 'none';
                        document.getElementsByClassName('mainBox2')[0].style.display = '';
                    };
                };
                xmlfile.ontimeout = function () {
                    restartNow2();
                };
                xmlfile.open('POST', '/cjson/system_para.json', true);
                xmlfile.send(`{\"string\":{\"4\":\"1\"}}`);
            };
        </script>
    </body>
    </html>)";

static const char DeviceMonitor_page_file[] PROGMEM =
R"(<!DOCTYPE html>
    <head>
        <meta charset='UTF-8'>
    </head>
    <body>
        <p class='title' lang='DeviceMonitor'></p>
        <p class='Introduction' lang='DeviceMonitorIntroduction'></p>
        <div class='card'>
            <div class='cTop' lang='PDInfoSNK'></div>
            <div class='cItem'>
                <span class='itemLeft' lang='PD_SNKVoltage'></span>
                <span id='pdsnkvoltage'></span>
            </div>
            <div class='cItem pluralItem'>
                <span class='itemLeft' lang='PD_SNKCurrent'></span>
                <span id='pdsnkcurrent'></span>
            </div>
            <div class='cItem'>
                <span class='itemLeft' lang='PD_SNKCapabilities'></span>
                <span id='pdsnkcapabilities'></span>
            </div>
        </div>
        <div class='card'>
            <div class='cTop' lang='PDInfoSRC'></div>
            <div class='cItem'>
                <span class='itemLeft' lang='PD_SRCVoltage'></span>
                <span id='pdsrcvoltage'></span>
            </div>
            <div class='cItem pluralItem'>
                <span class='itemLeft' lang='PD_SRCCurrent'></span>
                <span id='pdsrccurrent'></span>
            </div>
            <div class='cItem'>
                <span class='itemLeft' lang='PD_SRCCapabilities'></span>
                <span id='pdsrccapabilities'></span>
            </div>
        </div>
        <div class='card'>
            <div class='cTop' lang='WirelessChargeStatus'></div>
            <div class='cItem'>
                <span class='itemLeft' lang='WirelessCharge_Voltage'></span>
                <span id='wirelesschargevoltage'></span>
            </div>
            <div class='cItem pluralItem'>
                <span class='itemLeft' lang='WirelessCharge_Temperature'></span>
                <span id='wirelesschargetemperature'></span>
            </div>
        </div>
        <div class='card'>
            <div class='cTop' lang='NFCStatus'></div>
            <div class='cItem'>
                <span class='itemLeft' lang='NFC_UUID'></span>
                <span id='nfcuuid'></span>
            </div>
            <div class='cItem pluralItem'>
                <span class='itemLeft' lang='NFC_Sector0_Addr0'></span>
                <span id='nfcsector0addr0'></span>
            </div>
            <div class='cItem'>
                <span class='itemLeft' lang='NFC_Sector0_Addr1'></span>
                <span id='nfcsector0addr1'></span>
            </div>
            <div class='cItem pluralItem'>
                <span class='itemLeft' lang='NFC_Sector0_Addr2'></span>
                <span id='nfcsector0addr2'></span>
            </div>
            <div class='cItem'>
                <span class='itemLeft' lang='NFC_Sector0_Addr3'></span>
                <span id='nfcsector0addr3'></span>
            </div>
        </div>
        <div class='card'>
            <div class='cTop' lang='Restart'></div>
            <div class='formBox mainBox'>
                <div class='formItem' style='justify-content: center;'>
                    <button onclick='restartNow()' lang='RestartNow'></button>
                </div>
            </div>
            <div class='formBox tipsBox' style='display: none;'>
                <div class='formItem'>
                    <p class='tips'></p>
                    <button onclick='backMain()' lang='confirm'></button>
                </div>
            </div>
        </div>
        <script>
            checkUserCurLang();
            let lang = localStorage.getItem('lang');
            getSystemDevInfo();
            function getSystemDevInfo() {
                var xmlfile;
                if (window.XMLHttpRequest) {
                    xmlfile = new XMLHttpRequest();
                } else {
                    xmlfile = new ActiveXObject('Microsoft.XMLHTTP');
                };
                xmlfile.timeout = 3000;
                xmlfile.onreadystatechange = function () {
                    if (xmlfile.readyState == 4 && xmlfile.status == 200) {
                        let paramsList = JSON.parse(xmlfile.response);
                        if (document.getElementById('pdsnkvoltage') == null) {
                        } else {
                          document.getElementById('pdsnkvoltage').innerHTML = paramsList.string['28'];
                          document.getElementById('pdsnkcurrent').innerHTML = paramsList.string['29'];
                          document.getElementById('pdsnkcapabilities').innerHTML = paramsList.string['30'];
                          document.getElementById('pdsrcvoltage').innerHTML = paramsList.string['31'];
                          document.getElementById('pdsrccurrent').innerHTML = paramsList.string['32'];
                          document.getElementById('pdsrccapabilities').innerHTML = paramsList.string['33'];
                          document.getElementById('wirelesschargevoltage').innerHTML = paramsList.string['34'];
                          document.getElementById('wirelesschargetemperature').innerHTML = paramsList.string['35'];
                          document.getElementById('nfcuuid').innerHTML = paramsList.string['60'];
                          document.getElementById('nfcsector0addr0').innerHTML = paramsList.string['61'];
                          document.getElementById('nfcsector0addr1').innerHTML = paramsList.string['62'];
                          document.getElementById('nfcsector0addr2').innerHTML = paramsList.string['63'];
                          document.getElementById('nfcsector0addr3').innerHTML = paramsList.string['64'];
                        };
                    };
                };
                xmlfile.ontimeout = function () {
                    getSystemDevInfo();
                };
                xmlfile.open('GET', '/cjson/system_para.json', true);
                xmlfile.send();
            };
            function restartNow() {
                var xmlfile;
                if (window.XMLHttpRequest) {
                    xmlfile = new XMLHttpRequest();
                } else {
                    xmlfile = new ActiveXObject('Microsoft.XMLHTTP');
                };
                xmlfile.timeout = 3000;
                xmlfile.onreadystatechange = function () {
                    if (xmlfile.readyState == 4 && xmlfile.status == 200) {
                        let paramsList = JSON.parse(xmlfile.response);
                        if (paramsList.string['4'] == 1) {
                            if (lang == 'cn')
                                document.getElementsByClassName('tips')[0].innerHTML = '重启成功';
                            else
                                document.getElementsByClassName('tips')[0].innerHTML = 'Successful restart';
                        } else {
                            if (lang == 'cn')
                                document.getElementsByClassName('tips')[0].innerHTML = '重启失败,请重试';
                            else
                                document.getElementsByClassName('tips')[0].innerHTML = 'Restart error, please try again';
                        }
                        document.getElementsByClassName('tipsBox')[0].style.display = '';
                        document.getElementsByClassName('mainBox')[0].style.display = 'none';
                    };
                };
                xmlfile.ontimeout = function () {
                    restartNow();
                };
                xmlfile.open('POST', '/cjson/system_para.json', true);
                xmlfile.send(`{\"string\":{\"4\":\"1\"}}`);
            };
            function ReflashDev(){
              getSystemDevInfo();
            };
            clearInterval(intDev);
            intDev = setInterval(ReflashDev, 1000);
        </script>
    </body>
</html>)";

static const char firmware_page_file[] PROGMEM =
R"(<!DOCTYPE html>
    <head>
        <meta charset='UTF-8'>
        <style>
            progress {width: 35%;height: 32px}
            @media screen and (max-width: 1000px) {
                progress {flex: 1;margin-right: 80px;}
            }
        </style>
    </head>
    <body>
        <p class='title' lang='firmware'></p>
        <p class='Introduction' lang='firmwareIntroduction'></p>
        <div class='card'>
            <div class='cTop' lang='UpgradeSet'></div>
            <div class='formBox'>
                <div class='formItem'>
                    <label for='fileName' lang='FirmwareFile'></label>
                    <input type='file' id='btn_file' name='btn_file' style='display:none' accept='.bin,.img,.hex' onchange='fileSelected();'>
                    <input type='text' id='fileName' />
                    <button class='search' id='btn_select' onclick='select_bin();' lang='selFile'></button>
                </div>
                <div class='formItem'>
                    <label id='up_percent_labe'>0</label>
                    <progress id='up_percent_progress' value='0' max='100'></progress>
                </div>
                <div class='formItem'>
                    <div class='buttonBox'>
                        <button lang='upgrade' id='up_btn' onclick='uploadFile();'></button>
                        <button class='cancel' lang='cancel' onclick='cancelUpload();'></button>
                    </div>
                </div>
            </div>
        </div>
        <script>
            checkUserCurLang();
            let lang = localStorage.getItem('lang');
            function select_bin() {
                document.getElementById('btn_file').click();
            };
            function fileSelected() {
                var file_s = document.getElementById('btn_file').files[0];
                if (file_s) {
                    document.getElementById('fileName').value = file_s.name;
                    document.getElementById('up_percent_labe').innerHTML = '0';
                    document.getElementById('up_percent_progress').value = 0;
                };
            };
            function uploadFile() {
                if (document.getElementById('fileName').value != '') {
                    if (lang == 'cn')
                        document.getElementById('up_btn').innerHTML = '升级中...';
                    else
                        document.getElementById('up_btn').innerHTML = 'upgrading...';
                    document.getElementById('up_btn').disabled = 1;
                    document.getElementById('btn_select').disabled = 1;
                    confirmStatus();
                } else {
                    if (lang == 'cn')
                        alert('请选择升级文件!');
                    else
                        alert('Please select OTA File!');
                };
            };
            function cancelUpload() {
                var UpgradeStatus = localStorage.getItem('UpgradeStatus');
                if (UpgradeStatus == 1) {
                    var test_value;
                    if (lang == 'cn') {
                        test_value = confirm('停止升级?');
                    } else {
                        test_value = confirm('Stop upgrade?');
                    };
                    if (test_value) {
                        localStorage.setItem('UpgradeStatus', '2');
                        exitUpgrade();
                    } else {
                        return;
                    };
                };
            }
            function exitUpgrade() {
                var xmlfile;
                if (window.XMLHttpRequest) {
                    xmlfile = new XMLHttpRequest();
                } else {
                    xmlfile = new ActiveXObject('Microsoft.XMLHTTP');
                };
                xmlfile.timeout = 3000;
                xmlfile.onreadystatechange = function () {
                    if (xmlfile.readyState == 4 && xmlfile.status == 200) {
                        let paramsList = JSON.parse(xmlfile.response);
                        if (paramsList.string['36'] == 0) {
                          localStorage.setItem('UpgradeStatus', '0');
                          if (lang == 'cn')
                            document.getElementById('up_btn').innerHTML = '升级';
                          else
                            document.getElementById('up_btn').innerHTML = 'update';
                          document.getElementById('up_btn').disabled = 0;
                          document.getElementById('btn_select').disabled = 0;
                          if (lang == 'cn')
                            alert('停止升级成功!');
                          else
                            alert('Stop the upgrade successfully!');
                          start = 0;
                          end = start + LENGTH;
                        } else {
                          localStorage.setItem('UpgradeStatus', '0');
                          if (lang == 'cn')
                            document.getElementById('up_btn').innerHTML = '升级';
                          else
                            document.getElementById('up_btn').innerHTML = 'update';
                          document.getElementById('up_btn').disabled = 0;
                          document.getElementById('btn_select').disabled = 0;
                          if (lang == 'cn')
                            alert('停止升级异常!');
                          else
                            alert('Stop upgrading abnormally!');
                          start = 0;
                          end = start + LENGTH;
                        };
                    };
                    document.getElementById('up_percent_labe').innerHTML = '0';
                    document.getElementById('up_percent_progress').value = 0;
                    start = 0;
                    percent = 0;
                };
                xmlfile.ontimeout = function () {
                    exitUpgrade();
                };
                xmlfile.open('POST', '/cjson/system_para.json', true);
                xmlfile.send(`{\"string\":{\"36\":\"0\"}}`);
            };
            function confirmStatus() {
                var xmlfile;
                if (window.XMLHttpRequest) {
                    xmlfile = new XMLHttpRequest();
                } else {
                    xmlfile = new ActiveXObject('Microsoft.XMLHTTP');
                };
                xmlfile.timeout = 3000;
                xmlfile.onreadystatechange = function () {
                    if (xmlfile.readyState == 4 && xmlfile.status == 200) {
                        let paramsList = JSON.parse(xmlfile.response);
                        if (paramsList.string['36'] == 0) {
                            enterUpload();
                        } else {
                            if (lang == 'cn')
                                document.getElementById('up_btn').innerHTML = '升级';
                            else
                                document.getElementById('up_btn').innerHTML = 'update';
                            document.getElementById('up_btn').disabled = 0;
                            document.getElementById('btn_select').disabled = 0;
                            if (lang == 'cn')
                                alert('confirmStatus 设备正忙，请稍后再升级!');
                            else
                                alert('The device is busy, please upgrade later!');
                        };
                    };
                };
                xmlfile.ontimeout = function () {
                    confirmStatus();
                };
                xmlfile.open('GET', '/cjson/system_para.json', true);
                xmlfile.send();
            }
            var end_flg = 0;
            function enterUpload() {
                var xmlfile;
                if (window.XMLHttpRequest) {
                    xmlfile = new XMLHttpRequest();
                } else {
                    xmlfile = new ActiveXObject('Microsoft.XMLHTTP');
                };
                xmlfile.timeout = 6000;
                xmlfile.onreadystatechange = function () {
                    if (xmlfile.readyState == 4 && xmlfile.status == 200) {
                        let paramsList = JSON.parse(xmlfile.response);
                        if (paramsList.string['36'] == 1) {
                            end_flg = 0;
                            upload_file_inheart();
                        } else {
                            if (lang == 'cn')
                                document.getElementById('up_btn').innerHTML = '升级';
                            else
                                document.getElementById('up_btn').innerHTML = 'update';
                            document.getElementById('up_btn').disabled = 0;
                            document.getElementById('btn_select').disabled = 0;
                            if (lang == 'cn') {
                                alert('enterUpload 设备正忙，请稍后再升级!');
                            } else {
                                alert('The device is busy, please upgrade later!');
                            };
                        };
                    };
                };
                xmlfile.ontimeout = function () {
                    enterUpload();
                };
                xmlfile.open('POST', '/cjson/system_para.json', true);
                xmlfile.send(`{\"string\":{\"36\":\"1\"}}`);
            };
            var start = 0;
            var percent = 0;
            const LENGTH = 8192;
            var end = start + LENGTH;
            function upload_file_inheart() {
                var UpgradeStatus = localStorage.getItem('UpgradeStatus');
                if (UpgradeStatus == 2) {
                    localStorage.setItem('UpgradeStatus', '0');
                    return;
                };
                localStorage.setItem('UpgradeStatus', '1');
                var xmlfile;
                if (window.XMLHttpRequest) {
                    xmlfile = new XMLHttpRequest();
                } else {
                    xmlfile = new ActiveXObject('Microsoft.XMLHTTP');
                };
                xmlfile.open('GET', '/cjson/system_para.json', true);
                xmlfile.send();
                xmlfile.timeout = 3000;
                xmlfile.onreadystatechange = function () {
                    if (xmlfile.readyState == 4 && xmlfile.status == 200) {
                        let paramsList = JSON.parse(xmlfile.response);
                        if (paramsList.string['36'] == 1) {
                            if (paramsList.string['37'] == 1 && end_flg == 0) {
                                up_file_task();
                            } else if (paramsList.string['37'] == 2) {
                                if (lang == 'cn')
                                    document.getElementById('up_btn').innerHTML = '升级';
                                else
                                    document.getElementById('up_btn').innerHTML = 'update';
                                document.getElementById('up_btn').disabled = 0;
                                document.getElementById('btn_select').disabled = 0;
                                if (lang == 'cn')
                                    alert('OTA失败,通信失败');
                                else
                                    alert('OTA Failed');
                                start = 0;
                                end = start + LENGTH;
                                localStorage.setItem('UpgradeStatus', '0');
                                exitUpgrade();
                            } else if (paramsList.string['37'] == 0 || (end_flg == 1 && paramsList.string['37'] != 3)) {
                                upload_file_inheart();
                            } else if (paramsList.string['37'] == 3) {
                                start = 0;
                                end = start + LENGTH;
                                if (lang == 'cn') {
                                    alert('升级成功,设备即将重启');
                                    document.getElementById('up_btn').innerHTML = '升级';
                                } else {
                                    alert('OTA Succe ssed, Device ready to reboot');
                                    document.getElementById('up_btn').innerHTML = 'update';
                                };
                                document.getElementById('up_btn').disabled = 0;
                                document.getElementById('btn_select').disabled = 0;
                                end_flg = 0;
                                localStorage.setItem('UpgradeStatus', '0');
                            };
                        };
                    };
                };
                xmlfile.ontimeout = function () {
                    localStorage.setItem('UpgradeStatus', '0');
                    if (lang == 'cn')
                        document.getElementById('up_btn').innerHTML = '升级';
                    else
                        document.getElementById('up_btn').innerHTML = 'update';
                    document.getElementById('up_btn').disabled = 0;
                    document.getElementById('btn_select').disabled = 0;
                    if (UpgradeStatus != 2) {
                        if (lang == 'cn')
                            alert('OTA失败,通信超时');
                        else
                            alert('OTA Failed');
                    };
                    start = 0;
                    end = start + LENGTH;
                };
            };
            function up_file_task() {
                var UpgradeStatus = localStorage.getItem('UpgradeStatus');
                var file = document.getElementById('btn_file').files[0];
                var filename = document.getElementById('btn_file').files[0].name;
                document.getElementById('up_percent_labe').innerHTML = percent.toString() + '/100';
                document.getElementById('up_percent_progress').value = percent;
                var totalSize = file.size;
                if (totalSize < LENGTH) {
                  exitUpgrade();
                  return;
                };
                var fd = null;
                var blob = null;
                var xhr = null;
                fd = new FormData();
                xhr = new XMLHttpRequest();
                xhr.timeout = 6000;
                xhr.onreadystatechange = function () {
                    if (xhr.readyState == 4 && xhr.status == 200) {
                        if (UpgradeStatus == 1) {
                            document.getElementById('up_percent_labe').innerHTML = percent.toString() + '/100';
                            document.getElementById('up_percent_progress').value = percent;
                            if (end == totalSize) {
                                start = 0;
                                end = start + LENGTH;
                                end_flg = 1;
                                upload_file_inheart();
                            } else
                                upload_file_inheart();
                            start = end;
                            if (totalSize - end >= LENGTH)
                                end = start + LENGTH;
                            else if (totalSize - end < LENGTH)
                                end = start + totalSize - end;
                        };
                    };
                };
                xhr.ontimeout = function () {
                    localStorage.setItem('UpgradeStatus', '0');
                    if (lang == 'cn')
                        document.getElementById('up_btn').innerHTML = '升级';
                    else
                        document.getElementById('up_btn').innerHTML = 'update';
                    document.getElementById('up_btn').disabled = 0;
                    document.getElementById('btn_select').disabled = 0;
                    if (UpgradeStatus != 2) {
                        if (lang == 'cn')
                            alert('OTA失败,通信超时');
                        else
                            alert('OTA Failed');
                    };
                    start = 0;
                    end = start + LENGTH;
                };
                xhr.open('POST', '/update');
                fd.append('filename',filename);
                fd.append('file_size', totalSize);
                blob = file.slice(start, end);
                fd.append('file_data',blob);
                xhr.send(fd);
                percent = Math.round(end * 100 / totalSize);
            };
        </script>
    </body>
    </html>)";

#ifdef __cplusplus
}
#endif

#endif /* NOA_WEB_PAGE_H */
