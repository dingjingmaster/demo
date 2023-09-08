#include <MailCore/MCArray.h>
#include <MailCore/MCIMAPFolder.h>
#include <MailCore/MCIMAPMessage.h>
#include <MailCore/MCMessageConstants.h>
#include <MailCore/MCRange.h>
#include <cstdint>
#include <stdio.h>
#include <MailCore/MailCore.h>

int main ()
{
    // 创建 IMAP 会话
    mailcore::IMAPSession* session = new mailcore::IMAPSession();

    // 配置服务器设置
    // LRZRVSASFWLROZEE
    auto ip = mailcore::String::stringWithUTF8Format("imap.163.com");
    auto port = 993; // 143
    auto name = mailcore::String::stringWithUTF8Format("18635038785");
    auto passwd = mailcore::String::stringWithUTF8Format("LRZRVSASFWLROZEE");
    session->setHostname(ip);               // ip
    session->setPort(143);                  // port
    session->setUsername(name);             // name
    session->setPassword(passwd);           // passwd

    // 启动会话
    mailcore::ErrorCode error; 
    session->connect(&error); 
    if (mailcore::ErrorNone != error) {
        printf("连接失败: %d\n", error);
        return -1;
    }

    // 获取所有邮箱
    auto allFolders = session->fetchAllFolders(&error);
    //auto allFolders = session->fetchSubscribedFolders(&error);
    if (mailcore::ErrorNone != error) {
        printf("fetchAllFolders失败: %d\n", error);
        return -1;
    }

    for (int i = 0; i < allFolders->count(); ++i) {
        auto obj = (mailcore::IMAPFolder*) allFolders->objectAtIndex(i);
        printf("folder: %s\n", obj->path()->UTF8Characters());

        auto status = session->folderStatus(obj->path(), &error); 
        if (mailcore::ErrorNone != error) {
            printf("folder status error: %d\n", error);
            continue;
        }
#if 0
        session->select(obj->path(), &error);
        if (mailcore::ErrorNone != error) {
            printf("连接失败: %d\n", error);
            continue;
        }
#endif
        // 获取邮件列表
        auto* message = session->fetchMessagesByNumber(obj->path(),
                mailcore::IMAPMessagesRequestKindHeaders,
                mailcore::IndexSet::indexSetWithRange(mailcore::RangeMake(1, UINT64_MAX)), NULL, &error);
        if (mailcore::ErrorNone != error) {
            printf("获取邮件列表失败: %d\n", error);
            continue;
        }

        // 输出邮件列表
        for (unsigned int j = 0; j < message->count(); ++j) {
            auto* msg = (mailcore::IMAPMessage*) message->objectAtIndex(j);
            printf("    邮件主题：%s\n", msg->description()->UTF8Characters());
        }
    }

#if 0
#endif

    session->disconnect();
    session->release();

    return 0;
}
