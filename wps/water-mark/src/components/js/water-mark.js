function insertNewWaterMark()
{
    var app, shapeRange;
    try {
        // app = wpsFrame.Application;
        app=wps.WpsApplication();
        var doc = app.ActiveDocument;
        var ProtectionType=doc.ProtectionType;
        if(ProtectionType!==-1){//文档处于保护模式；解除保护模式
            // doc.Unprotect(password);
            // if(doc.ProtectionType!=-1){//解除保护模式失败
            //     alert("文档处于保护模式")
            //     return ;
            // }
            alert("保护模式下不支持水印");
        }
        var selection = doc.ActiveWindow.Selection;//当前光标对象
        var sectionCount = app.ActiveDocument.Sections.Count;
        for(var i=1;i<=sectionCount;i++){//遍历所有节，添加水印
            app.ActiveDocument.Sections.Item(i).Headers.Item(1).LinkToPrevious=true;
            selection.GoTo(0, 1, i);//跳转到指定节
            app.ActiveWindow.ActivePane.View.SeekView=9;
            app.ActiveDocument.Sections.Item(i).Headers.Item(1).Shapes.AddTextEffect(0, "公司绝密", "华文新魏", 36, false, false, 0, 0).Select();//插入文字水印，并选中
            shapeRange = app.Selection.ShapeRange;
            shapeRange.TextEffect.NormalizedHeight = false;
            shapeRange.Line.Visible = false;
            shapeRange.Fill.Visible = true;
            shapeRange.Fill.Solid();
            shapeRange.Fill.ForeColor.RGB = 12632256;       /* WdColor枚举 wdColorGray25 代表颜色值 */
            shapeRange.Fill.Transparency = 0.5;             /* 填充透明度，值为0.0~1.0 */
            shapeRange.LockAspectRatio = true;
            shapeRange.Height = 4.58 * 28.346;
            shapeRange.Width = 28.07 * 28.346;
            shapeRange.Rotation = 315;                      /* 图形按照Z轴旋转度数，正值为顺时针旋转，负值为逆时针旋转 */
            shapeRange.WrapFormat.AllowOverlap = true;
            shapeRange.WrapFormat.Side = 3;                 /* WdWrapSideType枚举 wdWrapLargest 形状距离页边距最远的一侧 */
            shapeRange.WrapFormat.Type = 3;
            shapeRange.RelativeHorizontalPosition = 0;
            shapeRange.RelativeVerticalPosition = 0;
            shapeRange.Left = -999995;
            shapeRange.Top = -999995;                         /* WdShapePosition枚举 wdShapeCenter 形状的位置在中央 */
        }
        if(selection.PageSetup.DifferentFirstPageHeaderFooter){
            selection.GoTo(1, 1, 2);//跳转到第二页
            app.ActiveWindow.ActivePane.View.SeekView=9;
            app.ActiveDocument.Sections.Item(1).Headers.Item(1).Shapes.AddTextEffect(0, "公司绝密", "华文新魏", 36, false, false, 0, 0).Select();//插入文字水印，并选中
            shapeRange = app.Selection.ShapeRange;
            shapeRange.TextEffect.NormalizedHeight = false;
            shapeRange.Line.Visible = false;
            shapeRange.Fill.Visible = true;
            shapeRange.Fill.Solid();
            shapeRange.Fill.ForeColor.RGB = 12632256;       /* WdColor枚举 wdColorGray25 代表颜色值 */
            shapeRange.Fill.Transparency = 0.5;             /* 填充透明度，值为0.0~1.0 */
            shapeRange.LockAspectRatio = true;
            shapeRange.Height = 4.58 * 28.346;
            shapeRange.Width = 28.07 * 28.346;
            shapeRange.Rotation = 315;                      /* 图形按照Z轴旋转度数，正值为顺时针旋转，负值为逆时针旋转 */
            shapeRange.WrapFormat.AllowOverlap = true;
            shapeRange.WrapFormat.Side = 3;                 /* WdWrapSideType枚举 wdWrapLargest 形状距离页边距最远的一侧 */
            shapeRange.WrapFormat.Type = 3;
            shapeRange.RelativeHorizontalPosition = 0;
            shapeRange.RelativeVerticalPosition = 0;
            shapeRange.Left = -999995;
            shapeRange.Top = -999995;
        }
        if(selection.PageSetup.OddAndEvenPagesHeaderFooter){
            selection.GoTo(1, 1, 2);//跳转到第二页
            app.ActiveWindow.ActivePane.View.SeekView=9;
            app.ActiveDocument.Sections.Item(1).Headers.Item(3).Shapes.AddTextEffect(0, "公司绝密", "华文新魏", 36, false, false, 0, 0).Select();//插入文字水印，并选中
            shapeRange = app.Selection.ShapeRange;
            shapeRange.TextEffect.NormalizedHeight = false;
            shapeRange.Line.Visible = false;
            shapeRange.Fill.Visible = true;
            shapeRange.Fill.Solid();
            shapeRange.Fill.ForeColor.RGB = 12632256;       /* WdColor枚举 wdColorGray25 代表颜色值 */
            shapeRange.Fill.Transparency = 0.5;             /* 填充透明度，值为0.0~1.0 */
            shapeRange.LockAspectRatio = true;
            shapeRange.Height = 4.58 * 28.346;
            shapeRange.Width = 28.07 * 28.346;
            shapeRange.Rotation = 315;                      /* 图形按照Z轴旋转度数，正值为顺时针旋转，负值为逆时针旋转 */
            shapeRange.WrapFormat.AllowOverlap = true;
            shapeRange.WrapFormat.Side = 3;                 /* WdWrapSideType枚举 wdWrapLargest 形状距离页边距最远的一侧 */
            shapeRange.WrapFormat.Type = 3;
            shapeRange.RelativeHorizontalPosition = 0;
            shapeRange.RelativeVerticalPosition = 0;
            shapeRange.Left = -999995;
            shapeRange.Top = -999995;
        }

        selection.GoTo(1, 1, 1);
        app.ActiveWindow.ActivePane.View.SeekView=0;
        // if(ProtectionType!=-1){
        //     doc.Protect(ProtectionType,"",password);
        // }
    } catch (error) {
        alert(error.message);
    }
}

function deleteAllWater()
{
    var app=wps.WpsApplication();
    var doc = app.ActiveDocument;
    // var ProtectionType=doc.ProtectionType;
    // if(ProtectionType!=-1){//文档处于保护模式；解除保护模式
        // doc.Unprotect(password);
        // if(doc.ProtectionType!=-1){//解除保护模式失败
        //     alert("密码错误")
        //     return ;
        // }
    // }
    var selection = doc.ActiveWindow.Selection;//当前光标对象
    var sectionCount = app.ActiveDocument.Sections.Count;
    for(var i=1;i<=sectionCount;i++){//遍历所有节，删除水印
        selection.GoTo(0, 1, i);//跳转到指定节
        app.ActiveDocument.Sections.Item(i).Headers.Item(1).Range.Text=""
    }
    // if(ProtectionType!=-1){
    //     doc.Protect(ProtectionType,"",password);
    // }
}

export default{
    insertNewWaterMark,
    deleteAllWater
}