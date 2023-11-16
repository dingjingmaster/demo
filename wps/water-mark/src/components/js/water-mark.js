
function wpsDrawWaterMark (app, str, fontSize, isBold, isItalic, what, which, count, idx, it)
{
    var shapeRange;
    var doc = app.ActiveDocument;
    var selection = doc.ActiveWindow.Selection;//当前光标对象
    selection.GoTo(what, which, count);//跳转到第二页
    app.ActiveWindow.ActivePane.View.SeekView=9;
    app.ActiveDocument.Sections.Item(idx).Headers.Item(it).Shapes.AddTextEffect(0, str, "黑体", fontSize, isBold, isItalic, 0, 0).Select();//插入文字水印，并选中
    shapeRange = app.Selection.ShapeRange;
    shapeRange.TextEffect.NormalizedHeight = false;
    shapeRange.Line.Visible = false;
    shapeRange.Fill.Visible = true;
    shapeRange.Fill.Solid();
    shapeRange.Fill.ForeColor.RGB = 12632256;       /* WdColor枚举 wdColorGray25 代表颜色值 */
    shapeRange.Fill.Transparency = 0.5;             /* 填充透明度，值为0.0~1.0 */
    shapeRange.LockAspectRatio = true;
    // shapeRange.Height = 4.58 * 28.346;              // 4.58  * 28.346
    // shapeRange.Width = 28.07 * 28.346;              // 28.07 * 28.346
    shapeRange.Height = 16 * 28.346;
    shapeRange.Width =  16 * 28.346;
    shapeRange.Rotation = 315;                      /* 图形按照Z轴旋转度数，正值为顺时针旋转，负值为逆时针旋转 */
    shapeRange.WrapFormat.AllowOverlap = true;
    shapeRange.WrapFormat.Side = 3;                 /* WdWrapSideType枚举 wdWrapLargest 形状距离页边距最远的一侧 */
    shapeRange.WrapFormat.Type = 3;
    shapeRange.RelativeHorizontalPosition = 0;

    shapeRange.RelativeVerticalPosition = 0;
    shapeRange.Left = -999995;
    shapeRange.Top = -999995;
}

function wpsInsertNewWaterMark(str, fontSize, isBold, isItalic)
{
    try {
        var app = wps.WpsApplication();
        var doc = app.ActiveDocument;
        var ProtectionType=doc.ProtectionType;
        if(ProtectionType!==-1){//文档处于保护模式；解除保护模式
            alert("保护模式下不支持水印");
        }
        var selection = doc.ActiveWindow.Selection;//当前光标对象
        var sectionCount = app.ActiveDocument.Sections.Count;
        for(var i=1;i<=sectionCount;i++){//遍历所有节，添加水印
            app.ActiveDocument.Sections.Item(i).Headers.Item(1).LinkToPrevious=true;
            wpsDrawWaterMark(app, str, fontSize, isBold, isItalic, 0, 1, i, i, 1);
        }
        if(selection.PageSetup.DifferentFirstPageHeaderFooter){
            wpsDrawWaterMark(app, str, fontSize, isBold, isItalic, 1, 1, 2, 1, 1);
        }
        if(selection.PageSetup.OddAndEvenPagesHeaderFooter){
            wpsDrawWaterMark(app, str, fontSize, isBold, isItalic, 1, 1, 2, 1, 3);
        }

        selection.GoTo(1, 1, 1);
        app.ActiveWindow.ActivePane.View.SeekView=0;
    } catch (error) {
        alert(error.message);
    }
}

function wpsDeleteAllWater()
{
    var app = wps.WpsApplication();
    var doc = app.ActiveDocument;
    var selection = doc.ActiveWindow.Selection;//当前光标对象
    var sectionCount = app.ActiveDocument.Sections.Count;
    for(var i=1;i<=sectionCount;i++){//遍历所有节，删除水印
        selection.GoTo(0, 1, i);//跳转到指定节
        app.ActiveDocument.Sections.Item(i).Headers.Item(1).Range.Text=""
    }
}

function wpsSaveAllWater() {

    var app = wps.WpsApplication();
    var doc = app.ActiveDocument;

    var selection = doc.ActiveWindow.Selection;//当前光标对象
    var sectionCount = app.ActiveDocument.Sections.Count;
    for (var i = 1; i <= sectionCount; i++) {//遍历所有节，添加水印
        selection.GoTo(0, 1, i);//跳转到指定节
        app.ActiveDocument.Save();
    }
}

function excelInsertNewWaterMark (str)
{
    var waterMarkInfo = {
        'visibleStatus': 1,
        'watermarkText': str,
        'opacity': 0.5,
        'font_size': 18,
        'angle': 45,
        'color': '#A8A8A8A8',
        'font': '黑体',
    };

    try {
        var app = wps.EtApplication();
        app.Application.GetApplicationEx().SetWatermarkInfo(JSON.stringify(waterMarkInfo));
    } catch (e) {
        console.log(e)
    }
}

function excelDeleteAllWater(str)
{
    var waterMarkInfo = {
        'visibleStatus': 0,
        'watermarkText': str,
        'opacity': 0.5,
        'font_size': 18,
        'angle': 45,
        'color': '#A8A8A8A8',
        'font': '黑体',
    };

    try {
        var app = wps.EtApplication();
        app.Application.GetApplicationEx().SetWatermarkInfo(JSON.stringify(waterMarkInfo));
    } catch (e) {
        console.log(e)
    }
}

function pptInsertNewWaterMark (str)
{
    var waterMarkInfo = {
        'visibleStatus': 1,
        'watermarkText': str,
        'opacity': 0.5,
        'font_size': 18,
        'angle': 45,
        'color': '#A8A8A8A8',
        'font': '黑体',
    };

    try {
        var app = wps.WppApplication();
        app.Application.GetApplicationEx().SetWatermarkInfo(JSON.stringify(waterMarkInfo));
    } catch (e) {
        console.log(e)
    }
}

export default{
    wpsSaveAllWater,
    wpsDeleteAllWater,
    wpsInsertNewWaterMark,

    excelDeleteAllWater,
    excelInsertNewWaterMark,

    pptInsertNewWaterMark,
}