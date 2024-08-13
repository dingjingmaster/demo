/*************************************************************************
> FileName: json.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 09 Aug 2024 07:26:42 PM CST
 ************************************************************************/
#include <QDebug>
#include <QString>
#include <QRegExp>
#include <QJsonDocument>

int main ()
{
    QString str = "[\"PATCH_UPGRADE_SCHEDULE_TASK_NOTICE\",\"{\\\"creator\\\":\\\"1912bd246281agrblapd\\\",\\\"modifier\\\":\\\"1912bd246281agrblapd\\\",\\\"createdStamp\\\":\\\"2024-08-10 11:41:17\\\",\\\"lastUpdatedStamp\\\":\\\"2024-08-10 11:41:31\\\",\\\"taskId\\\":\\\"1913a60521d1hn31kg8r\\\",\\\"taskName\\\":\\\"linux\\\",\\\"description\\\":null,\\\"status\\\":\\\"RUNNING\\\",\\\"categoryId\\\":\\\"PatchUpgradeSchedule\\\",\\\"schedulingCron\\\":null,\\\"schedulingMechanism\\\":\\\"NOW\\\",\\\"tenantId\\\":\\\"1909156897dl203tedc4\\\",\\\"scheduleContext\\\":null,\\\"finishedStamp\\\":null,\\\"penertrateZip\\\":null,\\\"fileTypeList\\\":null,\\\"bypassPath\\\":null,\\\"patchIssue\\\":{\\\"creator\\\":\\\"1912bd246281agrblapd\\\",\\\"modifier\\\":\\\"1912bd246281agrblapd\\\",\\\"createdStamp\\\":\\\"2024-08-10 11:41:17\\\",\\\"lastUpdatedStamp\\\":\\\"2024-08-10 11:41:17\\\",\\\"id\\\":\\\"1913a60521f10n8mn15o\\\",\\\"taskId\\\":\\\"1913a60521d1hn31kg8r\\\",\\\"patchVersion\\\":\\\"1\\\",\\\"patchId\\\":\\\"19136b1efff1k1qi09f1\\\",\\\"installMode\\\":\\\"DEFAULT\\\",\\\"completeSpeed\\\":null,\\\"patchInfo\\\":{\\\"creator\\\":\\\"1912bd246281agrblapd\\\",\\\"modifier\\\":\\\"1912bd246281agrblapd\\\",\\\"createdStamp\\\":\\\"2024-08-09 18:31:57\\\",\\\"lastUpdatedStamp\\\":\\\"2024-08-10 11:41:24\\\",\\\"id\\\":\\\"19136b1efff1k1qi09f1\\\",\\\"patchName\\\":\\\"andsec_1\\\",\\\"patchVersion\\\":\\\"1\\\",\\\"filePath\\\":\\\"/fileserver/data/uploads/2024/08/09/andsec_1.0.1_amd64.deb\\\",\\\"patchSystemType\\\":\\\"Linux\\\",\\\"description\\\":\\\"\\\",\\\"tenantId\\\":\\\"1909156897dl203tedc4\\\",\\\"status\\\":1,\\\"reduceVersionMode\\\":1,\\\"fileExtend\\\":null,\\\"fileName\\\":\\\"andsec_1.0.1_amd64.deb\\\",\\\"fileSize\\\":\\\"78740702\\\",\\\"fileMd5\\\":\\\"9d47455c695a0411931df610f2636b0c\\\"}}}\"]";


    QRegExp digit("^\\d+");
    QRegExp c("\\");

    str.replace(digit, "");
    //str.replace(c, "");

    //qInfo() << str;

//    QByteArray arr = QByteArray::fromRawData(str.toUtf8(), str.size());

    auto json = QJsonDocument::fromJson(str.toUtf8());
    if (json.isArray()) {
        qInfo() << json[0];
        qInfo() << json[1];
    }

    return 0;
}
