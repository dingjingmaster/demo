import Util from './js/util.js'
import WaterMark from './js/water-mark.js'

var serverINI = '/usr/local/ultrasec/server.ini';

function parseINIString(data)
{
    var regex = {
        section: /^\s*\[\s*([^\]]*)\s*\]\s*$/,
        // eslint-disable-next-line no-useless-escape
        param: /^\s*([\w\.\-\_]+)\s*=\s*(.*?)\s*$/,
        comment: /^\s*;.*$/
    };
    var value = {};
    var lines = data.split(/\r\n|\r|\n/);
    var section = null;
    lines.forEach(function(line){
        if(regex.comment.test(line)){
            return;
        }else if(regex.param.test(line)){
            var match = line.match(regex.param);
            if(section){
                value[section][match[1]] = match[2];
            }else{
                value[match[1]] = match[2];
            }
        }else if(regex.section.test(line)){
            // eslint-disable-next-line no-redeclare
            var match = line.match(regex.section);
            value[match[1]] = {};
            section = match[1];
        }else if(line.length === 0 && section){
            section = null;
        };
    });
    return value;
}

/**
 * @return {string}
 */
function GetWaterMarkString()
{
    var retStr = "";
    var userName = "";
    if (wps.FileSystem.Exists(serverINI)) {
        var str = wps.FileSystem.ReadFile(serverINI);
        if (str !== null && str !== undefined) {
            var ini = parseINIString(str);
            if (null !== ini && undefined !== ini) {
                var user = ini['user'];
                if (null !== user&& undefined !== user) {
                    userName = user['username'];
                    if (null === userName || undefined === userName) {
                        userName = '';
                    }
                }
            }
        }
    }

    var date = new Date();
    var dateStr = date.getFullYear().toString(10).padStart(2, '0') + '-' + date.getMonth().toString(10).padStart(2, '0') + '-' + date.getDay().toString(10).padStart(2, '0');
    if ("" !== userName) {
        retStr = userName + '\n';
    }
    retStr += dateStr;

    return retStr;
}

/**
 * @return {string}
 */
function GetApplicationObject()
{
    if (wps.WpsApplication !== undefined) {
        return wps.WpsApplication.name
    }
    else if (wps.EtApplication !== undefined) {
        return wps.EtApplication.name;
    }
    else if (wps.WppApplication !== undefined) {
        return wps.WppApplication.name;
    }

    return 'unknown';
}

//这个函数在整个wps加载项中是第一个执行的
/**
 * @return {boolean}
 */
function OnAddinLoad(ribbonUI)
{
    if (typeof (wps.ribbonUI) != "object"){
		wps.ribbonUI = ribbonUI
    }
    
    if (typeof (wps.Enum) != "object") { // 如果没有内置枚举值
        wps.Enum = Util.WPS_Enum
    }

    switch (GetApplicationObject()) {
        case 'EtApplication':{
            etAddWaterMark();
            wps.ApiEvent.AddApiEventListener('WorkbookOpen', function (fileUrl) { wpsAddWaterMark(); return fileUrl; });
            break
        }
        case 'WpsApplication': {
            wpsAddWaterMark();
            wps.ApiEvent.AddApiEventListener('DocumentOpen', function (fileUrl) { etAddWaterMark(); return fileUrl; });
            break
        }
        case 'WppApplication': {
            wppAddWaterMark();
            wps.ApiEvent.AddApiEventListener('PresentationOpen', function (fileUrl) { wppAddWaterMark(); return fileUrl; });
            break
        }
        default: {
            break
        }
    }

    return true
}

function wppAddWaterMark()
{

}

function etAddWaterMark()
{
    WaterMark.excelInsertNewWaterMark(GetWaterMarkString())
}

function wpsAddWaterMark()
{
    WaterMark.wpsDeleteAllWater();
    WaterMark.wpsInsertNewWaterMark(GetWaterMarkString(), 10, true, false);
    WaterMark.wpsSaveAllWater();
}

//
// var WebNotifycount = 0;
// function OnAction(control) {
//     const eleId = control.Id
//     switch (eleId) {
//         case "btnShowMsg":
//             {
//                 const doc = wps.WpsApplication().ActiveDocument
//                 if (!doc) {
//                     alert("当前没有打开任何文档")
//                     return
//                 }
//                 alert(doc.Name)
//             }
//             break;
//         case "btnIsEnbable":
//             {
//                 let bFlag = wps.PluginStorage.getItem("EnableFlag")
//                 wps.PluginStorage.setItem("EnableFlag", !bFlag)
//
//                 //通知wps刷新以下几个按饰的状态
//                 wps.ribbonUI.InvalidateControl("btnIsEnbable")
//                 wps.ribbonUI.InvalidateControl("btnShowDialog")
//                 wps.ribbonUI.InvalidateControl("btnShowTaskPane")
//                 //wps.ribbonUI.Invalidate(); 这行代码打开则是刷新所有的按钮状态
//                 break
//             }
//         case "btnShowDialog":
//             wps.ShowDialog(Util.GetUrlPath() + "dialog", "这是一个对话框网页", 400 * window.devicePixelRatio, 400 * window.devicePixelRatio, false)
//             break
//         case "btnShowTaskPane":
//             {
//                 let tsId = wps.PluginStorage.getItem("taskpane_id")
//                 if (!tsId) {
//                     let tskpane = wps.CreateTaskPane(Util.GetUrlPath() + "taskpane")
//                     let id = tskpane.ID
//                     wps.PluginStorage.setItem("taskpane_id", id)
//                     tskpane.Visible = true
//                 } else {
//                     let tskpane = wps.GetTaskPane(tsId)
//                     tskpane.Visible = !tskpane.Visible
//                 }
//             }
//             break
//         case "btnApiEvent":
//             {
//                 let bFlag = wps.PluginStorage.getItem("ApiEventFlag")
//                 let bRegister = bFlag ? false : true
//                 wps.PluginStorage.setItem("ApiEventFlag", bRegister)
//                 if (bRegister){
//                     wps.ApiEvent.AddApiEventListener('DocumentNew', 'ribbon.OnNewDocumentApiEvent')
//                 }
//                 else{
//                     wps.ApiEvent.RemoveApiEventListener('DocumentNew', 'ribbon.OnNewDocumentApiEvent')
//                 }
//
//                 wps.ribbonUI.InvalidateControl("btnApiEvent")
//             }
//             break
//         case "btnWebNotify":
//             {
//                 let currentTime = new Date()
//                 let timeStr = currentTime.getHours() + ':' + currentTime.getMinutes() + ":" + currentTime.getSeconds()
//                 wps.OAAssist.WebNotify("这行内容由wps加载项主动送达给业务系统，可以任意自定义, 比如时间值:" + timeStr + "，次数：" + (++WebNotifycount), true)
//             }
//             break
//         default:
//             break
//     }
//     return true
// }
//
// function GetImage(control) {
//     const eleId = control.Id
//     switch (eleId) {
//         case "btnShowMsg":
//             return "images/1.svg"
//         case "btnShowDialog":
//             return "images/2.svg"
//         case "btnShowTaskPane":
//             return "images/3.svg"
//         default:
//     }
//     return "images/newFromTemp.svg"
// }
//
// function OnGetEnabled(control) {
//     const eleId = control.Id
//     switch (eleId) {
//         case "btnShowMsg":
//             return true
//         case "btnShowDialog":
//             {
//                 let bFlag = wps.PluginStorage.getItem("EnableFlag")
//                 return bFlag
//             }
//         case "btnShowTaskPane":
//             {
//                 let bFlag = wps.PluginStorage.getItem("EnableFlag")
//                 return bFlag
//             }
//         default:
//             break
//     }
//     return true
// }
//
// function OnGetVisible(control){
//     const eleId = control.Id
//     console.log(eleId)
//     return true
// }
//
// function OnGetLabel(control){
//     const eleId = control.Id
//     switch (eleId) {
//         case "btnIsEnbable":
//             {
//                 let bFlag = wps.PluginStorage.getItem("EnableFlag")
//                 return bFlag ?  "按钮Disable" : "按钮Enable"
//             }
//         case "btnApiEvent":
//             {
//                 let bFlag = wps.PluginStorage.getItem("ApiEventFlag")
//                 return bFlag ? "清除新建文件事件" : "注册新建文件事件"
//             }
//     }
//     return ""
// }
//
// function OnNewDocumentApiEvent(doc){
//     alert("新建文件事件响应，取文件名: " + doc.Name)
// }

//这些函数是给wps客户端调用的
export default {
    OnAddinLoad,
    // OnAction,
    // GetImage,
    // OnGetEnabled,
    // OnGetVisible,
    // OnGetLabel,
    // OnNewDocumentApiEvent
};
